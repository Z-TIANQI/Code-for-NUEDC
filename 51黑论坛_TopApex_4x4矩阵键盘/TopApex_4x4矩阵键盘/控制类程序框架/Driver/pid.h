/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : pid.h
*����           : STM32103 pid��������ļ�
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 
*��ע           : 
*********************************************************************/
#ifndef _PID_H
#define _PID_H
#include "stm32f10x.h"
/**********�궨��**********/
#define PID_SEPARATION_INTEGRAL_MASK     0x01 //���ַ������ѡ��λ����1��ʾ��Ҫ�����ʽ
#define PID_ANTISATURATION_INTEGRAL_MASK 0x02 //�����ͻ��ֲ���ѡ��λ����1��ʾ��Ҫ�����ʽ
#define PID_INCOMPLETE_DIFFERENT_MASK    0x04 //����ȫ΢�ֲ���ѡ��λ����1��ʾ��Ҫ�����ʽ
#define PID_IS_OUT_LIMIT_MASK            0x08 //PID����Ƿ���Ҫ�޷�ѡ��λ����1��ʾ��Ҫ�����ʽ
/**************�������Ͷ���***************/
/*PID�����������*/
typedef struct PID
{
	float    kp;//����ϵ��
	float    ki;//����ϵ��
	float    kd;//΢��ϵ��
	
	float    integral;//����ֵ

	float    error;//���ε�ƫ��ֵ
	float    lastError;//��һ�ε�ƫ��ֵ	
	float    lastLastError;//���ϴε�ƫ��ֵ��Ӣ�Ĳ�̫׼ȷ����ѡbeforeLastError
	float    lastDifferent;//�ϴε�΢������ڲ���ȫ΢��
	
	const float separationIntegralThreshold;	//���ַ�����ֵ
	const float antiSaturationIntegralThreshold;//�����ͻ�����ֵ
	const float uncompleteDifferentCoefficient; //����ȫ΢��ϵ��
	const float pidOutLimtThreshold;//PID�޷�ֵ
	
	uint8_t  pidStrategy;//PID����ѡ�񣬸�����Ӧλ������PID���
	float    pidOut;//PID����֮�����ֵ
}PID;
/**************��������***************/
/********************************************************************
*��������       : calculatePid
*����˵��       : ����PID
*����˵��       :
                  [IN]
									      PID   pidInf    PID��Ϣ
                        float input     ��ǰ����ֵ
												float setValue  ����Ŀ��ֵ
                  [OUT]
                        ��
*��������				: V1.0
*�޸�ʱ��				:2017.3.31
*��ע						:
                 1.ʹ�õĹ�ʽΪ��kp*error+ki*����ֵ+kd*΢��ֵ��δ���ǻ��ֳ�����΢�ֳ���
                   ���ڳ�����˵������ֻ��һ�������Ĺ�ϵ
                 2.
*����           :����
*********************************************************************/
void calculatePid(PID *pidInf,float input,float setValue);
/********************************************************************
*��������       : cleanPid
*����˵��       : PID�������
*����˵��       :
                  [IN]
									     PID *pidInf						 
                  [OUT]
                        ��
*��������				: ��
*�޸�ʱ��				:2017.3.31
*��ע						:
*����           :����
*********************************************************************/
void cleanPid(PID *pidInf);
#endif
