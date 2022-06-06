#include "motor.h"
#include "delay.h"

void MOTOR_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //①使能定时器 4 时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM4_CH3->PB8 TIM4_CH4->PB9
	
	
		//PWM输出 PB0/PB1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;  //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
	
	  GPIO_SetBits(GPIOB,GPIO_Pin_8);//PB0置1
	  GPIO_SetBits(GPIOB,GPIO_Pin_9);//PB1置1
	
	  //两个电机的IO： PA4-7
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure); //①初始化 GPIO
		
		//PA4-7置0
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	  
		//初始化 TIM4
		TIM_TimeBaseStructure.TIM_Period = 899;  //设置在自动重装载周期值
		TIM_TimeBaseStructure.TIM_Prescaler =0;  //设置预分频值
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //③初始化 TIMx
		
		//初始化 TIM4 Channe3 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //④初始化外设 TIM4 OC3

		
		//初始化 TIM3 Channe4 PWM 模式
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //④初始化外设 TIM4 OC4
		
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能预装载寄存器
		
		TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
		TIM_Cmd(TIM4, ENABLE);  //⑤使能 TIM4
		
		
}

void motor_forward(int pwm)
{
	//第一个电机（左）
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//PA4置0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);   //PA5置1
	TIM_SetCompare3(TIM4,pwm+30);     	  //PB0 修改比较值pwm
	//第二个电机（右）
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//PA6置0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);   //PA7置1
	TIM_SetCompare4(TIM4,pwm);       	//PB1 修改比较值pwm
	
}

void motor_backward(int pwm)
{
  //第一个电机
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	    //PA4置1
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);   //PA5置0
	TIM_SetCompare3(TIM4,pwm);     	    //PB0 修改比较值pwm
	//第二个电机 
	GPIO_SetBits(GPIOA,GPIO_Pin_6);	    //PA6置1
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //PA7置0
	TIM_SetCompare4(TIM4,pwm);     	    //PB1 修改比较值pwm
	   
}

void motor_lock(void)
{
  //第一个电机
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	    //PA4置1
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	    //PA5置1
	//TIM_SetCompare3(TIM3,pwm1);     	//PB0 修改比较值pwm
	//第二个电机
	GPIO_SetBits(GPIOA,GPIO_Pin_6);	    //PA6置1
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	    //PA7置1
	//TIM_SetCompare4(TIM3,pwm2);     	//PB1 修改比较值pwm
}

void motor_stop()
{
  //第一个电机
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	    //PA4置0
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);	    //PA5置0
	//TIM_SetCompare3(TIM3,pwm1);         //PB0 修改比较值pwm
	//第二个电机 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	    //PA6置0
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);	    //PA7置0
	//TIM_SetCompare4(TIM3,pwm2);         //PB0 修改比较值pwm  
}

void turn_left(int pwm1,int pwm2)
{
  //第一个电机
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	    //PA4置0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	      //PA5置1
	TIM_SetCompare3(TIM4,pwm1+30);         	//PB0 修改比较值pwm
	//第二个电机 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	    //PA6置0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	      //PA7置1
	TIM_SetCompare4(TIM4,pwm2);         	//PB0 修改比较值pwm  
}	

void turn_right(int pwm1,int pwm2)
{
  //第一个电机
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	    //PA4置0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	      //PA5置1
	TIM_SetCompare3(TIM4,pwm1+30);         	//PB0 修改比较值pwm
	//第二个电机 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	    //PA6置0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	      //PA7置1
	TIM_SetCompare4(TIM4,pwm2);         	//PB0 修改比较值pwm  
}	




