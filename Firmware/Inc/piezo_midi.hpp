/*
 * piezo_midi.hpp
 *
 *  Created on: Mar 5, 2026
 *      Author: bagob
 */

#ifndef INC_PIEZO_MIDI_HPP_
#define INC_PIEZO_MIDI_HPP_


	#include <common_types.hpp>

	extern "C"
	{
		#include "stdint.h"
		#include "stdio.h"
	}

	namespace Piezo
	{
		struct ConfigParameters {

			uint8_t midi_channel;
			uint8_t midi_note;

			float hpf_alfa;
			float lpf_alfa;

			float trigger_treshold;
			uint32_t trigger_scan_time;
			uint32_t trigger_inactive_time;

			uint8_t trigger_min_vel;
			uint8_t trigger_max_vel;
			float trigger_min_float;
			float trigger_max_float;

			uint16_t adc_buffer_half_length;
			uint8_t adc_number_of_conversions;
			uint8_t adc_channel_rank;
		};

		class MidiConverter
		{
		private:
		 // constants
			struct MidiStatus{

				static constexpr uint8_t Note_On = 0x90;
				static constexpr uint8_t Note_Off = 0x80;
			};
		 // variables

			struct CommonVars{

				float io_value;
				uint32_t it_count;
			};

			struct MidiVars{

				Midi::Message output_message;
				uint8_t channel;
				uint8_t note;
			};

			struct HpfVars{

				float prev_output = 0.0f;
				float prev_input = 2000.0f;
				float alfa;
			};

			struct LpfVars{

				float prev_output = 0.0f;
				float alfa;
			};

			enum class TriggerState {IDLE, PEAK_DETECT, INACTIVE};

			struct TrgVars{

				TriggerState state;
				float treshold;
				float max_value;
				bool is_event_detected;
				bool is_event_ended;
				uint32_t scan_time;
				uint32_t inactive_time;
				uint32_t time_countdown;
			};

			struct FloatToMidiVars{

				uint8_t min_vel;
				uint8_t max_vel;
				float min_float;
				float max_float;
			};

			struct AnalyseBufferVars{

				uint16_t buffer_half_length;
				uint8_t number_of_conversions;
				uint8_t channel_rank;
			};

			HpfVars hpf = {0};
			CommonVars  common = {0};
			LpfVars lpf = {0};
			TrgVars trg = {.state = TriggerState::IDLE};
			FloatToMidiVars ftm = {0};
			AnalyseBufferVars anl_b = {0};
			MidiVars midi = {0};

		 // functions
			void hp_filter(void);
			void lp_filter(void);
			void rectifier(void);
			void trigger(void);
			void float_to_midi(void);

		public:
		 // variables

		 // functions
			MidiConverter() {};
			~MidiConverter() = default;

			void set_paratemers(ConfigParameters &cp);
			Midi::Message analyse_buffer(uint16_t* read_start_adress);
		};
	}

#endif /* INC_PIEZO_MIDI_HPP_ */
