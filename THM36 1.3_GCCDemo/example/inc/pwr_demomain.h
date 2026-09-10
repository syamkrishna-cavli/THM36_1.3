/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwr_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __PWR_DEMO_H
#define __PWR_DEMO_H

typedef enum {
	RTCwakeup = 0,
	IWDTwakeup = 1,
}wakeup_source_t;

void PWR_Stop_Demo_IRQ_WakeUp(uint8_t Cpu_Mode, wakeup_source_t wakeup_source);
void PWR_Standby_DemoMain(void);
void PWR_Stop_DemoMain(void);

/** @defgroup EXTI Wake Up Source define
  * @{
  */
#define EXTI_WAKEUP_SOURCE_GPIO_GB13				(1UL<<0)	 
#define EXTI_WAKEUP_SOURCE_GPIO_GD4					(1UL<<1)
#define EXTI_WAKEUP_SOURCE_USB							(1UL<<2)
#define EXTI_WAKEUP_SOURCE_RTC							(1UL<<3)
#define EXTI_WAKEUP_SOURCE_IWDT							(1UL<<4)

extern uint32_t EXTI_State_GPIO_A;
extern uint32_t EXTI_State_GPIO_B;
extern uint32_t EXTI_State_GPIO_C;
extern uint32_t EXTI_State_GPIO_D;
extern uint32_t EXTI_State_GPIO_E;
extern uint32_t EXTI_State_USB_Resume_Rsing;
extern uint32_t EXTI_State_RTC_Alarm;				
extern uint32_t EXTI_State_IWDT_Alarm;

void TMC_EXTI_CpltCallback_GPIOA(uint32_t uiState);
void TMC_EXTI_CpltCallback_GPIOB(uint32_t uiState);
void TMC_EXTI_CpltCallback_GPIOC(uint32_t uiState);
void TMC_EXTI_CpltCallback_GPIOD(uint32_t uiState);
void TMC_EXTI_CpltCallback_GPIOE(uint32_t uiState);
void TMC_EXTI_CpltCallback_USB(uint32_t uiState);
void TMC_EXTI_CpltCallback_RTC_Alarm(uint32_t uiState);
void TMC_EXTI_CpltCallback_IWDT_Alarm(uint32_t uiState);




#endif
