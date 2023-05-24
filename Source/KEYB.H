#ifndef _KEYB_H_
#define _KEYB_H_


/*
  возвращаемые функцией key_Get() результаты
*/
#define KEY_UP          1
#define KEY_DOWN        2
#define KEY_LEFT        4
#define KEY_RIGHT       8
#define KEY_ENTER       16


char key_Get(void);

/*
  ждет нажатия Enter и возвращает 0, если нажатие короткое,
                              или 1, если жать более секунды
*/
char key_GetEnter(void);



/*
  макросы определения нажатия кнопок
*/
#define key_Enter()   !RE2
#define key_Up()      !RB5
#define key_Down()    !RB1
#define key_Left()    !RB4
#define key_Right()   !RB3





#endif
