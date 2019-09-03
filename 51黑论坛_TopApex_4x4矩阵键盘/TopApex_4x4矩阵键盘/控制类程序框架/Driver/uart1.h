/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : uart.h
*描述           : STM32103 串口相关函数头文件
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 
*备注           : 还未进行细节整合，最简单的初始化
*********************************************************************/
#ifndef __uart_H__
#define __uart_H__
#include "stm32f10x.h"
#include <stdio.h>

#include "system.h"
/************************缓冲区长度****************************/
#define UART1_TRAN_BUFF_LENGTH  50   //串口发送缓冲区长度
#define UART1_RECV_BUFF_LENGTH  50   //串口接收缓冲区长度
/**********************缓冲区数组定义**************************/
extern uint8_t UART1_sendBuff[UART1_TRAN_BUFF_LENGTH];
extern uint8_t UART1_receiveBuff[UART1_RECV_BUFF_LENGTH];
extern uint32_t UART1_receivedFlag;
extern uint32_t UART1_sendFlag;
/********************************************************************
*函数名称       : UART1_init
*功能说明       : STM32103 UART1 接口初始化
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
void UART1_init(uint32_t boud);	
void UART1_sendByte(uint8_t ch);
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
void UART1_sendStringByDma(uint8_t *buffer,uint32_t length);
int UART1_receive(void);
void UART1_sendToCCDVIEW(unsigned char *data);
void UART1_sendToMyView(unsigned char *data);
void UART1_sentInf(int track);
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
void UART1_sendSignedChar(char ch);
void UART1_sendHex(unsigned char data);
void UART1_sendFloat(float data);
uint32_t systemRunUnpackReceiveBuff(uint8_t buffData[16]);
void sendCtrlInfToPc(unsigned char value);

void sendSteerDataInfToPc(unsigned char offest);
void sendMotorDataInfToPc(uint16_t leftEncoderValue,uint16_t RightEncoderValue);
uint32_t systemMotorTestUnpackReceiveBuff(uint8_t *buffData);
/*数据共连体用于数据发送*/
typedef union floatTo4char
{
	float floatData;
	unsigned char charData[4];
}floatTo4char;
#endif
