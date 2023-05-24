#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TFT9163.H"
#include "TEXT.H"

#include "KEYB.H"
#include "MENU.H"



#ifndef NULL
#  if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#    define NULL    0
#  else
#    define NULL    0L
#  endif
#endif


#include "bin.h"
#include "8x12.h"

#include "tester.h"

#include "cmosdata.h"
#include "cmosmenu.h"
#include "ttldata.h"
#include "ttlmenu.h"
#include "74menu.h"
#include "40menu.h"
#include "58Xdata.h"
#include "58Xmenu.h"
#include "514data.h"
#include "514menu.h"
#include "dramdata.h"
#include "drammenu.h"


const char copyright[] = "IC TESTER V1.04";


const char *itemMain[] = {"ТТЛ", "КМОП", "514", "58X", "74XX", "4XXX", "DRAM"};
const MENU *labMain[] = { &mnuTTL, &mnuCMOS, &mnu514, &mnux58, &mnu74XX, &mnu4XXX, &mnuDRAM };

const MENU mnuMain = { itemMain, MENU_ROOT|MENU_SINGLECOL, 7, labMain };


void main(void)
{
    dbgmode = 0;
    tft_Begin();
    txt_SetFont(Fnt8x12, FONT_WIDTH, FONT_HEIGHT, 0x20, 0x7F);
    /*
      самая важная часть программы - вывод копирайта
    */
    tft_Color(YELLOW, BLUE);
    txt_DrawString( 0, 12, copyright);

    mnu_Run(&mnuMain);
    while (1)
    {
        tst_RunTest(ic_name, ic_data);
        tft_BackGr(RED);
        tft_FillRect(127, 0, 1, 1);
        if (key_Get() == KEY_ESC)
            break;
        mnu_Run(0);
    }
    tft_Done();
}
