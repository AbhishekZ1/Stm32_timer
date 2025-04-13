#include "adc_driver.h"
/*
  PA0 - ADC12 0
	PA1 - ADC12 1
  PA2 - ADC12 2
	PA3 - ADC12 3
	PA4 - ADC12 4
	PA5 - ADC12 5
	PA6 - ADC12 6
  PA7 - ADC12 7
  
  PB0 - ADC12 8
  PB1 - ADC12 9

  PC0 - ADC12 10
	PC1 - ADC12 11
	PC2 - ADC12 12
	PC3 - ADC12 13
	PC4 - ADC12 14
	PC5 - ADC12 15
	
*/
char ADC_INIT(short port,short pin,short adc )
{
	char result =0;
	char channel ;
	if (port ==1)
	{
	  if (pin <8)
		{
			result =1;
			channel= pin;
		}

	}
	else if (port ==2)
	{
	  if (pin <2)
		{
			result =1;
			channel= 8+pin;
		}

	}
	else if (port ==3)
	{
	  if (pin <8)
		{
			result =1;
			channel= 10+pin;
		}
	}
	if (result)
	{
	  Gpio_init(port,pin,IN,I_GP_AI);
		if (adc==adc1)
		{
		 RCC->APB2ENR |= 0x201;
		 ADC1->CR2 =0;
		 ADC1->SQR3 = channel;
		 ADC1->CR2 |=1;
		 delay(100);
		 ADC1->CR2 |=1;
		 ADC1->CR2 |=2;


			
		}
		if (adc==adc2)
		{
		 RCC->APB2ENR |= 0x401;
		 ADC2->CR2 =0;
		 ADC2->SQR3 = channel;
		 ADC2->CR2 |=1;
		 delay(100);
		 ADC2->CR2 |=1;
		 ADC2->CR2 |=2;
		}
	
	}
	
	return result;
	
}

char ADC_check(short adc )
{
  char check =0;
	
	if (adc==adc1)
		{
		 if (ADC1->SR& 2)
		 {
			check=1;
		 }
		}
		if (adc==adc2)
		{
		  if (ADC2->SR& 2)
		 {
			check=1;
		 }
		}
	return check;

}
int Read_ADC(short port,short pin,short adc )
	{
    int data = 0;
		int result = 0;
		if (adc==adc1)
		{
		
			data= ADC1->DR;
		 
		}
		if (adc==adc2)
		{
			data=ADC2->DR;
		}
		result = (data*1000)/0xfff;
		return result;
	}
	
void Enable_ADC_interupt(short *ADCST,short adc )
	{
	 if (adc==adc1)
		 {
			ADC1->CR1 |=0x20;
			__disable_irq();
			NVIC_EnableIRQ(ADC1_2_IRQn);
			__enable_irq();
			 *ADCST=1;
		 }
		if (adc==adc2)
		 {
			ADC2->CR1 |=0x20;
			__disable_irq();
			NVIC_EnableIRQ(ADC1_2_IRQn);
			__enable_irq();
			*ADCST=1;
		 }

	}
void Enable_ADC_Watchdog(short adc, long LTR, long HTR,short *WDT)
	{
    if (adc==adc1)
			{
			 ADC1->CR1 |=0x800000;
			 ADC1->HTR =HTR;
			 ADC1->LTR=LTR;
			 ADC1->CR1 |=0x40;
			 __disable_irq();
				NVIC_EnableIRQ(ADC1_2_IRQn);
			 __enable_irq();
			 *WDT=1;
			}
		if (adc==adc2)
			{
			 ADC2->CR1 |=0x800000;
			 ADC2->HTR =HTR;
			 ADC2->LTR=LTR;
			 ADC2->CR1 |=0x40;
			 __disable_irq();
				NVIC_EnableIRQ(ADC1_2_IRQn);
			 __enable_irq();
			 *WDT=1;
			}
		
	}
void adc_multi_ch_init(char adc, char channels, char * adc_channels)
{
    int i = 0;

    // Initiate the pins
    for(i = 0; i < channels; i++)
    {
        if(adc_channels[i] < 8)
        {
            Gpio_init(PortA, adc_channels[i], IN, I_GP_AI);
        }
        else if(adc_channels[i] < 10)
        {
            Gpio_init(PortB, adc_channels[i] - 8, IN, I_GP_AI);
        }
        else if(adc_channels[i] < 16)
        {
            Gpio_init(PortC, adc_channels[i] - 10, IN,I_GP_AI);
        }
    }

    if(adc == 1)
    {
        // Setup the control registers
        RCC->APB2ENR |= 0x0201;
        ADC1->CR2 = 0;
        ADC1->CR2 |= 1;
        delay(100);
			  ADC1->SQR3 =adc_channels[1];
        ADC1->CR2 |= 2; // Continuous readings
        ADC1->CR2 |= 1; // Starting the conversion
    }
    else if(adc == 2)
    {
        // Setup the control registers
        RCC->APB2ENR |= 0x0401;
        ADC2->CR2 = 0;
        ADC2->CR2 |= 1;
        delay(100);
			  ADC2->SQR3 =adc_channels[1];
        ADC2->CR2 |= 2; // Continuous readings
        ADC2->CR2 |= 1; // Starting the conversion
    }
}
void adc_multi_ch_rx(char adc, char channels, char * adc_channels, int * analog_rx)
{
    int i = 0;
    int temp_rx = 0;
    if(adc == 1)
    {
        while(1)
        {
            if(ADC_check(adc1))
            {
                temp_rx = ADC1->DR;
                analog_rx[i] = (temp_rx * 1000) / 0xFFF;
                i++;
            }
            if(i == channels)
            {
                i = 0;
                ADC1->SQR1 = adc_channels[i];
                break;
            }
            else
            {
                ADC1->SQR1 = adc_channels[i];
            }
        }
    }
		else if(adc == 1)
			{
					while(1)
					{
							if(ADC_check(adc2))
							{
									temp_rx = ADC2->DR;
									analog_rx[i] = (temp_rx * 1000) / 0xFFF;
									i++;
							}
							if(i == channels)
							{
									i = 0;
									ADC2->SQR1 = adc_channels[i];
									break;
							}
							else
							{
									ADC2->SQR1 = adc_channels[i];
							}
					}
			}
}
