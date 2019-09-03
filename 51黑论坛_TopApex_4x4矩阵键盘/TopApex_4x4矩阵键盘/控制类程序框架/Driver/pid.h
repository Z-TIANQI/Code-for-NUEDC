/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : pid.h
*描述           : STM32103 pid控制相关文件
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 
*备注           : 
*********************************************************************/
#ifndef _PID_H
#define _PID_H
#include "stm32f10x.h"
/**********宏定义**********/
#define PID_SEPARATION_INTEGRAL_MASK     0x01 //积分分离策略选择位，置1表示需要这个方式
#define PID_ANTISATURATION_INTEGRAL_MASK 0x02 //抗饱和积分策略选择位，置1表示需要这个方式
#define PID_INCOMPLETE_DIFFERENT_MASK    0x04 //不完全微分策略选择位，置1表示需要这个方式
#define PID_IS_OUT_LIMIT_MASK            0x08 //PID输出是否需要限幅选择位，置1表示需要这个方式
/**************数据类型定义***************/
/*PID相关数据类型*/
typedef struct PID
{
	float    kp;//比例系数
	float    ki;//积分系数
	float    kd;//微分系数
	
	float    integral;//积分值

	float    error;//本次的偏差值
	float    lastError;//上一次的偏差值	
	float    lastLastError;//上上次的偏差值，英文不太准确，可选beforeLastError
	float    lastDifferent;//上次的微分项，用于不完全微分
	
	const float separationIntegralThreshold;	//积分分离阈值
	const float antiSaturationIntegralThreshold;//抗饱和积分阈值
	const float uncompleteDifferentCoefficient; //不完全微分系数
	const float pidOutLimtThreshold;//PID限幅值
	
	uint8_t  pidStrategy;//PID策略选择，根据相应位来计算PID输出
	float    pidOut;//PID计算之后输出值
}PID;
/**************函数申明***************/
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
                 2.
*作者           :武旭东
*********************************************************************/
void calculatePid(PID *pidInf,float input,float setValue);
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
void cleanPid(PID *pidInf);
#endif
