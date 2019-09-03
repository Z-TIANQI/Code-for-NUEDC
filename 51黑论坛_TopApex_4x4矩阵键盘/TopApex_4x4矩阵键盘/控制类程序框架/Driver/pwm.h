/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : PWM.h
*描述           : STM32103 PWM驱动函数头文件
*实验平台       : STM32F103ZET6最小系统板
*库版本					: V1.0
*嵌入式系统			: 无
*修改历史				: 
*备注						:
*作者						: 武旭东
*********************************************************************/
#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"
#include "gpio.h"
/******************PWM定时器通道号枚举定义****************/
typedef enum CHn 
{
	CH1=0,//通道1
	CH2,  //通道2
	CH3,  //通道3
	CH4   //通道4
}CHn;
/******************PWM定时器号枚举定义****************/
//此处定义是为了数组索引方便
typedef enum TIMEn //
{
	TIME1=0,//定时器1
	TIME2,  //定时器2
	TIME3,	//定时器3
	TIME4,	//定时器4
	TIME5,	//定时器5
	TIME6,  //定时器6、7为基本定时器，无PWM输出功能
	TIME7,
	TIME8		//定时器8
}TIMEn;
/******************定时器GPIO重映射枚举定义****************/
typedef enum GPIO_ramap
{
	NO_REMAP=0,	//无重映射
	PAR_REMAP1,	//部分重映射1
	PAR_REMAP2,	//部分重映射2
	FULL_REMAP	//完全重映射
}GPIO_ramap;
/******************函数申明******************/
/********************************************************************
*函数名称       : TIM_PWM_init
*功能说明       : STM32103 PWM产生初始化函数
*参数说明       :
                  [IN]
												TIMEn time                     产生需要使用的定时器号
												CHn ch                         定时器通道号
												TIM_GPIO_ramap CH_gpioRemap    PWM输出引脚重映射选择，使用默认引脚输入0，具体复用引脚请看数据手册
												double   freg                  PWM输出脉冲频率
												uint32_t duty                  PWM输出脉冲占空比，范围0~~999，代表占空比0%~~99.9%
                  [OUT]
                        无
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void TIM_PWM_init(TIMEn time,CHn ch,GPIO_ramap CH_gpioRemap,double freg,uint32_t duty);
/********************************************************************
*函数名称       : TIM_PWM_init
*功能说明       : STM32103 更改PWM输出脉冲频率
*参数说明       : 
                  [IN]
												TIMEn  time            产生需要使用的定时器号
                        double freg            PWM输出脉冲频率 
                  [OUT]
                        无												
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void TIM_PWM_freq(TIMEn time,double freg);
/********************************************************************
*函数名称       : TIM_PWM_duty
*功能说明       : STM32103 更改PWM输出脉冲占空比
*参数说明       : 
                  [IN]
												TIMEn time               产生需要使用的定时器号
												CHn ch
                        uint32_t duty            PWM输出脉冲频率 
                  [OUT]
                        无												
*函数返回				: V1.0
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void TIM_PWM_duty(TIMEn time,CHn ch,uint32_t duty);
uint32_t pscCal(double fre,uint32_t *nowFre);
#endif
