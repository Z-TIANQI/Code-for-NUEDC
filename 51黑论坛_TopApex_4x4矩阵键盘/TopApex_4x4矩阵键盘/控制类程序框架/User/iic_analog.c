/*-----------------------------------------------------------------------------------
						STM32F103ģ��I2CЭ��
	
	����: TopApex
	�汾: V1.0
	MCU : STM32F103
	ʱ��: 2017.6
------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "iic_analog.h"
/*------------------------------------------
 ��ʼ������
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
 ����ʱ��ʵ�ⲻ��Ҳ����
------------------------------------------*/
void IIC_Delay(void)
{
	uint8_t i = 0;
	while( i-- );
}

/*------------------------------------------
         ����IIC��ʼ�ź�
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
          ����IICֹͣ�ź�
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
          ������ACKӦ�� 
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
         ����ACKӦ��
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
IC����һ���ֽ�
���شӻ�����Ӧ��
1����Ӧ��
0����Ӧ��		   

�����ֽڶ���Ϊ������׼������ʵ��Щֱ��copy,���У����ù���
----------------------------------------------------------*/
uint8_t IIC_SendByte(uint8_t Data)
{
	 uint8_t i;
	 RESET_SCL;                        //����ʱ�ӿ�ʼ���ݴ���
	 for(i=0;i<8;i++)
	 {  
			//---------��y?Y?������----------
			if(Data & 0x80)
			{
				SET_SDA;                //#define SET_SDA		    { GPIO_SetBits( IIC_GPIO , IIC_SDA );  }
			}
			else
			{
				RESET_SDA;              //#define RESET_SDA	    { GPIO_ResetBits( IIC_GPIO , IIC_SDA );}
			} 
			Data <<= 1;                 //��ʼд��
			IIC_DELAY; //С��ʱ��΢�뼶��ʵ����Բ��ã�Ϊ���Ȼ����ð�
			
			SET_SCL;                    //д������
			IIC_DELAY;
			RESET_SCL;                  //���½���Ϊɶ��Ҫ�ľ���������࣬ʱ���������������ö���
			IIC_DELAY;//С��ʱ��΢�뼶��ʵ����Բ��ã�Ϊ���Ȼ����ð�
	 }
	 
	 SET_SDA;           //��Щ����Ϊ�˶��Ͻ���
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
 ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
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
			IIC_DELAY;           //С��ʱ��΢�뼶��ʵ����Բ��ã�Ϊ���Ȼ����ð�
			Dat <<= 1;
			if(IIC_SDA_STATE)  
			{
				Dat|=0x01; 
			}   
			RESET_SCL;         
			IIC_DELAY;        //С��ʱ��΢�뼶��ʵ����Բ��ã�Ϊ���Ȼ����ð� 
	 }
	 return Dat;
}
/*------------------------------------------
 д��һ���ֽڣ������������Ҫ�õ���MPU6050.c����
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
 ��ȡһ���ֽڣ������������Ҫ�õ���MPU6050.c����
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
 �������
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

