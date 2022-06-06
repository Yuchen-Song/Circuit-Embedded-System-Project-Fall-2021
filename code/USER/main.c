#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "motor.h"
#include "ir.h"       //����
#include "oled.h"
#include "exti.h"
//Ƕ��ʽ����5�Ӵ����Ȩ����
//���ߣ����곽����һ��
//���ߣ�
//PA4-7 PB8-9 PWM ���
//PA0 �Ҷȣ�Խ����ֵԽ������ɫ�����ʱ���������ֵӦ�ü��٣�
//PA1 ��ѹ�Ķ�ȡֵ
//PC4 PC5 ���� ��1��ʾʶ�𵽺��ߣ�
//PB11 PB10 ����3������ģ�� PB11��TX��PB10��RX
//PA9 PA10 ����1
//OLED: GND-��Դ��  VCC-��5V��3.3v��Դ  D0-��PA3��SCL��ʱ�� D1-��PB5��SDA������
//      RES-��PA2(��λ) DC-��PB6(��־λ���ж������������) CS-��PB7(Ƭѡ��������Ч��)
//���ֻҶ�PC1��
//�������źż��PC2��
//�����STBY--PC3.

unsigned char Battery_Image[5][1000];
static u16 motorPwmVal=250;  //������ֵ����Χ��ȡ0~899����ֵԽ��ת��Խ������ʱ�������йأ�500 400 350
static u16 motorSteerVal=300; //��ʾת��ʱ����������pwm�Ĳ�ֵ����ֵԽ��ת��Խ����170 220 250
u8 flag=0;             //��¼Ȧ���õı�����0��ʾ��ǰ����ɫ����(�������)�����ټ�����ֻ�е�1��ʱ��ſ��Լ���
//u8 numBlue=0;         //��¼������ɫ��ĸ���
//u8 numCircle=0;      //��¼Ȧ��
u8 battery_volume;     //����% 
u16 adcVoltage;      //��ʾ��ȡ���ĵ�ѹadc����
u8 numCoils=0;      //��ȡ�������Ȧ�ĸ���
void oled_show(void);
 int main(void)
 { 
		u8 count=0;//
	 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //STBY-->PC.3 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 
	  delay_init();	    	 //��ʱ������ʼ��	  
//	  uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	  LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	  Adc_Init();		  		//ADC��ʼ��	  
	  MOTOR_Init();
//		IR_Init();
		EXTIX_Init();
	  OLED_Init();         //OLED��ʾ����ʼ��
		OLED_DisPlay_On();
		OLED_ColorTurn(0);   //0������ʾ��1 ��ɫ��ʾ
		OLED_DisplayTurn(0); //0������ʾ 1 ��Ļ��ת��ʾ
		
//		//�����ʱ
//		for (j=0;j<45;j++) {
//			delay_ms(1000);
//		}
		
		while(1)
		{
			if (count%10==0) {
//				LED1=!LED1;//�ܵ�ʱ������˸
				oled_show();
			}
			count++;
			
//			adcVoltage=Get_Adc_Average(ADC_Channel_1,5);
//			battery_volume = adcVoltage*100/4096;// �����ٷֱ�(���ݵ�ѹ���ֵΪ9v��1.8vʱֹͣ���磬ת����ADC���ֵΪ3.3v����Ӧ��ֵ4096��820ʱֹͣ����)			
			//delay_ms(250);//����ע��һ��Ҫ��Ҫɾ��	
			
			//ǰ��Ȧѭ���ٶȱȽ���
			if(numCoils<16 && numCoils%5==0) {
				motorPwmVal=350;
				motorSteerVal=250;
			} else {
				motorPwmVal=250;
				motorSteerVal=300;
			}
			
			//***** ������Ѳ�߲��� *****//			
			if (IR0==1 && IR1==1){
				//printf("���Ҵ�������ʶ�𵽺��ߣ�ֹͣ\r\n");  //  |��  ��|
				                                            //     | |
				                                            //     | |
				motor_stop();//ֹͣ������������Ŀ��û���õ�
			} else {
				//printf("���Ҵ�������δʶ�𵽺��ߣ�ֱ��\r\n");
				motor_forward(motorPwmVal);//ֱ��	
			}
			
			//***** ��Ȧ�����߼����� *****//
			
			//�����ǰ�ڳ�����򣬶�����������·�źţ���ô��flag����1����ʾ���Լ��� 
			if (flag==0 && Charge==1) flag = 1;
			
			//***** ���������߼����� *****//
			//Charge=0��ʾ�ڳ��
//			if (Charge==0) {
//				
//			}
//			if (numCoils==0) {
//				GPIO_SetBits(GPIOC,GPIO_Pin_3);
//			}
			
			
			
			
			//***** OLED��ʾ *****//
			//��ʾ���֣�x���� 0~127��y���� 0~63��������ַ��������С��8,12,16,24���أ���ģʽ:0,��ɫ��ʾ;1,������ʾ
			//OLED_ShowString(0,20,"Battery Volume:",12,1);       //1.��ʾ��ص���
//			OLED_ShowNum(10,10,battery_volume,2,24,1);             
//			OLED_ShowString(40,10,"%",24,1);  
			
//			OLED_ShowString(50,40,"Circles",16,1);	            //2.��ʾȦ��
//			OLED_ShowNum(10,35,numCircles,2,24,1);               //��ʾ���� ��x,y,���֣�λ�������壬ģʽ�� numCircle
			
//      if (battery_volume>=80)
//			{
//				OLED_ShowPicture(70,15,40,15,Battery_Image[4],1);  //3.��ʾͼƬ���������x,y,ͼƬ����x,y,ͼƬ���飬ģʽ��
//			}
//			else if (battery_volume <80 && battery_volume>=60)
//			{
//				OLED_ShowPicture(70,15,40,15,Battery_Image[3],1);
//			}
//			else if (battery_volume <60 && battery_volume>=40)
//			{
//				OLED_ShowPicture(70,15,40,15,Battery_Image[2],1);
//			}
//			else if (battery_volume <40 && battery_volume>=20)
//			{
//				OLED_ShowPicture(70,15,40,15,Battery_Image[1],1);
//			}
//			else OLED_ShowPicture(70,15,40,15,Battery_Image[0],1);
//			OLED_Refresh();//���µ�oled����
			
			//���Դ���
//			printf("�Ҷȴ������Ķ�����%d, �Ѿ���ʻ��%dȦ, flag=%d, numBlue=%d\r\n",adcGray,numCircle,flag,numBlue);
//			motor_forward(motorPwmVal);
//			printf("�Ҷȴ������Ķ�����%d, �Ѿ���ʻ��%dȦ, flag=%d, numBlue=%d\r\n",Gray,numCircle,flag,numBlue);
			
			//adcx=Get_Adc_Average(ADC_CHANNEL_1,20);//��ȡͨ��1��ת��ֵ��20��ȡƽ�� PA1
			//temp = adcx;
			//temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
//			LED0=!LED0;
			//delay_ms(250);		
			
//			sprintf(c,"[v4][m0][t5]%dȦ",numCircle);
//			  SYN_FrameInfo(2,c);//delay��ʱ����Ҫ�����������ʱ�䳤
//				delay_ms(1000);
//				delay_ms(1000);
			

			
//			  delay_ms(10);
//				if(dir)battery_volume+=1;
//				else battery_volume-=1;
//				if(battery_volume>99)dir=0;
//				if(battery_volume==0)dir=1;
//				printf("adcVoltage: %d, Battery: %d\r\n",adcVoltage, battery_volume);
			
		}											    
}	
 

