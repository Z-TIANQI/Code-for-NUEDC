/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : adc.h
*描述           : STM32103 adc相关操作头文件
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 
*备注           : 
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
/******************ADC枚举定义****************/
typedef enum ADX
{
	AD1=0,	//GPIOA
	AD2,		//GPIO
	AD3,		//GPIOB
}ADX;
/********************************************************************
*函数名称       : ADC_gpioSet
*功能说明       : STM32103 AD 接口初始化
*参数说明       :
                  [IN]
												ADX ADX              要使用的ADC号
                        uint8_t ADC_Channel  要使用的ADC的通道
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void ADC_gpioSet(ADX ADX,uint8_t ADC_Channel);
#endif
