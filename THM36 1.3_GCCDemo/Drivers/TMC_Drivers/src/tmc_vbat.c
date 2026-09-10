/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_vbat.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "rcc.h"
#include "tmc_vbat.h"
#include "tmc_gpio.h"
#include "exti.h"
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_VBAT
  * @brief VBAT TMC modules driver 
  * @{
  */
/** @defgroup TMC_VBAT_Private_Defines TMC VBAT Private Defines
  * @{
  */
/**
  * @}
  */  
/** @defgroup TMC_VBAT_Exported_Functions TMC VBAT Exported Functions
  * @{
  */
/**
  * @brief  Fills each VBAT_HandleTypeDef member with its default value.
  * @param[in] VBAT_HandleTypeDef: pointer to a VBAT_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_VBAT_StructInit(VBAT_HandleTypeDef *hvbat)
{
	hvbat->State = TMC_VBAT_STATE_RESET;
	hvbat->ALARMCode = NULL;
}

/**
  * @brief  Initializes the VBAT according to the specified
  *			 parameters in the VBAT_InitTypeDef and create the associated handle.
  * @param  hvbat pointer to a VBAT_HandleTypeDef structure that contains
  *			the configuration information for VBAT.
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_VBAT_Init(VBAT_HandleTypeDef *hvbat,VBAT_FD3_HandleTypeDef *hvbat_fd3)
{
	volatile uint8_t  temp = 0;
	if(hvbat == NULL)
	{
		return TMC_ERROR;
	}
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		/* Allocate lock resource and initialize it */
		hvbat->Lock = HAL_UNLOCKED;
		/* Init the low level hardware */
		TMC_VBAT_MspInit(hvbat);
	}
	VBAT_ConfigSensorTimeValue(100);
	VBAT_SensorTimeStart(ENABLE);

	EFD3_Init(&hvbat_fd3->FD3_Init);
	BAT_RTC_Init(&hvbat->RTC_Init);
	BAT_RTC_AlarmInit(&hvbat->RTC_AlarmInit);
	BAT_IWDT_Init(&hvbat->IWDT_Init);
	VBAT_TD_Init(&hvbat->TD_Init);
	VBAT_VD_Init(&hvbat->VD_Init);
	VBAT_SDx_Init(&hvbat->SD_Init);
	hvbat->State = TMC_VBAT_STATE_READY;
	hvbat->ALARMCode = TMC_VBAT_ALARM_NONE;
	return TMC_OK;
}

/**
  * @brief  DeInitializes the VBAT according to the specified
  *		 parameters in the VBAT_InitTypeDef and create the associated handle.
  * @param  hvbat pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for VBAT.
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_VBAT_DeInit(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat == NULL)
	{
		return TMC_ERROR;
	}
	RCC_BATSoftResetCmd(RCC_BAT_RSTEN_VBSENSOR);
	BAT_IWDT_DeInit();
	BAT_RTC_DeInit();
	BAT_GPIO_DeInit();
	/* DeInit the low level hardware */
	TMC_VBAT_MspDeInit(hvbat);
	/* Update error code */
	hvbat->State = TMC_VBAT_STATE_RESET;
	hvbat->ALARMCode = TMC_VBAT_ALARM_NONE;
	return TMC_OK;
}

