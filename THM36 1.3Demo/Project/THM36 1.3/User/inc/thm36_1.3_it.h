/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: thm36_1.3_it.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

extern VBAT_HandleTypeDef hVbat;
//TMC(20240408)-zt:huartx->huart1
extern UART_HandleTypeDef huart1;
extern UART5_HandleTypeDef huart5;
//TMC(20240408)-zt:htimx->htim1
extern TIM_HandleTypeDef htim1; /*TIMx ??????*/
//TMC(20240408)-zt:TMC_SPIx->TMC_SPI3
extern SPI_HandleTypeDef TMC_SPI3;
//extern EXTI_StateTypeDef EXTI_State;
extern ADC_HandleTypeDef TMC_ADC;
//TMC(20240408)-zt:TMC_ISO7816M->TMC_ISO7816M2
extern M7816x_HandleTypeDef  TMC_ISO7816M2;
//extern void SPI_DMA_Handle(void);
//TMC(20240408)-zt:hpwm->hpwm1
extern PWM_HandleTypeDef hpwm1;
extern PWM_InitTypeDef	PWM_Init1;
//TMC(20240408)-zt:add hVbat_fd3
extern VBAT_FD3_HandleTypeDef hVbat_fd3;
//extern void M7816x_DMA_Handle(void);
//extern void UART_DMA_Handle(void);
//extern void ADC_DMA_Handle(void);
extern void DMA_InterruptHandle(void);


