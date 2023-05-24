#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>


#define GEN_TTL
//#define GEN_CMOS
//#define GEN_580
//#define GEN_514

// префикс имен меню
#if defined GEN_TTL
  #define PREFIX        "t"
  #define MNUNAME       "TTL"
  #define ANALOGNAME    "74XX"
  #define INFILE        "data_ttl.ic"
  #define OUTDATA       "ttldata.h"
  #define OUTMENU       "ttlmenu.h"
  #define OUTANALOG     "74menu.h"

#elif defined GEN_CMOS
  #define PREFIX        "c"
  #define MNUNAME       "CMOS"
  #define ANALOGNAME    "4XXX"
  #define INFILE        "data_cms.ic"
  #define OUTDATA       "cmosdata.h"
  #define OUTMENU       "cmosmenu.h"
  #define OUTANALOG     "40menu.h"
#elif defined GEN_580
  #define PREFIX        "x"
  #define MNUNAME       "580"
  #define ANALOGNAME    ""
  #define INFILE        "data_580.ic"
  #define OUTDATA       "580data.h"
  #define OUTMENU       "580menu.h"
  #define OUTANALOG     ""
#elif defined GEN_514
  #define PREFIX        "i"
  #define MNUNAME       "514"
  #define ANALOGNAME    ""
  #define INFILE        "data_514.ic"
  #define OUTDATA       "514data.h"
  #define OUTMENU       "514menu.h"
  #define OUTANALOG     ""

#else
  #error Please define menu type!
#endif


#define MC                  // компилировать под микроконтроллер (иначе под LCD)

/*****************************************************************************

******************************************************************************/
#define DELIMITER       1   // разделитель
#define QUOTE           2   // строка в кавычках
#define NUMBER          3   // число
#define COMMAND         4   // одна из команда CHIP, POWER и тд.
#define STRING          5   // команда или переменная


#define EOL             7   // конец строки
#define DELIM           8   // любой символ-разделитель

// токены команд
#define TOK_CHIP        1
#define TOK_POWER       2
#define TOK_IN          3
#define TOK_OUT         4
#define TOK_SET         5
#define TOK_TEST        6
#define TOK_CONFIG      7
#define TOK_PULSE       8
#define TOK_PULLUP      9
#define TOK_PULLDN      10

struct VARIABLES {
    char *name;
    char tok;
} cmd_table[] = {"CHIP", TOK_CHIP,
                    "POWER", TOK_POWER,
                    "IN", TOK_IN,
                    "OUT", TOK_OUT,
                    "SET", TOK_SET,
                    "TEST", TOK_TEST,
                    "CONFIG", TOK_CONFIG,
                    "PULSE", TOK_PULSE,
                    "PULLUP", TOK_PULLUP,
                    "PULLDN", TOK_PULLDN,
                    "", 0 };

/*****************************************************************************
                               КОДЫ КОМАНД
******************************************************************************/
#define ICMD_SET                    1   //
#define ICMD_SET_ALL                2   //
#define ICMD_TEST                   3   //
#define ICMD_TEST_ALL               4   //
#define ICMD_SET_ALL_AND_TEST       5   //
#define ICMD_PULSE                  6
#define ICMD_CONFIG                 7
#define ICMD_PULLUP                 8
#define ICMD_PULLDN                 9

#define CMD_INIT_8                  1   // +8  -4
#define CMD_INIT_14                 2   // +14 -7
#define CMD_INIT_14_1               3   // +14 -6
#define CMD_INIT_14_2               4   // +5  -10
#define CMD_INIT_14_3               5   // +4  -11
#define CMD_INIT_16                 6   // +16 - 8
#define CMD_INIT_16_1               7   // +16 -7
#define CMD_INIT_16_2               8   // +5  -12
#define CMD_INIT_16_3               9   // +8  -16
#define CMD_INIT_18_1               10  // +9  -18
#define CMD_INIT_20                 11  // +20 -10
#define CMD_INIT_20_1               12  // +20 -8
#define CMD_INIT_24                 13  // +24 -12

#define CMD_SET_TO_0                14  // (pin_no8) устанавливает ноль на одном пине
#define CMD_SET_TO_1                15  // (pin_no8) устанавливает единицу на одном пине
#define CMD_SET                     16  // (mask0[n], mask1[n])
#define CMD_SET_ALL                 17  // (mask[n]) устанавливает все входы
#define CMD_TEST                    18  // (mask0[n], mask1[n]) тест отдельных выходов
#define CMD_TEST_ALL                19  // (mask[n]) тест всех выходов
#define CMD_SET_ALL_AND_TEST        20  // (smask[n], tmask[n]) установка всех входов и тест всех выходов
#define CMD_PULSE                   21  // (pin_no8)
#define CMD_LAST_PULSE              22  //
#define CMD_LAST_PULSE_AND_TEST     23  // (mask[n])
#define CMD_LAST_REPEAT_PULSE       24  // (count8)
#define CMD_CONFIG                  25  // (in_out_mask[n])
#define CMD_PULLUP                  26
#define CMD_PULLDN                  27

#define CMD_END                     28


/*****************************************************************************
                           СТРУКТУРЫ ДЛЯ КОМАНД
******************************************************************************/

typedef struct tCMD {
    char            type;
    struct tCMD    *next;
} CMD;

typedef struct {    // SET: 0 -> x, x, x; 1 -> y, y, y
    CMD             cmd;
    unsigned long   mask0;  // маска для установки пинов в 0
    unsigned long   mask1;  // маска для установки пинов в 1
} CMDSET;

typedef struct {    // SETALL xxxxxxxxx
    CMD             cmd;
    unsigned long   mask;   // маска пинов
} CMDSETALL;

typedef struct {    // TEST: 11,12,13,14 -> 1
    CMD             cmd;
    unsigned long   mask0;  // маска для тестирования пинов на 0
    unsigned long   mask1;  // маска для тестирования пинов на 1
} CMDTEST;

typedef struct {    // TEST: 0000000
    CMD             cmd;
    unsigned long   mask;   // маска пинов
} CMDTESTALL;

typedef struct {    // TEST: 0000:1 => 0000001
    CMD             cmd;
    unsigned long   mset;   // маска для установки пинов
    unsigned long   mtst;   // маска для тестирования пинов
} CMDSETALLANDTEST;

typedef struct {    // CONFIG: 14,9,10,11,6,7 -> IN; 12,13, 5,3 -> OUT
    CMD             cmd;
    unsigned long   mask;   // новая маска пинов: 0 - выход, 1 - вход
} CMDCONFIG;

typedef struct {
    CMD             cmd;
    signed char     pin;
    unsigned char   cnt;
} CMDPULSE;



/*****************************************************************************
             ДАННЫЕ ДЛЯ ПРЕДВАРИТЕЛЬНОГО СКАНИРОВАНИЯ ФАЙЛА *.IC
******************************************************************************/


typedef struct tICDATA          // список команд теста для одной микросхемы
{
    unsigned char  *name;
    char            isOK;       // флаг наличия выводов с открытым коллектором
    int             pins;       // кол. выводов микросхемы
    int             plus;       // номер плюсового вывода
    int             gnd;        // номер общего вывода
    unsigned long   pinmask;    // начальная маска пинов (0 bit equ 1 pin) 0: out, 1: in
    CMD            *list;       // список команд (CMD_xxxx)
    CMD            *last;       // последняя в списке
    struct tICDATA *next;
} ICDATA;


