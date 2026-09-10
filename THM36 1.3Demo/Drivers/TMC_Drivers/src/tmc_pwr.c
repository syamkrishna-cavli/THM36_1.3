/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_pwr.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "RCC.h"
#include "TMC_PWR.h"
#include "VBAT.h"
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_PWR
  * @brief PWR TMC modules driver 
  * @{
  */
/** @defgroup TMC_PWR_Exported_Function TMC PWR Exported Function
  * @brief PWR TMC modules driver
  * @{
  */

/**
  * @brief config Chip into Stop Mode 
  * @param[in] Sotp_Mode
  *				@arg PWR_STOP_MODE_HSI_PWR_DOWN
  *				@arg PWR_STOP_MODE_FLASH_PWR_DOWN
  *				@arg PWR_STOP_MODE_LDO_PWR_DOWN
  * @param[in] Cpu_Mode
  *				@arg PWR_STOPEntry_WFI
  *				@arg PWR_STOPEntry_WFE  
  * @param[in] RamRang
  *				@arg SRAM_RANGE0PD 	//RAM 0x20000000 - 0x20018000 will power dwon in stop mode
				@arg SRAM_RANGE1PD	//RAM 0x20018000 - 0x20028000 will power dwon in stop mode
				@arg SRAM_RANGE2PD	//RAM 0x20030000 - 0x20034000 will power dwon in stop mode
				@arg SRAM_RANGE3PD	//RAM 0x20028000 - 0x20030000 will power dwon in stop mode
				@arg SRAM_RANGE4PD	//RAM 0x20034000 - 0x20050000 will power dwon in stop mode
				@arg SRAM_RANGE5PD	//RAM 0x20050000 - 0x20080000 will power dwon in stop mode
				@arg 0				//ALL RAM will not be keeped in stop mode
				
  * @retval  
  * @note  
  */ 
ErrorStatus TMC_PWR_EnterStopMode(uint32_t Sotp_Mode,uint8_t Cpu_Mode,uint32_t RamRang)
{

	PWR_ConfigModuleStateInStopMode(Sotp_Mode,ENABLE);
	PWR->PWRPSWC =RamRang;
	
	PWR_EnterStopMode(Cpu_Mode);
	return SUCCESS;
}


/**
  * @brief config Chip into Standby Mode 
  * @retval none
  * @note none
  */ 
ErrorStatus TMC_PWR_EnterStandByMode(void)
{
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_PWRC,ENABLE);
	BAT_WriteEnable(ENABLE);
	if((!(RCC_VBAT->LSECON&0x01))&&(!(RCC_VBAT->LSICON&0x01)))
	{
		return ERROR;
	}
	PWR_VBAT->WKUPSTS = 0xFFFFFFFF;
	PWR_EnterStandbyMode();
	return SUCCESS;
}
/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

