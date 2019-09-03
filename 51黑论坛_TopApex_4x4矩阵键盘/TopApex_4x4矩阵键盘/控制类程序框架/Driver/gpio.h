/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : gpio.h
*描述           : STM32103 GPIO通用操作函数
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
#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f10x.h"

/**************STM32位带操作，实现位控制**************/
/*******************IO操作宏定义**********************/
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
/**************IO口操作，只对但一的IO口，确保n小于16******************/
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
/******************GPIO枚举定义****************/
typedef enum  PX //直接换名为PX 以后用的时候就不用加enum
{
	PA=0,	//GPIOA
	PB,		//GPIO
	PC,		//GPIOB
	PD,		//GPIOC
	PE,		//GPIOD
	PF,		//GPIOE
	PG		//GPIOF
}PX;
/*******************GPIO引脚结构体定义********************/
typedef struct  GPIO //直接换名
{
	PX         PX; //使用哪个GPIO口，看枚举类型
	uint16_t   GPIO_Pin_n;//使用哪个引脚
}GPIO;
/********************************************************************
*函数名称       : GPIO_Init
*功能说明       : STM32103 GPIO初始化函数
*参数说明       : 
                  [IN]
												GPIO PXn                    需要初始化的引脚，GPIO为结构体，具体定义内容请看头文件
												GPIOMode_TypeDef GPIO_Mode  引脚的模式
                  [OUT]
                        无												
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void GPIO_init(GPIO PXn,GPIOMode_TypeDef GPIO_Mode);
/********************************************************************
*函数名称       : GPIO_setPin
*功能说明       : STM32103 GPIO置值操作
*参数说明       :
                  [IN]
												GPIO PXn      需要设置的引脚
                        uint32_t bit  设置的值
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void GPIO_setPin(GPIO PXn,uint32_t bit);
#endif
