/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: sensor.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Includes ------------------------------------------------------------------*/

#include "thm36_1.3.h"
#include "sensor.h"

/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup sensor
  * @brief sensor HAL modules driver 
  * @{
  */ 
/** @defgroup sensor_Private_Defines sensor Private Defines
  * @{
  */

#define EGSINTMSK	 				*(volatile unsigned long *)(0x40054000 + 0x004)
#define LDINTMSK	  				*(volatile unsigned long *)(0x40058000 + 0x004)
#define LOW_FREQ_ENABLE				1U
#define LOW_FREQ_DISABLE			~(1U)
#define HIGH_FREQ_ENABLE			2U
#define HIGH_FREQ_DISABLE			~(2U)


/**
  * @}
  */

/** @defgroup sensor_Exported_Functions sensor Exported Functions
  * @{
  */

/**
  * @brief confog ASH interrupt ENABLE or DISABLE
  * @param[in] Newstate
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */
void ASH_ConfigIT(FunctionalState Newstate)
{
	if(Newstate == ENABLE)
	{
		ASH->INTMSK &= ~1U;
	}
	else
	{
		ASH->INTMSK |= 1U;
	}
}

/**
  * @brief confog EGS interrupt ENABLE or DISABLE
  * @param[in] Newstate
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */
void EGS_ConfigIT(FunctionalState Newstate)
{
	if(Newstate == ENABLE)
	{
		EGSINTMSK &= ~1U;
	}
	else
	{
		EGSINTMSK |= 1U;
	}
}

/**
  * @brief  Fills each Alarm_InitStruct member with its default value.
  * @param[in] Alarm_InitStruct: pointer to a RTC_AlarmInitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
void EFD3_StructInit(BAT_FD3InitTypeDef *FD3_InitStruct)
{
	FD3_InitStruct->FD3_CheckClock= INSIDE_32K_CLOCK;
	FD3_InitStruct->FD3_Divsion = REFERENCE_CLOCK_DIVSION_NO;
	FD3_InitStruct->FD3_ReferenceClock = REFERENCE_CLOCK_HSE;
}

/**
  * @brief Initializes the FD3 peripheral according to 
  *   the specified parameters in the RTC_InitTypeDef.
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 
void EFD3_Init(BAT_FD3InitTypeDef *FD3_InitStruct)
{
	uint32_t temp =0;
	temp = FD3_InitStruct->FD3_CheckClock|FD3_InitStruct->FD3_Divsion|FD3_InitStruct->FD3_ReferenceClock;
	FD3->CON = temp;
}

/**
  * @brief config EFD3's Low Freq Frequency vale,and  High Freq Frequency vale
  * @param[in] LFN: Low Freq Frequency vale
  * @param[in] HFN: High Freq Frequency vale
  * @retval none
  * @note none
  */ 
void EFD3_ConfigValue(uint32_t LFN,uint32_t HFN)
{
	FD3->LFN = LFN;
	FD3->HFN = HFN;
}

/**
  * @brief config EFD2's Low Freq Frequency vale,and  High Freq Frequency vale
  * @param[in] LFN: Low Freq Frequency vale
  * @param[in] HFN: High Freq Frequency vale
  * @retval none
  * @note none
  */ 
void EFD2_ConfigValue(uint32_t LFN,uint32_t HFN)
{
	FD2->LFN = LFN;
	FD2->HFN = HFN;
}


/**
  * @brief config EFD3 the Low Frequency detection ENASBLE or DISABLE
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.

  * @retval ErrorStatus
  * @note none
  */
void  EFD3_LowFreDetectCmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FD3->CON |=LOW_FREQ_ENABLE;
	}
	else
	{
		FD3->CON &=LOW_FREQ_DISABLE;
	}
}

/**
  * @brief config EFD3 the High Frequency detection ENASBLE or DISABLE
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.

  * @retval ErrorStatus
  * @note none
  */
void  EFD3_HighFreDetectCmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FD3->CON |=HIGH_FREQ_ENABLE;
	}
	else
	{
		FD3->CON &=HIGH_FREQ_DISABLE;
	}
}


/**
  * @brief Get EFD3 Status
  * @param[in] Flags:
  *		@zrg FD3_FLAG_REDUNDANCY_CHECK_ALARM 
  *		@zrg FD3_FLAG_CHECK_OK
  *		@zrg FD3_FLAG_HIGH_ALARM
  *		@zrg FD3_FLAG_LOW_ALARM
  * @retval ErrorStatus
  * @note none
  */
FlagStatus EFD3_GetStatus(uint32_t Flags)
{
	if(FD3->STS&Flags)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief Get EFD3 Status
  * @param[in] Flags:
  *		@zrg FD3_FLAG_REDUNDANCY_CHECK_ALARM 
  *		@zrg FD3_FLAG_CHECK_OK
  *		@zrg FD3_FLAG_HIGH_ALARM
  *		@zrg FD3_FLAG_LOW_ALARM
  * @retval ErrorStatus
  * @note none
  */
void EFD3_ClearStatus(uint32_t Flags)
{
	FD3->STS = Flags;
}

/**
  * @brief Get EFD3 Status
  * @param[in] Flags:
  *		@zrg FD3_FLAG_CHECK_OK
  *		@zrg FD3_FLAG_HIGH_ALARM
  *		@zrg FD3_FLAG_LOW_ALARM
  * @retval ErrorStatus
  * @note none
  */
void EFD3_ConfigIT(uint32_t Flags,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FD3->INTMSK &= ~Flags;
	}
	else
	{
		FD3->INTMSK |= Flags;
	}
}

/**
  * @brief config FD2 the Low Frequency detection ENASBLE or DISABLE
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.

  * @retval ErrorStatus
  * @note none
  */
void  EFD2_LowFreDetectCmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FD2->CON |=LOW_FREQ_ENABLE;
	}
	else
	{
		FD2->CON &=LOW_FREQ_DISABLE;
	}
}

/**
  * @brief config FD2 the High Frequency detection ENASBLE or DISABLE
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.

  * @retval ErrorStatus
  * @note none
  */
void  EFD2_HighFreDetectCmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FD2->CON |=HIGH_FREQ_ENABLE;
	}
	else
	{
		FD2->CON &=HIGH_FREQ_DISABLE;
	}
}

/**
  * @brief Get EFD2 Status
  * @param[in] Flags:
  *		@zrg FD2_FLAG_HIGH_ALARM 
  *		@zrg FD2_FLAG_LOW_ALARM
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note none
  */
FlagStatus EFD2_GetStatus(uint32_t Flags)
{
	if(FD2->STS&Flags)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief Get EFD2 Status
  * @param[in] Flags:
  *		@zrg FD2_FLAG_HIGH_ALARM 
  *		@zrg FD2_FLAG_LOW_ALARM
  * @retval none
  * @note none
  */
void EFD2_ClearStatus(uint32_t Flags)
{
	FD2->STS = Flags;
}

/**
  * @brief Get EFD3 Status
  * @param[in] Flags:
  *		@zrg FD3_FLAG_ALARM 
  *		@zrg FD3_FLAG_CHECK_OK
  * @retval ErrorStatus
  * @note none
  */
void EFD2_ConfigIT(uint32_t Flags,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		FD2->INTMSK &= ~Flags;
	}
	else
	{
		FD2->INTMSK |= Flags;
	}
}

/**
  * @brief return the FD3 PER
  * @param[in] none
  * @retval uint32_t
  * @note none
  */
uint32_t EFD3_GetPER(void)
{
	return FD3->PER;
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

