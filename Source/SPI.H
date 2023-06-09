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

// ��������� ��� spi_Init()
// Sync mode
#define MASTER_OSC_DIV4     0x00    // �������, ������� ������������� = FOSC/4
#define MASTER_OSC_DIV16    0x01    // �������, ������� ������������� = FOSC/16
#define MASTER_OSC_DIV64    0x02    // �������, ������� ������������� = FOSC/64
#define MASTER_TMR2         0x03    // �������, ������� ������������� ������ TMR2
#define SLAVE_SS_ENABLE     0x04    // �������, �������� ����� /SS (Slave Select)
#define SLAVE_SS_DISABLE    0x05    // �������, �������� ����� /SS (Slave Select)
#define MODE_MAX            0x05    // ������������ �������� �������� SSPM
// Data sample, ���������� ����� ������������� ������� ������
#define DATA_SAMPLE_MIDDLE  0x00    // ������� ������ ������������� � �������� ��������� ��������������
#define DATA_SAMPLE_END     0x01    // ������� ������ ������������� � ����� ��������� ��������������
//Clock idle, ���������� ��������� ������ ������������� ��� ���������� ������ (���������� ��������� �������)
#define CLK_IDLE_LOW        0x00    // ��������� ������ ������� �������
#define CLK_IDLE_HIGH       0x01    // ��������� ������� ������� �������
// Transmit edge, ���������� ������ �������� ������
#define HIGH_2_LOW          0x00    // ������ ���������� �� ������
#define LOW_2_HIGH          0x01    // ������ ���������� �� �����


/*
�������� � ��������� ������ SPI
*/
void spi_Init(unsigned char sync_mode, unsigned char data_sample, unsigned char clock_idle, unsigned char transmit_edge);

/*
��������� ������ SPI �� �������
*/
void spi_Default();

// ��������� �����
void spi_Send(unsigned char b);

// ����� �����
unsigned char spi_Recv();

#endif
