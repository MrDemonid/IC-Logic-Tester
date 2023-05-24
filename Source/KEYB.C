#include <htc.h>

#include "delay.h"
#include "keyb.h"


char key_Get(void)
{
    while (-1)
    {
        if (key_Up())
        {
            DelayMs10(3);
            while (key_Up());
            return KEY_UP;

        } else if (key_Down())
        {
            DelayMs10(3);
            while (key_Down());
            return KEY_DOWN;

        } else if (key_Left())
        {
            DelayMs10(3);
            while (key_Left());
            return KEY_LEFT;

        } else if (key_Right())
        {
            DelayMs10(3);
            while (key_Right());
            return KEY_RIGHT;

        } else if (key_Enter())
        {
            DelayMs10(3);
            while (key_Enter());
            return KEY_ENTER;
        }
    }
    return 0;
}


/*
  ждет нажатия Enter и возвращает 0, если нажатие короткое,
                              или 1, если жать более секунды
*/
char key_GetEnter(void)
{
    char i = 0;

    while (!key_Enter());
    DelayMs10(4);
    // ждем отжатия кнопки
    while (key_Enter())
    {
        DelayMs10(2);
        i++;
        if (i > 50)
            return 1;     // зафиксировано длинное нажатие
    }
    return 0;             // просто нажатие кнопки
}
