#ifndef _TEXT_H_
#define _TEXT_H_

void txt_SetFont(const char *fnt, unsigned char w, unsigned char h, unsigned char first, int numchars);

void txt_DrawChar(int x, int y, unsigned char c);
char txt_DrawString(int x, int y, const char *str);

/*
    возвращает длину строки в пикселях
*/
int txt_GetStringLength(const char *str);


/*
    возвращает высоту и ширину символов текущего фонта
*/

#define txt_GetFontHeight()            fheight
#define txt_GetFontWidth()             fwidth



/*
    данные для инлайн-функций (через #define)
*/
extern unsigned char fwidth;           // ширина символа фонта
extern unsigned char fheight;          // высота символов фонта

#endif