/**
  * @brief VBAT Start up RTC,when RTC ALARM is SET,will be Trigger a EXTI interrupt.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_RTC_Start_IT(VBAT_HandleTypeDef *hvbat)
{
	
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	BAT_RTC_Cmd(ENABLE);
	BAT_RTC_AllAlarm_Cmd(ENABLE);
	EXTI_ConfigOtherMode(EXTI_RTC_ALM_WAKE_UP,ENABLE);
	NVIC_ClearPendingIRQ(EXTI81_IRQn);
	NVIC_EnableIRQ(EXTI81_IRQn);
	return TMC_OK;
}
 
/**
  * @brief VBAT STOP RTC.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
void TMC_VBAT_RTC_Stop_IT(VBAT_HandleTypeDef *hvbat)
{
	BAT_RTC_Cmd(DISABLE);
	BAT_RTC_AllAlarm_Cmd(DISABLE);
	EXTI_ConfigOtherMode(EXTI_RTC_ALM_WAKE_UP,DISABLE);
	NVIC_ClearPendingIRQ(EXTI81_IRQn);
	NVIC_DisableIRQ(EXTI81_IRQn);
}
 

 
/**
  * @brief VBAT Start up EFD3 to Check LSE clock, if Alarm,will be trigger a interrupt.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_EFD3_Start_IT(VBAT_FD3_HandleTypeDef *hvbat_fd3,uint32_t LowFrq,uint32_t HighFrq)
{
	volatile uint32_t PLL_L,divsion,tempL,tempH;
	if(hvbat_fd3->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	switch(hvbat_fd3->FD3_Init.FD3_Divsion)
	{
		case REFERENCE_CLOCK_DIVSION_NO:divsion =1;break;
		case REFERENCE_CLOCK_DIVSION_2:divsion =2;break;
		case REFERENCE_CLOCK_DIVSION_4:divsion =4;break;
		case REFERENCE_CLOCK_DIVSION_8:divsion =6;break;
	}
//	if(hvbat->FD3_Init.FD3_ReferenceClock == REFERENCE_CLOCK_HSE)
//	{
//		/*if FD3 ReferenceClock is more than 50MHZ or less tahn 16MHZ return ERROR */
//		if((HSE_VALUE/divsion>=50000000))//||(HSE_VALUE/divsion<=16000000)
//		{
//			return TMC_ERROR;
//		}
//		tempL = (HSE_VALUE/divsion)/LowFrq;
//		tempH = (HSE_VALUE/divsion)/HighFrq;
//		EFD3_ConfigValue(tempL,tempH);
//	}
	/*FD3 ReferenceClock is PLL_L */
	if(hvbat_fd3->FD3_Init.FD3_ReferenceClock == REFERENCE_CLOCK_PLL_L)
	{
		PLL_L = RCC_GetPLL_L_Clock();
		/*if the PLL_L's clock is more than 50MHZ return ERROR*/
		if(((PLL_L/divsion)>=50000000))//||((PLL_L/divsion)<=16000000)
		{
			return TMC_ERROR;
		}
		
		tempL = PLL_L/(divsion*LowFrq);
		tempH = PLL_L/(divsion*HighFrq);
		EFD3_ConfigValue(tempL,tempH);
	}
	if(EFD3_GetStatus(FD3_FLAG_REDUNDANCY_CHECK_ALARM)==SET)
	{
		return TMC_ERROR;
	}
	/* High Frequency Alarm Interrupt enable */
	EFD3_ConfigIT(FD3_FLAG_HIGH_ALARM,ENABLE);
	/* low Frequency Alarm Interrupt enable */
	EFD3_ConfigIT(FD3_FLAG_LOW_ALARM,ENABLE);	
	EFD3_ClearStatus(FD3_FLAG_REDUNDANCY_CHECK_ALARM|FD3_FLAG_CHECK_OK|FD3_FLAG_HIGH_ALARM|FD3_FLAG_LOW_ALARM);
	/* Low Frequency Alarm enable */
	EFD3_LowFreDetectCmd(ENABLE);

	/* High Frequency Alarm enable */
	EFD3_HighFreDetectCmd(ENABLE);
	EFD3_LowFreDetectCmd(ENABLE);
	return TMC_OK;
}


/**
  * @brief  Handles VBAT Sensor interrupt request.
  * @param  hdma: pointer to a VBAT_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT Stream.  
  * @retval None
  */