typedef struct tICNAM           // список имен микросхем с привязкой к меткам
{
    unsigned char  *name;       // имя микрухи
    unsigned char  *analog;     // ее аналог
    unsigned char  *label;      // метка данных для данной микросхемы
    struct tICNAM  *prev;
    struct tICNAM  *next;
} ICNAMES;



FILE           *file;           // input file *.IC & output *.h

unsigned long   nrow;           // номер строки читаемого файла
char            buff[512];      // input buffer for strings
unsigned char  *ptr;            // position in buff[]
int             token_type;     // текущий токен
int             tok;            //
char            token[128];     // он же в символьном виде


// м/с
ICDATA         *ic;             // список микросхем
ICDATA         *cur_ic;

ICNAMES        *names;          // список имен микросхем с привязкой к меткам
unsigned int    nlabel;

// конфигурация текущей микросхемы
char           *pname[40];      // имена пинов
unsigned char   in[40];         // текущие номера входов (в порядке объявления)
unsigned char   out[40];        // текущие номера выходов (в порядке объявления)
int             incnt;          // текущнее количество входов
int             outcnt;         // текущнее количество выходов
unsigned long   curmask;        // текущая маска ввода/вывода (0 bit equ 1 pin) 0: out, 1: in
unsigned char   lastpulse;      // номер ножки последнего импульса

// для статистики
int             numdata;        // количество тестов
int             numic;          // количество микросхем
unsigned int    totalsize;      // размер тестов в байтах




/*****************************************************************************
******************************************************************************
                       ВСПОМОГАТЕЛЬНЫЕ ПОДПРОГРАММЫ
******************************************************************************
******************************************************************************/

int bitcnt(unsigned long n)
{
    int cnt = 0;
    while (n)
    {
        if (n & 1)
            cnt++;
        n >>= 1;
    }
    return cnt;
}

int bit2pin(unsigned long n)
{
    int cnt = 0;
    while (n)
    {
        n >>= 1;
        cnt++;
    }
    return cnt;
}

void byte2bin(int b, char *str)
{
    int i;

    for (i = 0; i < 8; i++)
    {
        str[i] = ((b & 0x80) >> 7) + '0';
        b <<= 1;

    }
    str[8] = '\0';
}


void free_cmd(CMD *p)
{
    CMD *t;
    while (p)
    {
        t = p->next;
        free(p);
        p = t;
    }
}

void *free_ic(ICDATA *p)
{
    ICDATA *t;
    while (p)
    {
//        printf(" - %s\n", p->name);
        t = p->next;
        free_cmd(p->list);
        free(p->name);
        free(p);
        p = t;
    }
    return NULL;
}




/*****************************************************************************
******************************************************************************
                                 СКАНЕР
******************************************************************************
******************************************************************************/

// пропуск ведущих пробелов и символов конца строки
void skip_space(void)
{
    while (*ptr <= ' ')
    {
        if (*ptr == '\x0')
        {
                return;
        }
        ptr++;
    }
}
int get_command(char *str)
{
    int i;
    for (i = 0; *cmd_table[i].name; i++)
        if (!strcmp(cmd_table[i].name, str))
            return cmd_table[i].tok;
    return 0;
}

int get_token(void)
{
    char *p = &token[0];

    tok = 0;
    token_type = 0;

    if (!*ptr)
    {
        tok = EOL;              // конец строки или файла
        token_type = DELIMITER;
        token[0] = '\n';
        return token_type;
    }
    skip_space();               // пропускаем ведущие пробелы
    if (!*ptr)
    {                           // конец строки
        tok = EOL;
        token_type = DELIMITER;
        token[0] = '\n';
        return token_type;
    }

    if (strchr(" @#+-*^/%=:;(),><[]", *ptr))
    {
        // разделитель
        *p++ = *ptr++;
        *p = '\x0';
        token_type = DELIMITER;
        return token_type;
    }

    if (*ptr == '\'')
    {
        // строка в кавычках
        ptr++;
        while (*ptr != '\'' && *ptr != '\r')
            *p++ = *ptr++;
        if (*ptr == '\r')
        {
            printf("  **ERROR**[%lu] syntax error!\n", nrow);
            return 0;
        }
        ptr++;
        *p = '\x0';
        token_type = QUOTE;
        return token_type;
    }
    if (strchr("0123456789", *ptr))
    {
        // число
        while (*ptr)
        {
            if (strchr("0123456789", *ptr))
            {
                *p++ = *ptr++;
            } else if (*ptr == ':') {
                ptr++;
            } else {
                if (!strchr(" @#+-*^/%=:;(),><[]\r\n", *ptr))
                {
                    printf("  **ERROR**[%lu] number error!\n", nrow);
                    return 0;
                }
                break;
            }

        }
        *p = '\x0';
        token_type = NUMBER;
        return token_type;
    }
    if (isalpha(*ptr) || strchr("~_", *ptr))
    {
        // команда
        while (isalnum(*ptr) || strchr("~_", *ptr))
            *p++ = *ptr++;
        *p = '\x0';
        token_type = STRING;
        // проверяем, есть ли в списке команд
        tok = get_command(token);
        if (tok)
            token_type = COMMAND;
    }
    return token_type;
}


/*****************************************************************************
                   ПОДПРОГРАММЫ ВЫВОДА СООБЩЕНИЙ ОБ ОШИБКАХ
******************************************************************************/

char err_syntax(char *str)
{
    printf("  **ERROR**[%lu] syntax error: %s\n", nrow, str);
    return 0;
}

char err_expected(char *str)
{
    printf("  **ERROR**[%lu] expected %s\n", nrow, str);
    return 0;
}

char err_numberexp(void)
{
    printf("  **ERROR**[%lu] number expected!\n", nrow);
    return 0;
}

char err_badpin(int pin)
{
    printf("  **ERROR**[%lu] bad pin number '%i'!\n", nrow, pin);
    return 0;
}

char err_badpincount(void)
{
    printf("  **ERROR**[%lu] bad number of pins!\n", nrow);
    return 0;
}

char err_badpinlogic(void)
{
    printf("  **ERROR**[%lu] pin must be '0' or '1'\n!", nrow);
    return 0;
}

char err_pinpower(void)
{
    printf("  **ERROR**[%lu] pin can't be powers pin!\n", nrow);
    return 0;

}

/*****************************************************************************
                  ПОДПРОГРАММЫ ДЛЯ ФУНКЦИЙ ОБРАБОТКИ ТОКЕНОВ
******************************************************************************/


// проверка на символ '*str'
char read_delim(char *str)
{
    if (get_token() == DELIMITER)
    {
        if (strstr(token, str))
            return -1;
    }
    return 0;
}

char read_int(int *num)
{
    if (get_token() != NUMBER)
    {
        return 0;
    }
    *num = atoi(token);
    return -1;
}

char *read_quote(void)
{
    char *s;

    if (get_token() != QUOTE)
        return 0;
    s = strdup(token);

    return s;
}

// считывает номер или имя пина (num - номер считанного пина)
// на выходе: 0 - ошибка
char read_pin(int *num)
{
    int i;

    if (!get_token())
        return 0;

    if (token_type == NUMBER)
    {
        *num = atoi(token);
        if (*num <= 0 && *num > cur_ic->pins)
            return err_badpin(*num);
        if ((*num == cur_ic->gnd) || (*num == cur_ic->plus))
            return err_pinpower();
        return -1;
    }
    if (token_type != STRING)
        return err_expected("idetifier of pin");
    // ищем имя в таблице
    for (i = 0; i < 40; i++)
    {
        if (pname[i])
            if (strcmpi(token, pname[i]) == 0)
            {
                *num = i+1;
                return -1;
            }
    }
    printf("  **ERROR**[%lu] undefine pin name: '%s'\n", nrow, token);
    return 0;


}

