#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "my_include.h"

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



#define KeyDn    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����0
#define KeyLt    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����1
#define KeyMd    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����0
#define KeyRt    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����1
#define KeyUp    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����0


#define Boma1    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ����0
#define Boma2    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//��ȡ����1
#define Boma3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//��ȡ����0
#define Boma4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//��ȡ����1


#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO��ʼ��
uint8 Key_scan(void);  	//����ɨ�躯��		
void KYE3_SHOW(void);

#endif
