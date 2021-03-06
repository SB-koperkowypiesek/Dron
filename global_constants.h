/*
 * global_constants.h
 *
 *  Created on: 18.04.2021
 *      Author: symon
 */

#ifndef GLOBAL_CONSTANTS_H_
#define GLOBAL_CONSTANTS_H_
#include <stdbool.h>

//------DIFFERENT_CONSTANTS------------
#define MAX(a, b) ((a>b)?a:b)						// choose greater value
#define GYRO_ACC_SIZE 7								// 3 for gyro, 3 acc and 1 for temperature
#define ALL_ELEMENTS_TO_SEND 14						// telemetry information different values
#define GYRO_TO_DPS 1000/32767.f 					// convert gyro register into degrees per second unit
#define RAD_TO_DEG  180 / M_PI
#define DEG_TO_RAD  M_PI/180
#define GYRO_TO_RAD  (1.f / 32.767f * DEG_TO_RAD)	// convert gyro register into rad per second unit
#define ACC_TO_GRAVITY  1.f / 4096 					// convert acc register into gravity unit
#define TIMEOUT_VALUE 0.5							// time for SPI_failsafe activation [s]
#define TASK_PERIOD_KHZ(kHz) (1000000000 / (kHz))	// converts frequency in [kHz] into period in [us]
#define TASK_PERIOD_HZ(Hz) (1000000 / (Hz))			// converts frequency in [Hz] into [us]
#define SEC_TO_US(s) (s*1000000)					// converts [s] into [us]
#define MS_TO_US(s) (s*1000)						// converts [ms] into [us]
#define US_TO_SEC(s) (s/1000000.f)					// converts [us] into [s]


//-----------RX_SETTINGS----------
#define CHANNELS  10					//how many channels you want use (4 for steering; 14 is max)
#define MAX_RX_SIGNAL 2050
#define MIN_RX_SIGNAL 950
#define MAX_NO_SIGNAL_TIME 1 			//time after RX_failsafe occurs[s]
#define ARM_VALUE 1600
#define PREARM_VALUE 1600
//#define USE_PREARM 			//if you want use PREARM define USE_PREARM

//------------ESC_PROTOCOLS----------
#define ESC_PROTOCOL_DSHOT// ESC_PROTOCOL_PWM, ESC_PROTOCOL_ONESHOT125, ESC_PROTOCOL_ONESHOT_V1 - bledy , ESC_PROTOCOL_DSHOT, ESC_PROTOCOL_DSHOT_BURST - nieskonczone

#define DSHOT_MODE 600 		// 150 300 600 1200
#define DSHOT_BUFFER_LENGTH 18 // 16 bits of Dshot and 2 for clearing
#define DSHOT_PWM_FRAME_LENGTH 35
#define DSHOT_1_LENGTH 26
#define DSHOT_0_LENGTH 13

//----------MOTORS_AND_CORRECTIONS-------

#define MAX_I_CORRECTION 300 			// maximal I_corr for PIDs betwenn <0;4000>
#define IDLE_VALUE 1050

//----------STABILIZE_SETINGS------
#define MAX_ROLL_ANGLE 45
#define MAX_PITCH_ANGLE 45

#define STABILIZE_FILTER_MAGDWICK	//STABILIZE_FILTER_MAGDWICK, STABILIZE_FILTER_MAHONY, STABILIZE_FILTER_COMPLEMENTARY
#define MAGDWICK_NEW  //MAGDWICK_ORIGINAL, MAGDWICK_NEW

#define GYRO_PART 0.995f		//complementary filter
#define ACC_PART 0.005f			//complementary filter

//---------FREQUENCY_SETTINGS--------
#define FREQUENCY_PID_LOOP 600 //[Hz]   IF YOU' RE USING PWM MAX. IS 500[Hz] (little less), IF DSHOT you can go up to 1[kHz]
#define FREQUENCY_ESC_UPDATE 600 //[Hz]
#define FREQUENCY_IMU_READING 600 //[Hz]
#define FREQUENCY_TELEMETRY_UPDATE 1 //[Hz]

//------------FAILSAFE--------------------

enum failsafe_type {
	NO_FAILSAFE,
	DISARMED,
	INCORRECT_CHANNELS_VALUES,
	RX_TIMEOUT,
	SETUP_ERROR,
	I2C_ERROR,
	SPI_IMU_ERROR,
	SPI_FLASH_ERROR,
	PID_LOOP_TIMEOUT
};

//--------------BLACKBOX-------------------
#define USE_FLASH_BLACKBOX // USE_FLASH_BLACKBOX if you want BLACKBOX or NOT_USE_FLASH_BLACKBOX
/*define which parameters you would like to save:
 *
 * BLACKBOX_SAVE_FILTERED_GYRO_AND_ACC 	- 6 values
 * BLACKBOX_SAVE_FILTERED_GYRO 			- 3 values
 * BLACKBOX_SAVE_FILTERED_ACC			- 3 values
 * BLACKBOX_SAVE_RAW_GYRO_AND_ACC		- 6 values
 * BLACKBOX_SAVE_RAW_GYRO				- 3 values
 * BLACKBOX_SAVE_RAW_ACC				- 3 values
 * BLACKBOX_SAVE_EULER_ANGLES			- 3 values
 * BLACKBOX_SAVE_SET_ANGLES				- 3 values
 * BLACKBOX_SAVE_STICKS					- 3 values
 *
 */
#define BLACKBOX_SAVE_EULER_ANGLES
#define BLACKBOX_SAVE_SET_ANGLES
#define BLACKBOX_SAVE_FILTERED_GYRO

//-----------OFFSETS and CALIBRATIONS VALUE----------
#define PITCH_OFFSET -7
#define ROLL_OFFSET 0

#define GYRO_ROLL_OFFSET 99
#define GYRO_PITCH_OFFSET 8
#define GYRO_YAW_OFFSET -9

#define ACC_PITCH_OFFSET 125.6525f
#define ACC_ROLL_OFFSET 2.862f
#define ACC_YAW_OFFSET 391.325f

#define ACC_CALIBRATION_X_X 4249.908f
#define ACC_CALIBRATION_X_Y -43.456f
#define ACC_CALIBRATION_X_Z 108.501f

#define ACC_CALIBRATION_Y_X 184.890f
#define ACC_CALIBRATION_Y_Y 4107.778f
#define ACC_CALIBRATION_Y_Z 755.494f

#define ACC_CALIBRATION_Z_X -114.671f
#define ACC_CALIBRATION_Z_Y -279.031f
#define ACC_CALIBRATION_Z_Z 4521.060f

//-------------DEBUGGING--------------

//#define IMU_TEST // for IMU SELFTEST functions

#endif /* GLOBAL_CONSTANTS_H_ */
