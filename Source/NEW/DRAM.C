#include <htc.h>
#include "DELAY.H"
#include "TESTER.H"
#include "TFT9163.H"
#include "TEXT.H"
#include "UTIL.H"
#include "DRAM.H"

/*
            ТИП 0                        ТИП 1
        РУ5, РУ6, РУ7                     РУ9
       +------------+               +------------+
 D2 --1|A8(РУ7)  GND|16- D0   D2 --1|DI       GND|18- D0
 D3 --2|DI      ~CAS|15- D4   D3 --2|~WE       DO|17- D4
 D1 --3|~WE       DO|14- D5   D1 --3|~RAS    ~CAS|16- D5
 B7 --4|~RAS      A6|13- D6   B7 --4|T.F.      A9|15- D6
 A0 --5|A0        A3|12- D7   A0 --5|A0        A8|14- D7
 A1 --6|A2        A4|11- B0   A1 --6|A1        A7|13- B0
 A2 --7|A1        A5|10- B1   A2 --7|A2        A6|12- B1
 A3 --8|+5V       A7| 9- B2   A3 --8|A3        A5|11- B2
                              A5 --9|+5V       A4|10- B3
 A5 --9|            |--- B3
 E0 -10|            |--- B4   E0 -10|            |--- B4
 E1 -11|            |--- B5   E1 -11|            |--- B5
GND -12|            |--- B6  GND -12|            |--- B6
       +------------+               +------------+

           ТИП 2                        ТИП 3
        HY51C4256                      HY53C464
       +------------+               +------------+
 D2 --1|D0       GND|20- D0   D2 --1|~OE      GND|18- D0
 D3 --2|D1        D3|19- D4   D3 --2|D0        D3|17- D4
 D1 --3|~WE       D2|18- D5   D1 --3|D1      ~CAS|16- D5
 B7 --4|~RAS    ~CAS|17- D6   B7 --4|~WE       D2|15- D6
 A0 --5|x        ~OE|16- D7   A0 --5|~RAS      A0|14- D7
 A1 --6|A0        A8|15- B0   A1 --6|A6        A1|13- B0
 A2 --7|A1        A7|14- B1   A2 --7|A5        A2|12- B1
 A3 --8|A2        A6|13- B2   A3 --8|A4        A3|11- B2
 A5 --9|A3        A5|12- B3   A5 --9|+5V       A7|10- B3
 E0 -10|+5V       A4|11- B4
                              E0 -10|            |11- B4
 E1 -11|            |--- B5   E1 -11|            |--- B5
GND -12|            |--- B6  GND -12|            |--- B6
       +------------+               +------------+
*/
#define t0_RAS(n)    if (n > 0) { LATB |= (1 << 7); } else { LATB &= ~(1 << 7); }
#define t0_CAS(n)    if (n > 0) { LATD |= (1 << 4); } else { LATD &= ~(1 << 4); }
#define t0_WE(n)     if (n > 0) { LATD |= (1 << 1); } else { LATD &= ~(1 << 1); }
#define t0_DI(n)     if (n > 0) { LATD |= (1 << 3); } else { LATD &= ~(1 << 3); }

#define t1_RAS(n)    if (n > 0) { LATD |= (1 << 1); } else { LATD &= ~(1 << 1); }
#define t1_CAS(n)    if (n > 0) { LATD |= (1 << 5); } else { LATD &= ~(1 << 5); }
#define t1_WE(n)     if (n > 0) { LATD |= (1 << 3); } else { LATD &= ~(1 << 3); }
#define t1_DI(n)     if (n > 0) { LATD |= (1 << 2); } else { LATD &= ~(1 << 2); }

#define t2_RAS(n)    if (n > 0) { LATB |= (1 << 7); } else { LATB &= ~(1 << 7); }
#define t2_CAS(n)    if (n > 0) { LATD |= (1 << 6); } else { LATD &= ~(1 << 6); }
#define t2_WE(n)     if (n > 0) { LATD |= (1 << 1); } else { LATD &= ~(1 << 1); }
#define t2_OE(n)     if (n > 0) { LATD |= (1 << 7); } else { LATD &= ~(1 << 7); }