char add_pin_name(int pin, char *name)
{

    if (pname[pin-1])
    {
        printf("  **ERROR**[%lu] reassigned pin '%i'!\n", nrow, pin);
        return 0;
    }
    pname[pin-1] = strdup(name);
    return -1;
}

// считывает имя пина или разделитель (,) (для IN: и OUT:)
// на выходе: <0 - конец строки
//            >0 - все ок
//             0 - ошибка
char read_name(int pin)
{
    if (get_token() != DELIMITER)
        return err_syntax("bad format");
    if (tok == EOL)
        return -1;
    if (token[0] == ',')
        return 1;
    if (token[0] != '(')
        return err_expected("'('");
    if (get_token() != STRING)
        return err_expected("name of pin");
    // дабавляем имя пина в массив
    if (!add_pin_name(pin, token))
        return 0;
    if (get_token() != DELIMITER || token[0] != ')')
        return err_expected("')'");

    if (!read_delim(","))
        return -1;
    return 1;
}

void free_pin(void)
{
    int i;

    for (i = 0; i < 40; i++)
    {
        if (pname[i])
            free(pname[i]);
    }
    memset(pname, 0x00, sizeof(pname));
}



// создание новой структуры CMD
CMD *new_command(int cmd, int size)
{
    CMD *c;

    c = malloc(size);
    memset(c, 0x00, size);
    c->type = cmd;
    return c;
}

// добавление CMD в список команд текущей м/с
void add_command(CMD *cmd)
{
    if (cur_ic->list)
    {
        cur_ic->last->next = cmd;
    } else {
        cur_ic->list = cmd;
    }
    cur_ic->last = cmd;
    cur_ic->last->next = NULL;
}




/*****************************************************************************
******************************************************************************
                        ЧТЕНИЕ ТОКЕНОВ И ИХ ПАРАМЕТРОВ
******************************************************************************
******************************************************************************/

int do_Chip(void)
{
    int i;

    if (!ic)
    {
        cur_ic = malloc(sizeof(ICDATA));
        ic     = cur_ic;
    } else {
        cur_ic->next = malloc(sizeof(ICDATA));
        cur_ic = cur_ic->next;
    }
    if (!cur_ic)
    {
        printf("  **ERROR** not enought memory!\n");
        return 0;
    }
    memset(cur_ic, 0x00, sizeof(ICDATA));
    memset(in, 0x00, sizeof(in));
    memset(out, 0x00, sizeof(out));
    incnt = 0;
    outcnt = 0;
    free_pin();
    if (!read_delim("["))        return err_expected("'['");
    if (!read_int(&cur_ic->pins)) return err_numberexp();
    if (!read_delim("]"))        return err_expected("']'");
    cur_ic->name = read_quote();
    if (!cur_ic->name)
        return err_expected("quoted string");
    numdata++;
    curmask = 0;
    for (i = 0; i < cur_ic->pins; i++)
        curmask |= 1UL << i;
    cur_ic->pinmask = curmask;
    return -1;

}

int do_Power(void)
{
    int g, p;

    if (!read_delim(":")) return err_expected("':'");
    if (!read_delim("-")) return err_expected("'-'");
    if (!read_int(&g))    return err_numberexp();
    if (!read_delim("+"))
        if (token_type == DELIMITER && token[0] == '*')
            if (!read_delim("+"))
                return err_expected("'+'");
    if (!read_int(&p))    return err_numberexp();
    // проверяем корректность пина
    if (g < 1 || g > cur_ic->pins || p < 1 || p > cur_ic->pins)
    {
        printf("  **ERROR**[%lu] power pins can't be great IC pins!\n", nrow);
        return 0;
    }
    cur_ic->gnd = g;
    cur_ic->plus = p;
    // помечаем пины как вход
/*
    cur_ic->pinmask |= 1UL << (cur_ic->gnd-1);
    cur_ic->pinmask |= 1UL << (cur_ic->plus-1);
    curmask |= 1UL << (cur_ic->gnd-1);
    curmask |= 1UL << (cur_ic->plus-1);
*/
    read_delim("*");
    return -1;
}

//        IN: 4,2,1,5, 3
int do_In(void)
{
    int i = 0;
    int n;

    if (!read_delim(":")) return err_expected("':'");

    while (-1)
    {
        if (!read_int(&n))
            return err_numberexp();
        if (n <= 0 && n > cur_ic->pins)
            return err_badpin(n);
        if ((n == cur_ic->gnd) || (n == cur_ic->plus))
            return err_pinpower();

        in[i] = n;
        incnt++;
/*
        curmask |= 1UL << (n-1);
        cur_ic->pinmask |= 1UL << (n-1);
*/
        i++;
        n = read_name(n);
        if (!n)
            return 0;
        if (n < 0)
            break;
    }
    return -1;
}

//        OUT: 10,9,8,7,13,11,12
int do_Out(void)
{
    int i = 0;
    int n;

    if (!read_delim(":")) return err_expected("':'");
    while (-1)
    {
        if (!read_int(&n))
        {
            if (token_type != DELIMITER || token[0] != '@')
                return err_syntax(token);
            cur_ic->isOK = 1;
            if (!read_int(&n))
                return err_numberexp();
        }
        if (n <= 0 && n > cur_ic->pins)
            return err_badpin(n);
        if ((n == cur_ic->gnd) || (n == cur_ic->plus))
            return err_pinpower();
        out[i] = n;
        outcnt++;
        curmask &= ~(1UL << (n-1));
        cur_ic->pinmask &= ~(1UL << (n-1));
        i++;
        n = read_name(n);
        if (!n)
            return 0;
        if (n < 0)
            break;
    }
    return -1;
}


int do_Set(void)
{
    CMDSET *set;
    CMDSETALL *all;
    unsigned long *mask;
    unsigned long inmask;
    int n, pin;

    if (!read_delim(":")) return err_expected("':'");
    if (strstr(ptr, "->"))
    {
        // 1: SET: 0 -> 12,13,15 [; 1-> 2,4,5]
        set = (CMDSET *) new_command(ICMD_SET, sizeof(CMDSET));
        do
        {
            // считываем номер маски
            if (!read_int(&n)) return err_numberexp();

            switch (n)
            {
                case 0: { mask = &set->mask0; break; }
                case 1: { mask = &set->mask1; break; }
                default: {
                    printf("  **ERROR**[%lu] bad mask bit number!\n", nrow);
                    return 0;
                }
            }
            // должен быть разделитель '->'
            if (!read_delim("-")) return err_expected("'->'");
            if (!read_delim(">")) return err_expected("'->'");
            // считываем номера бит для маски
            while (-1)
            {
                if (!read_pin(&pin))
                    return 0;
                pin--;
                // если установка бита на выход, то это в(ы)ключение подтяжки
                if ( (curmask & (1UL << pin)) == 0 )
                {
                    return err_syntax("set output pin");
                } else {
                    *mask |= 1UL << pin;
                }
                if (!read_delim(","))
                    break;
            }
        } while (token_type == DELIMITER && token[0] == ';');
        // проверяем нельзяли преобразовать в SET_ALL?
        inmask = set->mask0 | set->mask1 | 1UL << (cur_ic->gnd-1) | 1UL << (cur_ic->plus-1);
        if (curmask == inmask)
        {
            // изменение всех пинов, преобразуем в SET_ALL
            inmask = set->mask1;
            free(set);
            all = (CMDSETALL *) new_command(ICMD_SET_ALL, sizeof(CMDSETALL));
            all->mask = inmask;
            // добавляем команду в список
            add_command((CMD *) all);
        } else {
            // добавляем команду в список
            add_command((CMD *) set);
        }

    } else {
        // 2: SET: 01010011
        if (!read_int(&n))      // подгружаем битовую маску в token[]
            return err_numberexp();
        if (strlen(token) != incnt)
            return err_badpincount();
        all = (CMDSETALL *) new_command(ICMD_SET_ALL, sizeof(CMDSETALL));
        for (n = 0; n < incnt; n++)
        {
            pin = token[n]-'0';
            if (pin == 1)
            {
                all->mask |= 1UL << (in[n]-1);
            } else if (pin == 0)
            {

            } else {
                return err_badpinlogic();
            }
        }
        // добавляем команду в список
        add_command((CMD *) all);
    }

    return -1;
}


