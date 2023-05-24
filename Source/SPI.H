#ifndef _SPI_H_
#define _SPI_H_

// SPI pins
#define _TRISSS             TRISA5  // pin /SS
#define _SS                 RA5
#define _TRISSCK            TRISC3  // pin SCK
#define _SCK                RC3
#define _TRISSDI            TRISC4  // pin SDI
#define _SDI                RC4
#define _TRISSDO            TRISC5  // pin SDO
#define _SDO                RC5

// Константы для spi_Init()
// Sync mode
#define MASTER_OSC_DIV4     0x00    // Ведущий, частота синхронизации = FOSC/4
#define MASTER_OSC_DIV16    0x01    // Ведущий, частота синхронизации = FOSC/16
#define MASTER_OSC_DIV64    0x02    // Ведущий, частота синхронизации = FOSC/64
#define MASTER_TMR2         0x03    // Ведущий, частоту синхронизации задает TMR2
#define SLAVE_SS_ENABLE     0x04    // Ведомый, разрешен вывод /SS (Slave Select)
#define SLAVE_SS_DISABLE    0x05    // Ведомый, запрещен вывод /SS (Slave Select)
#define MODE_MAX            0x05    // максимальное значение регистра SSPM
// Data sample, определяет когда строббируются входные данные
#define DATA_SAMPLE_MIDDLE  0x00    // входные данные строббируются в середине интервала синхроимпульса
#define DATA_SAMPLE_END     0x01    // входные данные строббируются в конце интервала синхроимпульса
//Clock idle, определяет состояние вывода синхронизации при отсутствии обмена (полярность тактового сигнала)
#define CLK_IDLE_LOW        0x00    // пассивный низкий уровень сигнала
#define CLK_IDLE_HIGH       0x01    // пассивный высокий уровень сигнала
// Transmit edge, определяет начало передачи данных
#define HIGH_2_LOW          0x00    // данные передаются по фронту
#define LOW_2_HIGH          0x01    // данные передаются по спаду


/*
Настрока и включение модуля SPI
*/
void spi_Init(unsigned char sync_mode, unsigned char data_sample, unsigned char clock_idle, unsigned char transmit_edge);

/*
Настройка модуля SPI по дефолту
*/
void spi_Default();

// перксылка байта
void spi_Send(unsigned char b);

// прием байта
unsigned char spi_Recv();

#endif
