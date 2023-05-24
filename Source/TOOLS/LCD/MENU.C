#include "TFT9163.H"
#include "TEXT.H"
#include "KEYB.H"
#include "MENU.H"

const char *ic_data;
const char *ic_name;


int max_height;   // максимальная высота экрана в пикселях
int col_width;    // ширина одного столбца в пикселях
int start_y;      // верхний отступ  в пикселях

// стэк пройденных меню
const MENU *stk[6];
signed char curr[6];
char        curstk;
const MENU *menu;

#define GET_START_Y(m, y)   if (m->flags & MENU_SINGLECOL) \
                            {                                \
                                y = start_y + ((tft_GetScreenHeight() - (m->count * txt_GetFontHeight())) >> 1); \
                            } else { \
                                y = start_y; \
                            }


void mnu_Show(const MENU *mnu)
{
    int i, sy, sx, x, y;

    GET_START_Y(mnu, sy);
    sx = 0;
    y = sy;
    tft_Color(MENU_FOREGROUND, MENU_BACKGROUND);
    tft_FillScreen();
    for (i = 0; i < mnu->count; i++)
    {
        x = (col_width - txt_GetStringLength(mnu->text[i])) >> 1;
        txt_DrawString(sx+x, y, mnu->text[i]);
        y += txt_GetFontHeight();
        if (y >= max_height)
        {
            y = sy;
            sx += col_width;
            if (sx >= tft_GetScreenWidth())
                break;
        }
    }
}

void mnu_Select(const MENU *mnu, char cur, char sel)
{
    int sx, sy;

    GET_START_Y(mnu, sy);
    sy += (cur * txt_GetFontHeight());
    if (sel)
    {
        tft_Color(MENU_FORESEL, MENU_BACKSEL);
    } else {
        tft_Color(MENU_FOREGROUND, MENU_BACKGROUND);
    }
    sx = (col_width - txt_GetStringLength(mnu->text[cur])) >> 1;

    if (!(mnu->flags & MENU_SINGLECOL))
    {
        sx += (sy / max_height)* col_width;
    }
    txt_DrawString(sx, sy % max_height, mnu->text[cur]);
}



void mnu_Run(const MENU *mnu)
{
    char c;
    signed char incr;

    if (mnu)
    {
        curstk = 0;
        stk[curstk] = mnu;
        curr[curstk] = 0;
        menu = mnu;
    }

    while(-1)
    {
        // настраиваем поля отображения
        max_height = tft_GetScreenHeight() - (tft_GetScreenHeight() % txt_GetFontHeight());
        start_y = (tft_GetScreenHeight() - max_height) >> 1;
        if (menu->flags & MENU_SINGLECOL)
        {
            col_width = tft_GetScreenWidth();
        } else {
            c = ((menu->count-1) * txt_GetFontHeight()) / max_height + 1;
            if (c <= 1)
                c++;
            col_width = tft_GetScreenWidth() / c;
        }

        mnu_Show(menu);
        mnu_Select(menu, curr[curstk], 1);
        // цикл опроса клавиатуры
        while (-1)
        {
            c = key_Get();
            if (c == KEY_ENTER)
            {
                if (!menu->next[curr[curstk]])
                {
                    // возврат на пред. уровень
                    if (curstk > 0)
                    {
                        curstk--;
                        menu = stk[curstk];
                    }
                } else if (menu->flags & MENU_ENDLVL)
                {
                    // заканчиваем работу и возвращаем указатель на выбранные данные
                    ic_data = (const char *) menu->next[curr[curstk]];
                    ic_name = menu->text[curr[curstk]];
                    return;
                } else {
                    // переходим на следующий уровень
                    menu = menu->next[curr[curstk]];
                    curstk++;
                    stk[curstk] = menu;
                    curr[curstk] = 0;
                }
                break;

            } else {
                mnu_Select(menu, curr[curstk], 0);
                switch (c)
                {
                    case KEY_LEFT:  { incr = -(max_height/txt_GetFontHeight()); break; }
                    case KEY_RIGHT: { incr = max_height/txt_GetFontHeight(); break; }
                    case KEY_UP:    { incr = -1; break; }
                    case KEY_DOWN:  { incr = 1; break; }
                    default: { break; }
                }
                curr[curstk] += incr;
                if (curr[curstk] < 0)
                    curr[curstk] = 0;
                else if (curr[curstk] > menu->count-1)
                    curr[curstk] = menu->count-1;
                mnu_Select(menu, curr[curstk], 1);
            }
        } // while keyboard
    }
}