void TMC_VBAT_EFD3_IRQHandler(VBAT_FD3_HandleTypeDef *hvbat_fd3)
{
	hvbat_fd3->State = TMC_VBAT_STATE_ALARM;
	
	if(EFD3_GetStatus(FD3_FLAG_HIGH_ALARM)==SET)
	{
		EFD3_ClearStatus(FD3_FLAG_HIGH_ALARM);
		hvbat_fd3->ALARMCode |=TMC_VBAT_ALARM_FD3_HIGH_FREQ;
	}
	if(EFD3_GetStatus(FD3_FLAG_LOW_ALARM)==SET)
	{
		EFD3_ClearStatus(FD3_FLAG_LOW_ALARM);
		hvbat_fd3->ALARMCode |=TMC_VBAT_ALARM_FD3_LOW_FREQ;
	}
	if(EFD3_GetStatus(FD3_FLAG_CHECK_OK)==SET)
	{
		EFD3_ClearStatus(FD3_FLAG_CHECK_OK);
		hvbat_fd3->ALARMCode |=TMC_VBAT_ALARM_FD3_NOT_ALARM;
	}
	TMC_VBAT_FD3AlarmCpltCallback(hvbat_fd3);
}
/**
  * @brief VBAT Stop EFD3 to Check LSE clock.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_EFD3_Stop_IT(VBAT_FD3_HandleTypeDef *hvbat_fd3)
{
	if(hvbat_fd3->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	EFD3_ConfigIT(FD3_FLAG_CHECK_OK,DISABLE);
	/* High Frequency Alarm Interrupt enable */
	EFD3_ConfigIT(FD3_FLAG_HIGH_ALARM,DISABLE);
	/* low Frequency Alarm Interrupt enable */
	EFD3_ConfigIT(FD3_FLAG_LOW_ALARM,DISABLE);
	/* Low Frequency Alarm enable */
	EFD3_LowFreDetectCmd(DISABLE);
	/* High Frequency Alarm enable */
	EFD3_HighFreDetectCmd(DISABLE);
	return TMC_OK;
}

 
/**
  * @brief VBAT Start up VD check,if Alarm,will be trigger a interrupt.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_VD_Start_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	MAIN_VD_ConfigIT(MAIN_VD_IT_LOW_LEVEL|MAIN_VD_IT_HIGH_LEVEL,ENABLE);
	VBAT_VD_HighLevelCmd(ENABLE);
	VBAT_VD_LowLevelCmd(ENABLE);
	return TMC_OK;
} 

/**
  * @brief VBAT STOP VD 
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_VD_Stop_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	MAIN_VD_ConfigIT(MAIN_VD_IT_LOW_LEVEL|MAIN_VD_IT_HIGH_LEVEL,DISABLE);
	VBAT_VD_HighLevelCmd(DISABLE);
	VBAT_VD_LowLevelCmd(DISABLE);
	return TMC_OK;
}
 
/**
  * @brief VBAT Start up VD check,if Alarm,will be trigger a interrupt.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_TD_Start_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	MAIN_TD_ConfigIT(MAIN_TD_IT_LOW_TEMPERATURE|MAIN_TD_IT_HIGH_TEMPERATURE,ENABLE);
	VBAT_TD_Cmd(ENABLE);
	return TMC_OK;
} 

/**
  * @brief VBAT STOP TD 
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_TD_Stop_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	MAIN_TD_ConfigIT(MAIN_TD_IT_LOW_TEMPERATURE|MAIN_TD_IT_HIGH_TEMPERATURE,DISABLE);
	VBAT_TD_Cmd(DISABLE);
	return TMC_OK;
} 
 
/**
  * @brief VBAT Start up SD check,if Alarm,will be trigger a interrupt.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_SD_Start_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	VBAT_SDx_ClearAlarmStatus(VBAT_SD0|VBAT_SD1|VBAT_SD2|VBAT_SD3|VBAT_SD4|VBAT_SD5|VBAT_SD6|VBAT_SD7);
	MAIN_SD_ConfigIT(MAIN_SD_IT_STATIC,ENABLE);
    MAIN_SD_ConfigIT(MAIN_SD_IT_DYNAMIC,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD0,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD1,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD2,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD3,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD4,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD5,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD6,ENABLE);
	VBAT_SDx_Cmd(VBAT_SD7,ENABLE);
	return TMC_OK;
} 

/**
  * @brief VBAT STOP SD 
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_SD_Stop_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	VBAT_SDx_Cmd(VBAT_SD0,DISABLE);
	VBAT_SDx_Cmd(VBAT_SD1,DISABLE);
	VBAT_SDx_Cmd(VBAT_SD2,DISABLE);
	VBAT_SDx_Cmd(VBAT_SD3,DISABLE);
	VBAT_SDx_Cmd(VBAT_SD4,DISABLE);
	VBAT_SDx_Cmd(VBAT_SD5,DISABLE);
    VBAT_SDx_Cmd(VBAT_SD6,DISABLE);
	VBAT_SDx_Cmd(VBAT_SD7,DISABLE);
	MAIN_SD_ConfigIT(MAIN_SD_IT_STATIC,DISABLE);
    MAIN_SD_ConfigIT(MAIN_SD_IT_DYNAMIC,DISABLE);
	return TMC_OK;
} 

/**
  * @brief VBAT Start up IWDT check,when IWDT ALARM is SET,will be Trigger a EXTI interrupt.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_IWDT_Start_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	BAT_IWDT_Cmd(ENABLE);
	EXTI_ConfigOtherMode(EXTI_IWDT_ALM_WAKE_UP,ENABLE);
	NVIC_ClearPendingIRQ(EXTI82_IRQn);
	NVIC_EnableIRQ(EXTI82_IRQn);
	return TMC_OK;
} 

/**
  * @brief VBAT STOP IWDT 
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
TMC_StatusTypeDef TMC_VBAT_IWDT_Stop_IT(VBAT_HandleTypeDef *hvbat)
{
	if(hvbat->State == TMC_VBAT_STATE_RESET)
	{
		return TMC_ERROR;
	}
	BAT_IWDT_Cmd(DISABLE);
	EXTI_ConfigOtherMode(EXTI_IWDT_ALM_WAKE_UP,DISABLE);
//	NVIC_ClearPendingIRQ(EXTI82_IRQn);
//	NVIC_DisableIRQ(EXTI82_IRQn);
	return TMC_OK;
} 

/**
  * @brief VBAT MSP Init.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
 __weak void TMC_VBAT_MspInit(VBAT_HandleTypeDef *hvbat)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hvbat);
}

/**
  * @brief VBAT MSP DeInit.
  * @param[in] hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *				the configuration information for the specified VBAT module.
  * @retval None
  */
 __weak void TMC_VBAT_MspDeInit(VBAT_HandleTypeDef *hvbat)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hvbat);
}


