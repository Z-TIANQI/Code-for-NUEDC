/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : pid.c
*����           : STM32103 pid��������ļ�
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 
*��ע           : 
*********************************************************************/
#include "pid.h"
/*����ʵ��*/
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
void cleanPid(PID *pidInf)
{
	pidInf->error=0;
	pidInf->lastError=0;
	pidInf->lastLastError=0;
	pidInf->lastDifferent=0;
	pidInf->integral=0;
	pidInf->pidOut=0;
}
/********************************************************************
*��������       : getAntiSaturationIntegral
*����˵��       : �õ������ͻ���ֵ
*����˵��       :
                  [IN]
									     float error      ���ֵ
                       float threshold	�����ͻ�����ֵ
                       float *integral  ����ֵ											 
                  [OUT]
                        ��
*��������				: ��
*�޸�ʱ��				:2017.3.31
*��ע						:
*����           :����
*********************************************************************/
void getAntiSaturationIntegral(float error,float threshold,float *integral)
{
	/*�������ֵ���������ֵ*/
	if(*integral>threshold)
	{
		if(error<0)/*ֻ�ۻ���ֵ*/
		{
			*integral+=error;
		}
	}
	else if(*integral<(-(threshold)))/*�������ֵС������Сֵ*/
	{
	  if(error>0)/*ֻ�ۻ���ֵ*/
		{
			*integral+=error;
		}
	}
	else/*����ֵ������Χ��*/
	{
		*integral+=error;
	}
}
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
                 2.PID���Կ���ѡ�񣬻��ַ��룬�����ͻ��֣�����ȫ΢�֣�PID����޷����ԣ�ͨ������ѡ��λ����ѡ��
*����           :����
*********************************************************************/
void calculatePid(PID *pidInf,float input,float setValue)
{
	/*----------����������΢����----------*/
	float proportion;
	float integral=pidInf->integral;//����ֵ��ֵ
	float differential;
	float pidOut;
	
	/*----------����ƫ��ֵ----------*/
	pidInf->error=setValue-input;
	
	/*------------------------------���������------------------------------*/
	proportion=pidInf->kp*pidInf->error;
	
	/*------------------------------���������------------------------------*/	
	/*���ʹ���˻��ַ������*/
	if(pidInf->pidStrategy&PID_SEPARATION_INTEGRAL_MASK)
	{
		/*��������˻��ַ�����ֵ����ʹ�û������ƫ���Сʱʹ�û�����*/
		if(pidInf->error>pidInf->separationIntegralThreshold||pidInf->error<(-(pidInf->separationIntegralThreshold)))
		{
			integral=0;
		}	
		else/*û�г������ַ�����ֵ��ʹ�û�����*/
		{
			/*���ʹ���˿����ֱ��Ͳ���*/
			if(pidInf->pidStrategy&PID_ANTISATURATION_INTEGRAL_MASK)
			{
				getAntiSaturationIntegral(pidInf->error,pidInf->antiSaturationIntegralThreshold,&integral);
			}
			else/*���û��ʹ�ÿ����ͻ��ֲ���*/
			{
				integral+=pidInf->error;
			}	
		}
	}
	else/*û��ʹ���˻��ַ������*/
	{		
			/*���ʹ���˿����ֱ��Ͳ���*/
			if(pidInf->pidStrategy&PID_ANTISATURATION_INTEGRAL_MASK)
			{
				getAntiSaturationIntegral(pidInf->error,pidInf->antiSaturationIntegralThreshold,&integral);
			}
			else/*���û��ʹ�ÿ����ͻ��ֲ���*/
			{
				integral+=pidInf->error;
			}				
	}
  integral=pidInf->ki*integral;/*�����������*/
	
	/*------------------------------����΢����------------------------------*/	
	/*���ʹ���˲���ȫ΢��*/
	if(pidInf->pidStrategy&PID_INCOMPLETE_DIFFERENT_MASK)
	{
		/*���㱾��΢����*/
		differential=pidInf->kd*(pidInf->error-pidInf->lastError);
		differential=differential*pidInf->uncompleteDifferentCoefficient+(1-pidInf->uncompleteDifferentCoefficient)*pidInf->lastDifferent;
	}
	else/*���û��ʹ��*/
	{
		differential=pidInf->kd*(pidInf->error-pidInf->lastError);
	}
	
	/*----------����PIDֵ----------*/	
	pidOut=proportion+integral+differential;
	
	/*-----------PID�޷�����---------*/	
  if(pidInf->pidStrategy&PID_IS_OUT_LIMIT_MASK)/*���PID��Ҫ�޷�*/
	{
		if(pidOut>pidInf->pidOutLimtThreshold)
		{
			pidOut=pidInf->pidOutLimtThreshold;
		}
		else if(pidOut<(-(pidInf->pidOutLimtThreshold)))
		{
			pidOut=-(pidInf->pidOutLimtThreshold);
		}
		else
		{
			
		}
	}
	else
	{
		
	}
	/*����PID���ֵ*/
	pidInf->pidOut=pidOut;
	
	/*----------��������----------*/
  pidInf->lastLastError=pidInf->lastError;	
	pidInf->lastError=pidInf->error;
	pidInf->lastDifferent=differential;
	pidInf->integral=integral;
}	