#define t3_RAS(n)    if (n > 0) { LATA |= (1 << 0); } else { LATA &= ~(1 << 0); }
#define t3_CAS(n)    if (n > 0) { LATD |= (1 << 5); } else { LATD &= ~(1 << 5); }
#define t3_WE(n)     if (n > 0) { LATB |= (1 << 7); } else { LATB &= ~(1 << 7); }
#define t3_OE(n)     if (n > 0) { LATD |= (1 << 2); } else { LATD &= ~(1 << 2); }


#define TABLE_NCELL     16  // количество отображаемых ячеек памяти по горизонтали (столько же по вертикали)
#define TABLE_CELLSHIFT 4   // сдвиг вместо деления на TABLE_NCELL

#define TABLE_X         2
#define TABLE_Y         13

#define CELL_SIZE       5   // размер ячейки в пикселях


void t0_SetAddr(int addr);
char dram_readbit(int row, int column);
void dram_writebit(int row, int column, char value);


unsigned char ic_type;          // тип: 0 - типа РУ5-РУ7; 1 - типа 411000
unsigned int  nrows;            // rows number: 0x80 - РУ6, 0x100 - РУ5, 0x200 - РУ7

unsigned char shift_bits;
unsigned char shift_mask;


/*
  инициализация и включение питания микросхемы
*/
void dram_Init(void)
{
    unsigned char i;

  RA3 = 0;
    // читаем данные о м/с
    ic_type = *ic_test++;
    i = *ic_test++;
    nrows   = 1U << i;

    shift_bits = *ic_test;
    shift_mask = 0;
    for (i = 0; i < shift_bits; i++)
        shift_mask = (shift_mask << 1) | 1;

  // включаем питание
  LATA |= 8;
//  DelayMs10(1);
    // 16 циклов чтения
    for (i = 0; i < 16; i++)
        dram_readbit(0, 0);
}





/*
  установка адреса на адресных линиях для РУ5, РУ6 и РУ7
*/
void t0_SetAddr(int addr)
{
    char a, b, d;

    a = LATA & 0b11111000;
    b = LATB & 0b11111000;
    d = LATD & 0b00111011;
    if (addr & 1)           // bit A0
        a |= (1 << 0);
    if (addr & 2)           // bit A1
        a |= (1 << 2);
    if (addr & 4)           // bit A2
        a |= (1 << 1);
    if (addr & 8)           // bit A3
        d |= (1 << 7);
    if (addr & 0x10)        // bit A4
        b |= (1 << 0);
    if (addr & 0x20)        // bit A5
        b |= (1 << 1);
    if (addr & 0x40)        // bit A6
        d |= (1 << 6);
    if (addr & 0x80)        // bit A7
        b |= (1 << 2);
    if (addr & 0x100)       // bit A8
        d |= (1 << 2);
    LATA = a;
    LATB = b;
    LATD = d;
}

/*
  установка адреса на адресных линиях для РУ9
*/
void t1_SetAddr(int addr)
{
    char a, b, d;

    a = LATA & 0b11110000;
    b = LATB & 0b11110000;
    d = LATD & 0b00111111;

    a |= (addr & 0x0F);     // bit A0-A3

    if (addr & 0x10)        // bit A4
        b |= (1 << 3);
    if (addr & 0x20)        // bit A5
        b |= (1 << 2);
    if (addr & 0x40)        // bit A6
        b |= (1 << 1);
    if (addr & 0x80)        // bit A7
        b |= (1 << 0);
    if (addr & 0x100)       // bit A8
        d |= (1 << 7);
    if (addr & 0x200)       // bit A9
        d |= (1 << 6);
    LATA = a;
    LATB = b;
    LATD = d;
}