int do_Test(void)
{
    CMDTEST            *tst;
    CMDTESTALL         *all;
    CMDSETALLANDTEST   *sta;
    unsigned long       mask;
    unsigned long       pmask;
    int                 n, pin;

    if (!read_delim(":")) return err_expected("':'");
    if (strstr(ptr, "->"))
    {
        // TEST: 11,12,13,14 -> 1
        tst = (CMDTEST *) new_command(ICMD_TEST, sizeof(CMDTEST));
        do
        {
            mask = 0;
            while (-1)
            {
                if (!read_pin(&pin))      // считываем номера пинов
                    return 0;
                if ((pin <= 0) || (pin > cur_ic->pins))
                    return err_badpin(pin);
                pin--;
                if ( (curmask & (1UL << pin)) == 1 )
                    return err_syntax("test output pin");
                mask |= 1UL << pin;
                if (!read_delim(","))
                    break;
            }
            if (token_type != DELIMITER || token[0] != '-' || (!read_delim(">")))
                return err_expected("'->'");

            if (!read_int(&pin))
                return err_numberexp();

            switch (pin)
            {
                case 0: { tst->mask0 = mask; break; }
                case 1: { tst->mask1 = mask; break; }
                default: return err_badpinlogic();
            }
        } while (read_delim(";"));
        // проверяем нельзяли преобразовать в TEST_ALL?
        pmask = 0;
        for (n = 0; n < cur_ic->pins; n++)
            pmask |= (1 << n);
        mask = (~curmask) & pmask;
        if (mask == (tst->mask0 | tst->mask1))
        {
            // тестирование всех пинов, преобразуем в TEST_ALL
            mask = tst->mask1;
            free(tst);
            all = (CMDTESTALL *) new_command(ICMD_TEST_ALL, sizeof(CMDTESTALL));
            all->mask = mask;
            add_command((CMD *) all);
        } else {
            // добавляем команду в список
            add_command((CMD *) tst);
        }

    } else if (strstr(ptr, "=>"))
    {
        // TEST: 0000:1 => 0000001
        // формируем маску для установки пинов
        if (!read_int(&n))      // подгружаем битовую маску в token[]
            return err_numberexp();
        if (strlen(token) != incnt)
            return err_badpincount();
        sta = (CMDSETALLANDTEST *) new_command(ICMD_SET_ALL_AND_TEST, sizeof(CMDSETALLANDTEST));
        for (n = 0; n < incnt; n++)
        {
            pin = token[n]-'0';
            if (pin == 1)
            {
                sta->mset |= 1UL << (in[n]-1);
            } else if (pin == 0)
            {

            } else {
                return err_badpinlogic();
            }
        }
        // должен быть разделитель
        if (!read_delim("=")) return err_expected("'=>'");
        if (!read_delim(">")) return err_expected("'=>'");
        // формируем маску для проверки
        if (!read_int(&n))      // подгружаем битовую маску в token[]
            return err_numberexp();
        if (strlen(token) != outcnt)
            return err_badpincount();
        for (n = 0; n < outcnt; n++)
        {
            pin = token[n]-'0';
            if (pin == 1)
            {
                sta->mtst |= 1UL << (out[n]-1);
            } else if (pin == 0)
            {

            } else {
                return err_badpinlogic();
            }
        }
        // добавляем команду в список
        add_command((CMD *) sta);

    } else {
        // TEST: 0000000
        all = (CMDTESTALL *) new_command(ICMD_TEST_ALL, sizeof(CMDTESTALL));
        if (!read_int(&n))      // подгружаем битовую маску в token[]
            return err_numberexp();
        if (strlen(token) != outcnt)
            return err_badpincount();
        for (n = 0; n < outcnt; n++)
        {
            pin = token[n]-'0';
            if (pin == 1)
            {
                all->mask |= 1UL << (out[n]-1);
            } else if (pin == 0)
            {

            } else {
                return err_badpinlogic();
            }
        }
        // добавляем команду в список
        add_command((CMD *) all);
    }
    return -1;
}


//        CONFIG: 14,15,4, 9,10,11,6,7 -> IN; 12,13, 2,1, 5,3 -> OUT
int do_Config(void)
{
    CMDCONFIG    *cfg;
    unsigned long mask;
    int           n, i;
    char          pins[40];

    if (!read_delim(":")) return err_expected("':'");
    cfg = (CMDCONFIG *) new_command(ICMD_CONFIG, sizeof(CMDCONFIG));
    // формируем начальную маску
    for (i = 0; i < cur_ic->pins; i++)
        cfg->mask |= 1UL << i;
    // парсим новую конфигурацию входов/выходов
    do
    {
        memset(&pins, 0x00, sizeof(pins));
        mask = 0;
        i = 0;
        while (-1)
        {
            if (!read_pin(&n))        // считываем номера пинов
                return 0;
            if (n == cur_ic->gnd || n == cur_ic->plus || n < 1 || n > cur_ic->pins)
                return err_badpin(n);
            mask |= 1UL << (n - 1);
            pins[i] = n;
            i++;
            if (!read_delim(","))
                break;
        }

        if (token_type != DELIMITER || token[0] != '-' || (!read_delim(">")))
            return err_expected("'->'");
        if (!get_token())
            return err_expected("IN or OUT");
        if (token_type == COMMAND && (tok == TOK_IN || tok == TOK_OUT))
        {
            switch (tok)
            {
                case TOK_IN: { cfg->mask |= mask;
                               memcpy(&in, &pins, sizeof(in));
                               incnt = i;
                               break;
                             }
                case TOK_OUT:{ cfg->mask &= ~(mask);
                               memcpy(&out, &pins, sizeof(out));
                               outcnt = i;
                               break;
                             }
            }
        } else {
            return err_expected("IN or OUT");
        }

    } while (read_delim(";"));
/*
    // помечаем пины питания как входы
    cfg->mask |= 1UL << (cur_ic->gnd-1);
    cfg->mask |= 1UL << (cur_ic->plus-1);
*/
    // проверяем пины питания
    if ((cfg->mask & (1UL << (cur_ic->gnd-1)) == 0) || (cfg->mask & (1UL << (cur_ic->plus-1)) == 0))
        return err_pinpower();

    curmask = cfg->mask;
    // добавляем команду в список
    add_command((CMD *) cfg);
    return -1;
}

