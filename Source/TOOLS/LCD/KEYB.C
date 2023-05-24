#include "tester.h"

#include "keyb.h"


char key_Get(void)
{
    char c;

    while (1)
    {
        asm {
            xor     ax, ax
            int     16h
            or      al, al
            jnz     done
            mov     al, ah
        }
        done:
        asm {
            mov     c, al
        }
        switch (c)
        {
            case 0x1B: return KEY_ESC;
            case 0x0D: return KEY_ENTER;
            case 0x4B: return KEY_LEFT;
            case 0x4D: return KEY_RIGHT;
            case 0x48: return KEY_UP;
            case 0x50: return KEY_DOWN;
        } // switch

    } // while
}


//        case 0x1B: return KEY_ESC;
