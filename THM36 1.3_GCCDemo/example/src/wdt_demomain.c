/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: wdt_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_wdt.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "wdt_demomain.h"

WDT_HandleTypeDef hwdt;
WDT_InitTypeDef	 wdt_Init; 
void WDT_Test(void)
{
	WWDT_Demo_TypefDef DemoMode;
	
//	RCC_OscInitTypeDef RCC_OscInitStruct;
//	RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
	GPIO_InitTypeDef GPIOx_Init;
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Otype = GPIO_MODE_OUTPUT_PP;
	GPIOx_Init.Pull = GPIO_PULLUP;
	GPIOx_Init.SMIT = GPIO_INPUTSCHMIT_DISABLE;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;
	GPIOx_Init.Pin = GPIO_PIN_14;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);
	DemoMode = WWDT_Demo_Interrupt;
	wdt_Init.Loadvalue =0;
	wdt_Init.Mode = 0;

	/*The level of GPIO14 to judge whether enter the STOP mode or not.*/
	if(TMC_GPIO_ReadPin(GPIOB,GPIO_PIN_14)== GPIO_PIN_RESET)
	{
		if(DemoMode == WWDT_Demo_Alarm)
		{
			WDT_AlarmMode();
			while(1);
		}
		if(DemoMode == WWDT_Demo_Interrupt)
		{
			WDT_InterruptMode();
		}
	}
}

/**
    * @brief WDT_AlarmMode:
                1.The clock source initializing WDT is HSI's 3-frequency division
				2.Set WDT alarm to reset the chip
				3.Set Loadvalue = 0x100000
                4.Wait for count overflow and chip reset
    * @param  None
    * @retval None
 */
void WDT_AlarmMode(void)
{
	
	hwdt.WDT = WWDT;
	hwdt.Init = &wdt_Init;
	/* Alarm reset system after the WDT count overflow */
	hwdt.Init->Mode = WDT_MODE_ALARM;
	
	/* Set Initial value */
	hwdt.Init->Loadvalue = 0x100000;
	
	if (TMC_WDT_Init(&hwdt) != TMC_OK)
	{
		return;
	}
	
	return;
}

/**
    * @brief WDT_InterruptMode:
                1.The clock source initializing WDT is HSI's 3-frequency division
				2.Set WDT alarm not reset the chip
				3.Set Loadvalue = 0x10
                4.Clear the overflow flag in the interrupt handler
				5.The Loadvalue reloads the count
    * @param  None
    * @retval ErrorStatus
 */
ErrorStatus WDT_InterruptMode(void)
{
	
	hwdt.WDT = WWDT;
	hwdt.Init = &wdt_Init;
	/* Interrupt occurs after the WDT count overflow */
	hwdt.Init->Mode = WDT_MODE_INT;
	
	/* Set Initial value */
	hwdt.Init->Loadvalue = 0x10;
	
	if (TMC_WDT_Init(&hwdt) != TMC_OK)
	{
		return ERROR;
	}
	
	return SUCCESS;
}

void TMC_WDT_MspInit(WDT_HandleTypeDef *hwdt)
{
	NVIC_EnableIRQ(WWDT_IRQn);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_WDT, ENABLE);
	RCC_WWDTClockConfig(RCC_WWDT_CLKSRC_HSI, 0x02, ENABLE);
	/*If this code runs,should consider that WDT will generate the system reset(If doesn't feed dog in time),debug function will lose efficacy.*/
	RCC_WDTResetSystemCmd(RCC_ALARM_RSTEN_WWDT, ENABLE);
}


void TMC_WDT_MspDeInit(WDT_HandleTypeDef *hwdt)
{
	NVIC_DisableIRQ(WWDT_IRQn);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_WDT, DISABLE);
	RCC_WWDTClockConfig(RCC_WWDT_CLKSRC_HSI, 0x02, DISABLE);
	/*If this code runs,should consider that WDT will generate the system reset(If doesn't feed dog in time),debug function will lose efficacy.*/
	RCC_WDTResetSystemCmd(RCC_ALARM_RSTEN_WWDT, DISABLE);
}
/**
  * @brief WDT MSP Init.
  * @param[in] hwdt: pointer to a WDT_HandleTypeDef structure that contains
  *				the configuration information for the specified WDT module.
  * @retval None
  */
void TMC_WDT_Callback(WDT_HandleTypeDef *hwdt)
{
	TMC_WDT_Refresh(hwdt);
}


void WDT_Demo_Main(void)
{
	/*If this code runs,should consider that WDT will generate the system reset(If doesn't feed dog in time),debug function will lose efficacy.*/
	WDT_Test(); 
}

/**
  * @}
  */ 

/**
  * @}
  */