/*           ТИП 2
        HY51C4256
       +------------+
 D2 --1|D0       GND|20- D0
 D3 --2|D1        D3|19- D4
 D1 --3|~WE       D2|18- D5
 B7 --4|~RAS    ~CAS|17- D6
 A0 --5|x        ~OE|16- D7
 A1 --6|A0        A8|15- B0
 A2 --7|A1        A7|14- B1
 A3 --8|A2        A6|13- B2
 A5 --9|A3        A5|12- B3
 E0 -10|+5V       A4|11- B4
       +------------+
*/
void t2_SetAddr(int addr)
{
    char a, b;

    a = LATA & 0b11010001;
    b = LATB & 0b11100000;

    if (addr & 1)           // bit A0
        a |= (1 << 1);
    if (addr & 2)           // bit A1
        a |= (1 << 2);
    if (addr & 4)           // bit A2
        a |= (1 << 3);
    if (addr & 8)           // bit A3
        a |= (1 << 5);
    if (addr & 0x10)        // bit A4
        b |= (1 << 4);
    if (addr & 0x20)        // bit A5
        b |= (1 << 3);
    if (addr & 0x40)        // bit A6
        b |= (1 << 2);
    if (addr & 0x80)        // bit A7
        b |= (1 << 1);
    if (addr & 0x100)       // bit A8
        b |= (1 << 0);
    LATA = a;
    LATB = b;
}

/*
           ТИП 3
          HY53C464
       +------------+
 D2 --1|~OE      GND|18- D0
 D3 --2|D0        D3|17- D4
 D1 --3|D1      ~CAS|16- D5
 B7 --4|~WE       D2|15- D6
 A0 --5|~RAS      A0|14- D7
 A1 --6|A6        A1|13- B0
 A2 --7|A5        A2|12- B1
 A3 --8|A4        A3|11- B2
 A5 --9|+5V       A7|10- B3
       +------------+
*/
void t3_SetAddr(int addr)
{
    char a, b, d;

    a = LATA & 0b11010001;
    b = LATB & 0b11100000;
    d = LATD & 0b11100000;

    if (addr & 1)           // bit A0
        d |= (1 << 7);
    if (addr & 2)           // bit A1
        b |= (1 << 0);
    if (addr & 4)           // bit A2
        b |= (1 << 1);
    if (addr & 8)           // bit A3
        b |= (1 << 2);
    if (addr & 0x10)        // bit A4
        a |= (1 << 3);
    if (addr & 0x20)        // bit A5
        a |= (1 << 2);
    if (addr & 0x40)        // bit A6
        a |= (1 << 1);
    if (addr & 0x80)        // bit A7
        b |= (1 << 3);
    LATA = a;
    LATB = b;
    LATD = d;
}


/*
  чтение бита со скрытой регенерацией
*/
char dram_readbit(int row, int column)
{
    char b;
    if (ic_type == 0)
    {
        t0_CAS(1);
        t0_RAS(1);

        t0_SetAddr(row);
        t0_RAS(0);

        t0_WE(1);

        t0_SetAddr(column);
        t0_CAS(0);

//        asm ("nop");
        b = RD5;

        // цикл регенерации (ROW уже записан во внутреннем буфере м/с)
//        t0_RAS(1);
//        t0_RAS(0);

        t0_CAS(1);
        t0_RAS(1);

    } else if (ic_type == 1) {
        // РУ9
        t1_CAS(1);
        t1_RAS(1);

        t1_SetAddr(row);
        t1_RAS(0);

        t1_WE(1);

        t1_SetAddr(column);
        t1_CAS(0);

//        asm ("nop");
        b = RD4;

        t1_CAS(1);
        t1_RAS(1);
    } else if (ic_type == 2) {
        TRISD |= 0b00111100;
        // 51C4256
        t2_CAS(1);
        t2_RAS(1);
        t2_OE(1);

        t2_SetAddr(row);
        t2_RAS(0);

        t2_WE(1);

        t2_SetAddr(column);
        t2_CAS(0);

        t2_OE(0);

//        asm ("nop");
        b = (LATD & 0b00111100) >> 2;

        t2_CAS(1);
        t2_RAS(1);
    } else {
/*
           ТИП 3
          HY53C464
       +------------+
 D2 --1|~OE      GND|18- D0
 D3 --2|D0        D3|17- D4
 D1 --3|D1      ~CAS|16- D5
 B7 --4|~WE       D2|15- D6
 A0 --5|~RAS      A0|14- D7
 A1 --6|A6        A1|13- B0
 A2 --7|A5        A2|12- B1
 A3 --8|A4        A3|11- B2
 A5 --9|+5V       A7|10- B3
       +------------+
*/
        TRISD |= 0b01011010;
        // 53C464
        t3_CAS(1);
        t3_RAS(1);

        t3_SetAddr(row);
        t3_RAS(0);

        t3_WE(1);

        t3_SetAddr(column);
        t3_CAS(0);

        t3_OE(0);

        asm ("nop");
        b = RD3 | (RD1 << 1) | (RD6 << 2) | (RD4 << 3);

        t3_CAS(1);
        t3_RAS(1);
    }
    return b;
}



