/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_vbat.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_VBAT_H
#define _TMC_VBAT_H



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "Sensor.h"
#include "VBAT.h"	
#include "VBAT_Sensor.h"
#include "GPIO.h"
#include "PWR.h"	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup VBAT
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup UART_Exported_Types UART Exported Types
  * @{
  */ 
typedef enum
{
  TMC_VBAT_STATE_RESET			 = 0x00U,  /*!< VBAT not yet initialized or disabled  */
  TMC_VBAT_STATE_READY			 = 0x01U,  /*!< VBAT initialized and ready for use	*/
  TMC_VBAT_STATE_ALARM			   = 0x04U   /*!< VBAT ALARM state					*/
}TMC_VBAT_StateTypeDef;

/** @defgroup VBAT_ALARM_Code VBAT ALARM Code
  * @{
  */
#define TMC_VBAT_ALARM_NONE	  						0x00000000U	/*!< VBAT NONE ALARM */
#define TMC_VBAT_ALARM_IWDT	   						0x00000001U	/*!< VBAT IWDT ALARM */
#define TMC_VBAT_ALARM_RTC	  						0x00000002U	/*!< VBAT RTC ALARM */
#define TMC_VBAT_ALARM_SD_0  							0x00000001U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_1  							0x00000002U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_2  							0x00000004U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_3  							0x00000008U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_4  							0x00000010U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_5  							0x00000020U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_6  							0x00000040U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_SD_7  							0x00000080U	/*!< VBAT SD ALARM  */
#define TMC_VBAT_ALARM_TD_LOW_TEMPERATURE	   		0x00000100U	/*!< VBAT TD LOW TEMPERATURE ALARM */
#define TMC_VBAT_ALARM_TD_HIGH_TEMPERATURE	   		0x00000200U	/*!< VBAT TD HIGH TEMPERATURE ALARM */
#define TMC_VBAT_ALARM_VD_LOW_LEVEL	   				0x00000400U	/*!< VBAT VD LOW LEVEL ALARM */
#define TMC_VBAT_ALARM_VD_HIGH_LEVEL					0x00000800U	/*!< VBAT VD HIGH LEVEL ALARM */
#define TMC_VBAT_ALARM_FD3_HIGH_FREQ	   				0x00001000U	/*!< VBAT FD3 ALARM */
#define TMC_VBAT_ALARM_FD3_LOW_FREQ	   				0x00002000U	/*!< VBAT FD3 ALARM */
#define TMC_VBAT_ALARM_FD3_NOT_ALARM	   				0x00004000U	/*!< VBAT FD3 ALARM */
/**
  * @}
  */
/** 
  * @brief  VBAT handle Structure definition
  */
typedef struct
{
	//BAT_FD3InitTypeDef					FD3_Init;
	RTC_InitTypeDef						RTC_Init;
	RTC_AlarmInitTypeDef				RTC_AlarmInit;
	IWDT_InitTypeDef					IWDT_Init;
	VBAT_VD_InitTypeDef					VD_Init;
	VBAT_TD_InitTypeDef					TD_Init;
	VBAT_SD_InitTypeDef					SD_Init;
	VBAT_GPIO_InitTypeDef				VBAT_GPIO_Init;
	
	HAL_LockTypeDef						Lock;
	volatile TMC_VBAT_StateTypeDef		State;
	 __IO uint32_t						ALARMCode;
}VBAT_HandleTypeDef;	 

/** 
  * @brief  VBAT_FD3 handle Structure definition
  */
//TMC(20240408)-zt:split FD3_Init from VBAT_HandleTypeDef to split IQHander enter-parameters
typedef struct
{
	BAT_FD3InitTypeDef					FD3_Init;
	volatile TMC_VBAT_StateTypeDef		State;
	 __IO uint32_t						ALARMCode;
}VBAT_FD3_HandleTypeDef;	

/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup VBAT_Exported_Constants UART Exported Constants
  * @{
  */

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup VBAT_Exported_Functions
  * @{
  */
TMC_StatusTypeDef TMC_VBAT_Init(VBAT_HandleTypeDef *hvbat,VBAT_FD3_HandleTypeDef *hvbat_fd3);
TMC_StatusTypeDef TMC_VBAT_DeInit(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_RTC_Start_IT(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_RTC_Stop_IT(VBAT_HandleTypeDef *hvbat);

TMC_StatusTypeDef TMC_VBAT_EFD3_Start_IT(VBAT_FD3_HandleTypeDef *hvbat_fd3,uint32_t LowFrq,uint32_t HighFrq);
TMC_StatusTypeDef TMC_VBAT_EFD3_Stop_IT(VBAT_FD3_HandleTypeDef *hvbat_fd3);
void TMC_VBAT_EFD3_IRQHandler(VBAT_FD3_HandleTypeDef *hvbat_fd3);

TMC_StatusTypeDef TMC_VBAT_VD_Start_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_VD_Stop_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_TD_Start_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_TD_Stop_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_SD_Start_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_SD_Stop_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_IWDT_Start_IT(VBAT_HandleTypeDef *hvbat);
TMC_StatusTypeDef TMC_VBAT_IWDT_Stop_IT(VBAT_HandleTypeDef *hvbat);

void TMC_VBAT_StructInit(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_MspInit(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_MspDeInit(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_SensorAlarmCpltCallback(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_FD3AlarmCpltCallback(VBAT_FD3_HandleTypeDef *hvbat_fd3);
void TMC_VBAT_IWDT_AlarmCpltCallback(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_RTC_AlarmCpltCallback(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_Sensor_IRQHandler(VBAT_HandleTypeDef *hvbat); 
void TMC_VBAT_RTC_Alarm_IRQHandler(VBAT_HandleTypeDef *hvbat);
void TMC_VBAT_IWDT_Alarm_IRQHandler(VBAT_HandleTypeDef *hvbat);

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

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
