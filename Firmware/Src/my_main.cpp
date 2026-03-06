/*
 * my_main.cpp
 *
 *  Created on: Mar 2, 2026
 *      Author: bagob
 */

#include <my_main.h>

#include <piezo_midi.hpp>
#include <common_types.hpp>
#include <circular_fifo.hpp>



template <size_t Buffersize>
struct Adc {

	const size_t buffer_length = Buffersize;
	const size_t buffer_half_length = (Buffersize / 2);

	uint16_t buffer[Buffersize];
	uint16_t read_start_index = 0;
	bool	 new_data_available = 0;
	uint8_t number_of_conversions;

};

static Adc<200> adc = {.number_of_conversions = 10 };

static FIFO::CircularFifo<Midi::Message,30> OutputBuffer;

static MainFunctions Main = {0};

static Piezo::MidiConverter DrumPad[1];

static Piezo::ConfigParameters ConfPad[1] = {

	[0] = 	{	.midi_channel = 0x00, .midi_note = 20,

				.hpf_alfa = 0.5f, .lpf_alfa = 0.5f,

				.trigger_treshold = 1.5f, .trigger_scan_time = 20,
				.trigger_inactive_time = 50,

				.trigger_min_vel = 10, .trigger_max_vel = 127,
				.trigger_min_float = 0.5f, .trigger_max_float = 2.5f,

				.adc_buffer_half_length = (uint16_t) adc.buffer_half_length,
				.adc_number_of_conversions = adc.number_of_conversions,
				.adc_channel_rank = 0,
			}
};

void my_main_init(MainFunctions* M)
{
	M->ADC_Start_DMA((uint32_t*)adc.buffer,adc.buffer_length);
	Main.UART_SendString = M->UART_SendString;

	DrumPad[0].set_paratemers(ConfPad[0]);
}


void my_main_loop(void)
{
	if(adc.new_data_available)
	{
		Midi::Message return_message = {0};

		return_message = DrumPad[0].analyse_buffer(&adc.buffer[adc.read_start_index]);

		if(return_message.status)
		{
			// push uart buffer
			OutputBuffer.push(return_message);
		}

		adc.new_data_available = false;
	}

	/*if(uart_dma == empty)
	{
		send_dma_uart ( buffer.pop());
	}*/
}

void my_main_IT(IT_Types type)
{
	switch(type)
	{
	case ADC_FULL_COMPLETE :

		adc.read_start_index = adc.buffer_half_length;
		adc.new_data_available = true;

		break;

	case ADC_HALF_COMPLETE :

		adc.read_start_index = 0;
		adc.new_data_available = true;

		break;

	case UART_TX_COMPLETE :

		break;
	default:
		break;
	}
}