int do_Pulse(void)
{
    CMDPULSE *pls;
    int       pin;
    char      sign;
    int       count;

    if (!read_delim(":")) return err_expected("':'");

    pls = (CMDPULSE *) new_command(ICMD_PULSE, sizeof(CMDPULSE));
    if (!get_token())
        return err_syntax("unexpected end of line");
    if (token_type != DELIMITER)
        return err_expected("sign of pulse");
    switch (token[0])
    {
        case '-': { sign = -1; break; }
        case '+': { sign = 0; break; }
        default : { return err_expected("'-' or '+'"); }
    }
    if (!read_pin(&pin))        // считываем номера пинов
                return 0;
    if (pin < 1 || pin > cur_ic->pins || (curmask & (1UL << (pin-1)) == 0))
        return err_syntax("incorrect pin");

    if (read_delim("["))
    {
        // считываем количество импульсов
        if (!read_int(&count))
            return err_numberexp();
        if (!read_delim("]"))
            return err_expected("']'");
        if (count < 1 || count > 255)
            return err_syntax("count pulse must be range [1..255]");
    } else {
        count = 1;
    }
    if (sign)
        pin = -pin;
    pls->pin = pin;
    pls->cnt = count;
    // добавляем команду в список
    add_command((CMD *) pls);

    return -1;
}

int do_Pull(int up)
{
    CMD *pull;

    if (up)
        pull = (CMD *) new_command(ICMD_PULLUP, sizeof(CMD));
    else
        pull = (CMD *) new_command(ICMD_PULLDN, sizeof(CMD));
    add_command((CMD *) pull);
    return -1;
}



char read_ic(char *fname)
{
    char ret;

    file = fopen(fname, "rt");
    if (!file)
    {
        printf("ERROR: can't open file IC!\n");
        return 0;
    }
    nrow = 0;
    ic = NULL;
    cur_ic = NULL;
    memset(pname, 0x00, sizeof(pname));
    numdata = 0;

    while (!feof(file))
    {
        // считываем очередную строку
        fgets(buff, 512, file);
        ptr = buff;
        nrow++;

        while (get_token())
        {
            if (token_type == DELIMITER)
            {
                if (tok == EOL)
                    break;
                if (token[0] == '#')    // это коментарий, пропускаем строку
                    break;
            }
            if (token_type == COMMAND)
            {
                switch (tok)
                {
                    case TOK_CHIP:  { ret = do_Chip(); break; }
                    case TOK_POWER: { ret = do_Power(); break; }
                    case TOK_IN:    { ret = do_In(); break; }
                    case TOK_OUT:   { ret = do_Out(); break; }
                    case TOK_SET:   { ret = do_Set(); break; }
                    case TOK_TEST:  { ret = do_Test(); break; }
                    case TOK_CONFIG:{ ret = do_Config(); break; }
                    case TOK_PULSE: { ret = do_Pulse(); break; }
                    case TOK_PULLUP:{ ret = do_Pull(1); break; }
                    case TOK_PULLDN:{ ret = do_Pull(0); break; }
                }
                if (!ret)
                {
                    return 0;
                }
            } else {
                printf("  **ERROR**[%lu] unknown token!\n", nrow);
                return 0;
            }
        } // while
    }
    fclose(file);
    return -1;
}




/*****************************************************************************
******************************************************************************
                              ЗАПИСЬ ТЕСТОВ
******************************************************************************
******************************************************************************/

/*
  сравнение имен советских микросхем
*/
int tnamecmp(char *s1, char *s2)
{
    int n1, n2, n;

    n = memcmp(s1, s2, 2);
    if ((n != 0) || (strlen(s1) <= 2) || (strlen(s2) <= 2))
        return n;
    n1 = atoi(&s1[2]);
    n2 = atoi(&s2[2]);


    if (n1 < n2)
    {
        n = -1;
    } else if (n1 > n2)
    {
        n = 1;
    } else {
        n = 0;
    }
    return n;
}

/*
  добавляет в список имя микросхемы
*/
void lab_Insert(ICNAMES *n)
{
    ICNAMES *t = names;

    numic++;

    if (!t)
    {
        names = n;
        names->prev = NULL;
        names->next = NULL;
    } else {
        while (t)
        {
            if (tnamecmp(t->name, n->name) > 0)
            {
                if (t->prev)
                {
                    // вставляем перед t
                    n->prev = t->prev;
                    n->next = t;
                    t->prev->next = n;
                    t->prev = n;
                } else {
                    // вставляем в начало
                    n->next = t;
                    n->prev = NULL;
                    t->prev = n;
                    names = n;
                }
                return;
            }
            n->prev = t;
            t = t->next;
        }
        // добавляем в конец списка
        t = n->prev;
        t->next = n;
        n->next = NULL;
    }
}

/*
  Создает метку для теста и привязывает к ней имена микросхем
*/
void gen_Label(char *text)
{
    char *s, *e;
    char *str;
    ICNAMES *name;

    char label[32];

    sprintf(label, "%slab%04X", PREFIX, nlabel);
    nlabel++;
    fprintf(file, "const char %s[] = { // %s\n", label, text);
    str = strtok(text, ", \n\r");
    while (str)
    {

        name = malloc(sizeof(ICNAMES));
        memset(name, 0x00, sizeof(ICNAMES));
        name->label = strdup(label);

        s = strchr(str, '(');
        e = strchr(str, ')');
        if (s)
        {
            if (!e)
            {
                printf("  **ERROR** bad name '%s'\n", str);
                return;
            }
            *s++ = '\0';
            *e = '\0';
            name->name = strdup(str);
            // добавляем имя аналога
            while ((*s < '0') || (*s > '9'))
            {
                if (*s == '\0')
                    break;
                s++;
            }
            if (*s != '\0')
                name->analog = strdup(s);
        } else {
            name->name = strdup(str);
        }
        lab_Insert(name);
        str = strtok(NULL, ", \n\r");
    }
}


/*
  Возвращает тип следующей команды теста, или 0 если достигнут конец теста
*/
int next_cmd_type(CMD *cmd)
{
    if (cmd->next)
        return cmd->next->type;
    else
        return 0;
}


/*
  Сохраняет текущую маску пинов
*/
void save_mask(unsigned long mask)
{
    int i;
    char bin[12];

    i = 0;
    while (i < cur_ic->pins)
    {
        byte2bin(mask & 0xFF, bin);
#ifdef MC
        fprintf(file, "0b%s, ", bin);
#else
        fprintf(file, "b%s, ", bin);
#endif
        mask >>= 8;
        i += 8;
        totalsize++;
    }
}


