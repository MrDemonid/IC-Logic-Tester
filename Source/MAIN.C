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
    INTCON = 0;                 // прерывания выключены
    INTCON3 = 0;                // прер. INT1 и INT2 запрещены
    PIE1 = 0;                   // прерывания parallel, ЦАП, USART, MSSP, CCP1, TMR2, TMR1 запрещены
    PIE2 = 0;                   // прерывания EEPROM, BCL, LVD, TMR3, CCP2 запрещены
    IPEN = 0;                   // приоритетная система прерываний запрещена (PCON.bit7)
    PSPMODE = 0;                // ведомый паралельный отключен (TRISE.bit4)
    TMR0ON = 0;                 // таймер 0 выключен
    TMR1ON = 0;                 // таймер 1 выключен
    TMR2ON = 0;                 // таймер 2 выключен
    TMR3ON = 0;                 // таймер 3 выключен
    CCP1CON = 0;                // ШИМ1 выключен
    CCP2CON = 0;                // ШИМ2 выключен
    SPEN = 0;                   // USART выключен
    ADON = 0;                   // модуль АЦП выключен
    ADCON1 = 0x07;              // линии PORTA и PORTE цифровые
    LVDEN = 0;                  // модуль LVD выключен
    tst_PowerOFF();             // настройка портов ввода/вывода
}


const char *itemMain[] = {"ТТЛ", "КМОП", "514", "580", "74XX", "4XXX", "DRAM"};
const MENU *labMain[] = { &mnuTTL, &mnuCMOS, &mnu514, &mnu580, &mnu74XX, &mnu4XXX, &mnuDRAM };

const MENU mnuMain = { itemMain, MENU_ROOT|MENU_SINGLECOL, 7, labMain };


void main()
{
    Init();
    spi_Init(MASTER_OSC_DIV4, DATA_SAMPLE_MIDDLE, CLK_IDLE_LOW, LOW_2_HIGH);
    tft_Begin();

    txt_SetFont(Fnt8x12, FONT_WIDTH, FONT_HEIGHT, 0x20, 0x7F);

    /*
      самая важная часть программы - вывод копирайта
    */
    tft_Color(YELLOW, BLUE);
    txt_DrawString( 0, 0, &version);
    txt_DrawString( 0, 12, &copyright);
    DelayMs10(20);

    // проверяем потребность входа в режим отладки
    if (key_Enter())            // нажат "ENTER"?
    {
        tft_Color(WHITE, RED);
        txt_DrawString( (128-14*8)/2, 128/2 - 12/2, "DEBUG MODE: ON");
        while (key_Enter());
        dbgmode = 1;
    } else {
        dbgmode = 0;
    }

    // основной цикл
    mnu_Run(&mnuMain);
    while(1)
    {
        tst_RunTest(ic_name, ic_data);
        mnu_Run(0);
    }
}
