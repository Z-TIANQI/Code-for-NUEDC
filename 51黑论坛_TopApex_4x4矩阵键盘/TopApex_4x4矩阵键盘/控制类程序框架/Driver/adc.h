/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : adc.h
*����           : STM32103 adc��ز���ͷ�ļ�
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 
*��ע           : 
*
*
*
*
*********************************************************************/
#ifndef _ADC_H
#define _ADC_H
#include "stm32f10x.h"
#include "gpio.h"
extern ADC_TypeDef  *ADC[3];
extern uint32_t ADC_IRQn[3];
/******************ADCö�ٶ���****************/
typedef enum ADX
{
	AD1=0,	//GPIOA
	AD2,		//GPIO
	AD3,		//GPIOB
}ADX;
/********************************************************************
*��������       : ADC_gpioSet
*����˵��       : STM32103 AD �ӿڳ�ʼ��
*����˵��       :
                  [IN]
												ADX ADX              Ҫʹ�õ�ADC��
                        uint8_t ADC_Channel  Ҫʹ�õ�ADC��ͨ��
                  [OUT]
                        ��
*��������				: V1.0
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void ADC_gpioSet(ADX ADX,uint8_t ADC_Channel);
#endif
