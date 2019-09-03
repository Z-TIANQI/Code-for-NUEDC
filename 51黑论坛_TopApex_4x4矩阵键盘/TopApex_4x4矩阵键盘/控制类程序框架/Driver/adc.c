/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : adc.c
*描述           : STM32103 adc相关操作头文件
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 
*备注           : 
*********************************************************************/
#include "adc.h"
/***************ADC索引表***************/
ADC_TypeDef  *ADC[3]={ADC1,ADC2,ADC3};
uint32_t ADC_IRQn[3]={ADC1_2_IRQn,ADC1_2_IRQn,ADC3_IRQn};
///***************AD通道重映射引脚索引表***************/
GPIO ADC1_GPIO[16]=
{
 {PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3},{PA,GPIO_Pin_4},{PA,GPIO_Pin_5},{PA,GPIO_Pin_6},{PA,GPIO_Pin_7},
 {PB,GPIO_Pin_0},{PB,GPIO_Pin_1},{PC,GPIO_Pin_0},{PC,GPIO_Pin_1},{PC,GPIO_Pin_2},{PC,GPIO_Pin_3},{PC,GPIO_Pin_4},{PC,GPIO_Pin_5}
};
GPIO ADC2_GPIO[16]=
{
 {PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3},{PA,GPIO_Pin_4},{PA,GPIO_Pin_5},{PA,GPIO_Pin_6},{PA,GPIO_Pin_7},
 {PB,GPIO_Pin_0},{PB,GPIO_Pin_1},{PC,GPIO_Pin_0},{PC,GPIO_Pin_1},{PC,GPIO_Pin_2},{PC,GPIO_Pin_3},{PC,GPIO_Pin_4},{PC,GPIO_Pin_5}
};
GPIO ADC3_GPIO[16]=
{
 {PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3},{PF,GPIO_Pin_6},{PF,GPIO_Pin_7},{PF,GPIO_Pin_8},{PF,GPIO_Pin_9},
 {PF,GPIO_Pin_10},{0,0},         {PC,GPIO_Pin_0},{PC,GPIO_Pin_1},{PC,GPIO_Pin_2},{PC,GPIO_Pin_3},{0,0},          {0,0}
};
 GPIO *ADC_GPIO[3]={ADC1_GPIO,ADC2_GPIO,ADC3_GPIO};
/***************ADC外设时钟使能索引表***************/
uint32_t rccADC[3]=
{
	RCC_APB2Periph_ADC1,
  RCC_APB2Periph_ADC2,
  RCC_APB2Periph_ADC3	
};
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
void ADC_gpioSet(ADX ADX,uint8_t ADC_Channel)
{
	
	RCC_APB2PeriphClockCmd(rccADC[ADX],ENABLE);//时钟初始化
	GPIO_init(ADC_GPIO[ADX][ADC_Channel],GPIO_Mode_AIN);//GPIO 初始化	
} 
