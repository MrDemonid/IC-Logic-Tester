#include <htc.h>

#include "tft9163.h"
#include "text.h"



//------------------------------
//        DATA
//------------------------------

static const char *curfont;            // текущий фонт
static unsigned char firstchar;        // первый символ
static unsigned char lastchar;         // последний символ

unsigned char fwidth;                  // ширина символа фонта
unsigned char fheight;                 // высота символов фонта



void txt_SetFont(const char *fnt, unsigned char w, unsigned char h, unsigned char first, int numchars)
{
    curfont = fnt;
    fwidth = w;
    fheight = h;
    firstchar = first;
    lastchar = firstchar + numchars - 1;
}


void txt_DrawChar(int x, int y, unsigned char c)
{
    const char *p;
    unsigned char i, j;

    if (c >= 0xC0)
        c -= 0x60;
    if (c < firstchar || c > lastchar)
    {
        c = '.';
    }
    p = curfont + (c - firstchar)*fheight;
    tft_SetAddr(x, y, x+fwidth-1, y+fheight-1);
    for (j = 0; j < fheight; j++)
    {
        c = *p++;
        for (i = 0; i < fwidth; i++)
        {
            if (c & 0x80)
            {
                tft_SendData16(_foregr);
            } else {
                tft_SendData16(_backgr);
            }
            c <<= 1;
        }
    }
}



char txt_DrawString(int x, int y, const char *str)
{
    const char *p = str;
    while (*p)
    {
        txt_DrawChar(x, y, *p);
        p++;
        x += fwidth;
    }
    return x;
}


int txt_GetStringLength(const char *str)
{
    int len;

    const char *p;
    len = 0;
    p = str;
    while (*p++)
    {
        len += fwidth;
        if (len > 256*8)        // небольшая защита
            break;
    }
    return len;
}
/*
int txt_GetFontHeight(void)
{
    return fheight;
}

int txt_GetFontWidth(void)
{
    return fwidth;
}

*/
