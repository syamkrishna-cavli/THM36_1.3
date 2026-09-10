/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: vbat_demomain.h 
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __VBAT_DEMO_H
#define __VBAT_DEMO_H

void delay(unsigned int time);
void VBAT_Test(void);
void TMC_VBAT_SD_Test(void);
void TMC_VBAT_TD_Test(void);
void TMC_VBAT_VD_Test(void);
void TMC_VBAT_RTC_Alarm_Test(void);
void VBAT_SENSOR_ExampleOfInitial(void);

void VBAT_DemoMain(void);

#endif

