/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tim_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_tim.h"
#include "tmc_rcc.h"
#include "tim_demomain.h"
#include "tmc_gpio.h"
//TMC(20240408)-zt:htimx -> htim1,timinit->timinit1
TIM_HandleTypeDef htim1; 
TIM_TimeInitTypeDef timinit1;

ErrorStatus TIM_ContinuityMode_Test(void);
volatile uint32_t TimeOverFlowNumbers;

ErrorStatus TIM_SingleMode(void);
ErrorStatus TIM_SingleMode_IT(void);

void TMC_TIM_StructInit(TIM_HandleTypeDef *htimx)
{
	htimx->Instance = TIM1;
	/* Reset the TIM1 state.*/
	htimx->State = TIM_STATE_RESET; 
	
	htimx->Init = &timinit1;
}

/**
    * @brief TIM_SingleMode:
                1.The clock source initializing TIM1 is HSI's 4-frequency division
				2.Set to single mode, 16bit counter, clock source regardless of frequency
				3.Set the count value to 0xFFFF and wait for the count to overflow
                4.Set 32bit counter, set the count value to 0x0001FFFF and wait for the count to overflow
    * @param  None
    * @retval None
 */
ErrorStatus TIM_SingleMode(void)
{
	htim1.Init = &timinit1;
	TMC_TIM_StructInit(&htim1);
	/******************************16 bits, single mode.******************************/
	TMC_TIM_DeInit(&htim1);
	/*TIM configure as SINGLE mode.*/
	timinit1.TIM_CounterMode = TIM_MODE_SINGEL;
	
	timinit1.TIM_Presclar = TIM_PRESCLAR_NOT_DIV;
	
	timinit1.TIM_SizeMode = TIM_SIZE_16BIT;
	
	TMC_TIM_StructInit(&htim1);
	
	
	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	/*Set value.*/
	if(TMC_TIM_Start(&htim1,0xFFFF)!= TMC_OK)
	{
		return ERROR;
	}
	TMC_TIM_Stop(&htim1);
	TMC_TIM_DeInit(&htim1);
	/******************************32 bits, single mode.******************************/
	
	TMC_TIM_DeInit(&htim1);
	timinit1.TIM_SizeMode = TIM_SIZE_32BIT;
	TMC_TIM_StructInit(&htim1);

	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	/*Set value.*/
	if(TMC_TIM_Start(&htim1,0x0001FFFF)!= TMC_OK)
	{
		return ERROR;
	}
	TMC_TIM_Stop(&htim1);
	TMC_TIM_DeInit(&htim1);
	return SUCCESS;
}

/**
    * @brief TIM_SingleMode_IT:
                1.The clock source initializing TIM1 is HSI's 4-frequency division
				2.Set to single mode, 16bit counter, clock source regardless of frequency
				3.Set the count value to 0xFFFF and enable interrupt then wait for the count to overflow
                4.Set 32bit counter, set the count value to 0x0001FFFF and enable interrupt then wait for the count to overflow
    * @param  None
    * @retval None
 */
ErrorStatus TIM_SingleMode_IT(void)
{
	htim1.Init = &timinit1;
	TMC_TIM_StructInit(&htim1);
	TMC_TIM_DeInit(&htim1);
	/******************************16 bits, single mode.******************************/
	/*TIM set as SINGLE mode.*/
	timinit1.TIM_CounterMode = TIM_MODE_SINGEL;

	timinit1.TIM_Presclar = TIM_PRESCLAR_NOT_DIV;

	timinit1.TIM_SizeMode = TIM_SIZE_16BIT;
	TMC_TIM_StructInit(&htim1);
	
	/*Timer initialize.*/
	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	/*Generate interrupt after 0xFFFF times.*/
	TMC_TIM_StartIT(&htim1,0xFFFF);
	while(htim1.State == TIM_STATE_BUSY);
	
	/******************************32 bits, single mode.******************************/
	TMC_TIM_DeInit(&htim1);

	timinit1.TIM_SizeMode = TIM_SIZE_32BIT;
	TMC_TIM_StructInit(&htim1);
	
	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	/*Generate interrupt after 0x0001FFFF times.*/
	TMC_TIM_StartIT(&htim1,0x0001FFFF);
	while(htim1.State == TIM_STATE_BUSY);
	TMC_TIM_DeInit(&htim1);
	return SUCCESS;
}

/**
    * @brief TIM_ContinuityMode_Test:
                1.The clock source initializing TIM1 is HSI's 4-frequency division
				2.Set to reload mode, 16bit counter, clock source regardless of frequency
				3.Set the count value to 0xFFFF and enable interrupt 
				4.TimeOverFlowNumbers increment in interrupt handler 
				5.when TimeOverFlowNumbers greater than 10 stop timer
                6.Set 32bit counter, set the count value to 0x0001FFFF and Repeat the above process
				7.Set to free-running mode, 16bit and 32bit counter and Repeat the above process
    * @param  None
    * @retval None
 */
