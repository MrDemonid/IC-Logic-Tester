#ifndef _KEYB_H_
#define _KEYB_H_


/*
  �����頥�� �㭪樥� key_Get() १�����
*/
#define KEY_UP          1
#define KEY_DOWN        2
#define KEY_LEFT        4
#define KEY_RIGHT       8
#define KEY_ENTER       16


char key_Get(void);

/*
  ���� ������ Enter � �����頥� 0, �᫨ ����⨥ ���⪮�,
                              ��� 1, �᫨ ���� ����� ᥪ㭤�
*/
char key_GetEnter(void);



/*
  ������ ��।������ ������ ������
*/
#define key_Enter()   !RE2
#define key_Up()      !RB5
#define key_Down()    !RB1
#define key_Left()    !RB4
#define key_Right()   !RB3





#endif
