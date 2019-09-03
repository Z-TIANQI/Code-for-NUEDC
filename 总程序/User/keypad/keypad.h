#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"

void KEYPad_Init1(void);//功能：行检测初始化
void Mode1_Key(void);
uint16_t KEYPAD_Scan(void);//功能：矩阵按键扫描，返回一个键值

					    
#endif

