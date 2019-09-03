/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : sch.h
*描述           : 系统调度器
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 
*备注           : 使用了TIM4做为系统调度时基			
*********************************************************************/
#include "stm32f10x.h"
/*系统任务时间结构体定义*/
typedef struct sysTaskTime
{
	/*前两个暂未用*/
	u8 check_flag;/*检测本次任务执行完毕没有*/
	u8 err_flag;	/*时间片出错标志*/
	/**/
	uint16_t cnt1ms;
	uint16_t cnt2ms;
	uint16_t cnt5ms;
	uint16_t cnt10ms;
	uint16_t cnt20ms;
	uint16_t cnt50ms;
	uint16_t cnt500ms;
	uint16_t cnt1000ms;
}sysTaskTime;

/********************************************************************
*函数名称       : SCH_init
*功能说明       : 调度器初始化函数
*参数说明       :
                  [IN]
									     uint32_t periodInMs  调度器周期，最小为1ms，单位为ms
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						: 使用了定时器7
*作者           : 武旭东
*********************************************************************/
void SCH_init(uint32_t periodInMs);
/********************************************************************
*函数名称       : SCH_loop
*功能说明       : 调度器任务处理函数
*参数说明       :
                  [IN]
									     无
                  [OUT]
                        无
*函数返回				: V1.0
*修改时间				: 
*备注						: 使用了定时器7
*作者           : 武旭东
*********************************************************************/
void SCH_loop(void);

