/*-----------------------------------------------------------------------------------
								STM32F103模拟I2C协议
	
	作者: TopApex
	版本: V1.0
	MCU : STM32F103
	时间: 2017.6
------------------------------------------------------------------------------------*/
#ifndef _IIC_ANALOG_H_
#define _IIC_ANALOG_H_

#include "stm32f10x.h"
/*------------------------------------------
			IIC引脚说明
------------------------------------------*/
#define IIC_GPIO     (GPIOB)
#define IIC_GOIO_SDA (GPIOB)
#define IIC_GPIO_SCL (GPIOB)
#define IIC_SDA      (GPIO_Pin_7)
#define IIC_SCL      (GPIO_Pin_6)
/*------------------------------------------
		引脚高低电平宏定义
------------------------------------------*/
#define SET_SDA		    { GPIO_SetBits( IIC_GPIO , IIC_SDA );  }
#define RESET_SDA	    { GPIO_ResetBits( IIC_GPIO , IIC_SDA );}
#define SET_SCL		    { GPIO_SetBits( IIC_GPIO , IIC_SCL );  }
#define RESET_SCL 	  { GPIO_ResetBits( IIC_GPIO , IIC_SCL); }
#define IIC_SDA_STATE (IIC_GPIO->IDR & IIC_SDA)
#define IIC_SCL_STATE (IIC_GPIO->IDR & IIC_SDA)
#define IIC_DELAY     { IIC_Delay(); }

enum IIC_REPLAY_ENUM
{
	IIC_NACK = 0,
	IIC_ACK = 1
};

enum IIC_BUS_STATE_ENUM
{
	IIC_BUS_READY = 0,
	IIC_BUS_BUSY=1,
	IIC_BUS_ERROR=2
};

void IIC_GPIO_Config(GPIO_TypeDef* GPIOx_SDA ,uint16_t SDA_Pin,GPIO_TypeDef* GPIOx_SCL ,uint16_t SCL_Pin);

void IIC_Delay(void);

uint8_t IIC_Start(void);

void IIC_Stop(void);

void IIC_SendACK(void);

void IIC_SendNACK(void);

uint8_t IIC_SendByte(uint8_t Data);

uint8_t IIC_RecvByte(void);

void IIC_WriteData(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);

uint8_t IIC_ReadData(uint8_t SlaveAddress,uint8_t REG_Address);

uint16_t IIC_GPIO_Filter(GPIO_TypeDef* GPIOx);

void delay_IIC(int ms);

#endif