ErrorStatus TIM_ContinuityMode_Test(void)
{
	TimeOverFlowNumbers = 0;
	htim1.Init = &timinit1;
	TMC_TIM_StructInit(&htim1);
	/******************************16 bits, reload mode.******************************/
	/*Timer set as Reload mode.*/
	timinit1.TIM_CounterMode = TIM_MODE_CONTINUITY_RELOAD;
	
	timinit1.TIM_Presclar = TIM_PRESCLAR_NOT_DIV;
	
	timinit1.TIM_SizeMode = TIM_SIZE_16BIT;
	TMC_TIM_StructInit(&htim1);
	TMC_TIM_DeInit(&htim1);
	
	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	
	/*Recover the value as 0xFFFF after timer overflow.*/
	TMC_TIM_StartIT(&htim1,0xFFFF);
	/*Exit after 10 times.*/
	while(TimeOverFlowNumbers<10);
	TMC_TIM_Stop(&htim1);
	TimeOverFlowNumbers = 0;
	/******************************32 bits, reload mode.******************************/
	TMC_TIM_DeInit(&htim1);

	timinit1.TIM_SizeMode = TIM_SIZE_32BIT;
	TMC_TIM_StructInit(&htim1);

	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	/*Recover the value as 0x1FFFF after timer overflow.*/
	TMC_TIM_StartIT(&htim1,0x1FFFF);
	/*Exit after 10 times.*/
	while(TimeOverFlowNumbers<10);
	TMC_TIM_Stop(&htim1);
	TimeOverFlowNumbers = 0;
	/******************************16 bits, free mode.******************************/
	TMC_TIM_DeInit(&htim1);
	
	timinit1.TIM_CounterMode = TIM_MODE_CONTINUITY_NO_RELOAD;
	timinit1.TIM_SizeMode = TIM_SIZE_16BIT;
	TMC_TIM_StructInit(&htim1);
	
	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	
	/*Recover the value as 0xFFFF after timer overflow.*/
	TMC_TIM_StartIT(&htim1,0xFFFF);
	/*Exit after 1 times.*/
	while(TimeOverFlowNumbers<1);
	TMC_TIM_Stop(&htim1);
	TimeOverFlowNumbers = 0;
	
	/******************************32 bits, free mode.******************************/
	TMC_TIM_DeInit(&htim1);

	timinit1.TIM_CounterMode = TIM_MODE_CONTINUITY_NO_RELOAD;
	timinit1.TIM_SizeMode = TIM_SIZE_32BIT;
	TMC_TIM_StructInit(&htim1);

	if (TMC_TIM_Init(&htim1) != TMC_OK)
	{
		return ERROR;
	}
	/*Recover the value as 0x1FFFF after timer overflow.*/
	TMC_TIM_StartIT(&htim1,0x1FFFF);
	/*Exit after 1 times.*/
	while(TimeOverFlowNumbers<1);
	TimeOverFlowNumbers = 0;
	TMC_TIM_Stop(&htim1);
	return SUCCESS;
	
}

void TMC_TIM_Callback(TIM_HandleTypeDef *htim)
{
	/*Overflow counter*/
	TimeOverFlowNumbers++;
}

void TMC_TIM_MspInit(TIM_HandleTypeDef *htimx)
{
	GPIO_InitTypeDef GPIOx_Init;
	TMC_GPIO_StructInit(&GPIOx_Init);
	
	/*This demo configure TIM1-TIM6,user can configure TIMs in condition.*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIMER1_2, ENABLE);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	NVIC_EnableIRQ(TIMER2_IRQn);
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_TIMER3_4, ENABLE);
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
	NVIC_EnableIRQ(TIMER3_IRQn);
	NVIC_ClearPendingIRQ(TIMER4_IRQn);
	NVIC_EnableIRQ(TIMER4_IRQn);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIMER5_6, ENABLE);
	
	NVIC_ClearPendingIRQ(TIMER5_IRQn);
	NVIC_EnableIRQ(TIMER5_IRQn);
	NVIC_ClearPendingIRQ(TIMER6_IRQn);
	NVIC_EnableIRQ(TIMER6_IRQn);
	/*Timer1 clock source, as HSI div (3+1).*/
	RCC_TimerClockConfig(PERIPHERAL_TIMER1, RCC_TIMER_CLKSRC_HSI, 3, ENABLE);
	/*Timer2 clock source, as HSI div (3+1).*/
	RCC_TimerClockConfig(PERIPHERAL_TIMER2, RCC_TIMER_CLKSRC_HSI, 3, ENABLE);
	/*Timer3 clock source, as HSI div (3+1).*/
	RCC_TimerClockConfig(PERIPHERAL_TIMER3, RCC_TIMER_CLKSRC_HSI, 3, ENABLE);
	/*Timer4 clock source, as HSI div (3+1).*/
	RCC_TimerClockConfig(PERIPHERAL_TIMER4, RCC_TIMER_CLKSRC_HSI, 3, ENABLE);
	/*Timer5 clock source, as HSI div (3+1).*/
	RCC_TimerClockConfig(PERIPHERAL_TIMER5, RCC_TIMER_CLKSRC_HSI, 3, ENABLE);
	/*Timer6 clock source, as HSI div (3+1).*/
	RCC_TimerClockConfig(PERIPHERAL_TIMER6, RCC_TIMER_CLKSRC_HSI, 3, ENABLE);

	/*GPIOB13 as a indicate pin.*/
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	GPIOx_Init.Mode = GPIO_MODE_OUT;
	
	GPIOx_Init.Otype = GPIO_MODE_OUTPUT_PP;
	GPIOx_Init.SMIT = GPIO_INPUTSCHMIT_DISABLE;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;
	GPIOx_Init.Pin = GPIO_PIN_13;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);
}

