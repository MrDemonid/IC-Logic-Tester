#include <htc.h>

__CONFIG (1, OSCSDIS & HS);
__CONFIG (2, BORDIS & PWRTDIS & WDTDIS);
__CONFIG (4, DEBUGDIS & LVPDIS & STVREN);
__CONFIG (5, UNPROTECT);
__CONFIG (7, SWDTDIS);

#pragma jis


#include "DELAY.H"
#include "SPI.H"
#include "TFT9163.H"
#include "TEXT.H"
#include "KEYB.H"
#include "MENU.H"
#include "TESTER.H"


#include "8x12.h"

#include "cmosdata.h"
#include "cmosmenu.h"
#include "ttldata.h"
#include "ttlmenu.h"
#include "74menu.h"
#include "40menu.h"
#include "514data.h"
#include "514menu.h"
#include "580data.h"
#include "580menu.h"
#include "dramdata.h"
#include "drammenu.h"


const char version[]   = "IC TESTER V1.04";
const char copyright[] = "ANDREY HLUS";


void Init()
{
    INTCON = 0;                 // ���������� ���������
    INTCON3 = 0;                // ����. INT1 � INT2 ���������
    PIE1 = 0;                   // ���������� parallel, ���, USART, MSSP, CCP1, TMR2, TMR1 ���������
    PIE2 = 0;                   // ���������� EEPROM, BCL, LVD, TMR3, CCP2 ���������
    IPEN = 0;                   // ������������ ������� ���������� ��������� (PCON.bit7)
    PSPMODE = 0;                // ������� ����������� �������� (TRISE.bit4)
    TMR0ON = 0;                 // ������ 0 ��������
    TMR1ON = 0;                 // ������ 1 ��������
    TMR2ON = 0;                 // ������ 2 ��������
    TMR3ON = 0;                 // ������ 3 ��������
    CCP1CON = 0;                // ���1 ��������
    CCP2CON = 0;                // ���2 ��������
    SPEN = 0;                   // USART ��������
    ADON = 0;                   // ������ ��� ��������
    ADCON1 = 0x07;              // ����� PORTA � PORTE ��������
    LVDEN = 0;                  // ������ LVD ��������
    tst_PowerOFF();             // ��������� ������ �����/������
}


const char *itemMain[] = {"���", "����", "514", "580", "74XX", "4XXX", "DRAM"};
const MENU *labMain[] = { &mnuTTL, &mnuCMOS, &mnu514, &mnu580, &mnu74XX, &mnu4XXX, &mnuDRAM };

const MENU mnuMain = { itemMain, MENU_ROOT|MENU_SINGLECOL, 7, labMain };


void main()
{
    Init();
    spi_Init(MASTER_OSC_DIV4, DATA_SAMPLE_MIDDLE, CLK_IDLE_LOW, LOW_2_HIGH);
    tft_Begin();

    txt_SetFont(Fnt8x12, FONT_WIDTH, FONT_HEIGHT, 0x20, 0x7F);

    /*
      ����� ������ ����� ��������� - ����� ���������
    */
    tft_Color(YELLOW, BLUE);
    txt_DrawString( 0, 0, &version);
    txt_DrawString( 0, 12, &copyright);
    DelayMs10(20);

    // ��������� ����������� ����� � ����� �������
    if (key_Enter())            // ����� "ENTER"?
    {
        tft_Color(WHITE, RED);
        txt_DrawString( (128-14*8)/2, 128/2 - 12/2, "DEBUG MODE: ON");
        while (key_Enter());
        dbgmode = 1;
    } else {
        dbgmode = 0;
    }

    // �������� ����
    mnu_Run(&mnuMain);
    while(1)
    {
        tst_RunTest(ic_name, ic_data);
        mnu_Run(0);
    }
}
