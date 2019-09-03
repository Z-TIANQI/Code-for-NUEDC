/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : gpio.c
*����           : STM32103 GPIOͨ�ò�������
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
#include "gpio.h"

/***************GPIO������***************/
GPIO_TypeDef  *GPIOX[7]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG};
/***************GPIO����ʱ��ʹ��������***************/
const uint32_t rccGPIO[7]=
{
	RCC_APB2Periph_GPIOA,
	RCC_APB2Periph_GPIOB,
	RCC_APB2Periph_GPIOC,
	RCC_APB2Periph_GPIOD,
	RCC_APB2Periph_GPIOE,
	RCC_APB2Periph_GPIOF,
	RCC_APB2Periph_GPIOG,
};
/********************************************************************
*��������       : GPIO_init
*����˵��       : STM32103 GPIO��ʼ������
*����˵��       : 
                  [IN]
												GPIO PXn                    ��Ҫ��ʼ�������ţ�GPIOΪ�ṹ�壬���嶨�������뿴ͷ�ļ�
												GPIOMode_TypeDef GPIO_Mode  ���ŵ�ģʽ
                  [OUT]
                        ��												
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void GPIO_init(GPIO PXn,GPIOMode_TypeDef GPIO_Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(rccGPIO[PXn.PX] , ENABLE);
	GPIO_InitStructure.GPIO_Pin = PXn.GPIO_Pin_n; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOX[PXn.PX], &GPIO_InitStructure);
}
/********************************************************************
*��������       : GPIO_setPin
*����˵��       : STM32103 GPIO��ֵ����
*����˵��       :
                  [IN]
												GPIO PXn      ��Ҫ���õ�����
                        uint32_t bit  ���õ�ֵ
                  [OUT]
                        ��
*��������				: V1.0
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void GPIO_setPin(GPIO PXn,uint32_t bit)
{
	BitAction BitVal;
	BitVal = (BitAction)bit;
	GPIO_WriteBit(GPIOX[PXn.PX],PXn.GPIO_Pin_n,BitVal);
}