void dram_writebit(int row, int column, char val)
{
    char b;
    if (ic_type == 0)
    {
        t0_CAS(1);
        t0_RAS(1);

        t0_SetAddr(row);
        t0_RAS(0);
        t0_WE(0);

        t0_DI(val);

        t0_SetAddr(column);
        t0_CAS(0);

        asm ("nop");
        // цикл регенерации (ROW уже записан во внутреннем буфере м/с)
        t0_RAS(1);
        t0_WE(1);
        t0_RAS(0);


        t0_CAS(1);
        t0_RAS(1);
    } else if (ic_type == 1)
    {
        t1_WE(1);

        t1_CAS(1);
        t1_RAS(1);

        t1_SetAddr(row);
        t1_RAS(0);
        t1_WE(0);

        t1_DI(val);

        t1_SetAddr(column);
        t1_CAS(0);

        asm ("nop");
        // цикл регенерации (ROW уже записан во внутреннем буфере м/с)
        t1_RAS(1);
        t1_WE(1);
        t1_RAS(0);
        asm ("nop");

        t1_CAS(1);
        t1_RAS(1);
    } else if (ic_type == 2)
    {
        b = val << 2;
        TRISD &= 0b11000011;
        t2_CAS(1);
        t2_RAS(1);
        t2_WE(1);

        t2_SetAddr(row);
        t2_RAS(0);

        t2_OE(1);

        t2_SetAddr(column);
        t2_CAS(0);

        LATD = (LATD & 0b11000011) | b;

        t2_WE(0);

//        asm ("nop");
        // цикл регенерации (ROW уже записан во внутреннем буфере м/с)
        t2_RAS(1);
        t2_WE(1);
        t2_RAS(0);
//        asm ("nop");

        t2_CAS(1);
        t2_RAS(1);
    } else {
/*
           ТИП 3
          HY53C464
       +------------+
 D2 --1|~OE      GND|18- D0
 D3 --2|D0        D3|17- D4
 D1 --3|D1      ~CAS|16- D5
 B7 --4|~WE       D2|15- D6
 A0 --5|~RAS      A0|14- D7
 A1 --6|A6        A1|13- B0
 A2 --7|A5        A2|12- B1
 A3 --8|A4        A3|11- B2
 A5 --9|+5V       A7|10- B3
       +------------+
*/
        b =  (val & 1) << 3; val >> 1; // bit 0
        b |= (val & 1) << 1; val >> 1; // bit 1
        b |= (val & 1) << 6; val >> 1; // bit 2
        b |= (val & 1) << 4;
        TRISD &= 0b10100101;
        t3_CAS(1);
        t3_RAS(1);

        t3_OE(1);

        t3_SetAddr(row);
        t3_RAS(0);

        LATD = (LATD & 0b10100101) | b;

        t3_WE(0);

        t3_SetAddr(column);
        t3_CAS(0);

        asm ("nop");
        // цикл регенерации (ROW уже записан во внутреннем буфере м/с)
        t3_RAS(1);
        t3_WE(1);
        t3_RAS(0);

        t3_CAS(1);
        t3_RAS(1);
    }
}


