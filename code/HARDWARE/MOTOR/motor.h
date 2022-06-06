#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
	

//马达和LED的GPIO初始化
void MOTOR_Init(void);

//控制两个马达的基础函数
void motor_forward(int pwm);
void motor_backward(int pwm);
void motor_lock(void);
void motor_stop(void);

//小车运动函数
void turn_left(int pwm1,int pwm2);
void turn_right(int pwm1,int pwm2);
#endif