unsigned char Battery_Image[5][1000] = { 
{
0xFC,0xFE,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x07,0xFF,0xFE,0xE0,0xE0,0x1F,0x3F,0x70,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0x7F,0x3F,0x03,0x03/*"0%"*/
},

{
0xFC,0xFE,0x07,0x03,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x07,0xFF,0xFE,0xE0,0xE0,0x1F,0x3F,0x70,0x60,0x67,0x67,0x67,0x67,
0x67,0x67,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0x7F,0x3F,0x03,0x03/*"25%"*/
},

{
0xFC,0xFE,0x07,0x03,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,0xF3,
0xF3,0xF3,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x07,0xFF,0xFE,0xE0,0xE0,0x1F,0x3F,0x70,0x60,0x67,0x67,0x67,0x67,
0x67,0x67,0x60,0x60,0x67,0x67,0x67,0x67,0x67,0x67,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0x7F,0x3F,0x03,0x03/*"50%"*/
},

	
{
0xFC,0xFE,0x07,0x03,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,0xF3,
0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x07,0xFF,0xFE,0xE0,0xE0,0x1F,0x3F,0x70,0x60,0x67,0x67,0x67,0x67,
0x67,0x67,0x60,0x60,0x67,0x67,0x67,0x67,0x67,0x67,0x60,0x60,0x67,0x67,0x67,0x67,
0x67,0x67,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0x7F,0x3F,0x03,0x03,/*"75%"*/
},

{
0xFC,0xFE,0x07,0x03,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,0xF3,
0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,0xF3,
0xF3,0xF3,0x03,0x07,0xFF,0xFE,0xE0,0xE0,0x1F,0x3F,0x70,0x60,0x67,0x67,0x67,0x67,
0x67,0x67,0x60,0x60,0x67,0x67,0x67,0x67,0x67,0x67,0x60,0x60,0x67,0x67,0x67,0x67,
0x67,0x67,0x60,0x60,0x67,0x67,0x67,0x67,0x67,0x67,0x60,0x70,0x7F,0x3F,0x03,0x03/*"100%"*/
},

	};

