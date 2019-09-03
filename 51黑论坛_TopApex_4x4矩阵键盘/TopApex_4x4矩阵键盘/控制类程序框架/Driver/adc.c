/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : adc.c
*����           : STM32103 adc��ز���ͷ�ļ�
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 
*��ע           : 
*********************************************************************/
#include "adc.h"
/***************ADC������***************/
ADC_TypeDef  *ADC[3]={ADC1,ADC2,ADC3};
uint32_t ADC_IRQn[3]={ADC1_2_IRQn,ADC1_2_IRQn,ADC3_IRQn};
///***************ADͨ����ӳ������������***************/
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
/***************ADC����ʱ��ʹ��������***************/
uint32_t rccADC[3]=
{
	RCC_APB2Periph_ADC1,
  RCC_APB2Periph_ADC2,
  RCC_APB2Periph_ADC3	
};
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
void ADC_gpioSet(ADX ADX,uint8_t ADC_Channel)
{
	
	RCC_APB2PeriphClockCmd(rccADC[ADX],ENABLE);//ʱ�ӳ�ʼ��
	GPIO_init(ADC_GPIO[ADX][ADC_Channel],GPIO_Mode_AIN);//GPIO ��ʼ��	
} 
