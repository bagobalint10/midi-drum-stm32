/*
 * wrapper.cpp
 *
 *  Created on: Mar 2, 2026
 *      Author: bagob
 */

#include <wrapper.hpp>
#include <my_main.hpp>


void my_main_init(void)
{
	My_Main::init();
}
void my_main_loop(void)
{
	My_Main::loop();
}
void my_main_IT(void)
{
	My_Main::IT();
}
