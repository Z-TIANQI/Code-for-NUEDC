/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : iicSoft.c
*����           : STM32103 IIC��غ���
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2017.4.3
*��ע           : ʹ�����ģ��IIC�����Ŷ�ӦΪ 
                  �˿��ڲ�ʹ������ʱ������ʹ��ʱ��������delayInit(),�����ʱ�����ĳ�ʼ������
*********************************************************************/
#include "iicSoft.h"
#include "delay.h"
/********************************************************************
*��������       : iicSoftInit
*����˵��       : ���IIC��ʼ������
*����˵��       :
                  [IN]
									     ��											 
                  [OUT]
                       ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
void iicSoftInit(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//��ʹ������IO PORTCʱ�� 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	IIC_SCL=1;
	IIC_SDA=1;

}
/********************************************************************
*��������       : iicSoftStart
*����˵��       : ���IIC��ʼ�ź�
*����˵��       :
                  [IN]
									     ��											 
                  [OUT]
                       ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
void iicSoftStart(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delayUs(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delayUs(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
/********************************************************************
*��������       : iicSoftStop
*����˵��       : ���IICֹͣ�ź�
*����˵��       :
                  [IN]
									     ��											 
                  [OUT]
                       ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
void iicSoftStop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delayUs(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delayUs(4);							   	
} 
/********************************************************************
*��������       : iicSoftWaitAck
*����˵��       : �ȴ�IIC�źŵ���
*����˵��       :
                  [IN]
									     uint32_t delayTimes  ��ʱ�ȴ�ʱ��										 
                  [OUT]
                       ��
*��������				: ERROR   ����Ӧ��ʧ��
                  SUCCESS ����Ӧ���ɹ�
*�޸�ʱ��				:
*��ע						:
*����           :����
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
/********************************************************************
*��������       : iicSoftAck
*����˵��       : ����IICӦ���ź�
*����˵��       :
                  [IN]
									     ��											 
                  [OUT]
                       ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : iicSoftNack
*����˵��       : ������IICӦ���ź�
*����˵��       :
                  [IN]
									     ��											 
                  [OUT]
                       ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : iicSoftSendByte
*����˵��       : ����1�ֽ�����
*����˵��       :
                  [IN]
									     u8 txd										 
                  [OUT]
                       ��
*��������				: 1 ��Ӧ��
                  2 ��Ӧ��
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : iicSoftReadByte
*����˵��       : IIC��ȡһ�ֽ�����
*����˵��       :
                  [IN]
									     unsigned char ack   ack=1ʱ����ACK ack=0ʱ����nACK									 
                  [OUT]
                       ��
*��������				: u8 ��ȡ��������
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : iicSoftCheckDevice
*����˵��       : ���IIC�������Ƿ�����Ӧ��ַ������
*����˵��       :
                  [IN]
									     uint8_t IicSlaveAddr
                  [OUT]
                       ��
*��������				: ERROR   ����Ӧ��ʧ��
                  SUCCESS ����Ӧ���ɹ�
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/	
u8 iicSoftCheckDevice(uint8_t IicSlaveAddr)
{
  u8 ack;
	
	/* ������ʼλ */
	iicSoftStart();
	/* ����IIC������ַ ������ѡ�� д �� 0 ����д 1 ���� ��  */
	iicSoftSendByte(IicSlaveAddr);
	/* �ȴ�ACK */
	ack = iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES);
	/* ����ֹͣλ */
	iicSoftStop();
	
	return ack;
}
/********************************************************************
*��������       : iicWriteLen
*����˵��       : дָ����������
*����˵��       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC�豸��ַ
                       uint8_t regAddr       �Ĵ�����ַ
                       uint32_t length       �������ݳ���
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/	 
u8 iicWriteLen(uint8_t iicSlaveAddr, uint8_t regAddr, uint32_t length,uint8_t * buffer )
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*����������ַ+д����*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�����Ӧ��*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*д�Ĵ�����ַ*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�Ӧ��*/
	{
		iicSoftStop();
		return 1;
	} 
	while(length--)
	{
		iicSoftSendByte(*buffer++);/*��������*/
		if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�Ӧ��*/
		{
			iicSoftStop();
			return 1;
		} 
	}
	iicSoftStop();
	return 0;	
}
/********************************************************************
*��������       : iicReadLen
*����˵��       : ��ָ����������
*����˵��       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC�豸��ַ
                       uint8_t regAddr       �Ĵ�����ַ
                       uint32_t length       �������ݳ���
                  [OUT]
                       ��
*��������				: ERROR   ����Ӧ��ʧ��
                  SUCCESS ����Ӧ���ɹ�
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/	
u8 iicReadLen(uint8_t iicSlaveAddr, uint8_t regAddr,uint32_t length,  uint8_t * buffer)
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*����������ַ+д����*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�����Ӧ��*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*д�Ĵ�����ַ*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�Ӧ��*/
	{
		iicSoftStop();
		return 1;
	} 
  iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|1);/*����������ַ+������*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)
	{
		iicSoftStop();
		return 1;
	}
	/* ��ʼ�������� */
	/* ����������Ϊ���һ����ȡ������Ҫ����NACK */
	length--;
	while(length--)
	{
		/* �������һ������ACK */
		*buffer++ = iicSoftReadByte(1);
	}

	/* ���һ������NACK */
	*buffer = iicSoftReadByte(0);

	iicSoftStop();	

	return 0;
}
/********************************************************************
*��������       : iicWriteByte
*����˵��       : д���ֽ�����
*����˵��       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC�豸��ַ
                       uint8_t regAddr       �Ĵ�����ַ
                       u8 data               ����
                  [OUT]
                       ��
*��������				: ERROR   ����Ӧ��ʧ��
                  SUCCESS ����Ӧ���ɹ�
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/	
u8 iicWriteByte(u8 iicSlaveAddr,u8 regAddr,u8 data)
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*����������ַ+д����*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�����Ӧ��*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*д�Ĵ�����ַ*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�Ӧ��*/
	{
		iicSoftStop();
		return 1;
	} 
	iicSoftSendByte(data);/*��������*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�Ӧ��*/
	{
		iicSoftStop();
		return 1;
	} 
	iicSoftStop();
	return 0;		
}
/********************************************************************
*��������       : iicReadByte
*����˵��       : ��ȡ���ֽ�����
*����˵��       :
                  [IN]
									     uint8_t iicSlaveAddr  IIC�豸��ַ
                       uint8_t regAddr       �Ĵ�����ַ
                       u8 data               ����
                  [OUT]
                       ��
*��������				: ERROR   ����Ӧ��ʧ��
                  SUCCESS ����Ӧ���ɹ�
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/	
u8 iicReadByte(u8 iicSlaveAddr,u8 regAddr,u8 *data)
{
	iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|0);/*����������ַ+д����*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�����Ӧ��*/
	{
		iicSoftStop();
		return 1;
	}
	iicSoftSendByte(regAddr);/*д�Ĵ�����ַ*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)/*�ȴ�Ӧ��*/
	{
		iicSoftStop();
		return 1;
	} 
  iicSoftStart();
	iicSoftSendByte((iicSlaveAddr<<1)|1);/*����������ַ+������*/
	if(iicSoftWaitAck(SOFTWARE_IIC_WAIT_ACK_TIMES) == 1)
	{
		iicSoftStop();
		return 1;
	}
	*data = iicSoftReadByte(0);
	iicSoftStop();	
	return 0;	
}
