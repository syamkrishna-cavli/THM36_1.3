/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: system_thm36_1.3.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
//#include "thm36_1.3.h"
#include "demo.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "flash_cache.h"
#include "tmc_systick.h"
#include "ramReset.h"
#include "ISO7816_EMV_L1.h" 


RCC_OscInitTypeDef RCC_OscInitStruct;
RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
extern GPIO_InitTypeDef GPIOA_Init;

/**
  * @brief  Setup the microcontroller system
  *		 Initialize the FPU setting, vector table location and External memory 
  *		 configuration.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
//TMC(20240401)-zt: select PLL frequency for EMV_test_mode/Other_modes
#ifndef EMV7816_10A
	Thm36RamReset();
	/*Enable cache.*/
	RCC_AHBClockCmd(RCC_AHBPeriph_FLASHCACHE,ENABLE);
	//RCC_Security1PeriphClockCmd(RCC_SEC1Periph_PWRC,ENABLE);
	/*Open Ichace & Dchace*/
	Flash_IcacheRest(Flash_ACC,DISABLE);
	Flash_IcacheCmd(Flash_ACC,ENABLE);
	Flash_DcacheRest(Flash_ACC,DISABLE);
	Flash_DcacheCmd(Flash_ACC,ENABLE);
	/*Enable HSI.*/
	RCC_HSICmd(ENABLE);
	
	/*Configure PLL_H as 192MHz.*/
	RCC_OscInitStruct.ClockSourceType = RCC_ClockSource_PLL;
	RCC_OscInitStruct.PLLConfig.PLL_Input = RCC_PLLC_PLLSRC_HSI;
	RCC_OscInitStruct.PLLConfig.PLL_M = 1;
	RCC_OscInitStruct.PLLConfig.PLL_N = 16;
	RCC_OscInitStruct.PLLConfig.PLL_Q = 4;
	RCC_OscInitStruct.PLLConfig.SourceClockVaule = HSI_VALUE;
	RCC_OscInitStruct.PLLState = ENABLE;
	TMC_RCC_OscConfig(&RCC_OscInitStruct);
	
	RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_PLL_H;
	RCC_SysclkInitStruct.SYSCLKDIV = 0x00;
	TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
#else	
	Thm36RamReset();
	/*Enable cache.*/
	RCC_AHBClockCmd(RCC_AHBPeriph_FLASHCACHE,ENABLE);
	//RCC_Security1PeriphClockCmd(RCC_SEC1Periph_PWRC,ENABLE);
	/*Open Ichace & Dchace*/
	Flash_IcacheRest(Flash_ACC,DISABLE);
	Flash_IcacheCmd(Flash_ACC,ENABLE);
	Flash_DcacheRest(Flash_ACC,DISABLE);
	Flash_DcacheCmd(Flash_ACC,ENABLE);
	
	RCC_HSICmd(ENABLE);
	/*Configure PLL_L as 30MHz.*/
	RCC_OscInitStruct.ClockSourceType = RCC_ClockSource_PLL;
	RCC_OscInitStruct.PLLConfig.PLL_Input = RCC_PLLC_PLLSRC_HSI;
	RCC_OscInitStruct.PLLConfig.PLL_M = 1;
	RCC_OscInitStruct.PLLConfig.PLL_N = 16;
	RCC_OscInitStruct.PLLConfig.PLL_Q = 4;
	RCC_OscInitStruct.PLLConfig.SourceClockVaule = HSI_VALUE;
	RCC_OscInitStruct.PLLState = ENABLE;
	TMC_RCC_OscConfig(&RCC_OscInitStruct);
	
	RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_PLL_H;
	RCC_SysclkInitStruct.SYSCLKDIV = 0x00;
	TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
	
#endif	
	
		
	//GPIO_MonitorClkOut(0,OSC_SOURCE_CLOCK_PLL_H,2);
}
