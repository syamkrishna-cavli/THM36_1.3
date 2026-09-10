/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: sensor.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_H
#define __SENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup SENSOR
  * @brief SENSOR HAL modules driver 
  * @{
  */ 

/** @defgroup SENSOR_Exported_Constants EFD Exported Constants
  * @{
  */

/** 
  * @brief  EFD3 Init structure definition 
  */	
typedef struct
{
	uint8_t FD3_CheckClock;			/*!< the clock of FD3 check 
									This parameter can be a value of @ref FD3_Check_Clock */
	uint8_t FD3_ReferenceClock;		/*!< the clock of FD3 Reference check 
									This parameter can be a value of @ref FD3_Reference_Clock */
	uint8_t FD3_Divsion;			/*!< the division of FD3 Reference Clock check 
									This parameter can be a value of @ref FD3_Reference_Divsion */
}BAT_FD3InitTypeDef;

#define LD0		1U
#define LD1		(1U<<1)
#define LD2		(1U<<2)
#define LD3		(1U<<3)
#define LD4		(1U<<4)
/** @defgroup FD3_Check_Clock FD3 Check Clock
  * @{
  */ 
#define INSIDE_32K_CLOCK			(1<<4)	
#define EXTERNAL_32K_CLOCK			0													
/**
  * @}
  */

/** @defgroup FD3_Reference_Clock FD3 Reference Clock 
  * @{
  */ 
#define REFERENCE_CLOCK_HSE			(1U<<2)	
#define REFERENCE_CLOCK_PLL_L		(0)															
/**
  * @}
  */

/** @defgroup FD3_Reference_Divsion FD3 Reference Divsion
  * @{
  */ 
#define REFERENCE_CLOCK_DIVSION_NO	0
#define REFERENCE_CLOCK_DIVSION_2	(1U<<5)
#define REFERENCE_CLOCK_DIVSION_4	(1U<<6)
#define REFERENCE_CLOCK_DIVSION_8	((1U<<6)|(1U<<5))
/**
  * @}
  */
  
/** @defgroup FD3_FLAG FD3 FLAG
  * @{
  */ 
#define FD3_FLAG_REDUNDANCY_CHECK_ALARM		(1U<<4)
#define FD3_FLAG_CHECK_OK					(1U<<2)
#define FD3_FLAG_HIGH_ALARM					(1U<<1)
#define FD3_FLAG_LOW_ALARM					(1U)
/**
  * @}
  */

/** @defgroup FD3_IT_MSK FD3 IT MSK
  * @{
  */ 
#define FD3_IT_MSK_CHECK_OK	(1U<<2)
#define FD3_IT_MSK_HIGH_ALARM	(1U<<1)
#define FD3_IT_MSK_LOW_ALARM	(1U)
/**
  * @}
  */

/** @defgroup SENSOR_Private_Macros EFD Private Macros
  * @{
  */
#define IS_FD3_SOURCE_CLOCK_CHECK(x) 	((x==INSIDE_32K_CLOCK)||(x==EXTERNAL_32K_CLOCK))
#define IS_FD3_REFERENCE_CLOCK_CHECK(x) ((x==REFERENCE_CLOCK_HSE)||(x==REFERENCE_CLOCK_PLL_L))
#define IS_FD3_REFERENCE_DIVSION_CHECK(x)	((x==REFERENCE_CLOCK_DIVSION_NO)||(x==REFERENCE_CLOCK_DIVSION_2)||(x==REFERENCE_CLOCK_DIVSION_4)||(x==REFERENCE_CLOCK_DIVSION_8))
/**
  * @}
  */  
/**
  * @}
  */ 
/** @addtogroup Sensor_exported_function Sensor exported function
  * @{
  */
/**
  * @brief  Fills each Alarm_InitStruct member with its default value.
  * @param[in] Alarm_InitStruct: pointer to a RTC_AlarmInitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  *		FD3_CheckClock is INSIDE_32K_CLOCK;
  *		   FD3_Divsion is REFERENCE_CLOCK_DIVSION_NO;
  *		   FD3_ReferenceClock is REFERENCE_CLOCK_HSE;
  */
__weak void EFD3_StructInit(BAT_FD3InitTypeDef *FD3_InitStruct);

/**
  * @brief Initializes the FD3 peripheral according to 
  *   the specified parameters in the RTC_InitTypeDef.
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 
void EFD3_Init(BAT_FD3InitTypeDef *FD3_InitStruct);
/**
  * @brief config EFD3 the Low Frequency detection ENASBLE or DISABLE
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.

  * @retval ErrorStatus
  * @note none
  */
void  EFD3_LowFreDetectCmd(FunctionalState NewState);
/**
  * @brief config EFD3 the High Frequency detection ENASBLE or DISABLE
  * @param[in] FD3_InitStruct: pointer to a BAT_FD3InitTypeDef structure,which will be initialized.

  * @retval ErrorStatus
  * @note none
  */
void  EFD3_HighFreDetectCmd(FunctionalState NewState);
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
FlagStatus EFD3_GetStatus(uint32_t Flags);
/**
  * @brief Get EFD3 Status
  * @param[in] Flags:
  *		@zrg FD3_FLAG_REDUNDANCY_CHECK_ALARM 
  *		@zrg FD3_FLAG_CHECK_OK
  *		@zrg FD3_FLAG_HIGH_ALARM
  *		@zrg FD3_FLAG_LOW_ALARM
  * @retval FlagStatus
  * @note none
  */
void EFD3_ClearStatus(uint32_t Flags);
/**
  * @brief Config EFD3's interrupt
  * @param[in] Flags:
  *		@zrg FD3_FLAG_CHECK_OK
  *		@zrg FD3_FLAG_HIGH_ALARM
  *		@zrg FD3_FLAG_LOW_ALARM
  * @param[in] NewState:  
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */
void EFD3_ConfigIT(uint32_t Flags,FunctionalState NewState);

/**
  * @brief config EFD3's Low Freq Frequency vale,and  High Freq Frequency vale
  * @param[in] LFN: Low Freq Frequency vale
  * @param[in] HFN: High Freq Frequency vale
  * @retval none
  * @note none
  */ 
void EFD3_ConfigValue(uint32_t LFN,uint32_t HFN);

/**
  * @brief return the FD3 PER
  * @param[in] none
  * @retval uint32_t
  * @note none
  */
uint32_t EFD3_GetPER(void);
/**
  * @}
  */
/**
  * @brief confog ASH interrupt ENABLE or DISABLE
  * @param[in] Newstate
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */
void ASH_ConfigIT(FunctionalState Newstate);

/**
  * @brief confog EGS interrupt ENABLE or DISABLE
  * @param[in] Newstate
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */
void EGS_ConfigIT(FunctionalState Newstate);

#define LD_0	1UL
#define LD_1	(1UL<<1)	 
#define LD_2	(1UL<<2)
#define LD_3	(1UL<<3)
/**
  * @brief  LD ENABLE or DISABLE
  * @param[in]	Ldx
  *		@arg LD0
  *		@arg LD1
  *		@arg LD2
  *		@arg LD3	 
  * @retval none
  * @note	if Enable LD0123,Ldx = LD0|LD1|LD2|LD3
  */ 	 
void LD_CMD(uint8_t Ldx);
	 
/**
  * @brief  ASH ENABLE or DISABLE
  * @param[in]	NewState
  *		@arg ENABLE
  *		@arg DISABLE	 
  * @retval none
  * @note	
  */ 
void ASH_CMD(FunctionalState NewState);
/**
  * @brief  EGS ENABLE or DISABLE
  * @param[in]	NewState
  *		@arg ENABLE
  *		@arg DISABLE	 
  * @retval none
  * @note	
  */ 
void EGS_CMD(FunctionalState NewState);	
/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif 

/************************ (C) COPYRIGHT TMC *****END OF FILE***********************/



