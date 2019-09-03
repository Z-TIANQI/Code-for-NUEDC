/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : pid.c
*描述           : STM32103 pid控制相关文件
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 
*备注           : 
*********************************************************************/
#include "pid.h"
/*函数实现*/
/********************************************************************
*函数名称       : cleanPid
*功能说明       : PID清除函数
*参数说明       :
                  [IN]
									     PID *pidInf						 
                  [OUT]
                        无
*函数返回				: 无
*修改时间				:2017.3.31
*备注						:
*作者           :武旭东
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
*函数名称       : getAntiSaturationIntegral
*功能说明       : 得到抗饱和积分值
*参数说明       :
                  [IN]
									     float error      误差值
                       float threshold	抗饱和积分阈值
                       float *integral  积分值											 
                  [OUT]
                        无
*函数返回				: 无
*修改时间				:2017.3.31
*备注						:
*作者           :武旭东
*********************************************************************/
void getAntiSaturationIntegral(float error,float threshold,float *integral)
{
	/*如果积分值超过了最大值*/
	if(*integral>threshold)
	{
		if(error<0)/*只累积负值*/
		{
			*integral+=error;
		}
	}
	else if(*integral<(-(threshold)))/*如果积分值小于了最小值*/
	{
	  if(error>0)/*只累积正值*/
		{
			*integral+=error;
		}
	}
	else/*积分值正常范围内*/
	{
		*integral+=error;
	}
}
/********************************************************************
*函数名称       : calculatePid
*功能说明       : 计算PID
*参数说明       :
                  [IN]
									      PID   pidInf    PID信息
                        float input     当前采样值
												float setValue  设置目标值
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				:2017.3.31
*备注						:
                 1.使用的公式为，kp*error+ki*积分值+kd*微分值，未考虑积分常数与微分常数
                   对于程序来说这两个只是一个倍数的关系
                 2.PID策略可以选择，积分分离，抗饱和积分，不完全微分，PID输出限幅策略，通过测量选择位进行选择
*作者           :武旭东
*********************************************************************/
void calculatePid(PID *pidInf,float input,float setValue)
{
	/*----------比例项，积分项，微分项----------*/
	float proportion;
	float integral=pidInf->integral;//积分值赋值
	float differential;
	float pidOut;
	
	/*----------计算偏差值----------*/
	pidInf->error=setValue-input;
	
	/*------------------------------计算比例项------------------------------*/
	proportion=pidInf->kp*pidInf->error;
	
	/*------------------------------计算积分项------------------------------*/	
	/*如果使用了积分分离策略*/
	if(pidInf->pidStrategy&PID_SEPARATION_INTEGRAL_MASK)
	{
		/*如果超过了积分分离阈值，不使用积分项，在偏差较小时使用积分项*/
		if(pidInf->error>pidInf->separationIntegralThreshold||pidInf->error<(-(pidInf->separationIntegralThreshold)))
		{
			integral=0;
		}	
		else/*没有超过积分分离阈值，使用积分项*/
		{
			/*如果使用了抗积分饱和策略*/
			if(pidInf->pidStrategy&PID_ANTISATURATION_INTEGRAL_MASK)
			{
				getAntiSaturationIntegral(pidInf->error,pidInf->antiSaturationIntegralThreshold,&integral);
			}
			else/*如果没有使用抗饱和积分策略*/
			{
				integral+=pidInf->error;
			}	
		}
	}
	else/*没有使用了积分分离策略*/
	{		
			/*如果使用了抗积分饱和策略*/
			if(pidInf->pidStrategy&PID_ANTISATURATION_INTEGRAL_MASK)
			{
				getAntiSaturationIntegral(pidInf->error,pidInf->antiSaturationIntegralThreshold,&integral);
			}
			else/*如果没有使用抗饱和积分策略*/
			{
				integral+=pidInf->error;
			}				
	}
  integral=pidInf->ki*integral;/*计算出积分项*/
	
	/*------------------------------计算微分项------------------------------*/	
	/*如果使用了不完全微分*/
	if(pidInf->pidStrategy&PID_INCOMPLETE_DIFFERENT_MASK)
	{
		/*计算本次微分项*/
		differential=pidInf->kd*(pidInf->error-pidInf->lastError);
		differential=differential*pidInf->uncompleteDifferentCoefficient+(1-pidInf->uncompleteDifferentCoefficient)*pidInf->lastDifferent;
	}
	else/*如果没有使用*/
	{
		differential=pidInf->kd*(pidInf->error-pidInf->lastError);
	}
	
	/*----------计算PID值----------*/	
	pidOut=proportion+integral+differential;
	
	/*-----------PID限幅计算---------*/	
  if(pidInf->pidStrategy&PID_IS_OUT_LIMIT_MASK)/*如果PID需要限幅*/
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
	/*计算PID输出值*/
	pidInf->pidOut=pidOut;
	
	/*----------更新数据----------*/
  pidInf->lastLastError=pidInf->lastError;	
	pidInf->lastError=pidInf->error;
	pidInf->lastDifferent=differential;
	pidInf->integral=integral;
}	
