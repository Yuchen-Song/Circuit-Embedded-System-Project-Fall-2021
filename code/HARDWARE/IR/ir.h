#ifndef __IR_H
#define __IR_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 

#define IR0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//��ȡ����0
#define IR1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//��ȡ����1
//#define Gray GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//��ȡ�Ҷ�
#define Charge GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)//��ȡ������


void IR_Init(void);//IO��ʼ��			    
#endif
