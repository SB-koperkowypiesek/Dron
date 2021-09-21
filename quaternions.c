/*
 * quaternions.c
 *
 *  Created on: 28.05.2021
 *      Author: symon
 */

#include "stm32f4xx.h"
#include "global_constants.h"
#include "global_variables.h"
#include "global_functions.h"
#include "quaternions.h"

#include <math.h>

static float time_flag = 0;


ThreeF Rotate_Vector_with_Quaternion(ThreeF vector, Quaternion q,
		int8_t Transposition) {
	//Transposition say if you want rotate with rotation matrix made from quaternion (0) or you want rotate with transponce of this matrix (1)
	if (Transposition == 0) {
		vector.roll = (q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z)
				* vector.roll + 2 * (q.x * q.y - q.w * q.z) * vector.pitch
				+ 2 * (q.x * q.z + q.w * q.y) * vector.yaw;
		vector.pitch = 2 * (q.x * q.y + q.w * q.z) * vector.roll
				+ (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) * vector.pitch
				+ 2 * (q.y * q.z - q.w * q.x) * vector.yaw;
		vector.yaw = 2 * (q.x * q.z - q.w * q.y) * vector.roll
				+ 2 * (q.y * q.z + q.w * q.x) * vector.pitch
				+ (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z) * vector.yaw;

	} else if (Transposition == 1) {
		vector.roll = (q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z)
				* vector.roll + 2 * (q.x * q.y + q.w * q.z) * vector.pitch
				+ 2 * (q.x * q.z - q.w * q.y) * vector.yaw;
		vector.pitch = 2 * (q.x * q.y - q.w * q.z) * vector.roll
				+ (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) * vector.pitch
				+ 2 * (q.y * q.z + q.w * q.x) * vector.yaw;
		vector.yaw = 2 * (q.x * q.z + q.w * q.y) * vector.roll
				+ 2 * (q.y * q.z - q.w * q.x) * vector.pitch
				+ (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z) * vector.yaw;
	}

	return vector;
}

Quaternion Rotate_Quaternion(Quaternion q1) {
	static float delta_time;
	static Quaternion q1_prim;
	static Quaternion angular_velocity;
	const float GYRO_TO_RAD = 1.f / 32.768f * DEG_TO_RAD;
	angular_velocity.w = 0;
	angular_velocity.x = Gyro_Acc[0] * GYRO_TO_RAD;
	angular_velocity.y = Gyro_Acc[1] * GYRO_TO_RAD;
	angular_velocity.z = Gyro_Acc[2] * GYRO_TO_RAD;

	q1_prim = quaternion_multiply(
			quaternions_multiplication(angular_velocity, q1), -0.5);
	delta_time = get_Global_Time() - time_flag;
	time_flag = get_Global_Time();

	q1 = quaternions_sum(q1, quaternion_multiply(q1_prim, delta_time));
	//normalize quaternion:
	q1 = quaternion_multiply(q1, 1 / quaternion_norm(q1));
	return q1;
}

ThreeF Quaternion_to_Euler_angles(Quaternion q) {
	static ThreeF angles;
// roll (x-axis rotation)
	float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	float cosr_cosp = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
	angles.roll = atan2f(sinr_cosp, cosr_cosp) * RAD_TO_DEG;

// pitch (y-axis rotation)
	angles.pitch = asinf(2 * (q.w * q.y - q.z * q.x)) * RAD_TO_DEG;

// yaw (z-axis rotation)
	float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	float cosy_cosp = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
	angles.yaw = atan2f(siny_cosp, cosy_cosp) * RAD_TO_DEG;

	return angles;
}
Quaternion Euler_angles_to_Quaternion(ThreeF euler_angles) {

	float cr = cosf(euler_angles.roll * 0.5 * DEG_TO_RAD);
	float sr = sinf(euler_angles.roll * 0.5 * DEG_TO_RAD);
	float cp = cosf(euler_angles.pitch * 0.5 * DEG_TO_RAD);
	float sp = sinf(euler_angles.pitch * 0.5 * DEG_TO_RAD);
	float cy = cosf(euler_angles.yaw * 0.5 * DEG_TO_RAD);
	float sy = sinf(euler_angles.yaw * 0.5 * DEG_TO_RAD);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;
	q = quaternion_multiply(q, 1 / quaternion_norm(q));
	return q;
}

Quaternion quaternions_multiplication(Quaternion q1, Quaternion q2) {
	Quaternion q3;
	q3.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	q3.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	q3.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	q3.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;

	return q3;
}
Quaternion quaternions_sum(Quaternion q1, Quaternion q2) {
	Quaternion q3;
	q3.w = q1.w + q2.w;
	q3.x = q1.x + q2.x;
	q3.y = q1.y + q2.y;
	q3.z = q1.z + q2.z;

	return q3;
}
Quaternion quaternions_sub(Quaternion q1, Quaternion q2) {
	Quaternion q3;
	q3.w = q1.w - q2.w;
	q3.x = q1.x - q2.x;
	q3.y = q1.y - q2.y;
	q3.z = q1.z - q2.z;

	return q3;
}
Quaternion quaternion_multiply(Quaternion q1, float x) {
	q1.w *= x;
	q1.x *= x;
	q1.y *= x;
	q1.z *= x;

	return q1;
}
float quaternion_norm(Quaternion q1) {
	return sqrtf(q1.w * q1.w + q1.x * q1.x + q1.y * q1.y + q1.z * q1.z);
}

Quaternion quaternion_conjugate(Quaternion q1) {
	q1.x *= -1;
	q1.y *= -1;
	q1.z *= -1;
	return q1;
}

float skalar_quaternions_multiplication(Quaternion q1, Quaternion q2) {
	return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}
