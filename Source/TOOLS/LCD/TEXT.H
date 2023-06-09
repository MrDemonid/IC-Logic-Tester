#ifndef _TEXT_H_
#define _TEXT_H_

void txt_SetFont(const char *fnt, unsigned char w, unsigned char h, unsigned char first, int numchars);

void txt_DrawChar(int x, int y, unsigned char c);
char txt_DrawString(int x, int y, const char *str);

/*
    ���������� ����� ������ � ��������
*/
int txt_GetStringLength(const char *str);


/*
    ���������� ������ � ������ �������� �������� �����
*/

#define txt_GetFontHeight()            fheight
#define txt_GetFontWidth()             fwidth



/*
    ������ ��� ������-������� (����� #define)
*/
extern unsigned char fwidth;           // ������ ������� �����
extern unsigned char fheight;          // ������ �������� �����

#endif
