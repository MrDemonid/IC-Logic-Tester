#include <htc.h>

#include "tft9163.h"
#include "text.h"
#include "keyb.h"
#include "util.h"

#include "tester.h"
#include "dram.h"

#include "img.h"
#include "delay.h"


#pragma jis


/*
         +------+
    D2 --|1 \/  |-- D0
    D3 --|2     |-- D4
    D1 --|3     |-- D5
    B7 --|4     |-- D6
    A0 --|5     |-- D7
    A1 --|6     |-- B0
    A2 --|7     |-- B1
    A3 --|8     |-- B2
    A5 --|9     |-- B3
    E0 --|10    |-- B4
    E1 --|11    |-- B5
   GND --|12    |-- B6
         +------+

C0 - PwrL
C1 - PwrR
C2 - Lcd DC
C3 - SPI_SCK
C4 - SPI_SDI
C5 - SPI_SDO
C6 - PullUp
C7 - --
           LOGIC |33222222|22221111|111111  |
            BITs |10987654|32109876|54321098|76543210
           ------+--------+--------+--------+--------
           BYTE: |   +3   |   +2   |   +1   |   +0
           ------+--------+--------+--------+--------
           PORT: |    E   |    D   |    B   |    A
*/

#define PULLUPREG           RC6         // ������� ������� ������������� ����������
#define PULLUP(n)           RC6 = n     // �(�)�������� ������������� ����������
#define POWERM(n)           RC0 = n     // �(�)�������� ������� (������� ����� ���)
#define POWERR(n)           RC1 = n     // �(�)�������� ������� (������� ������ ���)

/*
  �����
*/
#define COL_BACKGROUND      CYAN        // ���� ���� ������ ������������
#define COL_IC              BLACK
#define COL_PIN_0           BLUE
#define COL_PIN_1           RED
#define COL_PIN_NO          BLACK
#define COL_GND             BLACK
#define COL_PLUS            MAGENTA


// ��������� � (��)�������� ������������
static const char *szError[] = { "������","���:   ","" };
static const char *szGood[] = { "����","�������!","" };


// ������� ������������ ����������� � ����������� �������� ����� ����������� �/�
static const char pin_map24[24] = {18,19,17,15, 0, 1, 2, 3, 5,24,25,31,14,13,12,11,10, 9, 8,23,22,21,20,16 };
char pin_map[24];


bit             dbgmode;    // ���� ��������� ���������� ������
bit             isDRAM;
bit             isSRAM;
char            isOC;       // ���� ������� ������� � �������� �����������

char const     *ic_test;    // ��������� �� ������ �����
unsigned char   pins;       // ���������� ����� ������� �/�
LONG            outmap;     // ����� ������� �/� (������ ��)
LONG            inmap;      // ����� ������ �/� (������� ��)
unsigned char   pin_gnd;    // ����� ����� ����� (��������� �� ����)
unsigned char   pin_plus;   // ����� ����� ������� (��������� �� ����)
LONG            indata;     // ������ ��� ������ �/� (������� ��) � ���������� �������
LONG            outdata;    // ������ � ������� �/� (������ ��) � ���������� �������
char            pin_pulse;  // ����� ���������� ��������
char            sgn_pulse;  // ��� �������� (���. ��� ���.)

LONG            errpins;    // ������������� � ��������� ����������� ����



/*
  ���������� ����� �� ������ ����
*/
unsigned long BITMASK(char b)
{
    return (1UL << b);
}


/*
  ����������� �� ��������� ���� � ����������
*/
unsigned long MAP(unsigned long src)
{
    char i;
    unsigned long temp = 0;

    for (i = 0; i < pins; i++)
    {
        if (src & BITMASK(i))
            temp |= BITMASK(pin_map[i]);
    }
    return temp;
}



/*
  ������ ���� ���� �� ������� ���������� �����
*/
unsigned char read_byte(void)
{
    return *ic_test++;
}

/*
  ������ ����� ����� �� ������� ���������� �����
  �� �����:
    pins     - ���������� ����� �/c
  �� ������: - ��������� �����

*/
unsigned long read_mask(void)
{
    LONG m;
    char i = 0;
    char j = 0;
    m.l = 0;
    while (i < pins)
    {
        m.CHARs[j++] = read_byte();
        i += 8;
    }
    return m.l;
}

