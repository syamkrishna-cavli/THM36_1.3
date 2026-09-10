/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mag.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "RCC.h"
#include "mag.h"


/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup MAG
  * @brief MAG HAL modules driver 
  * @{
  */
/** @defgroup MAG_private_defines MAG private defines
  * @{
  */

/**
  * @}
  */
/** @defgroup MAG_exported_function MAG exported function
  * @{  
  */

/**
  * @brief Get Magx's Status
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] Flag: the status of MAGx
  *				@arg MAG_STS_DETECTED_SWIPE_ACTION
  *				@arg MAG_STS_DATA_OVERFLOWER
  *				@arg MAG_STS_WAITE_NOISE_MONITOR_OVER  
  *				@arg MAG_STS_SWIPE_CARD_COMPLETED
  *				@arg MAG_STS_DATA_BUFFER_HAVE_DATA
  * @retval FlagStatus
  * @note  
  */

FlagStatus MAG_GetStatus(MAG_TypeDef* Magx, uint32_t status)
{
	assert_param(IS_MAG_CHECK_STS(status));
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	if(Magx->MAGSTS&status)
	{
		return SET;
	}
	return RESET;
}


/**
  * @brief Clear Magx's Status
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] Flag: the status of MAGx
  *				@arg MAG_STS_DETECTED_SWIPE_ACTION
  *				@arg MAG_STS_DATA_OVERFLOWER
  *				@arg MAG_STS_WAITE_NOISE_MONITOR_OVER  
  *				@arg MAG_STS_SWIPE_CARD_COMPLETED
  *				@arg MAG_STS_DATA_BUFFER_HAVE_DATA
  * @retval FlagStatus
  * @note  
  */
void MAG_ClearStatus(MAG_TypeDef* Magx, uint32_t status)
{
	assert_param(IS_MAG_CHECK_STS(status));
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	Magx->MAGSTS = status;
}


/**
  * @brief config Magx's interrupt
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] Flag: the status of MAGx
  *				@arg MAG_IT_DETECTED_SWIPE_ACTION
  *				@arg MAG_IT_DATA_OVERFLOWER
  *				@arg MAG_IT_WAITE_NOISE_MONITOR_OVER  
  *				@arg MAG_IT_SWIPE_CARD_COMPLETED
  *				@arg MAG_IT_DATA_BUFFER_HAVE_DATA
  * @retval FlagStatus
  * @note  
  */
void MAG_ITConfig(MAG_TypeDef* Magx, uint32_t Mask,FunctionalState NewState)
{
	assert_param(IS_MAG_CHECK_IT(Mask));
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	if(NewState == ENABLE)
	{
		Magx->MAGMASK&=(~Mask);
	}
	else
	{
		Magx->MAGMASK|=Mask;
	}
}

/**
  * @brief Get Magx IT Mask
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] nonr
  * @retval uint32_t the value of MAGMASK	
  * @note  
  */
uint32_t MAG_GetITMask(MAG_TypeDef* Magx)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	return Magx->MAGMASK;
}
/**
  * @brief Get Card Swipe Value
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[out] Voltage: the point of Card's Voltage(Significant bits are only 12 bit)
  * @param[out] time: the point of Card's time
  * @retval FlagStatus
  * @note  
  */
void MAG_GetCardSwipeValue(MAG_TypeDef* Magx,uint16_t* Voltage, uint16_t* time)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	*Voltage =(Magx->MAGDAT)>>16;
	*time = (Magx->MAGDAT)&0x000007FF;
}

/**
  * @brief Get Card Swipe Value
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[out] HighWhiteNoise: the point of HighWhiteNoise's Voltage(Significant bits are only 12 bit)
  * @param[out] LowWhiteNoise: the point of LowWhiteNoise's Voltage(Significant bits are only 12 bit)
  * @retval FlagStatus
  * @note  
  */
