#ifndef _MENU_H_
#define _MENU_H_


// ����� ����
#define MENU_FOREGROUND     WHITE       // ���� ������
#define MENU_BACKGROUND     BLUE        // � ����
#define MENU_FORESEL        YELLOW      // ���� ������ ���������� ������ ����
#define MENU_BACKSEL        RED         // � ����

// ����� ������ ����
#define MENU_SINGLECOL      0x01        // ���� �������
#define MENU_ENDLVL         0x80        // ���� ���������� ������ ����
#define MENU_ROOT           0x40        // ���� ��������� ����


typedef struct TMENU {
    const char * const         *text;   // ������ �����
    const unsigned char         flags;  // ����� �������
    const signed char           count;  // ���������� �����
    const struct TMENU * const *next;   // ����. ������� ���� ��� ������
} MENU;


// ������������ mnu_Run() ������
extern const char *ic_data;             // ������ �����
extern const char *ic_name;             // ��� ����������� ����������

void mnu_Run(const MENU *mnu);          // ���� ������ ����������



#endif