/*
  ������ �������������� �������� ������ �� ����� �/�
  �� �����:
    indata - ��������� ������ �/�
*/
void put_data(void)
{
    LONG temp;

    indata.l &= inmap.l;
    temp.l = MAP( (indata.l | BITMASK(pin_plus) ) & (~BITMASK(pin_gnd)) );
    LATA = temp.BYTEs.b0;
    LATB = temp.BYTEs.b1;
    LATD = temp.BYTEs.b2;
    LATE = temp.BYTEs.b3;
}

/*
  ������ ������ � ������� �/�
  �� ������:
    outdata - ������� ��������� ������� �/� � �������� �������
*/
void get_data(void)
{
    LONG temp;
    char i;

    temp.BYTEs.b0 = PORTA;
    temp.BYTEs.b1 = PORTB;
    temp.BYTEs.b2 = PORTD;
    temp.BYTEs.b3 = PORTE;
    outdata.l = 0;
    // ����������� � �������� ���
    for (i = 0; i < pins; i++)
    {
        if (temp.l & BITMASK(pin_map[i]))
            outdata.l |= BITMASK(i);
    }
    outdata.l &= outmap.l;              // ������� ����� �� ������
}


/*
  ������ � ��������� ����� ������/�������
*/
void tst_ReadPinMask(void)
{
    // ������ ����� ������/�������
    inmap.l  = read_mask();
    outmap.l = ~inmap.l;
    // ��������� ����� �������
    outmap.l &= ~BITMASK(pin_gnd);
    outmap.l &= ~BITMASK(pin_plus);
    inmap.l  &= ~BITMASK(pin_gnd);
    inmap.l  &= ~BITMASK(pin_plus);
}


/*
  ��������� ������� ����������
*/
void tst_PowerON(void)                  // ��������� ������� ����������
{
    LONG tr;

    RBPU = 1;                           // ��������� ������������� ��������� PORTB
    PULLUP(0);                          // ��������� ������� ������������� ���������
    outdata.l = 0;
    indata.l = 0;
    tr.l = MAP(outmap.l);
    tr.l &= ~BITMASK(pin_map[pin_gnd]); // ����� �� �����
    if ((pin_plus == 4-1) || (pin_gnd >= 16-1))
    {
        tr.l &= ~BITMASK(pin_map[pin_plus]);// ���� �� �����
    } else {
        tr.l |= BITMASK(pin_map[pin_plus]); // ���� �� ����
        if (pin_plus == 5-1)
            POWERM(1);
        else
            POWERR(1);
    }

    // ����������� �������� TRIS
    TRISC = 0x80;
    TRISA = tr.BYTEs.b0 | 0x10;         // RA4 - �� ����
    TRISB = tr.BYTEs.b1;
    TRISD = tr.BYTEs.b2;
    TRISE = (tr.BYTEs.b3 & 0x07) | 0x4; // RE2 - �� ���� + ��������� ��������� ������������� �����
    put_data();
    // �������� ������������� ���������, ���� ������ � ��
    if (isOC)
        PULLUP(1);
}

/*
  ���������� ������� ����������
*/
void tst_PowerOFF(void)         // ���������� ������� ����������
{
    TRISC = 0x80;               // ���. �������� �� �����
    PORTC = 0;                  // ����. ������� � �������� ���� �����
    TRISA = 0xFF;               // ��������� �� ����
    TRISB = 0xFF;
    TRISD = 0xFF;
    TRISE = 0x07;               // ��������� ��������� ������������� �����
    RBPU = 0;                   // �������� ������������� ��������� PORTB
}


/*****************************************************************************
******************************************************************************
**************************** ������� ������������ ****************************
******************************************************************************
*****************************************************************************/

