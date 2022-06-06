#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "motor.h"
#include "ir.h"       //红外
#include "oled.h"
#include "exti.h"
//嵌入式绝绝5子代码版权所有
//作者：宋雨辰，寿一阳
//接线：
//PA4-7 PB8-9 PWM 电机
//PA0 灰度（越白数值越大，在蓝色区域的时候读到的数值应该减少）
//PA1 电压的读取值
//PC4 PC5 红外 （1表示识别到黑线）
//PB11 PB10 串口3，语音模块 PB11接TX，PB10接RX
//PA9 PA10 串口1
//OLED: GND-电源地  VCC-接5V或3.3v电源  D0-接PA3（SCL）时钟 D1-接PB5（SDA）数据
//      RES-接PA2(复位) DC-接PB6(标志位，判断是命令还是数据) CS-接PB7(片选（拉低有效）)
//数字灰度PC1，
//自启动信号检测PC2，
//输出到STBY--PC3.

unsigned char Battery_Image[5][1000];
static u16 motorPwmVal=250;  //具体数值（范围可取0~899，数值越大转速越慢，与时钟配置有关）500 400 350
static u16 motorSteerVal=300; //表示转弯时，两个轮子pwm的差值，差值越大，转弯越剧烈170 220 250
u8 flag=0;             //记录圈数用的变量，0表示当前在蓝色区域(充电区域)，不再计数，只有当1的时候才可以计数
//u8 numBlue=0;         //记录遇到蓝色块的个数
//u8 numCircle=0;      //记录圈数
u8 battery_volume;     //电量% 
u16 adcVoltage;      //表示读取到的电压adc读数
u8 numCoils=0;      //读取到充电线圈的个数
void oled_show(void);
 int main(void)
 { 
		u8 count=0;//
	 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //STBY-->PC.3 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 
	  delay_init();	    	 //延时函数初始化	  
//	  uart_init(115200);	 	//串口初始化为9600
	  LED_Init();		  		//初始化与LED连接的硬件接口
 	  Adc_Init();		  		//ADC初始化	  
	  MOTOR_Init();
//		IR_Init();
		EXTIX_Init();
	  OLED_Init();         //OLED显示屏初始化
		OLED_DisPlay_On();
		OLED_ColorTurn(0);   //0正常显示，1 反色显示
		OLED_DisplayTurn(0); //0正常显示 1 屏幕翻转显示
		
//		//充电延时
//		for (j=0;j<45;j++) {
//			delay_ms(1000);
//		}
		
		while(1)
		{
			if (count%10==0) {
//				LED1=!LED1;//跑的时候红灯闪烁
				oled_show();
			}
			count++;
			
//			adcVoltage=Get_Adc_Average(ADC_Channel_1,5);
//			battery_volume = adcVoltage*100/4096;// 电量百分比(电容电压最大值为9v、1.8v时停止供电，转化成ADC最大值为3.3v，对应数值4096，820时停止供电)			
			//delay_ms(250);//这里注意一下要不要删掉	
			
			//前两圈循迹速度比较慢
			if(numCoils<16 && numCoils%5==0) {
				motorPwmVal=350;
				motorSteerVal=250;
			} else {
				motorPwmVal=250;
				motorSteerVal=300;
			}
			
			//***** 传感器巡线部分 *****//			
			if (IR0==1 && IR1==1){
				//printf("左右传感器都识别到黑线，停止\r\n");  //  |〇  〇|
				                                            //     | |
				                                            //     | |
				motor_stop();//停止，但在我们项目中没有用到
			} else {
				//printf("左右传感器都未识别到黑线，直走\r\n");
				motor_forward(motorPwmVal);//直走	
			}
			
			//***** 数圈数的逻辑代码 *****//
			
			//如果当前在充电区域，读到自启动电路信号，那么将flag调回1，表示可以计数 
			if (flag==0 && Charge==1) flag = 1;
			
			//***** 自启动的逻辑代码 *****//
			//Charge=0表示在充电
//			if (Charge==0) {
//				
//			}
//			if (numCoils==0) {
//				GPIO_SetBits(GPIOC,GPIO_Pin_3);
//			}
			
			
			
			
			//***** OLED显示 *****//
			//显示汉字：x坐标 0~127，y坐标 0~63，输出的字符，字体大小（8,12,16,24像素），模式:0,反色显示;1,正常显示
			//OLED_ShowString(0,20,"Battery Volume:",12,1);       //1.显示电池电量
//			OLED_ShowNum(10,10,battery_volume,2,24,1);             
//			OLED_ShowString(40,10,"%",24,1);  
			
//			OLED_ShowString(50,40,"Circles",16,1);	            //2.显示圈数
//			OLED_ShowNum(10,35,numCircles,2,24,1);               //显示数字 （x,y,数字，位数，字体，模式） numCircle
			
//      if (battery_volume>=80)
//			{
//				OLED_ShowPicture(70,15,40,15,Battery_Image[4],1);  //3.显示图片（起点坐标x,y,图片长宽x,y,图片数组，模式）
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
//			OLED_Refresh();//更新到oled屏上
			
			//调试代码
//			printf("灰度传感器的读数是%d, 已经行驶了%d圈, flag=%d, numBlue=%d\r\n",adcGray,numCircle,flag,numBlue);
//			motor_forward(motorPwmVal);
//			printf("灰度传感器的读数是%d, 已经行驶了%d圈, flag=%d, numBlue=%d\r\n",Gray,numCircle,flag,numBlue);
			
			//adcx=Get_Adc_Average(ADC_CHANNEL_1,20);//获取通道1的转换值，20次取平均 PA1
			//temp = adcx;
			//temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
//			LED0=!LED0;
			//delay_ms(250);		
			
//			sprintf(c,"[v4][m0][t5]%d圈",numCircle);
//			  SYN_FrameInfo(2,c);//delay的时间需要比语音读完的时间长
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
			//printf("左传感器识别到黑线，左转\r\n");      //   |〇  |〇
			turn_left(motorPwmVal+120,motorPwmVal-motorSteerVal+120);//左转
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI4线路挂起位
}
 void EXTI9_5_IRQHandler(void)
{			
	if(IR0==0)
	{	  
		while(IR1!=0) {  
			//printf("右传感器识别到黑线，右转\r\n");      //   |〇  |〇
			turn_right(motorPwmVal-motorSteerVal+300,motorPwmVal+120);//右转
		}
	}
 	 EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}	


// void EXTI1_IRQHandler(void)
//{			
//	u8 i=0;
//	//如果在白区域，读到蓝，则将flag变为0，且记录一次遇到蓝色块的数
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
////			battery_volume = adcVoltage*100/4096;// 电量百分比(电容电压最大值为9v、1.8v时停止供电，转化成ADC最大值为3.3v，对应数值4096，820时停止供电)
////			OLED_ShowNum(10,10,battery_volume,2,24,1);             
////			OLED_ShowString(40,10,"%",24,1);  			
////			OLED_ShowString(50,40,"#",16,1);	            //2.显示圈数
////			OLED_ShowNum(10,35,numCircle,2,24,1);               //显示数字 （x,y,数字，位数，字体，模式） numCircle
////			OLED_Refresh();//更新到oled屏上
//		}
// 	 EXTI_ClearITPendingBit(EXTI_Line1);    //清除LINE1上的中断标志位  
//}

void EXTI2_IRQHandler(void) //表示检测到充电
{			
	 u8 i=0;
	 if (numCoils==0)	{ //最开始的充电
		 while (Charge!=1) { //在未检测到断电前进入死循环
			 oled_show();
			 if (i%15==0) {
				 LED0=!LED0;//充电时蓝灯闪烁
			 }
			 i++;
		 }
		 GPIO_SetBits(GPIOC,GPIO_Pin_3);
		 LED0=0;
		 numCoils++;
	 } else {  //不是最开始的充电
		 if (flag==1) { //在白色区域后检测到充电
			 numCoils++;
			 if (numCoils%5==1 && numCoils<17) {
//					delay_ms(100);
					motor_lock();
					for(i=0;i<30;i++) {
						LED0=!LED0;//充电时蓝灯闪烁
						oled_show();
						if (battery_volume>65) break;			//电压保护			
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
 	 EXTI_ClearITPendingBit(EXTI_Line2);    //清除LINE2上的中断标志位  
}

void oled_show(void){
	adcVoltage=Get_Adc_Average(ADC_Channel_1,10);
	battery_volume = adcVoltage*100/4096;// 电量百分比
	OLED_ShowNum(10,10,battery_volume,2,24,1);             
	OLED_ShowString(40,10,"%",24,1); 
	if (battery_volume>=80)
	{
		OLED_ShowPicture(70,15,40,15,Battery_Image[4],1);  //3.显示图片（起点坐标x,y,图片长宽x,y,图片数组，模式）
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
	OLED_Refresh();//更新到oled屏上
}
