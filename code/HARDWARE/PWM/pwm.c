#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//TIM3 PWM 部分初始化
//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数


//计划使用TIM4的CH3和CH4，即PB8和PB9
void TIM2_PWM_Init(u16 arr,u16 psc)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //①使能定时器 4 时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM3_CH2->PB5
	
		//设置该引脚为复用输出功能,输出 TIM4 CH34 的 PWM 脉冲波形 GPIOB.8/9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;  //TIM4_CH89
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
	
		//初始化 TIM4
		TIM_TimeBaseStructure.TIM_Period = arr;  //设置在自动重装载周期值
		TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //③初始化 TIMx
		
		//初始化 TIM4 Channe3 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC3

		
		//初始化 TIM4 Channe4 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC4
		
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
		
		TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
		TIM_Cmd(TIM4, ENABLE);  //⑤使能 TIM3
}