/*****************************************************************************
                             ГРАФИЧЕСКАЯ ЧАСТЬ
*****************************************************************************/

unsigned char shift;        // размер сдвига для перевода в номер ячейки
unsigned char tbl_result[TABLE_NCELL*TABLE_NCELL];


void tbl_DrawCell(char row, char column)
{
    int x, y;

    y = row * CELL_SIZE + TABLE_Y;
    x = column * CELL_SIZE + TABLE_X;
    if (tbl_result[row*TABLE_NCELL+column])
        tft_BackGr(RED);
    else
        tft_BackGr(GREEN);
    tft_FillRect(x, y, CELL_SIZE-1, CELL_SIZE-1);
}


/*
  вывод ячеек памяти на экран
*/
void tbl_Show(char percent)
{
    char i, j;

    for (i = 0; i < TABLE_NCELL; i++)
        for (j = 0; j < TABLE_NCELL; j++)
            tbl_DrawCell(i, j);

    tft_BackGr(BLUE);
    tft_FillRect(3, 95, percent, 6);
}

void tbl_Fill(char n)
{
    int i;
    for (i = 0; i < TABLE_NCELL*TABLE_NCELL; i++)
        tbl_result[i] = n;
}

/*
  вычисляет сдвиг, для приведения row и column в координаты экрана
*/
unsigned char calck_shift(unsigned int num)
{
    unsigned char b = 0;

    num >>= TABLE_CELLSHIFT;        // num - количество бит на одну ячейку

    while (num)
    {
        b++;
        num >>= 1;
    }
    return --b;
}

// mask - маска заполнения
void dram_test(char mask)
{
    int i, j;
    char nbit;
    char b;

    // заполняем
    nbit = 0;
    for (i = 0; i < nrows; i++)
    {
        b = (mask >> nbit) & shift_mask;
        nbit = (nbit + shift_bits) & 0x07;
        for (j = 0; j < nrows; j++)
        {
            dram_writebit(j, i, b);
        }
    }
    // проверяем
    nbit = 0;
    for (i = 0; i < nrows; i++)
    {
        b = (mask >> nbit) & shift_mask;
        nbit = (nbit + shift_bits) & 0x07;
        for (j = 0; j < nrows; j++)
        {
            if (dram_readbit(j, i) != b)
                tbl_result[(i >> shift)*TABLE_NCELL+(j >> shift)] = 1;
        }
    }
}



char tst_TestDRAM(const char *name)
{
    char b;

    // инициализируем прерывания регенерации
    dram_Init();

    // вычисляем сдвиг, для приведения row и column в координаты экрана
    shift = calck_shift(nrows);

    // выводим имя микросхемы
    tft_ForeGr(BLACK);
    b = txt_GetStringLength(name) >> 1;
    txt_DrawString(64 - b, 1, name);
    // выводим размеры ячеек
    txt_DrawString(0, 104, "CELL:");
    b = txt_DrawString(5*8, 104, byte2dec(nrows >> TABLE_CELLSHIFT));
    txt_DrawString(b+4, 104, "BITS");
    // и легенду
    txt_DrawString(0, 116, "MEM:");
    tft_Color(BLACK, RED);
    txt_DrawString(5*8, 116, "POOR");
    tft_BackGr(GREEN);
    txt_DrawString(10*8, 116, "GOOD");

    // проводим тесты
    tft_Color(BLACK, BLACK);
    tft_FillRect(TABLE_X-1, TABLE_Y-1, CELL_SIZE*TABLE_NCELL+1, CELL_SIZE*TABLE_NCELL+1);
    tft_DrawRect(2, 94, 124, 8);
    tbl_Fill(0);
    tbl_Show(0);

    // собственно тесты
    dram_test(0b00000000);
    tbl_Show(21);
    dram_test(0b11111111);
    tbl_Show(42);
    dram_test(0b01010101);
    tbl_Show(63);
    dram_test(0b10101010);
    tbl_Show(84);
    dram_test(0b11110000);
    tbl_Show(105);
    dram_test(0b00001111);
    tbl_Show(122);

    return 0;
}
