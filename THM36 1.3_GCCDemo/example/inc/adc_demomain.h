/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: adc_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __ADC_DEMO_H
#define __ADC_DEMO_H

#include "tmc_adc.h"

extern ADC_HandleTypeDef TMC_ADC;
void ADC_InterruptHandle(void);

void ADC_Polling_Demo(void);
void ADC_SelSingleChannel_Demo(void);
void ADC_IRQ_Demo(void);
void ADC_DMA_Demo(void);
void ADC_DeomMain(void);
void ADC_DMA_Handle(void);

void ADC_Deom_Main(void);

#endif
