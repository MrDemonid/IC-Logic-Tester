#include <htc.h>

#include "delay.h"
#include "spi.h"
#include "tft9163.h"


#define tft_BoundaryCheck(x, y)  ((int)x >= _width) || ((int)y >= _height)


int           _width;       // Display w/h as modified by current rotation
int           _height;


unsigned int  _foregr;      // цвет текста/линий
unsigned int  _backgr;      // и фона/закраски



void tft_SendCommand(uint8 b)
{
    TYPE_COMMAND;           // RS = 0
    DISP_SELECT;            // CS = 0
    spi_Send(b);
    DISP_DESELECT;          // CS = 1
}

void tft_SendData(uint8 b)
{
    TYPE_DATA;              // RS = 1;
    DISP_SELECT;            // CS = 0
    spi_Send(b);
    DISP_DESELECT;          // CS = 1
}

void tft_SendData16(uint16 w)
{
    TYPE_DATA;              // RS = 1;
    DISP_SELECT;            // CS = 0
    spi_Send(w >> 8);
    spi_Send(w & 0xFF);
    DISP_DESELECT;          // CS = 1
}

void tft_SetBitRate(uint32 t)
{
}

uint16 Color565(uint8 r, uint8 g, uint8 b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16 Color24To565(uint32 color_)
{
    return ((((color_ >> 16) & 0xFF) / 8) << 11) | ((((color_ >> 8) & 0xFF) / 4) << 5) | (((color_) &  0xFF) / 8);
}

void tft_Begin()
{
    uint8 i;
    // настраиваем пины RS, CS и RESET
    DISP_DESELECT;          // CS = 1
    RST_HIGH;               // RST = 1;
    DISP_OUT;               // CS на выход
    TYPE_OUT;               // RS на выход
    RST_OUT;                // RST на выход
    // настройка SPI
    // сброс дисплея
    DISP_SELECT;
    RST_HIGH;               // RST = 1;
    DelayMs10(25);
    RST_LOW;
    DelayMs10(25);
    RST_HIGH;
    DelayMs10(25);

    // инициализация дисплея
//    _Mactrl_Data = 0b00000000;
    _colorspaceData = __COLORSPC;   //start with default data;
    sleep = 0;

    // chip init
    tft_SendCommand(CMD_SWRESET);   //software reset
    DelayMs10(50);
    tft_SendCommand(CMD_SLPOUT);    //exit sleep
    __delay_ms(5);
    tft_SendCommand(CMD_PIXFMT);    //Set Color Format 16bit
    tft_SendData(0x05);
    __delay_ms(5);
    tft_SendCommand(CMD_GAMMASET);  //default gamma curve 3
    tft_SendData(0x04);             //0x04
    __delay_ms(1);
    tft_SendCommand(CMD_GAMRSEL);   //Enable Gamma adj
    tft_SendData(0x01);
    __delay_ms(1);
    tft_SendCommand(CMD_NORML);
    tft_SendCommand(CMD_DFUNCTR);
    tft_SendData(0b11111111);       //
    tft_SendData(0b00000110);       //
    tft_SendCommand(CMD_PGAMMAC);   //Positive Gamma Correction Setting
    for (i=0; i<15; i++)
    {
        tft_SendData(pGammaSet[i]);
    }
    tft_SendCommand(CMD_NGAMMAC);   //Negative Gamma Correction Setting
    for (i=0; i<15; i++)
    {
        tft_SendData(nGammaSet[i]);
    }
    tft_SendCommand(CMD_FRMCTR1);   //Frame Rate Control (In normal mode/Full colors)
    tft_SendData(0x08);             //0x0C//0x08
    tft_SendData(0x02);             //0x14//0x08
    __delay_ms(1);
    tft_SendCommand(CMD_DINVCTR);   //display inversion
    tft_SendData(0x07);
    __delay_ms(1);
    tft_SendCommand(CMD_PWCTR1);    //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
    tft_SendData(0x0A);             //4.30 - 0x0A
    tft_SendData(0x02);             //0x05
    __delay_ms(1);
    tft_SendCommand(CMD_PWCTR2);    //Set BT[2:0] for AVDD & VCL & VGH & VGL
    tft_SendData(0x02);
    __delay_ms(1);
    tft_SendCommand(CMD_VCOMCTR1);  //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
    tft_SendData(0x50);             //0x50
    tft_SendData(99);               //0x5b
    __delay_ms(1);
    tft_SendCommand(CMD_VCOMOFFS);
    tft_SendData(0);                //0x40
    __delay_ms(1);
    tft_SendCommand(CMD_CLMADRS);   //Set Column Address
    tft_SendData16(0x00);
    tft_SendData16(_GRAMWIDTH-1);
    tft_SendCommand(CMD_PGEADRS);   //Set Page Address
    tft_SendData16(0X00);
    tft_SendData16(_GRAMHEIGH-1);

    tft_DefineScrollArea(0, 0);
    tft_SetRotation(0);
    tft_SendCommand(CMD_DISPON);    //display ON
    __delay_ms(1);
    tft_SendCommand(CMD_RAMWR);     //Memory Write
    __delay_ms(1);
    tft_Color(WHITE, BLACK);
    tft_FillScreen();
}

/*
Colorspace selection:
0: RGB
1: GBR
*/
void tft_ColorSpace(uint8 cspace)
{
    if (cspace < 1)
    {
        _Mactrl_Data &= ~(1 << 3);
    } else {
        _Mactrl_Data |= (1 << 3);
    }
}

void tft_InvertDisplay(bool i)
{
    tft_SendCommand(i ? CMD_DINVON : CMD_DINVOF);
}

void tft_Display(bool OnOff)
{
    if (OnOff)
    {
        tft_SendCommand(CMD_DISPON);
    } else {
        tft_SendCommand(CMD_DISPOFF);
    }
}

void tft_IdleMode(bool OnOff)
{
    if (OnOff)
    {
        tft_SendCommand(CMD_IDLEON);
    } else {
        tft_SendCommand(CMD_IDLEOF);
    }
}

void tft_SleepMode(bool mode)
{
    if (mode)
    {
        if (sleep == 1)
            return;                 //already sleeping
        sleep = 1;
        tft_SendCommand(CMD_SLPIN);
        __delay_ms(5);              //needed
    } else {
        if (sleep == 0)
            return;                 //Already awake
        sleep = 0;
        tft_SendCommand(CMD_SLPOUT);
        DelayMs10(12);          //needed
    }
}

void tft_DefineScrollArea(uint16 tfa, uint16 bfa)
{
    int vsa;

    tfa += __OFFSET;
    vsa = _GRAMHEIGH - tfa - bfa;
    if (vsa >= 0)
    {
        tft_SendCommand(CMD_VSCLLDEF);
        tft_SendData16(tfa);
        tft_SendData16(vsa);
        tft_SendData16(bfa);
    }
}

void tft_Scroll(uint16 adrs)
{
    if (adrs <= _GRAMHEIGH)
    {
        tft_SendCommand(CMD_VSSTADRS);
        tft_SendData16(adrs + __OFFSET);
    }
}

//corrected! v3
void tft_FillScreen(void)
{
    int px;

    tft_SetAddr(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH);    //go home
    for (px = 0; px < _GRAMSIZE; px++)
    {
        tft_SendData16(_backgr);
    }
}

void tft_WriteScreen24(const uint32 *bitmap, uint16 size)
{
    uint16 color;
    uint16 px;

    tft_SendCommand(CMD_RAMWR);
    for (px = 0; px < size; px++)
    {
        color = Color24To565(bitmap[px]);
        tft_SendData16(color);
    }
    tft_SetAddr(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH);     // tft_HomeAddress();
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
//        h--;
    }
}

