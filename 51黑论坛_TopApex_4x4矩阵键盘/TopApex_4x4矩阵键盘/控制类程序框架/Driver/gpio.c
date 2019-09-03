/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : gpio.c
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
#include "gpio.h"

/***************GPIO索引表***************/
GPIO_TypeDef  *GPIOX[7]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG};
/***************GPIO外设时钟使能索引表***************/
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
*函数名称       : GPIO_init
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
void GPIO_setPin(GPIO PXn,uint32_t bit)
{
	BitAction BitVal;
	BitVal = (BitAction)bit;
	GPIO_WriteBit(GPIOX[PXn.PX],PXn.GPIO_Pin_n,BitVal);
}
