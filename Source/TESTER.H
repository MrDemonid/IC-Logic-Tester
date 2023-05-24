#ifndef _TESTER_H_
#define _TESTER_H_

// флаги
#define FL_OC                       0x80    // флаг наличия выводов с открытым коллектором
#define FL_DRAM                     0x40    // флаг режима тестирования динамической памяти
#define FL_SRAM                     0x20    // флаг режима тестирования статической памяти
#define FL_MASKDIPTYPE              0x1F    // маска для выделения типа корпуса

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
#define CMD_INIT_20_2               13  // +10 -20
#define CMD_INIT_24                 14  // +24 -12

#define CMD_SET_TO_0                15  // (pin_no8) устанавливает ноль на одном пине
#define CMD_SET_TO_1                16  // (pin_no8) устанавливает единицу на одном пине
#define CMD_SET                     17  // (mask0[n], mask1[n])
#define CMD_SET_ALL                 18  // (mask[n]) устанавливает все входы
#define CMD_TEST                    19  // (mask0[n], mask1[n]) тест отдельных выходов
#define CMD_TEST_ALL                20  // (mask[n]) тест всех выходов
#define CMD_SET_ALL_AND_TEST        21  // (smask[n], tmask[n]) установка всех входов и тест всех выходов
#define CMD_PULSE                   22  // (pin_no8)
#define CMD_LAST_PULSE              23  //
#define CMD_LAST_PULSE_AND_TEST     24  // (mask[n])
#define CMD_LAST_REPEAT_PULSE       25  // (count8)
#define CMD_CONFIG                  26  // (in_out_mask[n])
#define CMD_PULLUP                  27
#define CMD_PULLDN                  28

#define CMD_END                     29



typedef union {
  unsigned long l;
  unsigned char CHARs[4];       // Char 4 (0x0:LLo; 0x1:LHi; 0x2:HLo; 0x3:HHi;)
  struct {                      // Nibbles x 8 bits
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
  } BYTEs;
  struct {                      // Nibbles x 16 bits
    unsigned int iLo;           // IntL
    unsigned int iHi;           // IntH
  } INTs;
  struct {                      // BITs
    unsigned b00 :1;
    unsigned b01 :1;
    unsigned b02 :1;
    unsigned b03 :1;
    unsigned b04 :1;
    unsigned b05 :1;
    unsigned b06 :1;
    unsigned b07 :1;
    unsigned b08 :1;
    unsigned b09 :1;
    unsigned b10 :1;
    unsigned b11 :1;
    unsigned b12 :1;
    unsigned b13 :1;
    unsigned b14 :1;
    unsigned b15 :1;
    unsigned b16 :1;
    unsigned b17 :1;
    unsigned b18 :1;
    unsigned b19 :1;
    unsigned b20 :1;
    unsigned b21 :1;
    unsigned b22 :1;
    unsigned b23 :1;
    unsigned b24 :1;
    unsigned b25 :1;
    unsigned b26 :1;
    unsigned b27 :1;
    unsigned b28 :1;
    unsigned b29 :1;
    unsigned b30 :1;
    unsigned b31 :1;
  } BITs;
} LONG;


extern bit dbgmode;
extern char const   *ic_test;    // указатель на данные теста


void tst_PowerON(void);             // включение питания и настройка TRIS
void tst_PowerOFF(void);            // выключение питания микросхемы

void tst_RunTest(const char *name, const char *data); // тест микросхемы


#endif