/*
   Запись команды CMD_INIT
*/
char mk_cmd_init(void)
{
    char *cmd;

    curmask = cur_ic->pinmask;
    lastpulse = 0;
    switch (cur_ic->pins)
    {
        case 8:  { cmd = "CMD_INIT_8"; break; }
        case 14: { switch (cur_ic->gnd)
                   {
                       case 7:  { cmd = "CMD_INIT_14"; break; }
                       case 6:  { cmd = "CMD_INIT_14_1"; break; }
                       case 10: { cmd = "CMD_INIT_14_2"; break; }
                       case 11: { cmd = "CMD_INIT_14_3"; break; }
                   }
                   break;
                 }
        case 16: { switch (cur_ic->gnd)
                   {
                       case 7:  { cmd = "CMD_INIT_16_1"; break; }
                       case 8:  { cmd = "CMD_INIT_16";   break; }
                       case 12: { cmd = "CMD_INIT_16_2"; break; }
                       case 16: { cmd = "CMD_INIT_16_3"; break; }
                   }
                   break;
                 }
        case 18: { switch (cur_ic->gnd)
                   {
                       case 18: { cmd = "CMD_INIT_18_1"; break; }
                   }
                   break;
                 }
        case 20: { switch (cur_ic->gnd)
                   {
                       case 10: { cmd = "CMD_INIT_20"; break; }
                       case 8:  { cmd = "CMD_INIT_20_1"; break; }
                   }
                   break;
                 }
        case 24: { cmd = "CMD_INIT_24"; break; }
        default: { cmd = NULL; };
    }
    if (!cmd)
    {
        printf("  **WARNING** unknown corpuce on '%s'\n", cur_ic->name);
        return 0;
    }
    // генерируем метку и привязыавем к ней имена микросхем
    gen_Label(cur_ic->name);
    if (cur_ic->isOK)
        fprintf(file, "%s|FL_OC, ", cmd);
    else
        fprintf(file, "%s, ", cmd);
    save_mask(cur_ic->pinmask);
    fprintf(file, "\n");

    totalsize ++;
    return -1;
}

/*
  Запись команд CMD_SET_0, CMD_SET_1 и CMD_SET
*/
CMD *save_Set(CMD *cmd)
{
    unsigned long mask;
    CMDSET *c = (CMDSET *) cmd;

    if (c->mask0 == 0 && bitcnt(c->mask1) == 1)
    {
        fprintf(file, "CMD_SET_TO_1, %i, \n", bit2pin(c->mask1));
        totalsize += 2;
    } else if (c->mask1 == 0 && bitcnt(c->mask0) == 1)
    {
        fprintf(file, "CMD_SET_TO_0, %i, \n", bit2pin(c->mask0));
        totalsize += 2;
    } else {
        // CMD_SET_ALL
        fprintf(file, "CMD_SET, "); //, cur_ic->pins);
        save_mask(c->mask0);
        save_mask(c->mask1);
        fprintf(file, "\n");
    }
    totalsize ++;
    return cmd;
}

/*
  Запись команд CMD_SET_ALL и CMD_SET_ALL_AND_TEST
*/
CMD *save_SetAll(CMD *cmd)
{
    int n, i;
    unsigned long mask;
    CMDSETALL  *c = (CMDSETALL *) cmd;
    CMDTESTALL *ta;
    CMDTEST    *t;

    n = next_cmd_type(cmd);
    if (n == ICMD_TEST_ALL)
    {
        // объединяем две команды в одну
        ta = (CMDTESTALL *)c->cmd.next;
        fprintf(file, "CMD_SET_ALL_AND_TEST, "); //, cur_ic->pins);
        save_mask(c->mask);
        save_mask(ta->mask);
        fprintf(file, "\n");
        totalsize ++;
        return (CMD *)ta;
    }
    fprintf(file, "CMD_SET_ALL, "); //, cur_ic->pins);
    save_mask(c->mask);
    fprintf(file, "\n");
    totalsize ++;
    return cmd;
}

/*
  Запись команды CMD_TEST
*/
CMD *save_Test(CMD *cmd)
{
    CMDTEST *tst = (CMDTEST *) cmd;

    fprintf(file, "CMD_TEST, "); //, cur_ic->pins);
    save_mask(tst->mask0);
    save_mask(tst->mask1);
    fprintf(file, "\n");
    totalsize ++;
    return cmd;
}

/*
  Запись команды CMD_TEST_ALL
*/
CMD *save_TestAll(CMD *cmd)
{
    CMDTESTALL *all = (CMDTESTALL *) cmd;

    fprintf(file, "CMD_TEST_ALL, "); //, cur_ic->pins);
    save_mask(all->mask);
    fprintf(file, "\n");
    totalsize ++;
    return cmd;
}

/*
  Запись команды CMD_SET_ALL_AND_TEST
*/
CMD *save_SetAllAndTest(CMD *cmd)
{
    CMDSETALLANDTEST *ta = (CMDSETALLANDTEST *) cmd;

    fprintf(file, "CMD_SET_ALL_AND_TEST, "); //, cur_ic->pins);
    save_mask(ta->mset);
    save_mask(ta->mtst);
    fprintf(file, "\n");
    totalsize ++;
    return cmd;
}

/*
  Запись команд CMD_PULSE, CMD_LAST_PULSE, CMD_LAST_PULSE_AND_TEST и CMD_LAST_REPEAT_PULSE
*/
CMD *save_Pulse(CMD *cmd)
{
    int         n;
    CMDTESTALL *t;
    CMDPULSE   *p = (CMDPULSE *) cmd;

    if (p->pin == lastpulse)
    {
        // это либо LAST_PULSE, либо LAST_AND_TEST, либо LAST_REPEAT
        if (p->cnt > 1)
        {
            fprintf(file, "CMD_LAST_REPEAT_PULSE, %hu,\n", p->cnt);
            totalsize ++;
        } else {
            n = next_cmd_type(cmd);
            if (n == ICMD_TEST_ALL)
            {
                t = (CMDTESTALL *)p->cmd.next;
                fprintf(file, "CMD_LAST_PULSE_AND_TEST, "); //, cur_ic->pins);
                save_mask(t->mask);
                fprintf(file, "\n");
                totalsize ++;
                return (CMD *) t;
            } else {
                fprintf(file, "CMD_LAST_PULSE,\n");
            }
        }
    } else {
        // просто PULSE
        if (p->cnt > 1)
        {
            fprintf(file, "CMD_PULSE, %hi, \n", p->pin);
            lastpulse = p->pin;
            fprintf(file, "CMD_LAST_REPEAT_PULSE, %hi, \n", p->cnt-1);
        } else {
            fprintf(file, "CMD_PULSE, %hi, \n", p->pin);
            lastpulse = p->pin;
        }
    }
    totalsize ++;
    return cmd;
}

/*
  Запись команды CMD_CONFIG
*/
CMD *save_Config(CMD *cmd)
{
    CMDCONFIG *c = (CMDCONFIG *) cmd;

    fprintf(file, "CMD_CONFIG, "); //, cur_ic->pins);
    save_mask(c->mask);
    fprintf(file, "\n");
    totalsize ++;
    return cmd;
}

/*
  Запись команды CMD_PULLUP
*/
CMD *save_PullUp(CMD *cmd)
{
    fprintf(file, "CMD_PULLUP,\n");
    totalsize ++;
    return cmd;
}

/*
  Запись команды CMD_PULLDN
*/
CMD *save_PullDn(CMD *cmd)
{
    fprintf(file, "CMD_PULLDN,\n");
    totalsize ++;
    return cmd;
}



