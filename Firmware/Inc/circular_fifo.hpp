/*
 * circular_fifo.hpp
 *
 *  Created on: Mar 6, 2026
 *      Author: bagob
 */

#ifndef INC_CIRCULAR_FIFO_HPP_
#define INC_CIRCULAR_FIFO_HPP_

	extern "C"
	{
		#include "stdint.h"
		#include "stdio.h"
	}

	namespace FIFO
	{

	template <typename T, size_t Buffersize>
		class CircularFifo
		{
		private:
			T buffer[Buffersize] = {0};
			T default_message = {0};

			uint16_t write_i = 0;
			uint16_t read_i = 0;

			bool isempty = true;
			bool isfull = false;

		public:
			CircularFifo() = default;
			~CircularFifo() = default;

			void push(const T &input)
			{

				if(write_i >= ((uint16_t) Buffersize - 1)) // write = last element
				{
					if(read_i == 0) isfull = true;			// read = first element
					else write_i = 0, isfull = false;
				}else
				{
					if(read_i == (write_i+1))				//full
					{
						isfull = true;
					}else
					{
						isfull = false;
						write_i++;
					}
				}

				if(isfull == false)
				{
					buffer[write_i] = input;
				}
			}
			T pop(void)
			{
				if(write_i == read_i)
				{
					isempty = true;

				} else
				{
					isempty = false;
				}

				if(!isempty)
				{
					read_i++;
					if(read_i >= (uint16_t) Buffersize) read_i = 0;
					return buffer[read_i];
				}

				return default_message;
			}
			bool is_empty()
			{
				return isempty;
			}
			bool is_full()
			{
				return isfull;
			}

		};
	}






#endif /* INC_CIRCULAR_FIFO_HPP_ */
