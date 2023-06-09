#include "BIN.H"
#include "TESTER.H"
#include "TFT9163.H"
#include "TEXT.H"
#include "UTIL.H"
#include "DRAM.H"
#include "KEYB.H"

/*         ��5, ��6, ��7                          ��9
          +------------+                     +------------+
    D2 --1|A8(��7)  GND|16- D0         D2 --1|DI       GND|18- D0
    D3 --2|DI      ~CAS|15- D4         D3 --2|~WE       DO|17- D4
    D1 --3|~WE       DO|14- D5         D1 --3|~RAS    ~CAS|16- D5
    B7 --4|~RAS      A6|13- D6         B7 --4|T.F.      A9|15- D6
    A0 --5|A0        A3|12- D7         A0 --5|A0        A8|14- D7
    A1 --6|A2        A4|11- B0         A1 --6|A1        A7|13- B0
    A2 --7|A1        A5|10- B1         A2 --7|A2        A6|12- B1
    A3 --8|+5V       A7| 9- B2         A3 --8|A3        A5|11- B2
                                       A5 --9|+5V       A4|10- B3
    A5 --9|            |--- B3
    E0 -10|            |--- B4         E0 -10|            |--- B4
    E1 -11|            |--- B5         E1 -11|            |--- B5
   GND -12|            |--- B6        GND -12|            |--- B6
          +------------+                     +------------+
*/

#define t0_RAS(n)            if (n) { LATB |= (1 << 7); } else { LATB &= ~(1 << 7); }
#define t0_CAS(n)            if (n) { LATD |= (1 << 4); } else { LATD &= ~(1 << 4); }
#define t0_WE(n)             if (n) { LATD |= (1 << 1); } else { LATD &= ~(1 << 1); }
#define t0_DI(n)             if (n) { LATD |= (1 << 3); } else { LATD &= ~(1 << 3); }

#define t1_RAS(n)            if (n) { LATD |= (1 << 1); } else { LATD &= ~(1 << 1); }
#define t1_CAS(n)            if (n) { LATD |= (1 << 5); } else { LATD &= ~(1 << 5); }
#define t1_WE(n)             if (n) { LATD |= (1 << 3); } else { LATD &= ~(1 << 3); }
#define t1_DI(n)             if (n) { LATD |= (1 << 2); } else { LATD &= ~(1 << 2); }



#define TABLE_NCELL     16  // ���������� ������������ ����� ������ �� ����������� (������� �� �� ���������)
#define TABLE_CELLSHIFT 4   // ����� ������ ������� �� TABLE_NCELL

#define TABLE_X         2
#define TABLE_Y         13

#define CELL_SIZE       5   // ������ ������ � ��������


void t0_SetAddr(int addr);
char dram_readbit(int row, int column);
void dram_writebit(int row, int column, char value);


unsigned char ic_type;          // ���: 0 - ���� ��5-��7; 1 - ���� 411000
unsigned int  nrows;            // rows number: 0x80 - ��6, 0x100 - ��5, 0x200 - ��7



/*
  ������������� � ��������� ������� ����������
*/
void dram_Init(void)
{
    unsigned char i;

  RA3 = 0;
    // ������ ������ � �/�
    ic_type = *ic_test++;
    i = *ic_test;
    nrows   = 1U << i;

  // �������� �������
  LATA |= 8;
//  DelayMs10(1);
    // 16 ������ ������
    for (i = 0; i < 16; i++)
        dram_readbit(0, 0);
}





/*
  ��������� ������ �� �������� ������ ��� ��5, ��6 � ��7
*/
void t0_SetAddr(int addr)
{
    char a, b, d;

    a = LATA & b11111000;
    b = LATB & b11111000;
    d = LATD & b00111011;
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
  ��������� ������ �� �������� ������ ��� ��9
*/
void t1_SetAddr(int addr)
{
    char a, b, d;

    a = LATA & b11110000;
    b = LATB & b11110000;
    d = LATD & b00111111;

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


/*
  ������ ���� �� ������� ������������
*/

char dram_readbit(int row, int column)
{
    char b;
    if (ic_type == 0)
    {
        PORTA = row;
        b = 1;
    } else {
        PORTA = column;
        b = row;
    }
    return b;
}


void dram_writebit(int row, int column, char val)
{
    if (ic_type == 0)
    {
        PORTA = row;
    } else {
        PORTA = column;
        PORTB = val;
    }
}



/*****************************************************************************
                             ����������� �����
*****************************************************************************/

unsigned char shift;        // ������ ������ ��� �������� � ����� ������
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
  ����� ����� ������ �� �����
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
  ��������� �����, ��� ���������� row � column � ���������� ������
*/
unsigned char calck_shift(unsigned int num)
{
    unsigned char b = 0;

    num >>= TABLE_CELLSHIFT;        // num - ���������� ��� �� ���� ������

    while (num)
    {
        b++;
        num >>= 1;
    }
    return --b;
}

// mask - ����� ����������
void dram_test(char mask)
{
    int i, j;
    char nbit;
    char b;

    // ���������
    nbit = 0;
    for (i = 0; i < nrows; i++)
    {
        b = (mask >> nbit) & 1;
        nbit = (nbit + 1) & 0x07;
        for (j = 0; j < nrows; j++)
        {
            dram_writebit(j, i, b);
        }
    }
    // ���������
    nbit = 0;
    for (i = 0; i < nrows; i++)
    {
        b = (mask >> nbit) & 1;
        nbit = (nbit + 1) & 0x07;
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

    // �������������� ���������� �����������
    dram_Init();

    // ��������� �����, ��� ���������� row � column � ���������� ������
    shift = calck_shift(nrows);

    // ������� ��� ����������
    tft_ForeGr(BLACK);
    b = txt_GetStringLength(name) >> 1;
    txt_DrawString(64 - b, 1, name);
    // ������� ������� �����
    txt_DrawString(0, 104, "CELL:");
    b = txt_DrawString(5*8, 104, byte2dec(nrows >> TABLE_CELLSHIFT));
    txt_DrawString(b+4, 104, "BITS");
    // � �������
    txt_DrawString(0, 116, "MEM:");
    tft_Color(BLACK, RED);
    txt_DrawString(5*8, 116, "POOR");
    tft_BackGr(GREEN);
    txt_DrawString(10*8, 116, "GOOD");

    // �������� �����
    tft_Color(BLACK, BLACK);
    tft_FillRect(TABLE_X-1, TABLE_Y-1, CELL_SIZE*TABLE_NCELL+1, CELL_SIZE*TABLE_NCELL+1);
    tft_DrawRect(2, 94, 124, 8);
    tbl_Fill(0);
    tbl_Show(0);

    // ���������� �����
    dram_test(b00000000);
    tbl_Show(21);
    dram_test(b11111111);
    tbl_Show(42);
    dram_test(b01010101);
    tbl_Show(63);
    dram_test(b10101010);
    tbl_Show(84);
    dram_test(b11110000);
    tbl_Show(105);
    dram_test(b00001111);
    tbl_Show(122);

    return 0;
}
