#ifndef _KEYB_H_
#define _KEYB_H_


/*
  возвращаемые функцией key_Get() результаты
*/

#define KEY_UP          1
#define KEY_DOWN        2
#define KEY_LEFT        3
#define KEY_RIGHT       4
#define KEY_ENTER       5

#define KEY_ESC         6

char key_Get(void);


#endif
