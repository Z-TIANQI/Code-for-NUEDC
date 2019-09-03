/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : gpio.h
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
#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f10x.h"

/**************STM32λ��������ʵ��λ����**************/
/*******************IO�����궨��**********************/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
/*******************IO?????**********************/
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
/**************IO�ڲ�����ֻ�Ե�һ��IO�ڣ�ȷ��nС��16******************/
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n) 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n) 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n) 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n) 

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n) 

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  
extern  GPIO_TypeDef *GPIOX[7];
/******************GPIOö�ٶ���****************/
typedef enum  PX //ֱ�ӻ���ΪPX �Ժ��õ�ʱ��Ͳ��ü�enum
{
	PA=0,	//GPIOA
	PB,		//GPIO
	PC,		//GPIOB
	PD,		//GPIOC
	PE,		//GPIOD
	PF,		//GPIOE
	PG		//GPIOF
}PX;
/*******************GPIO���Žṹ�嶨��********************/
typedef struct  GPIO //ֱ�ӻ���
{
	PX         PX; //ʹ���ĸ�GPIO�ڣ���ö������
	uint16_t   GPIO_Pin_n;//ʹ���ĸ�����
}GPIO;
/********************************************************************
*��������       : GPIO_Init
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
void GPIO_init(GPIO PXn,GPIOMode_TypeDef GPIO_Mode);
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
void GPIO_setPin(GPIO PXn,uint32_t bit);
#endif