void save_data(char *fname)
{
    CMD *cmd;

    file = fopen(fname, "wt");
    if (!file)
    {
        printf("ERROR: can't create file '%s'!\n", fname);
        return;
    }
    cur_ic = ic;
    names = NULL;
    nlabel = 0;
    numic = 0;
    totalsize = 0;

    while (cur_ic)
    {
        // формируем команду CMD_INIT
        if (mk_cmd_init())
        {
            cmd = cur_ic->list;
            while (cmd)
            {
                switch (cmd->type)
                {
                    case ICMD_SET: { cmd = save_Set(cmd); break; }
                    case ICMD_SET_ALL: { cmd = save_SetAll(cmd); break; }
                    case ICMD_TEST: { cmd = save_Test(cmd); break; }
                    case ICMD_TEST_ALL: { cmd = save_TestAll(cmd); break; }
                    case ICMD_SET_ALL_AND_TEST: { cmd = save_SetAllAndTest(cmd); break; }
                    case ICMD_PULSE: {cmd = save_Pulse(cmd); break; }
                    case ICMD_CONFIG: {cmd = save_Config(cmd); break; }
                    case ICMD_PULLUP: {cmd = save_PullUp(cmd); break; }
                    case ICMD_PULLDN: {cmd = save_PullDn(cmd); break; }
                    default: {
                        printf("  **WARNING** unknown command: %hi\n", cmd->type);
                        break;
                    }
                } // cmd->type

                cmd = cmd->next;
            }
            fprintf(file, "CMD_END };\n");
        }
        cur_ic = cur_ic->next;
    }
    fclose(file);
    return;
}



/*****************************************************************************
******************************************************************************
                            ЗАПИСЬ МЕНЮ
******************************************************************************
******************************************************************************/

// DOS                       //А   Б   В   Г   Д   Е   Ж   З   И   Й   К   Л   М   Н   О   П   Р   С   Т   У   Ф   Х   Ц   Ч   Ш   Щ   Ъ   Ы   Ь   Э   Ю   Я
const unsigned char xlat[] = {'A','B','V','G','D','E','J','Z','I','I','K','L','M','N','O','P','R','S','T','U','F','X','C','W','H','H','-','-','-','-','Q','Y',
                              'a','b','v','g','d','e','j','z','i','i','k','l','m','n','o','p','r','s','t','u','f','x','c','w','h','h','-','-','-','-','q','y',};

void rus2lat(unsigned char src[], unsigned char dst[])
{
    int len, i;

    len = strlen(src);
    strcpy(dst, src);
    for (i = 0; i < len; i++)
    {
//        if (src[i] >= 0x80 && src[i] <= 0x9F)  // !DOS!
//            dst[i] = xlat[src[i] - 0x80];

        if (src[i] >= 0xC0)  // !WIN!
            dst[i] = xlat[src[i] - 0xC0];
        else
            dst[i] = src[i];
    }
}


/*
  запись количества пунктов подменю
*/
void mnu_Defines(char *name)
{
    ICNAMES *t = names;
    char     lat[4];
    int      count = 1;

    rus2lat(name, lat);
    while (t)
    {
        if (!memcmp(t->name, name, 2))
        {
            count++;
        }
        t = t->next;
    }
    fprintf(file, "#define %slen%s    %i\n", PREFIX, lat, count);
}

/*
  запись списка меток одного подменю
  (const char * const tlabAP[] = { 0,tlabXXXX,tlabXXXX,tlabXXXX,...}; )
*/
void mnu_SubLabels(char *name)
{
    ICNAMES *t = names;
    char     lat[4];

    rus2lat(name, lat);
    fprintf(file, "const char * const %slab%s[] = { 0", PREFIX, lat);
    while (t)
    {
        if (!memcmp(t->name, name, 2))
        {
            fprintf(file, ",%s", t->label);
        }
        t = t->next;
    }
    fprintf(file, " };\n");
}


/*
  запись списка имен одного подменю
  (const char * const titemAP[] = { "#$","АП3","АП4","АП5",.... }; )
*/
int mnu_SubNames(char *name)
{
    ICNAMES *t = names;
    char     lat[4];
    int      count = 1;

    rus2lat(name, lat);
    fprintf(file, "const char * const %sitem%s[] = { \"#$\"", PREFIX, lat);
    while (t)
    {
        if (!memcmp(t->name, name, 2))
        {
            fprintf(file, ",\"%s\"", t->name);
            count++;
        }
        t = t->next;
    }
    fprintf(file, " };\n");
    return count;
}

/*
  запись подменю
  (const MENU tmnuAP = { titemAP, 0x80, tlenAP, (MENU **) tlabAP }; )
*/
void mnu_SubMenu(char *name)
{
    char     lat[4];

    rus2lat(name, lat);

    fprintf(file, "const MENU %smnu%s = { %sitem%s, 0x80, %slen%s, (MENU **) %slab%s };\n",
                   PREFIX,lat, PREFIX,lat, PREFIX,lat, PREFIX,lat);
}


void mnu_SaveSubMenu(void)
{
    char     str[4];
    int      cnt = 1;
    ICNAMES *t;

    // пишем #define xxxx
    t = names;
    while (t)
    {
        memset(str, 0x00, sizeof(str));
        memcpy(str, t->name, 2);
        mnu_Defines(str);
        cnt++;
        while (t && !memcmp(t->name, str, 2))
            t = t->next;
    }
    fprintf(file, "#define %slen%s    %i\n", PREFIX, MNUNAME, cnt);

    // пишем списки меток
    t = names;
    while (t)
    {
        memset(str, 0x00, sizeof(str));
        memcpy(str, t->name, 2);
        mnu_SubLabels(str);
        while (t && !memcmp(t->name, str, 2))
            t = t->next;
    }
    // пишем списки имен
    t = names;
    while (t)
    {
        memset(str, 0x00, sizeof(str));
        memcpy(str, t->name, 2);
        mnu_SubNames(str);
        while (t && !memcmp(t->name, str, 2))
            t = t->next;
    }
    // пишем подменю
    t = names;
    while (t)
    {
        memset(str, 0x00, sizeof(str));
        memcpy(str, t->name, 2);
        mnu_SubMenu(str);
        while (t && !memcmp(t->name, str, 2))
            t = t->next;
    }
}


/*
  запись списка имен меню
  const char * const titemTTL[] = { "#$","АП", "ИВ", ... };
*/
void mnu_Names(void)
{
    char     str[4];
    ICNAMES *t = names;

    fprintf(file, "const char * const %sitem%s[] = { \"#$\"",PREFIX, MNUNAME );
    while (t)
    {
        memset(str, 0x00, sizeof(str));
        memcpy(str, t->name, 2);
        fprintf(file, ",\"%s\"", str);
        while (t && !memcmp(t->name, str, 2))
            t = t->next;
    }
    fprintf(file, "};\n");
}

/*
const MENU * const tlabTTL[] = { 0,&tmnuAP, &tmnuIV, &tmnuID, &tmnuIE, &tmnuIM, &tmnuIP, &tmnuIR, &tmnuKP, &tmnuLA, &tmnuLE, &tmnuLI, &tmnuLL, &tmnuLN, &tmnuLP, &tmnuLR, &tmnuPR, &tmnuRU, &tmnuSP, &tmnuTV, &tmnuTL, &tmnuTM, &tmnuTR };
*/

void mnu_Labels(void)
{
    char     str[4];
    ICNAMES *t = names;
    char     lat[4];


    fprintf(file, "const MENU * const %slab%s[] = { 0", PREFIX, MNUNAME);
    while (t)
    {
        memset(str, 0x00, sizeof(str));
        memcpy(str, t->name, 2);
        rus2lat(str, lat);
        fprintf(file, ",&%smnu%s", PREFIX, lat);
        while (t && !memcmp(t->name, str, 2))
            t = t->next;
    }
    fprintf(file, "};\n");


}

/*
const MENU mnuTTL = { titemTTL, 0, tlenTTL, tlabTTL };
*/
void mnu_Menu(void)
{
    fprintf(file, "const MENU mnu%s = { %sitem%s, 0, %slen%s, %slab%s  };\n", MNUNAME, PREFIX,MNUNAME, PREFIX,MNUNAME, PREFIX,MNUNAME);
}


