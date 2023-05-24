#include <htc.h>
#include "delay.h"

void DelayMs10(unsigned int cnt)
{
    while (cnt)
    {
        __delay_ms(10);
        cnt--;
    }
}

