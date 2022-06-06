#ifndef _MOTOR_H
#define _MOTOR_H
#include "sys.h"
	

//����LED��GPIO��ʼ��
void MOTOR_Init(void);

//�����������Ļ�������
void motor_forward(int pwm);
void motor_backward(int pwm);
void motor_lock(void);
void motor_stop(void);

//С���˶�����
void turn_left(int pwm1,int pwm2);
void turn_right(int pwm1,int pwm2);
#endif

