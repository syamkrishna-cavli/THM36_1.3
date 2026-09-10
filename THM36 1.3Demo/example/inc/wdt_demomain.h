/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: wdt_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __WDT_DEMO_H
#define __WDT_DEMO_H

typedef enum 
{
	WWDT_Demo_Alarm = 0,
	WWDT_Demo_Interrupt	 = 1,
} WWDT_Demo_TypefDef;

void WDT_AlarmMode(void);
ErrorStatus WDT_InterruptMode(void);
void WDT_Test(void);

void WDT_Demo_Main(void);
#endif
