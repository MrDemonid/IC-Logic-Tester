#include "tft9163.h"


#define tft_BoundaryCheck(x, y)  ((int)x >= _width) || ((int)y >= _height)


int           _width;       // Display w/h as modified by current rotation
int           _height;


unsigned char _foregr;      // цвет текста/линий
unsigned char _backgr;      // и фона/закраски


int left, top;                 // координаты активной области
int right, bottom;
int curx, cury;                // тек. координаты в акт. области



void tft_SendData16(uint8 w)
{
    unsigned int offs;

    if (curx >= left && curx <= right && cury >= top && cury <= bottom)
    {
        offs = cury*320+curx;
        asm {
            mov     ax, 0xA000
            mov     es, ax
            mov     di, offs
            mov     al, w
            mov     es:[di], al
        }
        curx++;
        if (curx > right)
        {
            curx = left;
            cury++;
        }
    }
}


void tft_Begin(void)
{
    asm {
        mov     ax, 0x13
        int     0x10
    }
    _width = _GRAMWIDTH;                      // ширина и высота экрана
    _height = _GRAMHEIGH;
    curx = 0;
    cury = 0;

    tft_Color(WHITE, BLACK);
    tft_FillScreen();
}

void tft_Done(void)
{
    asm {
        mov     ax, 0x03
        int     0x10
    }
}


void tft_FillScreen(void)
{
    int px;

    tft_SetAddr(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH);    //go home
    for (px = 0; px < _GRAMSIZE; px++)
    {
        tft_SendData16(_backgr);
    }
}

void tft_DrawPixel(int x, int y, uint8 color)
{
    if ((x < 0) || (y < 0) || tft_BoundaryCheck(x, y))
        return;
    tft_SetAddr(x, y, x+1, y+1);
    tft_SendData16(color);
}


void tft_DrawFastVLine(int x, int y, int h)
{
    // Rudimentary clipping
/*
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    if ((x < 0) || (h <= 0) || tft_BoundaryCheck(x, y))
*/
    if (tft_BoundaryCheck(x, y))
        return;

    if (((y + h) - 1) >= _height)
        h = _height-y;
    tft_SetAddr(x, y, x, (y+h)-1);
    while (h-- > 0)
    {
        tft_SendData16(_foregr);
    }
}

/*
bool tft_BoundaryCheck(int x, int y)
{
    if ((x >= _width) || (y >= _height))
        return -1;
    return 0;
}
*/

void tft_DrawFastHLine(int x, int y, int w)
{
    // Rudimentary clipping
/*
    if (x < 0)
    {
        w += x;
        x = 0;
    }
    if ((y < 0) || (w <= 0) || tft_BoundaryCheck(x, y))
*/
    if (tft_BoundaryCheck(x, y))
        return;
    if (((x+w) - 1) >= _width)
        w = _width-x;
    tft_SetAddr(x, y, (x+w)-1, y);
    while (w-- > 0)
    {
        tft_SendData16(_foregr);
    }
}

// fill a rectangle
void tft_FillRect(int x, int y, int w, int h)
{
/*    if (x < 0)
    {
        w += x;
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    if ((w <= 0) || (h <= 0) || tft_BoundaryCheck(x, y))
*/
    if (tft_BoundaryCheck(x, y))
        return;
    if (((x + w) - 1) >= _width)
        w = _width  - x;
    if (((y + h) - 1) >= _height)
        h = _height - y;
    tft_SetAddr(x, y, x+w-1, y+h-1);
    while (h-- > 0)
    {
        for (x = 0; x < w; x++)
            tft_SendData16(_backgr);
    }
}

void tft_DrawRect(int x, int y, int w, int h)
{
    tft_DrawFastHLine(x, y, w);
    tft_DrawFastVLine(x, y, h);
    tft_DrawFastHLine(x, y+h-1, w);
    tft_DrawFastVLine(x+w-1, y, h);
}


void tft_SetAddr(int x0, int y0, int x1, int y1)
{
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= _width) x1 = _width-1;
    if (y1 >= _height) y1 = _height-1;
    if (x1 < x0 || y1 < y0)
        return;
    left = x0;
    top = y0;
    right = x1;
    bottom = y1;
    curx = left;
    cury = top;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// GFX ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gfx_BitmapNoTransparent(int x, int y, const unsigned char *bitmap, uint8 w, uint8 h, uint8 color)
{
    uint8 b;
    uint8 i;
    const unsigned char *p = bitmap;

    tft_SetAddr(x, y, x+w-1, y+h-1);

    while (h > 0)
    {
        for(i=0; i<w; i++)
        {
            if(i & 7)
            {
                b <<= 1;
            } else {
                b   = *p++;
            }
            if (b & 0x80)
            {
                tft_SendData16(color);
            } else {
                tft_SendData16(_backgr);
            }
        }
        h--;
    }
}
