#include "stm32f10x.h" 
#include "gpio_driver.h"
#include "uart.h"
#include "systick_timer.h"

# define adc1 1
# define adc2 2

enum channels
{
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0, PB1,
    PC0, PC1, PC2, PC3, PC4, PC5,
    temp_sensor
};


char ADC_INIT(short port, short pin, short adc);
char ADC_check(short adc );
int Read_ADC(short port,short pin,short adc );

void Enable_ADC_interupt(short *ADCST,short adc );
void Enable_ADC_Watchdog(short adc, long LTR, long HTR,short *WDT);

void adc_multi_ch_init(char adc, char channels, char * adc_channels);

void adc_multi_ch_rx(char adc, char channels, char * adc_channels, int * analog_rx);