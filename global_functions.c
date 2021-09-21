/*
 * global_functions.c

 *
 *  Created on: 03.05.2021
 *      Author: symon
 */
#include "stm32f4xx.h"
#include "global_variables.h"
#include "global_functions.h"

void delay_micro(uint16_t time) {

	TIM7->ARR=time;
	TIM7->EGR |= TIM_EGR_UG;
	TIM7->CR1 |= TIM_CR1_CEN;

	while (Tim_7_flag!=1) {
		; //wait for time [us]
	}
	Tim_7_flag=0;
}
void delay_mili(uint16_t time) {
//wait for time [ms]
	for (uint16_t i = 0; i < time; i++) {
		delay_micro(1000);
	}
}
double timer(double t1) {

	double delta_time;
	double t2 = (TIM2->CNT) *0.000001;
	delta_time = t2 - t1;
	if (delta_time < 0) {
		delta_time += (double) (TIM2->ARR) * 0.000001;
	}
	return delta_time;
}
void TIM6_DAC_IRQHandler() {
	if (TIM_SR_UIF & TIM6->SR) {
		TIM6->SR &= ~TIM_SR_UIF;
		Global_Time += (TIM6->ARR ) * 0.000001;
	}
}
void TIM7_IRQHandler() {
	if (TIM_SR_UIF & TIM7->SR) {
		TIM7->SR &= ~TIM_SR_UIF;
		TIM7->CR1 &= ~TIM_CR1_CEN;
		Tim_7_flag=1;
	}
}


double get_Global_Time() {
	return (Global_Time + (TIM6->CNT) * 0.000001);
}

void set_motors(ThreeF corr) {
	const uint16_t max_pwm=2000;
	const uint16_t idle_pwm=1000;
	//	Make corrections:
	//	right back:
	pwm_m1 = Throttle + corr.pitch - corr.yaw - corr.roll;
	//	right front:
	pwm_m2 = Throttle - corr.pitch + corr.yaw - corr.roll;
	//	left back:
	pwm_m3 = Throttle + corr.pitch + corr.yaw + corr.roll;
	//	left front:
	pwm_m4 = Throttle - corr.pitch - corr.yaw + corr.roll;
	if (pwm_m1 < idle_pwm) {
		pwm_m1 = idle_pwm;
	} else if (pwm_m1 > max_pwm)
		pwm_m1 = max_pwm;
	if (pwm_m2 <idle_pwm) {
		pwm_m2 = idle_pwm;
	} else if (pwm_m2 > max_pwm)
		pwm_m2 = max_pwm;
	if (pwm_m3 < idle_pwm) {
		pwm_m3 = idle_pwm;
	} else if (pwm_m3 > max_pwm)
		pwm_m3 = max_pwm;
	if (pwm_m4 < idle_pwm) {
		pwm_m4 = idle_pwm;
	} else if (pwm_m4 > max_pwm)
		pwm_m4 = max_pwm;

}
void turn_ON_BLUE_LED(){
	GPIOB->BSRR |=GPIO_BSRR_BR5;
	}

void turn_OFF_BLUE_LED(){
	GPIOB->BSRR |=GPIO_BSRR_BS5;
	}
void turn_ON_RED_LED(){
	GPIOB->BSRR |=GPIO_BSRR_BR4;
	}

void turn_OFF_RED_LED(){
	GPIOB->BSRR |=GPIO_BSRR_BS4;
	}

//FAILSAFE HANDLER:
void EXTI15_10_IRQHandler() {
	if ((EXTI->PR & EXTI_PR_PR10)) {
		EXTI->PR |= EXTI_PR_PR10; // clear this bit setting it high
	}
	if ((EXTI->PR & EXTI_PR_PR11)) {
		EXTI->PR |= EXTI_PR_PR11; // clear this bit setting it high
	}
	if ((EXTI->PR & EXTI_PR_PR12)) {
		EXTI->PR |= EXTI_PR_PR12; // clear this bit setting it high
	}
	if ((EXTI->PR & EXTI_PR_PR13)) {
		EXTI->PR |= EXTI_PR_PR13; // clear this bit setting it high
	}
	if ((EXTI->PR & EXTI_PR_PR14)) {
		EXTI->PR |= EXTI_PR_PR14; // clear this bit setting it high
	}
	//FAILSAFE ARM SWITCH is set as DISARM:
	if ((EXTI->PR & EXTI_PR_PR15)) {
		static uint16_t err_counter[6];
		EXTI->PR |= EXTI_PR_PR15; // clear(setting 1) this bit (and at the same time bit SWIER15)
		TIM2->CCR4 = 1000 ; 			//wypelneinie motor 1
		TIM3->CCR3 = 1000 ; 			//wypelneinie motor 2
		TIM3->CCR4 = 1000 ; 			//wypelneinie motor 3
		TIM2->CCR3 = 1000 ; 			//wypelneinie motor 4

		PWM_M1 = &motor_off;
		PWM_M2 = &motor_off;
		PWM_M3 = &motor_off;
		PWM_M4 = &motor_off;
		switch (failsafe_type) {
		case 1:
				err_counter[0]++;
				failsafe_type = 0;
				break;
			case 2:
				err_counter[1]++;
				failsafe_type = 0;
				break;
			case 3:
				err_counter[2]++;
				failsafe_type = 0;
				break;
			case 4:
				err_counter[3]++;
				failsafe_type = 0;
				break;
			case 5:
				err_counter[4]++;
				failsafe_type = 0;
				break;
			case 6:
				err_counter[5]++;
				failsafe_type = 0;
				break;
		}
	}
}







