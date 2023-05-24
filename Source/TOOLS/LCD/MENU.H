#ifndef _MENU_H_
#define _MENU_H_


// цвета меню
#define MENU_FOREGROUND     WHITE       // цвет текста
#define MENU_BACKGROUND     BLUE        // и фона
#define MENU_FORESEL        YELLOW      // цвет текста выделенной строки меню
#define MENU_BACKSEL        RED         // и фона

// флаги вывода меню
#define MENU_SINGLECOL      0x01        // один столбец
#define MENU_ENDLVL         0x80        // флаг последнего уровня меню
#define MENU_ROOT           0x40        // флаг корневого меню


typedef struct TMENU {
    const char * const         *text;   // массив строк
    const unsigned char         flags;  // флаги свойств
    const signed char           count;  // количество строк
    const struct TMENU * const *next;   // след. уровень меню или данные
} MENU;


// возвращаемые mnu_Run() данные
extern const char *ic_data;             // данные теста
extern const char *ic_name;             // имя тестируемой микросхемы

void mnu_Run(const MENU *mnu);          // цикл выбора микросхемы



#endif

