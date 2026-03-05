/*
 * my_main.cpp
 *
 *  Created on: Mar 2, 2026
 *      Author: bagob
 */

#include <my_main.h>

#define ADC_BUF_LEN 200
#define ADC_NO_CONV 10
#define ADC_CH 0 			// 0-9


static uint16_t ADC_buf[ADC_BUF_LEN];
static int ADC_buf_read_adress = 0;
static int ADC_new_data = 0;

MainFunctions Main = {0};



void my_main_init(MainFunctions* M)
{
	M->ADC_Start_DMA((uint32_t*)ADC_buf,ADC_BUF_LEN);

	Main.UART_SendString = M->UART_SendString;
}


void my_main_loop(void)

{
	if(ADC_new_data)		// only for monitoring and debugging
	{
		for(int i = ADC_CH; i < (ADC_BUF_LEN / 2); i+= ADC_NO_CONV)
		{
			uint8_t send_buffer[2];

			send_buffer[0] = (uint8_t) ADC_buf[ADC_buf_read_adress + i];
			send_buffer[1] = (uint8_t) (ADC_buf[ADC_buf_read_adress + i] >> 8);

			Main.UART_SendString((uint8_t*)send_buffer, 2);
		}

		ADC_new_data = 0;
	}
}

void my_main_IT(IT_Types type)
{
	switch(type)
	{
	case ADC_FULL_COMPLETE :

		ADC_buf_read_adress = ADC_BUF_LEN / 2;
		ADC_new_data = 1;
		break;

	case ADC_HALF_COMPLETE :

		ADC_buf_read_adress = 0;
		ADC_new_data = 1;
		break;

	case UART_TX_COMPLETE :

		break;
	default:
		break;
	}
}