void tft_DrawRect(int x, int y, int w, int h)
{
    tft_DrawFastHLine(x, y, w);
    tft_DrawFastVLine(x, y, h);
    tft_DrawFastHLine(x, y+h-1, w);
    tft_DrawFastVLine(x+w-1, y, h);
}


//void tft_SetAddr(uint16 x0, uint16 y0, uint16 x1, uint16 y1)

void tft_SetAddr(int x0, int y0, int x1, int y1)
{
    tft_SendCommand(CMD_CLMADRS);   // Column
    if (rotation == 0 || rotation > 1)
    {
        tft_SendData16(x0);
        tft_SendData16(x1);
    } else {
        tft_SendData16(x0 + __OFFSET);
        tft_SendData16(x1 + __OFFSET);
    }
    tft_SendCommand(CMD_PGEADRS);   // Page
    if (rotation == 0)
    {
        tft_SendData16(y0 + __OFFSET);
        tft_SendData16(y1 + __OFFSET);
    } else {
        tft_SendData16(y0);
        tft_SendData16(y1);
    }
    tft_SendCommand(CMD_RAMWR);     //Into RAM
}

void tft_SetRotation(uint8 m)
{
    rotation = m & 0x03;            // can't be higher than 3
    switch (rotation)
    {
        case 0:
            _Mactrl_Data = 0b00001000;
            _width  = _TFTWIDTH;
            _height = _TFTHEIGHT;   //-__OFFSET;
            break;
        case 1:
            _Mactrl_Data = 0b01101000;
            _width  = _TFTHEIGHT;   //-__OFFSET;
            _height = _TFTWIDTH;
            break;
        case 2:
            _Mactrl_Data = 0b11001000;
            _width  = _TFTWIDTH;
            _height = _TFTHEIGHT;   //-__OFFSET;
            break;
        case 3:
            _Mactrl_Data = 0b10101000;
            _width  = _TFTWIDTH;
            _height = _TFTHEIGHT;   //-__OFFSET;
            break;
    }
    tft_ColorSpace(_colorspaceData);
    tft_SendCommand(CMD_MADCTL);
    tft_SendData(_Mactrl_Data);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// GFX ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gfx_BitmapNoTransparent(int x, int y, const char *bitmap, uint8 w, uint8 h, uint16 color)
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
