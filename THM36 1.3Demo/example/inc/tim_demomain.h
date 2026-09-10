/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tim_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

#ifndef __TIMER_DEMO_H
#define __TIMER_DEMO_H

void TIM_Test(void);

void TIMER_DemoMain(void);
void TMC_Timer_Delay_Init(void);
void TMC_Timer_Delay_us(uint32_t delay_us);
void TMC_Timer_Delay_DeInit(void);

typedef enum 
{
	TIM_Single = 0,
	TIM_Single_IT =1,
	TIM_Continuity_IT = 2,
} TIM_Demo_TypefDef;

#endif

