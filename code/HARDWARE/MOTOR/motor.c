#include "motor.h"
#include "delay.h"

void MOTOR_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //��ʹ�ܶ�ʱ�� 4 ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //��ʹ�� GPIO �� AFIO ���ù���ʱ��
		//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����ӳ�� TIM4_CH3->PB8 TIM4_CH4->PB9
	
	
		//PWM��� PB0/PB1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;  //TIM_CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
	
	  GPIO_SetBits(GPIOB,GPIO_Pin_8);//PB0��1
	  GPIO_SetBits(GPIOB,GPIO_Pin_9);//PB1��1
	
	  //���������IO�� PA4-7
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure); //�ٳ�ʼ�� GPIO
		
		//PA4-7��0
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	  
		//��ʼ�� TIM4
		TIM_TimeBaseStructure.TIM_Period = 899;  //�������Զ���װ������ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =0;  //����Ԥ��Ƶֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���ģʽ
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //�۳�ʼ�� TIMx
		
		//��ʼ�� TIM4 Channe3 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM4 OC3

		
		//��ʼ�� TIM3 Channe4 PWM ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ�� PWM ģʽ 2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //�ܳ�ʼ������ TIM4 OC4
		
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���
		
		TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM4, ENABLE);  //��ʹ�� TIM4
		
		
}

void motor_forward(int pwm)
{
	//��һ���������
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//PA4��0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);   //PA5��1
	TIM_SetCompare3(TIM4,pwm+30);     	  //PB0 �޸ıȽ�ֵpwm
	//�ڶ���������ң�
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//PA6��0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);   //PA7��1
	TIM_SetCompare4(TIM4,pwm);       	//PB1 �޸ıȽ�ֵpwm
	
}

void motor_backward(int pwm)
{
  //��һ�����
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	    //PA4��1
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);   //PA5��0
	TIM_SetCompare3(TIM4,pwm);     	    //PB0 �޸ıȽ�ֵpwm
	//�ڶ������ 
	GPIO_SetBits(GPIOA,GPIO_Pin_6);	    //PA6��1
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //PA7��0
	TIM_SetCompare4(TIM4,pwm);     	    //PB1 �޸ıȽ�ֵpwm
	   
}

void motor_lock(void)
{
  //��һ�����
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	    //PA4��1
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	    //PA5��1
	//TIM_SetCompare3(TIM3,pwm1);     	//PB0 �޸ıȽ�ֵpwm
	//�ڶ������
	GPIO_SetBits(GPIOA,GPIO_Pin_6);	    //PA6��1
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	    //PA7��1
	//TIM_SetCompare4(TIM3,pwm2);     	//PB1 �޸ıȽ�ֵpwm
}

void motor_stop()
{
  //��һ�����
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	    //PA4��0
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);	    //PA5��0
	//TIM_SetCompare3(TIM3,pwm1);         //PB0 �޸ıȽ�ֵpwm
	//�ڶ������ 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	    //PA6��0
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);	    //PA7��0
	//TIM_SetCompare4(TIM3,pwm2);         //PB0 �޸ıȽ�ֵpwm  
}

void turn_left(int pwm1,int pwm2)
{
  //��һ�����
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	    //PA4��0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	      //PA5��1
	TIM_SetCompare3(TIM4,pwm1+30);         	//PB0 �޸ıȽ�ֵpwm
	//�ڶ������ 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	    //PA6��0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	      //PA7��1
	TIM_SetCompare4(TIM4,pwm2);         	//PB0 �޸ıȽ�ֵpwm  
}	

void turn_right(int pwm1,int pwm2)
{
  //��һ�����
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	    //PA4��0
	GPIO_SetBits(GPIOA,GPIO_Pin_5);	      //PA5��1
	TIM_SetCompare3(TIM4,pwm1+30);         	//PB0 �޸ıȽ�ֵpwm
	//�ڶ������ 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	    //PA6��0
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	      //PA7��1
	TIM_SetCompare4(TIM4,pwm2);         	//PB0 �޸ıȽ�ֵpwm  
}	