void mnu_SaveMenu(void)
{
    mnu_Names();
    mnu_Labels();
    mnu_Menu();
}


void save_menu(char *fname)
{
    file = fopen(fname, "wt");
    if (!file)
    {
        printf("ERROR: can't create file '%s'!\n", fname);
        return;
    }
    mnu_SaveSubMenu();
    mnu_SaveMenu();

    fclose(file);
}



/*****************************************************************************
******************************************************************************
                         ЗАПИСЬ МЕНЮ БУРЖУЙСКИХ М/С
******************************************************************************
******************************************************************************/

typedef struct {
    int     min;
    int     max;
} RANGE;

#if defined GEN_TTL
RANGE icrange[] = {{0, 19}, {20, 49}, {50, 99}, {100, 149}, {150, 189}, {190, 279}, {280, 599}, {600, 9000}};
#elif defined GEN_CMOS
RANGE icrange[] = {{4000, 4499}, {4500, 9000}};
#elif defined GEN_580
RANGE icrange[] = {{0, 1}};
#elif defined GEN_514
RANGE icrange[] = {{0, 1}};
#else
  #error Please define menu type!
#endif

/*
  сортирует список имен буржуйских микросхем
*/
void lab_SortAnalog(void)
{
    ICNAMES *i, *j;
    unsigned char *t;
    unsigned int ncur, nnext;

        if (!names)
            return;
        i = names->next;
        while (i)
        {
            j = names;
            while (j->next)
            {
                ncur = atoi(j->analog);
                nnext = atoi(j->next->analog);
                if (ncur > nnext)
                {
                    // swap name
                    t = j->name;
                    j->name = j->next->name;
                    j->next->name = t;
                    // sawp analog
                    t = j->analog;
                    j->analog = j->next->analog;
                    j->next->analog = t;
                    // swap label
                    t = j->label;
                    j->label = j->next->label;
                    j->next->label = t;
                }
                j = j->next;
        }
        i = i->next;
    }
}

/*
  запись количества пунктов подменю
*/
void mnu_DefinesAnalog(int min, int max)
{
    ICNAMES     *t = names;
    unsigned int n;
    int          count = 1;

    while (t)
    {
        if (t->analog)
        {
            n = atoi(t->analog);
            if ((n >= min) && (n <= max))
                count++;
        }
        t = t->next;
    }
    fprintf(file, "#define %slen%i_%i    %i\n", PREFIX, min, max, count);
}

/*
  запись списка меток одного подменю
  (const char * const tlab0_19[] = { 0,tlabXXXX,tlabXXXX,tlabXXXX,...}; )
*/
void mnu_SubLabelsAnalog(int min, int max)
{
    ICNAMES *t = names;
    unsigned int n;

    fprintf(file, "const char * const %slab%i_%i[] = { 0", PREFIX, min, max);
    while (t)
    {
        if (t->analog)
        {
            n = atoi(t->analog);
            if ((n >= min) && (n <= max))
            {
                fprintf(file, ",%s", t->label);
            }
        }
        t = t->next;
    }
    fprintf(file, " };\n");
}

/*
  запись списка имен одного подменю
  (const char * const titem0_19[] = { "#$","00","01","02",.... }; )
*/
void mnu_SubNamesAnalog(int min, int max)
{
    ICNAMES *t = names;
    unsigned int n;

    fprintf(file, "const char * const %sitem%i_%i[] = { \"#$\"", PREFIX, min, max);
    while (t)
    {
        if (t->analog)
        {
            n = atoi(t->analog);
            if ((n >= min) && (n <= max))
            {
                fprintf(file, ",\"%s\"", t->analog);
            }
        }
        t = t->next;
    }
    fprintf(file, " };\n");
}

/*
  запись подменю
  (const MENU tmnu0_19 = { titem0_19, 0x80, tlen0_19, (MENU **) tlab0_19 }; )
*/
void mnu_SubMenuAnalog(int min, int max)
{
    fprintf(file, "const MENU %smnu%i_%i = { %sitem%i_%i, 0x80, %slen%i_%i, (MENU **) %slab%i_%i };\n",
                   PREFIX,min,max, PREFIX,min,max, PREFIX,min,max, PREFIX,min,max);
}


void mnu_SaveSubMenuAnalog(void)
{
    int      maxrange = sizeof(icrange) / sizeof(icrange[0]);
    int      i;

    // пишем #define xxxx
    for (i = 0; i < maxrange; i++)
        mnu_DefinesAnalog(icrange[i].min, icrange[i].max);
    fprintf(file, "#define %slen%s    %i\n", PREFIX, ANALOGNAME, maxrange+1);
    // пишем списки меток
    for (i = 0; i < maxrange; i++)
        mnu_SubLabelsAnalog(icrange[i].min, icrange[i].max);
    // пишем списки имен
    for (i = 0; i < maxrange; i++)
        mnu_SubNamesAnalog(icrange[i].min, icrange[i].max);
    // пишем подменю
    for (i = 0; i < maxrange; i++)
        mnu_SubMenuAnalog(icrange[i].min, icrange[i].max);
}



void mnu_SaveMenuAnalog(void)
{
    int      maxrange = sizeof(icrange) / sizeof(icrange[0]);
    int      i;

    // запись списка имен меню
    // const char * const titemSN74[] = { "#$","0-19", "20-49", ... };
    fprintf(file, "const char * const %sitem%s[] = { \"#$\"",PREFIX, ANALOGNAME );
    for (i = 0; i < maxrange; i++)
        fprintf(file, ",\"%i+\"", icrange[i].min);
    fprintf(file, "};\n");
    // пишем названия меток типов микросхем
    // const MENU * const tlabSN74[] = { 0,&tmnu0_19, &tmnu20_49, ... };
    fprintf(file, "const MENU * const %slab%s[] = { 0", PREFIX, ANALOGNAME);
    for (i = 0; i < maxrange; i++)
        fprintf(file, ",&%smnu%i_%i", PREFIX, icrange[i].min, icrange[i].max);
    fprintf(file, "};\n");
    // собственно главное меню
    // const MENU mnuSN74 = { titemSN74, 0, tlenSN74, tlabSN74 };
    fprintf(file, "const MENU mnu%s = { %sitem%s, 0, %slen%s, %slab%s  };\n", ANALOGNAME, PREFIX,ANALOGNAME, PREFIX,ANALOGNAME, PREFIX,ANALOGNAME);
}

void save_analog(char *fname)
{
    ICNAMES *t;

    if (*fname == '\0')
        return;
    file = fopen(fname, "wt");
    if (!file)
    {
        printf("ERROR: can't create file '%s'!\n", fname);
        return;
    }
    lab_SortAnalog();
/*
    t = names;
    while (t)
    {
        fprintf(file, "%s\t\t%s\n",t->analog, t->label);

        t = t->next;
    }
    return;
*/
    mnu_SaveSubMenuAnalog();
    mnu_SaveMenuAnalog();
    fclose(file);
}


void main(void)
{
//    tnamecmp("└╧12", "╥┬1");
//    tnamecmp("└╧12", "└╧2");
    printf("IC to H converter.\n");
    if (read_ic(INFILE))
    {
        save_data(OUTDATA);
        save_menu(OUTMENU);
        printf("  -total ic: %i\n  -total tests: %i\n  -total size test: %u\n", numic, numdata, totalsize);
        save_analog(OUTANALOG);
    }
    ic = free_ic(ic);
}

