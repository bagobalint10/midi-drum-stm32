/*
 * wrapper.hpp
 *
 *  Created on: Mar 2, 2026
 *      Author: bagob
 */

#ifndef WRAPPER_HPP_
#define WRAPPER_HPP_


	#ifdef __cplusplus			// wrapper for C
	extern "C" {
	#endif

		//#include "stm32f4xx_hal.h"

		void my_main_init(void);
		void my_main_loop(void);

		void my_main_IT(void);

	#ifdef __cplusplus
	}
	#endif



#endif /* WRAPPER_HPP_ */
