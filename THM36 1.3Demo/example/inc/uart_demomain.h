/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/
#ifndef __UART_DEMO_H
#define __UART_DEMO_H

void UART_DMA_Handle(void);
void UART_DemoMain(void);

void UART_Demo_Main(void);

typedef enum 
{
	Uartx_Demo_Polling  = 0,
	Uartx_Demo_IRQ	  	= 1,
	Uartx_Demo_DMA	  	= 2
} Uartx_Demo_TypefDef;

#endif
