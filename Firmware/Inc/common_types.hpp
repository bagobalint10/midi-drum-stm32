/*
 * common_types.hpp
 *
 *  Created on: Mar 5, 2026
 *      Author: bagob
 */

#ifndef INC_COMMON_TYPES_HPP_
#define INC_COMMON_TYPES_HPP_

	extern "C"
	{
		#include "stdint.h"
	}

	namespace Midi
	{
		struct Message{

			uint8_t status;
			uint8_t note;
			uint8_t velocity;
		};
	}


#endif /* INC_COMMON_TYPES_HPP_ */