/**
  * @brief  Handles VBAT Sensor interrupt request.
  * @param  hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *			the configuration information for the specified VBAT Stream.  
  * @retval None
  */
void TMC_VBAT_Sensor_IRQHandler(VBAT_HandleTypeDef *hvbat)
{
	hvbat->State = TMC_VBAT_STATE_ALARM;
	
	if(MAIN_TD_GetAlarmStatus(MAIN_TD_FLAG_LOW_TEMPERATURE) == SET)
	{
		hvbat->ALARMCode |=TMC_VBAT_ALARM_TD_LOW_TEMPERATURE;
		VBAT_TD_ClearStatus(VBAT_LOW_TEMPER_ALARM);
	}
	
	if(MAIN_TD_GetAlarmStatus(MAIN_TD_FLAG_HIGH_TEMPERATURE) == SET)
	{
		hvbat->ALARMCode |=TMC_VBAT_ALARM_TD_HIGH_TEMPERATURE;
		VBAT_TD_ClearStatus(VBAT_HIGH_TEMPER_ALARM);
	}
	
	if(MAIN_VD_GetAlarmStatus(MAIN_VD_FLAG_LOW_LEVEL) == SET)
	{
		hvbat->ALARMCode |=TMC_VBAT_ALARM_VD_LOW_LEVEL;
		VBAT_VD_ClearStatus(VBAT_LOW_LEVEL_ALARM);
	}
	
	if(MAIN_VD_GetAlarmStatus(MAIN_VD_FLAG_HIGH_LEVEL) == SET)
	{
		hvbat->ALARMCode |=TMC_VBAT_ALARM_VD_HIGH_LEVEL;
		VBAT_VD_ClearStatus(VBAT_HIGH_LEVEL_ALARM);
	}
	
	if(MAIN_SD_GetAlarmStatus(MAIN_SD_FLAG_STATIC) == SET)
	{
		if(VBAT_SDx_GetAlarmStatus(VBAT_SD0)==SET)
		{
			hvbat->ALARMCode|= TMC_VBAT_ALARM_SD_0;
			VBAT_SDx_ClearAlarmStatus(VBAT_SD0);
		}
		if(VBAT_SDx_GetAlarmStatus(VBAT_SD1)==SET)
		{
			hvbat->ALARMCode|= TMC_VBAT_ALARM_SD_1;
			VBAT_SDx_ClearAlarmStatus(VBAT_SD1);
		}
		if(VBAT_SDx_GetAlarmStatus(VBAT_SD4)==SET)
		{
			hvbat->ALARMCode|= TMC_VBAT_ALARM_SD_4;
			VBAT_SDx_ClearAlarmStatus(VBAT_SD4);
		}
		if(VBAT_SDx_GetAlarmStatus(VBAT_SD5)==SET)
		{
			hvbat->ALARMCode|= TMC_VBAT_ALARM_SD_5;
			VBAT_SDx_ClearAlarmStatus(VBAT_SD5);
		}
		
	}
	
	if(MAIN_SD_GetAlarmStatus(MAIN_SD_FLAG_DYNAMIC) == SET)
	{
		if(VBAT_SDx_GetAlarmStatus(VBAT_SD6)==SET)
		{
			hvbat->ALARMCode|= TMC_VBAT_ALARM_SD_6;
			VBAT_SDx_ClearAlarmStatus(VBAT_SD6);
		}
		if(VBAT_SDx_GetAlarmStatus(VBAT_SD7)==SET)
		{
			hvbat->ALARMCode|= TMC_VBAT_ALARM_SD_7;
			VBAT_SDx_ClearAlarmStatus(VBAT_SD7);
		}
	}
	TMC_VBAT_SensorAlarmCpltCallback(hvbat);
}

