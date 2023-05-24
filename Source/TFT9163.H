#ifndef _TFT9163_H_
#define _TFT9163_H_


// chip select
#ifdef _CS
  // line CS is present
  #define DISP_SELECT   RC1 = 0
  #define DISP_DESELECT RC1 = 1
  #define DISP_OUT      TRISC1 = 0      // вывод CS как выход
#else
  #define DISP_SELECT
  #define DISP_DESELECT
  #define DISP_OUT
#endif

// reset
#ifdef _RST
  // line RESET is present
  #define RST_HIGH      RC0 = 1
  #define RST_LOW       RC0 = 0
  #define RST_OUT       TRISC0 = 0      // вывод RST как выход
#else
  #define RST_HIGH
  #define RST_LOW
  #define RST_OUT
#endif

// линия A0 or RS or D/C
#define TYPE_COMMAND    RC2 = 0         // передача команды
#define TYPE_DATA       RC2 = 1         // передача данных
#define TYPE_OUT        TRISC2 = 0      // вывод RS как выход


typedef char            bool;
typedef signed char     sint8;
typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;



#define BLACK           0x0000
#define BLUE            0x001F      //   0,   0, 255
#define RED             0xF800      // 255,   0,   0
#define GREEN           0x07E0      //   0, 255,   0
#define CYAN            0x07FF      //   0, 255, 255
#define MAGENTA         0xF81F      // 255,   0, 255
#define YELLOW          0xFFE0      // 255, 255,   0
#define WHITE           0xFFFF      // 255, 255, 255
#define NAVY            0x000F      //   0,   0, 128
#define DARKGREEN       0x03E0      //   0, 128,   0
#define DARKCYAN        0x03EF      //   0, 128, 128
#define MAROON          0x7800      // 128,   0,   0
#define PURPLE          0x780F      // 128,   0, 128
#define OLIVE           0x7BE0      // 128, 128,   0
#define GRAY            0xC618      // 192, 192, 192
#define DARKGRAY        0x7BEF      // 128, 128, 128
#define ORANGE          0xFD20      // 255, 165,   0
#define GREENYELLOW     0xAFE5      // 173, 255,  47
#define PINK            0xF81F











// function prototype
void tft_Begin();
//void tft_ChipInit();
void tft_ColorSpace(uint8 cspace);
void tft_InvertDisplay(bool i);
void tft_Display(bool OnOff);
void tft_IdleMode(bool OnOff);
void tft_SleepMode(bool mode);
void tft_DefineScrollArea(uint16 tfa, uint16 bfa);
void tft_Scroll(uint16 adrs);
void tft_WriteScreen24(const uint32 *bitmap, uint16 size);

void tft_DrawFastVLine(int x, int y, int h);
void tft_DrawFastHLine(int x, int y, int w);
void tft_FillScreen(void);
void tft_FillRect(int x, int y, int w, int h);
void tft_DrawRect(int x, int y, int w, int h);

void tft_SetAddr(int x0, int y0, int x1, int y1);
void tft_SetRotation(uint8 m);
void tft_SendData16(uint16 w);

//-----------------------------
//          GFX
//-----------------------------
void gfx_BitmapNoTransparent(int x, int y, const char *bitmap, uint8 w, uint8 h, uint16 color);



//#pragma inline(Color565)
//#pragma inline(Color24To565)
uint16 Color565(uint8 r, uint8 g, uint8 b);
uint16 Color24To565(uint32 color_);


//------------------------------
//        DATA
//------------------------------

volatile uint8 _Mactrl_Data;   //container for the memory access control data
uint8          _colorspaceData;
uint8          sleep;


/*
    установка цвета текста и фона для операций вывода
*/
#define tft_Color(fg, bg)           _foregr = (unsigned int)fg; _backgr = (unsigned int)bg
#define tft_ForeGr(fg)              _foregr = (unsigned int)fg
#define tft_BackGr(bg)              _backgr = (unsigned int)bg


#define tft_GetScreenWidth()  _width
#define tft_GetScreenHeight()  _height

// GFX
extern int           _width;        // Display w/h as modified by current rotation
extern int           _height;

extern unsigned int  _foregr;            // цвет текста/линий
extern unsigned int  _backgr;            // и фона/закраски


uint8         rotation;


//----------------------------------------------------------------------------
//ILI9163C registers-----------------------
#define CMD_NOP         0x00        //Non operation
#define CMD_SWRESET     0x01        //Soft Reset
#define CMD_SLPIN       0x10        //Sleep ON
#define CMD_SLPOUT      0x11        //Sleep OFF
#define CMD_PTLON       0x12        //Partial Mode ON
#define CMD_NORML       0x13        //Normal Display ON
#define CMD_DINVOF      0x20        //Display Inversion OFF
#define CMD_DINVON      0x21        //Display Inversion ON
#define CMD_GAMMASET    0x26        //Gamma Set (0x01[1],0x02[2],0x04[3],0x08[4])
#define CMD_DISPOFF     0x28        //Display OFF
#define CMD_DISPON      0x29        //Display ON
#define CMD_IDLEON      0x39        //Idle Mode ON
#define CMD_IDLEOF      0x38        //Idle Mode OFF
#define CMD_CLMADRS     0x2A        //Column Address Set
#define CMD_PGEADRS     0x2B        //Page Address Set

