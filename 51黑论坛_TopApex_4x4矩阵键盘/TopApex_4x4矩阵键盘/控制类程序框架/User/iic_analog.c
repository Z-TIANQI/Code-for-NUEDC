/*-----------------------------------------------------------------------------------
						STM32F103模拟I2C协议
	
	作者: TopApex
	版本: V1.0
	MCU : STM32F103
	时间: 2017.6
------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "iic_analog.h"
/*------------------------------------------
 初始化引脚
------------------------------------------*/
void IIC_GPIO_Config(GPIO_TypeDef* GPIOx_SDA ,uint16_t SDA_Pin,GPIO_TypeDef* GPIOx_SCL ,uint16_t SCL_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	  
	GPIO_InitStructure.GPIO_Pin =  SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_SDA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  SCL_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_SCL, &GPIO_InitStructure);
	
	
	SET_SDA;
	SET_SCL;  
}
/*------------------------------------------
 简单延时，实测不用也可以
------------------------------------------*/
void IIC_Delay(void)
{
	uint8_t i = 0;
	while( i-- );
}

/*------------------------------------------
         产生IIC起始信号
------------------------------------------*/
uint8_t IIC_Start(void)
{
	SET_SDA;
	IIC_DELAY;

	SET_SCL;
	IIC_DELAY;

	if( IIC_SDA_STATE == RESET )
	{
		return IIC_BUS_BUSY;
	}

	RESET_SDA;
	IIC_DELAY;

	RESET_SCL;
	IIC_DELAY;

	if( IIC_SDA_STATE == SET )
	{
		return IIC_BUS_ERROR;
	}

	return IIC_BUS_READY;
}
/*------------------------------------------
          产生IIC停止信号
------------------------------------------*/
void IIC_Stop(void)
{
	RESET_SDA;
	IIC_DELAY;

	SET_SCL;
	IIC_DELAY;

	SET_SDA;
	IIC_DELAY;
}

/*------------------------------------------
          不产生ACK应答 
------------------------------------------*/
void IIC_SendNACK(void)
{
	RESET_SDA;
	IIC_DELAY;
	SET_SCL;
	IIC_DELAY;
	RESET_SCL; 
	IIC_DELAY; 
}
/*------------------------------------------
         产生ACK应答
------------------------------------------*/
void IIC_SendACK(void)
{
	SET_SDA;
	IIC_DELAY;
	SET_SCL;
	IIC_DELAY;
	RESET_SCL; 
	IIC_DELAY;
}

/*---------------------------------------------------------
IC发送一个字节
返回从机有无应答
1，有应答
0，无应答		   

发送字节都是为下面做准备，其实这些直接copy,就行，不用管它
----------------------------------------------------------*/
uint8_t IIC_SendByte(uint8_t Data)
{
	 uint8_t i;
	 RESET_SCL;                        //拉低时钟开始数据传输
	 for(i=0;i<8;i++)
	 {  
			//---------êy?Y?¨á￠----------
			if(Data & 0x80)
			{
				SET_SDA;                //#define SET_SDA		    { GPIO_SetBits( IIC_GPIO , IIC_SDA );  }
			}
			else
			{
				RESET_SDA;              //#define RESET_SDA	    { GPIO_ResetBits( IIC_GPIO , IIC_SDA );}
			} 
			Data <<= 1;                 //开始写了
			IIC_DELAY; //小延时，微秒级，实测可以不用，为了稳还是用吧
			
			SET_SCL;                    //写完升高
			IIC_DELAY;
			RESET_SCL;                  //再下降，为啥？要的就是这个节奏，时序这玩意儿，节奏得对上
			IIC_DELAY;//小延时，微秒级，实测可以不用，为了稳还是用吧
	 }
	 
	 SET_SDA;           //这些都是为了对上节奏
	 IIC_DELAY;
	 SET_SCL;                   
	 IIC_DELAY;  
	 if(IIC_SDA_STATE)
	 {
			RESET_SCL;
			return IIC_NACK;
	 }
	 else
	 {
			RESET_SCL;
			return IIC_ACK;  
	 }    
}
/*------------------------------------------
 读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
------------------------------------------*/
uint8_t IIC_RecvByte(void)
{
	 uint8_t i,Dat = 0;
	 SET_SDA;
	 RESET_SCL; 
	 Dat = 0;
	 for(i=0; i<8; i++)
	 {
			SET_SCL;           
			IIC_DELAY;           //小延时，微秒级，实测可以不用，为了稳还是用吧
			Dat <<= 1;
			if(IIC_SDA_STATE)  
			{
				Dat|=0x01; 
			}   
			RESET_SCL;         
			IIC_DELAY;        //小延时，微秒级，实测可以不用，为了稳还是用吧 
	 }
	 return Dat;
}
/*------------------------------------------
 写入一个字节，这个才是咱们要用的在MPU6050.c里用
------------------------------------------*/
void IIC_WriteData(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
	IIC_Start();                  
	IIC_SendByte(SlaveAddress);   
	IIC_SendByte(REG_Address);    
	IIC_SendByte(REG_data);       
	IIC_Stop();                   
}

/*------------------------------------------
 读取一个字节，这个才是咱们要用的在MPU6050.c里用
------------------------------------------*/
uint8_t IIC_ReadData(uint8_t SlaveAddress,uint8_t REG_Address)
{  
	uint8_t REG_data;
	IIC_Start();                          
	IIC_SendByte(SlaveAddress);           
	IIC_SendByte(REG_Address);           	
	IIC_Start();                          
	IIC_SendByte(SlaveAddress+1);         
	REG_data = IIC_RecvByte();            
	IIC_SendACK();   
	IIC_Stop();                           
  return REG_data; 
}
/*------------------------------------------
 这个不用
------------------------------------------*/
uint16_t IIC_GPIO_Filter(GPIO_TypeDef* GPIOx)
{	 
	uint32_t RCC_GPIOx = 0; 

	if( GPIOx == GPIOA )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOA;
	}
	else if( GPIOx == GPIOA )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOA;
	}
	else if( GPIOx == GPIOB )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOB;
	}
	else if( GPIOx == GPIOC )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOC;
	}
	else if( GPIOx == GPIOD )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOD;
	}
	else if( GPIOx == GPIOE )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOE;
	}
	else if( GPIOx == GPIOF )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOF;
	}
	else if( GPIOx == GPIOG )
	{
		RCC_GPIOx = RCC_APB2Periph_GPIOG;
	}

	return RCC_GPIOx;
}