void EXTI4_IRQHandler(void)
{
	if(IR1==0)
	{	
		while(IR0!=0) {  
			//printf("�󴫸���ʶ�𵽺��ߣ���ת\r\n");      //   |��  |��
			turn_left(motorPwmVal+120,motorPwmVal-motorSteerVal+120);//��ת
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI4��·����λ
}
 void EXTI9_5_IRQHandler(void)
{			
	if(IR0==0)
	{	  
		while(IR1!=0) {  
			//printf("�Ҵ�����ʶ�𵽺��ߣ���ת\r\n");      //   |��  |��
			turn_right(motorPwmVal-motorSteerVal+300,motorPwmVal+120);//��ת
		}
	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //���LINE5�ϵ��жϱ�־λ  
}	


// void EXTI1_IRQHandler(void)
//{			
//	u8 i=0;
//	//����ڰ����򣬶���������flag��Ϊ0���Ҽ�¼һ��������ɫ�����
//		if (flag==1) {  
//			numBlue++;
//			if (numBlue%5==0 && numBlue<17) {
//				delay_ms(100);
//				motor_lock();
//				for(i=0;i<10;i++) delay_ms(1000);
//			}
//			flag = 0;
//		}
//		if (numBlue%4==0) {
//			numCircle = numBlue/4;
////			adcVoltage=Get_Adc_Average(ADC_Channel_1,10);
////			battery_volume = adcVoltage*100/4096;// �����ٷֱ�(���ݵ�ѹ���ֵΪ9v��1.8vʱֹͣ���磬ת����ADC���ֵΪ3.3v����Ӧ��ֵ4096��820ʱֹͣ����)
////			OLED_ShowNum(10,10,battery_volume,2,24,1);             
////			OLED_ShowString(40,10,"%",24,1);  			
////			OLED_ShowString(50,40,"#",16,1);	            //2.��ʾȦ��
////			OLED_ShowNum(10,35,numCircle,2,24,1);               //��ʾ���� ��x,y,���֣�λ�������壬ģʽ�� numCircle
////			OLED_Refresh();//���µ�oled����
//		}
// 	 EXTI_ClearITPendingBit(EXTI_Line1);    //���LINE1�ϵ��жϱ�־λ  
//}

void EXTI2_IRQHandler(void) //��ʾ��⵽���
{			
	 u8 i=0;
	 if (numCoils==0)	{ //�ʼ�ĳ��
		 while (Charge!=1) { //��δ��⵽�ϵ�ǰ������ѭ��
			 oled_show();
			 if (i%15==0) {
				 LED0=!LED0;//���ʱ������˸
			 }
			 i++;
		 }
		 GPIO_SetBits(GPIOC,GPIO_Pin_3);
		 LED0=0;
		 numCoils++;
	 } else {  //�����ʼ�ĳ��
		 if (flag==1) { //�ڰ�ɫ������⵽���
			 numCoils++;
			 if (numCoils%5==1 && numCoils<17) {
//					delay_ms(100);
					motor_lock();
					for(i=0;i<30;i++) {
						LED0=!LED0;//���ʱ������˸
						oled_show();
						if (battery_volume>65) break;			//��ѹ����			
						delay_ms(1000);
					}
			 }
			 flag=0;
			 LED0=0;
		 }
//		 if (numCoils%4==0) {
//			 numCircle = numCoils/4;
//		 }
	 } 
 	 EXTI_ClearITPendingBit(EXTI_Line2);    //���LINE2�ϵ��жϱ�־λ  
}

void oled_show(void){
	adcVoltage=Get_Adc_Average(ADC_Channel_1,10);
	battery_volume = adcVoltage*100/4096;// �����ٷֱ�
	OLED_ShowNum(10,10,battery_volume,2,24,1);             
	OLED_ShowString(40,10,"%",24,1); 
	if (battery_volume>=80)
	{
		OLED_ShowPicture(70,15,40,15,Battery_Image[4],1);  //3.��ʾͼƬ���������x,y,ͼƬ����x,y,ͼƬ���飬ģʽ��
	}
	else if (battery_volume <80 && battery_volume>=60)
	{
		OLED_ShowPicture(70,15,40,15,Battery_Image[3],1);
	}
	else if (battery_volume <60 && battery_volume>=40)
	{
		OLED_ShowPicture(70,15,40,15,Battery_Image[2],1);
	}
	else if (battery_volume <40 && battery_volume>=20)
	{
		OLED_ShowPicture(70,15,40,15,Battery_Image[1],1);
	}
	else OLED_ShowPicture(70,15,40,15,Battery_Image[0],1);	
	OLED_Refresh();//���µ�oled����
}