#define CMD_RAMWR       0x2C        //Memory Write
#define CMD_RAMRD       0x2E        //Memory Read
#define CMD_CLRSPACE    0x2D        //Color Space : 4K/65K/262K
#define CMD_PARTAREA    0x30        //Partial Area
#define CMD_VSCLLDEF    0x33        //Vertical Scroll Definition
#define CMD_TEFXLON     0x35        //Tearing Effect Line ON
#define CMD_TEFXLOF     0x34        //Tearing Effect Line OFF
#define CMD_MADCTL      0x36        //Memory Access Control
#define CMD_VSSTADRS    0x37        //Vertical Scrolling Start address
#define CMD_PIXFMT      0x3A        //Interface Pixel Format
#define CMD_FRMCTR1     0xB1        //Frame Rate Control (In normal mode/Full colors)
#define CMD_FRMCTR2     0xB2        //Frame Rate Control(In Idle mode/8-colors)
#define CMD_FRMCTR3     0xB3        //Frame Rate Control(In Partial mode/full colors)
#define CMD_DINVCTR     0xB4        //Display Inversion Control
#define CMD_RGBBLK      0xB5        //RGB Interface Blanking Porch setting
#define CMD_DFUNCTR     0xB6        //Display Fuction set 5
#define CMD_SDRVDIR     0xB7        //Source Driver Direction Control
#define CMD_GDRVDIR     0xB8        //Gate Driver Direction Control

#define CMD_PWCTR1      0xC0        //Power_Control1
#define CMD_PWCTR2      0xC1        //Power_Control2
#define CMD_PWCTR3      0xC2        //Power_Control3
#define CMD_PWCTR4      0xC3        //Power_Control4
#define CMD_PWCTR5      0xC4        //Power_Control5
#define CMD_VCOMCTR1    0xC5        //VCOM_Control 1
#define CMD_VCOMCTR2    0xC6        //VCOM_Control 2
#define CMD_VCOMOFFS    0xC7        //VCOM Offset Control
#define CMD_PGAMMAC     0xE0        //Positive Gamma Correction Setting
#define CMD_NGAMMAC     0xE1        //Negative Gamma Correction Setting
#define CMD_GAMRSEL     0xF2        //GAM_R_SEL



//----------------------------------------------------------------------------
// Settings
//DID YOU HAVE A RED PCB, BLACk PCB or WHAT DISPLAY TYPE????????????
//  ---> SELECT HERE <----

#define __144_BLACK_PCB__//128x128

//#define __144_RED_PCB__//128x128
//#define __144_BLACK_PCB__//128x128
//#define __22_RED_PCB__//240x320
//---------------------------------------


#if defined(__144_RED_PCB__)
/*
This display:
http://www.ebay.com/itm/Replace-Nokia-5110-LCD-1-44-Red-Serial-128X128-SPI-Color-TFT-LCD-Display-Module-/271422122271
This particular display has a design error! The controller has 3 pins to configure to constrain
the memory and resolution to a fixed dimension (in that case 128x128) but they leaved those pins
configured for 128x160 so there was several pixel memory addressing problems.
I solved by setup several parameters that dinamically fix the resolution as needed so below
the parameters for this diplay. If you have a strain or a correct display (can happen with chinese)
you can copy those parameters and create setup for different displays.
*/
    #define _TFTWIDTH       128     //the REAL W resolution of the TFT
    #define _TFTHEIGHT      128     //the REAL H resolution of the TFT
    #define _GRAMWIDTH      128
    #define _GRAMHEIGH      160     //160
    #define _GRAMSIZE       _GRAMWIDTH * _GRAMHEIGH     //*see note 1
    #define __COLORSPC      1       // 1:GBR - 0:RGB
    #define __GAMMASET3             //uncomment for another gamma
    #define __OFFSET        32                          //*see note 2
    //Tested!
#elif defined (__144_BLACK_PCB__)
    #define _TFTWIDTH       128     //the REAL W resolution of the TFT
    #define _TFTHEIGHT      128     //the REAL H resolution of the TFT
    #define _GRAMWIDTH      128
    #define _GRAMHEIGH      128
    #define _GRAMSIZE       _GRAMWIDTH * _GRAMHEIGH//*see note 1
    #define __COLORSPC      1       // 1:GBR - 0:RGB
    #define __GAMMASET1             //uncomment for another gamma
    #define __OFFSET        0
    //not tested
#elif defined (__22_RED_PCB__)
/*
Like this one:
http://www.ebay.it/itm/2-2-Serial-SPI-TFT-LCD-Display-Module-240x320-Chip-ILI9340C-PCB-Adapter-SD-Card-/281304733556
Not tested!
*/
    #define _TFTWIDTH       240     //the REAL W resolution of the TFT
    #define _TFTHEIGHT      320     //the REAL H resolution of the TFT
    #define _GRAMWIDTH      240
    #define _GRAMHEIGH      320
    #define _GRAMSIZE       _GRAMWIDTH * _GRAMHEIGH
    #define __COLORSPC      1       // 1:GBR - 0:RGB
    #define __GAMMASET1             //uncomment for another gamma
    #define __OFFSET        0
#else
    #define _TFTWIDTH       128     //128
    #define _TFTHEIGHT      160     //160
    #define _GRAMWIDTH      128
    #define _GRAMHEIGH      160
    #define _GRAMSIZE       _GRAMWIDTH * _GRAMHEIGH
    #define __COLORSPC      1       // 1:GBR - 0:RGB
    #define __GAMMASET1
    #define __OFFSET        0
#endif

#if defined(__GAMMASET1)
    const uint8 pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
    const uint8 nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
#elif defined(__GAMMASET2)
    const uint8 pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
    const uint8 nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
#elif defined(__GAMMASET3)
    const uint8 pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
    const uint8 nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
#else
    const uint8 pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
    const uint8 nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
#endif
/*
Note 1: The __144_RED_PCB__ display has hardware addressing of 128 x 160
but the tft resolution it's 128 x 128 so the dram should be set correctly

Note 2: This is the offset between image in RAM and TFT. In that case 160 - 128 = 32;
*/

#endif