/*
  ������������� (CMD_INIT)
*/
char tst_Init(char const *data)
{
    unsigned char lo, hi, hs;
    unsigned char diptype;            // ��� ����������

    isDRAM = 0;
    isSRAM = 0;
    tst_PowerOFF();
    // ������ ��� �������
    ic_test = data;
    isOC = *ic_test & FL_OC;
    if (*ic_test & FL_DRAM)
        isDRAM = 1;
    diptype = *ic_test++ & FL_MASKDIPTYPE;
    switch (diptype)
    {
        case CMD_INIT_8:    {pins = 8; pin_gnd = 4-1; pin_plus = 8-1; break; } // +8  -4
        case CMD_INIT_14:   {pins = 14; pin_gnd = 7-1; pin_plus = 14-1; break; } // +14 -7
        case CMD_INIT_14_1: {pins = 14; pin_gnd = 6-1; pin_plus = 14-1; break; } // +14 -6
        case CMD_INIT_14_2: {pins = 14; pin_gnd = 10-1; pin_plus = 5-1; break; } // +5  -10
        case CMD_INIT_14_3: {pins = 14; pin_gnd = 11-1; pin_plus = 4-1; break; } // +4  -11
        case CMD_INIT_16:   {pins = 16; pin_gnd = 8-1; pin_plus = 16-1; break; } // +16 - 8
        case CMD_INIT_16_1: {pins = 16; pin_gnd = 7-1; pin_plus = 16-1; break; } // +16 -7
        case CMD_INIT_16_2: {pins = 16; pin_gnd = 12-1; pin_plus = 5-1; break; } // +5  -12
        case CMD_INIT_16_3: {pins = 16; pin_gnd = 16-1; pin_plus = 8-1; break; } // +8  -16
        case CMD_INIT_18_1: {pins = 18; pin_gnd = 18-1; pin_plus = 9-1; break; } // +9  -18
        case CMD_INIT_20:   {pins = 20; pin_gnd = 10-1; pin_plus = 20-1; break; } // +20 -10
        case CMD_INIT_20_1: {pins = 20; pin_gnd = 8-1; pin_plus = 20-1; break; } // +20 -8
        case CMD_INIT_20_2: {pins = 20; pin_gnd = 20-1; pin_plus = 10-1; break; } // +10 -20
        case CMD_INIT_24:   {pins = 24; pin_gnd = 12-1; pin_plus = 24-1; break; } // +24 -12
        default: return 0;
    }
    lo = 0;
    hi = pins-1;
    hs = 23;
    while (lo < (pins >> 1))
    {
        pin_map[lo] = pin_map24[lo];
        pin_map[hi] = pin_map24[hs];
        lo++;
        hi--;
        hs--;
    }
    // ������ ����� ������/�������
    tst_ReadPinMask();
    return 1;
}

void tst_Config(void)
{
    // �����
    tst_PowerOFF();
    // ������ ����� ������/�������
    tst_ReadPinMask();
    tst_PowerON();
}

/*
  CMD_SET_TO_0
*/
void tst_SetTo0(void)
{
    unsigned char pin;

    pin = read_byte();
    indata.l &= ~BITMASK(pin-1);
    put_data();
}

/*
  CMD_SET_TO_1
*/
void tst_SetTo1(void)
{
    unsigned char pin;

    pin = read_byte();
    indata.l |= BITMASK(pin-1);
    put_data();
}

/*
  CMD_SET
*/
void tst_Set(void)
{
    indata.l &= ~(read_mask());     // ���������� ���� � "0"
    indata.l |= read_mask();        // ������������� ���� � "1"
    put_data();
}

/*
  CMD_SET_ALL
*/
void tst_SetAll(void)
{
    indata.l = read_mask();
    put_data();
}

/*
  CMD_TEST
*/
char tst_Test(void)
{
    LONG mask0, mask1;

    get_data();                     // �������� ������� ��������� ������� �/�
    mask0.l = read_mask();           // �������� ����� ������� ���
    errpins.l = outdata.l & mask0.l;
    if (errpins.l)
        return 0;                   // ������ - ����������� ���� �� �������
    mask1.l = read_mask();           // �������� ����� ��������� ���
    errpins.l = (outdata.l & mask1.l) ^ mask1.l;
    if (errpins.l)
        return 0;                   // ������ - ����������� ���� �� ���������
    return 1;
}

