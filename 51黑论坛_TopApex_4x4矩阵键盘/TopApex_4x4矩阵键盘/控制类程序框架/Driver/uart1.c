/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : uart1.c
*描述           : STM32103 串口相关函数
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.4.3
*备注           : 1.还未进行细节整合，最简单的初始化
                  2.2016.10.26晚进行整合优化
*********************************************************************/
#include "uart1.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 
UART1_sendStringByDma((u8*)&ch,1);	
	UART1_sendByte(ch);    
	return ch;
}
#endif 
/**********************缓冲区数组定义**************************/
uint8_t UART1_sendBuff[UART1_TRAN_BUFF_LENGTH];//发送缓冲
uint8_t UART1_receiveBuff[UART1_RECV_BUFF_LENGTH];//接收缓冲
/*接收发送标志*/
uint32_t UART1_receivedFlag=0;
uint32_t UART1_sendFlag=0;
/********************************************************************
*函数名称       : UART1_init
*功能说明       : STM32103 UART11 接口初始化
*参数说明       :
                  [IN]
												uint32_t boud 波特率
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void UART1_init(uint32_t boud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef    DMA_Initstructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE );

  /*串口中断配置*/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
  /*串口配置*/
	USART_InitStructure.USART_BaudRate=boud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);
	
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);/*开启空闲中断*/
 
  USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	/*开启发送接收DMA*/
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC); 
	
	/*串口引脚初始化*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*串口DMA通道配置*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
  /*串口接收DMA通道配置*/
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
	
	/*串口发送DMA通道配置*/
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
*函数名称       : UART1_sendByteDma
*功能说明       : STM32103 UART1使用DMA发送数据
*参数说明       :
                  [IN]
												
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void UART1_sendStringByDma(uint8_t *buffer,uint32_t length)
{
	/*如果还没发送完成*/
	if(DMA1_Channel4->CNDTR)
	{
	}
	else/*开启这次传输*/
	{
		DMA_Cmd(DMA1_Channel4,DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC4);
		DMA1_Channel4->CNDTR=length;
		DMA1_Channel4->CMAR=(uint32_t)buffer;
		DMA_Cmd(DMA1_Channel4,ENABLE);
	}
}
/********************************************************************
*函数名称       : UART1_sendByte
*功能说明       : STM32103 UART11不使用中断发送
*参数说明       :
                  [IN]
												char ch 待发送的字符
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void UART1_sendByte(uint8_t ch)
{
	
	USART_SendData(USART1, (uint16_t) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
/********************************************************************
*函数名称       : UART1_sendByte
*功能说明       : STM32103 UART11不使用中断接收
*参数说明       :
                  [IN]
												无
                  [OUT]
                        无
*函数返回				: 接收到的数据
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
int UART1_receive(void)
{
	u16 temp;
	while(!USART_GetFlagStatus(USART1,USART_FLAG_RXNE));//判断 是否 接收完成  
  temp = USART_ReceiveData(USART1);
	return temp;
}
/********************************************************************
*函数名称       : UART1_sendSignedChar
*功能说明       : STM32103 UART1发送有符号的char型数据，文本格式发送
*参数说明       :
                  [IN]
												无
                  [OUT]
                        无
*函数返回				: 接收到的数据
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void UART1_sendSignedChar(char ch)
{
	uint8_t temp;
	if(ch&0x80)//负数
	{
		temp=~(ch-1);
		UART1_sendByte('-');
		UART1_sendHex(temp);
	}
	else//正数或者0
	{
		UART1_sendHex(ch);
	}
}
/********************************************************************
*函数名称       : UART1_sendFloat
*功能说明       : STM32103 UART1发送float类型的数据，发送数据的原始存储类型
*参数说明       :
                  [IN]
												无
                  [OUT]
                        无
*函数返回				: 接收到的数据
*修改时间				: 
*备注						:
*作者           : 武旭东
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
*函数名称       : UART1_sendHex
*功能说明       : STM32103 UART11将数转化为16进制发送
*参数说明       :
                  [IN]
												unsigned char data 待发送的数据
                  [OUT]
                        无
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
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
*函数名称       : UART1_sendToMyView
*功能说明       : STM32103 UART11发送图像数据给PC机，Myview显示
*参数说明       :
                  [IN]
												unsigned char *data 待发送的数据
                  [OUT]
                        无
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
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
*函数名称       : UART1_sendToCCDVIEW
*功能说明       : STM32103 UART11发送图像数据给PC机，CCDview显示
*参数说明       :
                  [IN]
												unsigned char *data 待发送的数据
                  [OUT]
                        无
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
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
       num = USART1->SR;/*读取状态标志位，读取就会清除标志位*/
       num = USART1->DR; 
       DMA_Cmd(DMA1_Channel5,DISABLE);
       num = UART1_RECV_BUFF_LENGTH -  DMA_GetCurrDataCounter(DMA1_Channel5);
       UART1_receiveBuff[num] = '\0';
       DMA1_Channel5->CNDTR=128;     
       DMA_Cmd(DMA1_Channel5,ENABLE);
       UART1_receivedFlag = 1;           
    }
}
