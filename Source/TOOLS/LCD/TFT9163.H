#ifndef _TFT9163_H_
#define _TFT9163_H_


typedef char            bool;
typedef signed char     sint8;
typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;



#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define GRAY            7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15


// function prototype
void tft_Begin(void);
void tft_Done(void);
void tft_DrawPixel(int x, int y, uint8 color);

void tft_DrawFastVLine(int x, int y, int h);
void tft_DrawFastHLine(int x, int y, int w);
void tft_FillScreen(void);
void tft_FillRect(int x, int y, int w, int h);
void tft_DrawRect(int x, int y, int w, int h);

void tft_SetAddr(int x0, int y0, int x1, int y1);
void tft_SendData16(uint8 w);

//-----------------------------
//          GFX
//-----------------------------
void gfx_BitmapNoTransparent(int x, int y, const unsigned char *bitmap, uint8 w, uint8 h, uint8 color);



//------------------------------
//        DATA
//------------------------------

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

extern unsigned char _foregr;            // цвет текста/линий
extern unsigned char _backgr;            // и фона/закраски


#define _TFTWIDTH       128     //the REAL W resolution of the TFT
#define _TFTHEIGHT      128     //the REAL H resolution of the TFT
#define _GRAMWIDTH      128
#define _GRAMHEIGH      128
#define _GRAMSIZE       _GRAMWIDTH * _GRAMHEIGH//*see note 1

#endif
