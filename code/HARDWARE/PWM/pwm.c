#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//TIM3 PWM ���ֳ�ʼ��
//PWM �����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��


//�ƻ�ʹ��TIM4��CH3��CH4����PB8��PB9
void TIM2_PWM_Init(u16 arr,u16 psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //��ʹ�ܶ�ʱ�� 4 ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����ӳ�� TIM3_CH2->PB5
	
		//���ø�����Ϊ�����������,��� TIM4 CH34 �� PWM ���岨�� GPIOB.8/9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;  //TIM4_CH89
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
		//��ʼ�� TIM4
		TIM_TimeBaseStructure.TIM_Period = arr;  //�������Զ���װ������ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc;  //����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx
		
		//��ʼ�� TIM4 Channe3 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC3

		
		//��ʼ�� TIM4 Channe4 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM3 OC4
		
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		
		TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM4, ENABLE);  //��ʹ�� TIM3
}