/*
  CMD_TEST_ALL
*/
char tst_TestAll(void)
{
    LONG mask;
    mask.l = read_mask();
    get_data();
    if (outdata.l != mask.l)
    {
        errpins.l = outdata.l ^ mask.l;
        return 0;
    }
    return 1;
}

/*
  CMD_SET_ALL_AND_TEST
*/
char tst_SetAllAndTest(void)
{
    tst_SetAll();
    return tst_TestAll();
}

/*
  CMD_LAST_PULSE
*/
void tst_LastPulse(void)
{
    if (sgn_pulse)
    {
        // ������������� �������
        indata.l |= BITMASK(pin_pulse-1);
        put_data();
        indata.l &= ~BITMASK(pin_pulse-1);
    } else {
        // ������������� �������
        indata.l &= ~BITMASK(pin_pulse-1);
        put_data();
        indata.l |= BITMASK(pin_pulse-1);
    }
//    __delay_us(1);
    asm ("nop");
    put_data();
}


/*
  CMD_PULSE
*/
void tst_Pulse(void)
{
    pin_pulse = read_byte();
    sgn_pulse = pin_pulse & 0x80;
    if (sgn_pulse)
        pin_pulse = -pin_pulse;
    tst_LastPulse();
}

/*
  CMD_LAST_PULSE_AND_TEST
*/
char tst_LastPulseAndTest(void)
{
    tst_LastPulse();
    return tst_TestAll();
}

/*
  CMD_LAST_REPEAT_PULSE
*/
void tst_RepeatPulse(void)
{
    char cnt;

    cnt = read_byte();
    while (cnt--)
    {
        tst_LastPulse();
    }
}






/*
  ��������� ������ ������ ����������:
    pin_no - ����� ������ [1..pins]
    color  - ����
*/
void draw_Pin(char pin_no, uint16 color)
{
    const unsigned char *img_no = &imgPinNo[(pin_no-1)*5];
    const unsigned char *img_pin;
    char is_inp;
    int y;
    char x1 = 10-7;
    char x2 = 10+3;

    is_inp = (inmap.l & BITMASK(pin_no-1)) > 0 ? 1 : 0;

    if (pin_no > (pins >> 1))
    {
        is_inp ^= 1;
        x1 += 7+36;
        x2 += 36-8-2-3;
        y = 5 + (pins-pin_no)*10;
    } else {
        y = 5 + (pin_no-1)*10;
    }

    if (pin_no == pin_gnd+1)
    {
        img_pin = &imgGND[0];
        color = COL_GND;
    } else if (pin_no == pin_plus+1)
    {
        img_pin = &imgPlus[0];
        color = COL_PLUS;
    } else {
        if (is_inp)
            img_pin = &imgLPin[0];
        else
            img_pin = &imgRPin[0];
    }
    gfx_BitmapNoTransparent(x1, y, img_pin, 8, 7, color);
    gfx_BitmapNoTransparent(x2, y+1, img_no, 8, 5, COL_PIN_NO);
}


void tst_ShowIC(char redraw)
{
    int i;
    unsigned long m = 1;
    unsigned int col;

    tft_Color(COL_IC, COL_BACKGROUND);
    if (redraw)
    {
        tft_FillRect(0, 0, 7*8-1, 128);
    }
    if (PULLUPREG)
    {
        tft_Color(YELLOW, RED);
        txt_DrawString(22, 4, "%");
        tft_Color(COL_IC, COL_BACKGROUND);
    } else {
        tft_FillRect(22, 4, 8, 12);
    }
    for (i = 1; i <= pins; i++)
    {
        col = COL_PIN_0;
        if (inmap.l & m)
        {
            // ��� ���� �/�
            if (indata.l & m)
            {
                col = COL_PIN_1;
            }
        } else {
            // ��� ����� �/�
            if (outdata.l & m)
            {
                col = COL_PIN_1;
            }
        }
        draw_Pin(i, col);

        m <<= 1;
    }
    tft_DrawRect(10, 2, 36, (pins>>1)*10+3);
}


