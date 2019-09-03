/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : uart1.c
*����           : STM32103 ������غ���
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2017.4.3
*��ע           : 1.��δ����ϸ�����ϣ���򵥵ĳ�ʼ��
                  2.2016.10.26����������Ż�
*********************************************************************/
#include "uart1.h"
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 
UART1_sendStringByDma((u8*)&ch,1);	
	UART1_sendByte(ch);    
	return ch;
}
#endif 
/**********************���������鶨��**************************/
uint8_t UART1_sendBuff[UART1_TRAN_BUFF_LENGTH];//���ͻ���
uint8_t UART1_receiveBuff[UART1_RECV_BUFF_LENGTH];//���ջ���
/*���շ��ͱ�־*/
uint32_t UART1_receivedFlag=0;
uint32_t UART1_sendFlag=0;
/********************************************************************
*��������       : UART1_init
*����˵��       : STM32103 UART11 �ӿڳ�ʼ��
*����˵��       :
                  [IN]
												uint32_t boud ������
                  [OUT]
                        ��
*��������				: V1.0
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_init(uint32_t boud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef    DMA_Initstructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE );

  /*�����ж�����*/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
  /*��������*/
	USART_InitStructure.USART_BaudRate=boud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);
	
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);/*���������ж�*/
 
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	/*�������ͽ���DMA*/
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC); 
	
	/*�������ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*����DMAͨ������*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
  /*���ڽ���DMAͨ������*/
  DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USART1->DR);
  DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)UART1_receiveBuff;
  DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_Initstructure.DMA_BufferSize = UART1_RECV_BUFF_LENGTH;
  DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;
  DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;
  DMA_Initstructure.DMA_Priority = DMA_Priority_High;
  DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel5,&DMA_Initstructure);  
  DMA_Cmd(DMA1_Channel5,ENABLE);	
	
	/*���ڷ���DMAͨ������*/
	DMA_Initstructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	DMA_Initstructure.DMA_MemoryBaseAddr = (uint32_t)UART1_sendBuff;
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Initstructure.DMA_BufferSize = 0;
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;
	DMA_Initstructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_Initstructure);	
	 
}
/********************************************************************
*��������       : UART1_sendByteDma
*����˵��       : STM32103 UART1ʹ��DMA��������
*����˵��       :
                  [IN]
												
                  [OUT]
                        ��
*��������				: V1.0
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendStringByDma(uint8_t *buffer,uint32_t length)
{
	/*�����û�������*/
	if(DMA1_Channel4->CNDTR)
	{
	}
	else/*������δ���*/
	{
		DMA_Cmd(DMA1_Channel4,DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC4);
		DMA1_Channel4->CNDTR=length;
		DMA1_Channel4->CMAR=(uint32_t)buffer;
		DMA_Cmd(DMA1_Channel4,ENABLE);
	}
}
/********************************************************************
*��������       : UART1_sendByte
*����˵��       : STM32103 UART11��ʹ���жϷ���
*����˵��       :
                  [IN]
												char ch �����͵��ַ�
                  [OUT]
                        ��
*��������				: V1.0
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendByte(uint8_t ch)
{
	
	USART_SendData(USART1, (uint16_t) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
/********************************************************************
*��������       : UART1_sendByte
*����˵��       : STM32103 UART11��ʹ���жϽ���
*����˵��       :
                  [IN]
												��
                  [OUT]
                        ��
*��������				: ���յ�������
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
int UART1_receive(void)
{
	u16 temp;
	while(!USART_GetFlagStatus(USART1,USART_FLAG_RXNE));//�ж� �Ƿ� �������  
  temp = USART_ReceiveData(USART1);
	return temp;
}
/********************************************************************
*��������       : UART1_sendSignedChar
*����˵��       : STM32103 UART1�����з��ŵ�char�����ݣ��ı���ʽ����
*����˵��       :
                  [IN]
												��
                  [OUT]
                        ��
*��������				: ���յ�������
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendSignedChar(char ch)
{
	uint8_t temp;
	if(ch&0x80)//����
	{
		temp=~(ch-1);
		UART1_sendByte('-');
		UART1_sendHex(temp);
	}
	else//��������0
	{
		UART1_sendHex(ch);
	}
}
/********************************************************************
*��������       : UART1_sendFloat
*����˵��       : STM32103 UART1����float���͵����ݣ��������ݵ�ԭʼ�洢����
*����˵��       :
                  [IN]
												��
                  [OUT]
                        ��
*��������				: ���յ�������
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendFloat(float data)
{
	floatTo4char dataUnion;
	dataUnion.floatData=data;
	UART1_sendByte(dataUnion.charData[0]);
	UART1_sendByte(dataUnion.charData[1]);
	UART1_sendByte(dataUnion.charData[2]);
	UART1_sendByte(dataUnion.charData[3]);
}
/********************************************************************
*��������       : UART1_sendHex
*����˵��       : STM32103 UART11����ת��Ϊ16���Ʒ���
*����˵��       :
                  [IN]
												unsigned char data �����͵�����
                  [OUT]
                        ��
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendHex(unsigned char data)
{
	unsigned char temp;
	uint8_t buff;
	temp=data>>4;
	if(temp>=10)
	{
		buff=temp-10+'A';
		UART1_sendStringByDma(&buff,1);

	}
	else
	{
		buff=temp+'0';
		UART1_sendStringByDma(&buff,1);
	}
	temp=data&0x0f;
	if(temp>=10)
	{
		buff=temp-10+'A';
		UART1_sendStringByDma(&buff,1);

	}
	else
	{
		buff=temp+'0';
		UART1_sendStringByDma(&buff,1);
	}
}
/********************************************************************
*��������       : UART1_sendToMyView
*����˵��       : STM32103 UART11����ͼ�����ݸ�PC����Myview��ʾ
*����˵��       :
                  [IN]
												unsigned char *data �����͵�����
                  [OUT]
                        ��
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendToMyView(unsigned char *data)
{
	uint8_t check=0;
	uint32_t i;
	UART1_sendByte(0xAA);
	UART1_sendByte(0x6A);
	check=0xAA^0x6A;
	for(i=0;i<128;i++)
	{
		check=check^data[i];
		UART1_sendByte(data[i]);		
	}	
	UART1_sendByte(check);
}
/********************************************************************
*��������       : UART1_sendToCCDVIEW
*����˵��       : STM32103 UART11����ͼ�����ݸ�PC����CCDview��ʾ
*����˵��       :
                  [IN]
												unsigned char *data �����͵�����
                  [OUT]
                        ��
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void UART1_sendToCCDVIEW(unsigned char *data)
{
	uint32_t i;
	UART1_sendByte('*');
	UART1_sendByte('L');
	UART1_sendByte('D');
	UART1_sendHex(0);
	UART1_sendHex(132);
	UART1_sendHex(0);
	UART1_sendHex(0);
	UART1_sendHex(0);
	UART1_sendHex(0);
	for(i=0;i<128;i++)
	{
		UART1_sendHex(data[i]);		
	}
	UART1_sendHex(1);
	UART1_sendByte('#');
}
void USART1_IRQHandler(void)
{
    unsigned char num=0;
    if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
    {
       num = USART1->SR;/*��ȡ״̬��־λ����ȡ�ͻ������־λ*/
       num = USART1->DR; 
       DMA_Cmd(DMA1_Channel5,DISABLE);
       num = UART1_RECV_BUFF_LENGTH -  DMA_GetCurrDataCounter(DMA1_Channel5);
       UART1_receiveBuff[num] = '\0';
       DMA1_Channel5->CNDTR=128;     
       DMA_Cmd(DMA1_Channel5,ENABLE);
       UART1_receivedFlag = 1;           
    }
}
