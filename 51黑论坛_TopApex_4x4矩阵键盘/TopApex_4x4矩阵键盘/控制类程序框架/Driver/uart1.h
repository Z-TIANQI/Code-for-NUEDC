/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : uart.h
*����           : STM32103 ������غ���ͷ�ļ�
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 
*��ע           : ��δ����ϸ�����ϣ���򵥵ĳ�ʼ��
*********************************************************************/
#ifndef __uart_H__
#define __uart_H__
#include "stm32f10x.h"
#include <stdio.h>

#include "system.h"
/************************����������****************************/
#define UART1_TRAN_BUFF_LENGTH  50   //���ڷ��ͻ���������
#define UART1_RECV_BUFF_LENGTH  50   //���ڽ��ջ���������
/**********************���������鶨��**************************/
extern uint8_t UART1_sendBuff[UART1_TRAN_BUFF_LENGTH];
extern uint8_t UART1_receiveBuff[UART1_RECV_BUFF_LENGTH];
extern uint32_t UART1_receivedFlag;
extern uint32_t UART1_sendFlag;
/********************************************************************
*��������       : UART1_init
*����˵��       : STM32103 UART1 �ӿڳ�ʼ��
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
void UART1_init(uint32_t boud);	
void UART1_sendByte(uint8_t ch);
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
void UART1_sendStringByDma(uint8_t *buffer,uint32_t length);
int UART1_receive(void);
void UART1_sendToCCDVIEW(unsigned char *data);
void UART1_sendToMyView(unsigned char *data);
void UART1_sentInf(int track);
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
void UART1_sendSignedChar(char ch);
void UART1_sendHex(unsigned char data);
void UART1_sendFloat(float data);
uint32_t systemRunUnpackReceiveBuff(uint8_t buffData[16]);
void sendCtrlInfToPc(unsigned char value);

void sendSteerDataInfToPc(unsigned char offest);
void sendMotorDataInfToPc(uint16_t leftEncoderValue,uint16_t RightEncoderValue);
uint32_t systemMotorTestUnpackReceiveBuff(uint8_t *buffData);
/*���ݹ������������ݷ���*/
typedef union floatTo4char
{
	float floatData;
	unsigned char charData[4];
}floatTo4char;
#endif
