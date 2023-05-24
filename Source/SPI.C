#include <htc.h>

#include "SPI.H"

/*
Настрока и включение модуля SPI
*/
void spi_Init(unsigned char sync_mode, unsigned char data_sample, unsigned char clock_idle, unsigned char transmit_edge)
{
    SSPCON1 = sync_mode & 0x0F;     // SSPEN = 0, SSPM = sync_mode
    CKP = clock_idle;               // CKP = CLK_IDLE_LOW or CLK_IDLE_HIGH
    SMP = data_sample;              // SMP = DATA_SAMPLE_MIDDLE or DATA_SAMPLE_END
    CKE = transmit_edge;            // CKE = HIGH_2_LOW or LOW_2_HIGH
    switch (sync_mode){
        case SLAVE_SS_ENABLE:
//            ADCON1 |= 0b00000100;   // define A5 pin as digital
            _TRISSS = 1;            // define /SS pin as input
        case SLAVE_SS_DISABLE:
            _TRISSCK = 1;           // define clock pin as input
            SMP = 0;                // must be cleared in slave SPI mode
            break;
        default:                    // master mode
            _TRISSCK = 0;           // define clock pin as output
            break;
    }
    _TRISSDO = 0;                   // define SDO as output (master or slave)
    _TRISSDI = 1;                   // define SDI as input (master or slave)
    SSPEN = 1;                      // enable SPI
}

/*
Настройка модуля SPI по дефолту
*/
void spi_Default()
{
    spi_Init(MASTER_OSC_DIV4, DATA_SAMPLE_MIDDLE, CLK_IDLE_LOW, LOW_2_HIGH);
}

//spi_Init(MASTER_OSC_DIV4, DATA_SAMPLE_MIDDLE, CLK_IDLE_HIGH, LOW_2_HIGH);

void spi_Send(unsigned char b)
{
    SSPBUF = b;
    while (!BF);
}

unsigned char spi_Recv()
{
    while (!BF);
    return (SSPBUF);
}