void TMC_TIM_MspDeInit(TIM_HandleTypeDef *htimx)
{
	
	/*This demo configure TIM1-TIM6,user can configure TIMs in condition.*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIMER1_2, DISABLE);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_DisableIRQ(TIMER1_IRQn);
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	NVIC_DisableIRQ(TIMER2_IRQn);

	RCC_APB3PeriphClockCmd(RCC_APB3Periph_TIMER3_4, DISABLE);
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
	NVIC_DisableIRQ(TIMER3_IRQn);
	NVIC_ClearPendingIRQ(TIMER4_IRQn);
	NVIC_DisableIRQ(TIMER4_IRQn);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIMER5_6, DISABLE);
	NVIC_ClearPendingIRQ(TIMER5_IRQn);
	NVIC_DisableIRQ(TIMER5_IRQn);
	NVIC_ClearPendingIRQ(TIMER6_IRQn);
	NVIC_DisableIRQ(TIMER6_IRQn);	
	RCC_TimerClockConfig(PERIPHERAL_TIMER1, RCC_TIMER_CLKSRC_HSI, 3, DISABLE);
	RCC_TimerClockConfig(PERIPHERAL_TIMER2, RCC_TIMER_CLKSRC_HSI, 3, DISABLE);
	RCC_TimerClockConfig(PERIPHERAL_TIMER3, RCC_TIMER_CLKSRC_HSI, 3, DISABLE);
	RCC_TimerClockConfig(PERIPHERAL_TIMER4, RCC_TIMER_CLKSRC_HSI, 3, DISABLE);
	RCC_TimerClockConfig(PERIPHERAL_TIMER5, RCC_TIMER_CLKSRC_HSI, 3, DISABLE);
	RCC_TimerClockConfig(PERIPHERAL_TIMER6, RCC_TIMER_CLKSRC_HSI, 3, DISABLE);	
}

void TIM_Test(void)
{
	TIM_Demo_TypefDef TIM_Demo;
	TIM_Demo = TIM_Single_IT;
	if(TIM_Demo == TIM_Single)
	{
		TIM_SingleMode();
	}
	if(TIM_Demo == TIM_Single_IT)
	{
		TIM_SingleMode_IT();
	}
	if(TIM_Demo == TIM_Continuity_IT)
	{
		TIM_ContinuityMode_Test();
	}
}

void TIMER_DemoMain(void)
{
	TIM_Test();
}



/**
    * @brief  Initial of using Timer moudle to delay time
    * @param  None
    * @retval None
 */
void TMC_Timer_Delay_Init(void)
{
	/* Open Timer peripheral clock ,Set the Timer Division / Timer Clock Enable / Timer Source Clock*/
	RCC_CLK->CLKSLV2SCON |= RCC_APB2Periph_TIMER1_2;
	RCC_CLK->CLKTIM1SEL = RCC_TIMER_CLKSRC_HSI | 0x1UL << 4U | (0xBUL << 8U);
	/* Set the Timer Prescaler Value / Timer Size / Timer Mode*/
	TIM1->CONTROL = 0;
	TIM1->CONTROL |= TIM_PRESCLAR_NOT_DIV | TIM_SIZE_32BIT | TIM_MODE_SINGEL;
	/* Set the Timer reload value */
	TIM1->LOAD = 0xFFFFFFFF;
	/* Timer Enable */
	TIM1->CONTROL |= 1UL << 7U; 
}
/**
    * @brief  Use Timer moudle to delay time,which is only effective within 71 minutes after TMC_Timer_delay_us_Init
    * @param  None
    * @retval None
 */
void TMC_Timer_Delay_us(uint32_t delay_us)
{
	uint32_t LoadFirstValue;
	LoadFirstValue = TIM1->VALUE;
	while(LoadFirstValue - TIM1->VALUE < delay_us);
}
/**
    * @brief  Deinitial of using Timer moudle to delay time
    * @param  None
    * @retval None
 */
void TMC_Timer_Delay_DeInit(void)
{
	/* Clear Timer Status*/
	TIM1->CLR = 1UL;
	/* Timer Disable */
	TIM1->CONTROL &= ~(1UL<<7U); 
	/* Close Timer peripheral clock ,Disable the Timer Clock*/
	RCC_CLK->CLKSLV2SCON &= ~RCC_APB2Periph_TIMER1_2;
	RCC_CLK->CLKTIM1SEL  &= ~(0x1UL << 4U);
}

/**
  * @}
  */ 

/**
  * @}
  */