/*
  ����� ������� �����
*/
char msg_Print(char y, const char **msg)
{
    const char *txt;
    char len, i = 0;

    while (i < 4)
    {
        txt = msg[i];
        if (*txt == '\0')
            break;
        len = txt_GetStringLength(txt) >> 1;
        len = txt_DrawString(92 - len, y, txt);
        y += 12;
        i ++;
    }
    return len;
}



void err_Show(unsigned char line)
{
    char i;
    int x, y;

    tft_Color(YELLOW, RED);
    tft_FillRect(7*8+2, 64-12-2, 68, 12*2+4);
    x = msg_Print(64-12, szError);
    txt_DrawString(x-16, 64-12+12, byte2dec(line));
    // �������� ����������� ����
    tft_ForeGr(RED);
    for (i = 0; i < pins; i++)
    {
        if (errpins.l & BITMASK(i))
        {
            x = 10-7-1-1;
            if (i >= (pins >> 1))
            {
                x += 7+36-1-8-1;
                y = 4 + (pins-i-1)*10;
            } else {
                y = 5 + i*10 - 1;
            }
            tft_DrawRect(x, y, 8+2+8+3, 8+1);
            }
    }
}

void msg_Done(void)
{
    tft_Color(YELLOW, BLUE);
    tft_FillRect(7*8+2, 64-12-2, 68, 12*2+4);
    msg_Print(64-12, szGood);
}

char tst_TestLogic(const char *name)
{
    char res;
    char cmd;
    char line = 0;

    tst_ShowIC(1);
    // ������� ���������� � ����������� �/�
    tft_Color(BLACK, COL_BACKGROUND);
    res = txt_GetStringLength(name) >> 1;
    if (isOC)
        res += 1*8;
    res = txt_DrawString(92 - res, 2, name);
    if (isOC)
    {
        tft_ForeGr(RED);
        txt_DrawString(res, 2, " /");
    }
    tft_ForeGr(BLUE);
    txt_DrawString(92 - 3*8, 128/2-6, "����...");

    while (*ic_test != CMD_END)
    {
        cmd = *ic_test;
        res = 1;
        switch (read_byte())
        {
            case CMD_SET_TO_0:           { tst_SetTo0(); break; }
            case CMD_SET_TO_1:           { tst_SetTo1(); break; }
            case CMD_SET:                { tst_Set(); break; }
            case CMD_SET_ALL:            { tst_SetAll(); break; }
            case CMD_TEST:               { res = tst_Test(); break; }
            case CMD_TEST_ALL:           { res = tst_TestAll(); break; }
            case CMD_SET_ALL_AND_TEST:   { res = tst_SetAllAndTest(); break; }
            case CMD_LAST_PULSE:         { tst_LastPulse(); break; }
            case CMD_PULSE:              { tst_Pulse(); break; }
            case CMD_LAST_PULSE_AND_TEST:{ res = tst_LastPulseAndTest(); break; }
            case CMD_LAST_REPEAT_PULSE:  { tst_RepeatPulse(); break; }
            case CMD_PULLUP:             { PULLUP(1); break; }
            case CMD_PULLDN:             { PULLUP(0); break; }
            case CMD_CONFIG:             { tst_Config(); break; }
            default: {
                // ������ ������� ��� ������������ ������ ������
                return 0;
            }
        }
        line++;
        tst_ShowIC(cmd == CMD_CONFIG);
        if (dbgmode)
        {
            // ��������� ����� �������
            if (!res)
                err_Show(line);     // �� ������� �/� ������
            if (key_GetEnter())
            {
                return 0;
            }
            tst_ShowIC(1);
        } else {
            // ������� ����� ������������
            if (!res)
            {
                err_Show(line);     // �� ������� �/� ������
                key_GetEnter();
                return 0;
            }
        }
    } // while
    return 1;
}


/*
  ���� ����������
*/
void tst_RunTest(const char *name, const char *data)
{
    char res;

    tft_BackGr(COL_BACKGROUND);
    tft_FillScreen();

    if (tst_Init(data))
    {
        tst_PowerON();

        if (isDRAM)
        {
            res = tst_TestDRAM(name);
        }
        else
        {
            res = tst_TestLogic(name);
        }
    }
    tst_PowerOFF();
    if (res)
        msg_Done();
    key_Get();
}
