/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : iicSoft.c
*描述           : STM32103 IIC相关函数
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.4.3
*备注           : 使用软件模拟IIC，引脚对应为 
                  此库内部使用了延时函数，使用时候必须进行delayInit(),完成延时函数的初始化工作
*********************************************************************/
#include "iicSoft.h"
#include "delay.h"
/********************************************************************
*函数名称       : iicSoftInit
*功能说明       : 软件IIC初始化函数
*参数说明       :
                  [IN]
									     无											 
                  [OUT]
                       无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
void iicSoftInit(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//先使能外设IO PORTC时钟 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	IIC_SCL=1;
	IIC_SDA=1;

}
/********************************************************************
*函数名称       : iicSoftStart
*功能说明       : 软件IIC开始信号
*参数说明       :
                  [IN]
									     无											 
                  [OUT]
                       无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
void iicSoftStart(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delayUs(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delayUs(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
/********************************************************************
*函数名称       : iicSoftStop
*功能说明       : 软件IIC停止信号
*参数说明       :
                  [IN]
									     无											 
                  [OUT]
                       无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
void iicSoftStop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delayUs(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delayUs(4);							   	
} 
/********************************************************************
*函数名称       : iicSoftWaitAck
*功能说明       : 等待IIC信号到来
*参数说明       :
                  [IN]
									     uint32_t delayTimes  超时等待时间										 
                  [OUT]
                       无
*函数返回				: ERROR   接收应答失败
                  SUCCESS 接收应到成功
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 iicSoftWaitAck(uint32_t delayTimes)
{
	u8 ucErrTime=0;
	SDA_IN();    
	IIC_SDA=1;delayUs(1);	   
	IIC_SCL=1;delayUs(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>delayTimes)
		{
			iicSoftStop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
/********************************************************************
*函数名称       : iicSoftAck
*功能说明       : 产生IIC应答信号
*参数说明       :
                  [IN]
									     无											 
                  [OUT]
                       无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
void iicSoftAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delayUs(2);
	IIC_SCL=1;
	delayUs(2);
	IIC_SCL=0;
}
/********************************************************************
*函数名称       : iicSoftNack
*功能说明       : 不产生IIC应答信号
*参数说明       :
                  [IN]
									     无											 
                  [OUT]
                       无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	    
void iicSoftNack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delayUs(2);
	IIC_SCL=1;
	delayUs(2);
	IIC_SCL=0;
}	
/********************************************************************
*函数名称       : iicSoftSendByte
*功能说明       : 发送1字节数据
*参数说明       :
                  [IN]
									     u8 txd										 
                  [OUT]
                       无
*函数返回				: 1 有应答
                  2 无应答
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	 	  
void iicSoftSendByte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delayUs(2);   
		IIC_SCL=1;
		delayUs(2); 
		IIC_SCL=0;	
		delayUs(2);
    }	 
}
/********************************************************************
*函数名称       : iicSoftReadByte
*功能说明       : IIC读取一字节数据
*参数说明       :
                  [IN]
									     unsigned char ack   ack=1时发生ACK ack=0时发送nACK									 
                  [OUT]
                       无
*函数返回				: u8 读取到的数据
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	 
u8 iicSoftReadByte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
  for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delayUs(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delayUs(1); 
    }					 
    if (!ack)
        iicSoftNack();
    else
        iicSoftAck();  
    return receive;
}
/********************************************************************
*函数名称       : iicSoftCheckDevice
*功能说明       : 检查IIC总线上是否有相应地址的器件
*参数说明       :
                  [IN]
									     uint8_t IicSlaveAddr
                  [OUT]
                       无
*函数返回				: ERROR   接收应答失败
                  SUCCESS 接收应到成功
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	
u8 iicSoftCheckDevice(uint8_t IicSlaveAddr)
{
  u8 ack;
	
	/* 产生起始位 */
	iicSoftStart();
	/* 发送IIC器件地址 方向上选择 写 ， 0 代表写 1 代表 读  */
	iicSoftSendByte(IicSlaveAddr);
	/* 等待ACK */
	ack = iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES);
	/* 产生停止位 */
	iicSoftStop();
	
	return ack;
}
/********************************************************************
*函数名称       : iicWriteLen
*功能说明       : 写指定长度数据
*参数说明       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC设备地址
                       uint8_t regAddr       寄存器地址
                       uint32_t length       发送数据长度
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	 
u8 iicWriteLen(uint8_t iicSlaveAddr, uint8_t regAddr, uint32_t length,uint8_t * buffer )
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*发送器件地址+写命令*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待器件应答*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*写寄存器地址*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待应答*/
	{
		iicSoftStop();
		return 1;
	} 
	while(length--)
	{
		iicSoftSendByte(*buffer++);/*发送数据*/
		if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待应答*/
		{
			iicSoftStop();
			return 1;
		} 
	}
	iicSoftStop();
	return 0;	
}
/********************************************************************
*函数名称       : iicReadLen
*功能说明       : 读指定长度数据
*参数说明       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC设备地址
                       uint8_t regAddr       寄存器地址
                       uint32_t length       发送数据长度
                  [OUT]
                       无
*函数返回				: ERROR   接收应答失败
                  SUCCESS 接收应到成功
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	
u8 iicReadLen(uint8_t iicSlaveAddr, uint8_t regAddr,uint32_t length,  uint8_t * buffer)
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*发送器件地址+写命令*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待器件应答*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*写寄存器地址*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待应答*/
	{
		iicSoftStop();
		return 1;
	} 
  iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|1);/*发送器件地址+读命令*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)
	{
		iicSoftStop();
		return 1;
	}
	/* 开始接受数据 */
	/* 这样做是因为最后一个读取的数据要给出NACK */
	length--;
	while(length--)
	{
		/* 不是最后一个给出ACK */
		*buffer++ = iicSoftReadByte(1);
	}

	/* 最后一个给出NACK */
	*buffer = iicSoftReadByte(0);

	iicSoftStop();	

	return 0;
}
/********************************************************************
*函数名称       : iicWriteByte
*功能说明       : 写单字节数据
*参数说明       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC设备地址
                       uint8_t regAddr       寄存器地址
                       u8 data               数据
                  [OUT]
                       无
*函数返回				: ERROR   接收应答失败
                  SUCCESS 接收应到成功
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	
u8 iicWriteByte(u8 iicSlaveAddr,u8 regAddr,u8 data)
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*发送器件地址+写命令*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待器件应答*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*写寄存器地址*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待应答*/
	{
		iicSoftStop();
		return 1;
	} 
	iicSoftSendByte(data);/*发送数据*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待应答*/
	{
		iicSoftStop();
		return 1;
	} 
	iicSoftStop();
	return 0;		
}
/********************************************************************
*函数名称       : iicReadByte
*功能说明       : 读取单字节数据
*参数说明       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC设备地址
                       uint8_t regAddr       寄存器地址
                       u8 data               数据
                  [OUT]
                       无
*函数返回				: ERROR   接收应答失败
                  SUCCESS 接收应到成功
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/	
u8 iicReadByte(u8 iicSlaveAddr,u8 regAddr,u8 *data)
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*发送器件地址+写命令*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待器件应答*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*写寄存器地址*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*等待应答*/
	{
		iicSoftStop();
		return 1;
	} 
  iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|1);/*发送器件地址+读命令*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)
	{
		iicSoftStop();
		return 1;
	}
	*data = iicSoftReadByte(0);
	iicSoftStop();	
	return 0;	
}
