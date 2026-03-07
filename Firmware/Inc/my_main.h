/*
 * my_main.h
 *
 *  Created on: Mar 2, 2026
 *      Author: bagob
 */

#ifndef FIRMWARE_MY_MAIN_H
#define FIRMWARE_MY_MAIN_H

	#ifdef __cplusplus
	extern "C" {
	#endif

		#include "stdint.h"
		#include "stdio.h"


		typedef enum  	{	ADC_HALF_COMPLETE, ADC_FULL_COMPLETE, UART_TX_COMPLETE } IT_Types;

		typedef enum  	{	RED, GREEN, BLUE } LEDS;

		typedef struct 	{ 	void (*UART_SendString)(uint8_t*,uint16_t);
							void (*ADC_Start_DMA)(uint32_t*, uint16_t);
							void (*LED_Write)(LEDS, uint8_t); 			} MainFunctions;

		void my_main_init(MainFunctions* M);
		void my_main_loop(void);
		void my_main_IT(IT_Types type);


	#ifdef __cplusplus
	}
	#endif

#endif /* FIRMWARE_MY_MAIN_H */
