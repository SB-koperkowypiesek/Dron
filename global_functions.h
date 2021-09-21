/*
 * global_functions.h
 *
 *  Created on: 03.05.2021
 *      Author: symon
 */

#ifndef GLOBAL_FUNCTIONS_H_
#define GLOBAL_FUNCTIONS_H_

void delay_micro(uint16_t);
void delay_mili(uint16_t);
double get_Global_Time();
double timer(double t1);
void TIM6_DAC_IRQHandler();
void TIM7_IRQHandler();
void set_motors(ThreeF);
void turn_ON_BLUE_LED();
void turn_OFF_BLUE_LED();
void turn_ON_RED_LED();
void turn_OFF_RED_LED();

#endif /* GLOBAL_FUNCTIONS_H_ */