void MAG_GetWhiteNoise(MAG_TypeDef* Magx)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	if((Magx == MAG1)||(Magx == MAG3))
	{
		Magx->MAGTH = (((Magx->MAGDAT + 0x00500000) >> 16 ) <<16) + (((Magx->MAGDAT - 0x0050) << 16) >>16);				 /*6FC 6F9*/	
	}
	else
	{
		Magx->MAGTH = (((Magx->MAGDAT + 0x00300000) >> 16 ) <<16) + (((Magx->MAGDAT - 0x0030) << 16) >>16);				 /*6FC 6F9*/	
	}
}


/**
  * @brief Config Card Swipe Voltage's Threshold
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] HighVoltage: the point of Card's High Voltage Threshold(Significant bits are only 12 bit)
  * @param[in] LowVoltage: the point of Card's Low Voltage Threshold(Significant bits are only 12 bit)
  * @retval none
  * @note  
  */
void MAG_ConfigCardSwipeVoltageThreshold(MAG_TypeDef* Magx,uint16_t HighVoltage,uint16_t LowVoltage)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	Magx->MAGTH = 0;
	Magx->MAGTH =(HighVoltage<<16)|LowVoltage;
}


/**
  * @brief config the times of check the WhiteNoise
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] times : the times of ADC acquisition times
  * @retval none
  * @note  
  */
void MAG_ConfigWitheNoiseCheckTimes(MAG_TypeDef* Magx,uint16_t times)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	Magx->MAGNOISEPA = times;
}


/**
  * @brief config Digital Filtering
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] mode : the mode of DigitalFilter
  *		@arg MAG_FILTER_2_COUNS_DATA_AVERAGES
  *		@arg MAG_FILTER_3_COUNS_DATA_AVERAGES
  *		@arg MAG_FILTER_4_COUNS_DATA_AVERAGES
  *		@arg MAG_FILTER_5_COUNS_DATA_AVERAGES
  * @retval none
  * @note  
  */
void MAG_ConfigDigitalFilter(MAG_TypeDef* Magx,uint16_t mode)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	Magx->MAGFLTPA= mode;
}

/**
  * @brief config Digital Filtering
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] mode : the mode of DigitalFilter
  *		@arg MAG_FILTER_2_COUNS_DATA_AVERAGES
  *		@arg MAG_FILTER_3_COUNS_DATA_AVERAGES
  *		@arg MAG_FILTER_4_COUNS_DATA_AVERAGES
  *		@arg MAG_FILTER_5_COUNS_DATA_AVERAGES
  * @retval none
  * @note  
  */
void MAG_ConfigTopAndLowThreshold(MAG_TypeDef* Magx,uint16_t TopVoltage,uint16_t LowVoltage)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	Magx->MAGPEEL_TH = (TopVoltage<<16)|LowVoltage;
}


/**
  * @brief config Digital Filtering
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] times : the Times of one-time Card Swpie Detection time
  * @retval none
  * @note  No extreme value of swipe card was detected in time. The end of this swipe card was detected
  */
void MAG_ConfigCardSwipeCheckTime(MAG_TypeDef* Magx,uint16_t times)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	Magx->MAGVOPA = times;
}


/**
  * @brief config Digital Filtering
  * @param[in] select the MAG1 MAG2 MAG3 peripheral.
  *		@arg MAG1
  *		@arg MAG2
  *		@arg MAG3
  * @param[in] mode : MAG Config Value
  * 	@arg MAG_CONFIG_AUTO_THRESHOLD
  *		@arg MAG_CONFIG_PEEL			
  *		@arg MAG_CONFIG_FILTER		
  *	 @arg MAG_CONFIG_WITHE_NOSIE	
  *	 @arg MAG_CONFIG_TOP_LOW_THRESHOLD				
  * @param[in] NewState : ENABLE or DISABLE
  * @retval none
  * @note  No extreme value of swipe card was detected in time. The end of this swipe card was detected
  */
void MAG_Cmd(MAG_TypeDef* Magx,uint32_t mode,FunctionalState NewState)
{
	assert_param(IS_MAG_BASE_ADDRESS(Magx));
	assert_param(IS_MAG_CONFIG_VALUE(mode));
	if(NewState == ENABLE)
	{
		Magx->MAGCON|= mode;
	}
	else
	{
		Magx->MAGCON&= ~(mode);
	}
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



