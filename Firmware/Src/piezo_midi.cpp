/*
 * piezo_midi.cpp
 *
 *  Created on: Mar 5, 2026
 *      Author: bagob
 */

#include <piezo_midi.hpp>

#include "cmath"


/* PRIVATE *///////////////////////////////////////

void Piezo::MidiConverter::hp_filter(void)
{
	float output = 0;
	float input = common.io_value;

	output = hpf.prev_output + input - (hpf.prev_input * hpf.alfa);

	hpf.prev_input = input;
	hpf.prev_output = output;
	common.io_value = output;
}

void Piezo::MidiConverter::lp_filter(void)
{
	float output = 0;
	float input = common.io_value;

	output = (lpf.alfa * input) + ( (1.0f - lpf.alfa) * lpf.prev_output );

	lpf.prev_output = output;
	common.io_value = output;
}

void Piezo::MidiConverter::rectifier(void)
{
	common.io_value = std::fabs(common.io_value);
}

void Piezo::MidiConverter::trigger(void)
{

	auto treshold_detect = [&]()
	{
		if(common.io_value > trg.treshold)
		{
			trg.state = TriggerState::INACTIVE;

			trg.max_value = 0;
			trg.time_countdown = trg.scan_time;
		}
	};

	auto peak_detect = [&]()
	{
		if(common.io_value > trg.max_value)
		{
			trg.max_value = common.io_value;
		}
	};


	auto scan_time_countdown = [&]()
	{
		if(trg.time_countdown == 1)					// last-1  it cycle -> send midi, end of the buffer analyse
		{
			trg.is_event_detected = true;

		}else if(trg.time_countdown == 0)			// last it cycle	-> set INACTIVE state
		{
			trg.state = TriggerState::INACTIVE;
			trg.time_countdown = trg.inactive_time;
		}
	};

	auto inactive_time_countdown = [&]()
	{
		if(trg.time_countdown == 1)					// last-1  it cycle
		{
			trg.is_event_ended = true;

		}else if(trg.time_countdown == 0)			// last it cycle
		{
			trg.state = TriggerState::IDLE;
		}
	};


	switch(trg.state)
	{
	case TriggerState::IDLE :

			treshold_detect();
			break;

	case TriggerState::PEAK_DETECT :

			peak_detect();
			scan_time_countdown();
			break;

	case TriggerState::INACTIVE :

			inactive_time_countdown();
			break;
	}
}

void Piezo::MidiConverter::float_to_midi(void)
{
	auto calculate_velocity_to_float = [](float &input, float &in_min, float &in_max,
														uint8_t &out_min, uint8_t &out_max) -> uint8_t
		{
			if(input <= in_min)
			{
				return out_min;

			}else if(input >= in_max)
			{
				return out_max;

			}else	// if input is in the correct interval (min_f - max_f)
			{
				float value = (input-in_min) * (float)(out_max - out_min);
				value = (value / (in_max - in_min)) + (float)out_min;

				return (uint8_t) value;
			}
		};

	if(trg.is_event_ended)
	{
		midi.output_message.status = (MidiStatus::Note_On | midi.channel);
		midi.output_message.note = midi.note;
		midi.output_message.velocity = 0;

		trg.is_event_ended = false;

	}else if(trg.is_event_detected)
	{
		uint8_t calculated_velocity = calculate_velocity_to_float(common.io_value, ftm.min_float, ftm.max_float,
																			ftm.min_vel, ftm.max_vel);
		midi.output_message.status = (MidiStatus::Note_On | midi.channel);
		midi.output_message.note = midi.note;
		midi.output_message.velocity = calculated_velocity;

		trg.is_event_detected = false;

	}else
	{
		midi.output_message.status = 0x00;		// MIDI message -> undefined --> hasn't got now message
	}
}

/* PUBLIC *///////////////////////////////////////

Midi::Message Piezo::MidiConverter::analyse_buffer(uint16_t* read_start_adress)
{
	for(int i = 0; i < anl_b.buffer_half_length; i += anl_b.number_of_conversions)
	{
		common.io_value = *(read_start_adress + i + anl_b.channel_rank);

		/*DSP CHAIN */
		Piezo::MidiConverter::hp_filter();
		Piezo::MidiConverter::rectifier();
		Piezo::MidiConverter::lp_filter();
		Piezo::MidiConverter::trigger();
	}

	Piezo::MidiConverter::float_to_midi();

	return midi.output_message;
}

void Piezo::MidiConverter::set_paratemers(ConfigParameters &cp)
{
	midi.channel = cp.midi_channel;
	midi.note = cp.midi_note;

	hpf.alfa = cp.hpf_alfa;
	lpf.alfa = cp.lpf_alfa;

	trg.treshold = cp.trigger_treshold;
	trg.scan_time = cp.trigger_scan_time;
	trg.inactive_time = cp.trigger_inactive_time;

	ftm.min_vel = cp.trigger_min_vel;
	ftm.max_vel = cp.trigger_max_vel;
	ftm.min_float = cp.trigger_min_float;
	ftm.max_float = cp.trigger_max_float;

	anl_b.buffer_half_length = cp.adc_buffer_half_length;
	anl_b.number_of_conversions = cp.adc_number_of_conversions;
	anl_b.channel_rank = cp.adc_channel_rank;
}