/**
  * @brief  Handles VBAT Sensor interrupt request.
  * @param  hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT Stream.  
  * @retval None
  */
void TMC_VBAT_RTC_Alarm_IRQHandler(VBAT_HandleTypeDef *hvbat)
{
	BAT_RTC_ClearAlarmStatus();
	EXTI_ClearOtherStatus(EXTI_RTC_ALM_WAKE_UP);
	hvbat->State = TMC_VBAT_STATE_ALARM;
	hvbat->ALARMCode |= TMC_VBAT_ALARM_RTC;
	TMC_VBAT_RTC_AlarmCpltCallback(hvbat);
}

/**
  * @brief  Handles VBAT IWDT interrupt request.
  * @param  hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT Stream.  
  * @retval None
  */
void TMC_VBAT_IWDT_Alarm_IRQHandler(VBAT_HandleTypeDef *hvbat)
{
	EXTI_ClearOtherStatus(EXTI_IWDT_ALM_WAKE_UP);
	hvbat->State = TMC_VBAT_STATE_ALARM;
	hvbat->ALARMCode |= TMC_VBAT_ALARM_IWDT;
	TMC_VBAT_IWDT_AlarmCpltCallback(hvbat);
}
/**
  * @brief VBAT SENSOR ALARM callback.
  * @param  hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT Stream. 
  * @retval None
  */
__weak void TMC_VBAT_SensorAlarmCpltCallback(VBAT_HandleTypeDef *hvbat)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hvbat);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxCpltCallback should be implemented in the user file
	*/
}
/**
  * @brief VBAT FD3 ALARM callback.
  * @param  hvbat: pointer to a VBAT_FD3_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT_FD3 Stream. 
  * @retval None
  */
__weak void TMC_VBAT_FD3AlarmCpltCallback(VBAT_FD3_HandleTypeDef *hvbat_fd3)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hvbat_fd3);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxCpltCallback should be implemented in the user file
	*/
}
/**
  * @brief VBAT IWDT ALARM callback.
  * @param  hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT Stream. 
  * @retval None
  */
__weak void TMC_VBAT_IWDT_AlarmCpltCallback(VBAT_HandleTypeDef *hvbat)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hvbat);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief VBAT RTC ALARM callback.
  * @param  hvbat: pointer to a VBAT_HandleTypeDef structure that contains
  *			   the configuration information for the specified VBAT Stream. 
  * @retval None
  */
__weak void TMC_VBAT_RTC_AlarmCpltCallback(VBAT_HandleTypeDef *hvbat)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hvbat);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxCpltCallback should be implemented in the user file
	*/
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
  
/************************ (C) COPYRIGHT TMC *****END OF FILE****/  

