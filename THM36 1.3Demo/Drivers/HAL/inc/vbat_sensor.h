/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: VBAT_Sensor.h 
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VBAT_SENSOR_H
#define __VBAT_SENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup BAT
  * @brief BAT HAL modules driver 
  * @{
  */ 

/** @defgroup BAT_Exported_Constants BAT Exported Constants
  * @{
  */
	 
/** 
  * @brief  VBAT VD Init structure definition 
  */	
typedef struct
{
	uint32_t CheckMode; 	/*!< the VBAT check Mode,This parameter can be a value of @ref VBAT_Check_Mode*/
	uint32_t LowAlarmMode;	/*!< the VD Low Level Alarm Mode,This parameter can be a value of @ref VD_LOW_LEVEL_ALARM_CLEAR_BAT_RAM*/
	uint32_t HighAlarmMode; /*!< the VD high Level Alarm Mode,This parameter can be a value of @ref VD_HIGH_LEVEL_ALARM_CLEAR_BAT_RAM*/
	uint32_t LowLevelMode;	/*!< the VD low Level Alarm Value,This parameter can be a value of @ref LOW_LEVEL_ALARM_VALUE*/
}VBAT_VD_InitTypeDef;

/** 
  * @brief  VBAT VD Init structure definition 
  */	
typedef struct
{
	uint32_t CheckMode; 			/*!< the VBAT check Mode,This parameter can be a value of @ref VBAT_Check_Mode*/
	uint32_t LowTemperatureMode;	/*!< the TD Low TEMPER Alarm Mode,This parameter can be a value of @ref TD_HIGH_TEMPERATURE_ALARM_CLEAR_BAT_RAM*/
	uint32_t HighTemperatureMode; 	/*!< the TD high TEMPER Alarm Mode,This parameter can be a value of @ref TD_LOW_TEMPERATURE_ALARM_CLEAR_BAT_RAM*/
}VBAT_TD_InitTypeDef;

typedef struct
{
	//uint32_t DynamicMode;   /*!< the VBAT Dynamic Clear BuckupReg Mode,This parameter can be a value of @ref SD_DYNAMIC_ALARM_CLEAR_BAT_RAM*/
	uint8_t Round;		  	/*!< the Rounds of one time DYNAMIC check,the Range of Round is 0-7,0 is Round 1 */
	uint8_t	ReceiveDelay; 	/*!<input interface the Receive delay time of Out interface Completed send the Range of ReceiveDelay is 0-0xFF (the delay time is ReceiveDelay*4+1 BVAT clock)*/
	uint8_t TotalNumber ; 	/*!< Total Numbers of Fault Tolerance with one times Dynamic SD check(the default value is 7. the Range of TotalTimes is 0-0x0f 0:1 time 0x0f:16 times )*/
	uint8_t FaultNumber ; 	/* allow Numbers of Fault Tolerance with one times Dynamic SD check(the Range of FaultNumber is 0-0x07,the default value is 1, 0:not allow Fault Tolerance,7:allow 7 times Fault Tolerance)*/
	uint16_t ClearBuckUpReg; /*!< DYNAMIC Alarm clear Buck up Reg or not Clear Buck up Reg,This parameter can be a value of @ref SD_DYNAMIC_ALARM_CLEAR_BAT_RAM*/
}VBAT_DynamicSD_InitTypeDef;

typedef struct
{
	//uint32_t StaticMode;   /*!< the VBAT Static Clear BuckupReg Mode,This parameter can be a value of @ref SD_STATIC_ALARM_CLEAR_BAT_RAM*/
	uint8_t TotalNumber ;  /*!< Total Numbers of Fault Tolerance with one times Static SD check(the default value is 7. the Range of TotalTimes is 0-0x0f 0:1 time 0x0f:16 times )*/
	uint8_t FaultNumber ;  /* allow Numbers of Fault Tolerance with one times Static SD check(the Range of FaultNumber is 0-0x07,the default value is 1, 0:not allow Fault Tolerance,7:allow 7 times Fault Tolerance*/
	uint16_t ClearBuckUpReg;/*!< STATIC Alarm clear Buck up Reg or not Clear Buck up Reg,This parameter can be a value of @ref SD_STATIC_ALARM_CLEAR_BAT_RAM*/
}VBAT_StaticSD_InitTypeDef;
/** 
  * @brief  VBAT SD Init structure definition VBAT_SDx_TYPE
  */	
typedef struct
{
	VBAT_DynamicSD_InitTypeDef Dynamic_Init;
	VBAT_StaticSD_InitTypeDef  Static_Init;
	uint32_t SD0_ConfigMode; 	/*!< Config the SD0 is Static or Dymic,This parameter can be a value of @ref VBAT_SDx_TYPE*/
	uint32_t SD1_ConfigMode; 	/*!< Config the SD1 is Static or Dymic,This parameter can be a value of @ref VBAT_SDx_TYPE*/
	uint32_t SD2_ConfigMode; 	/*!< Config the SD2 is Static or Dymic,This parameter can be a value of @ref VBAT_SDx_TYPE*/
	uint32_t SD3_ConfigMode; 	/*!< Config the SD3 is Static or Dymic,This parameter can be a value of @ref VBAT_SDx_TYPE*/
	uint32_t SD0_Alarm_Static;	/*!< SD0 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD1_Alarm_Static;	/*!< SD1 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD2_Alarm_Static;	/*!< SD2 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD3_Alarm_Static;	/*!< SD3 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD4_Alarm_Static;	/*!< SD4 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD5_Alarm_Static;	/*!< SD5 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD6_Alarm_Static;	/*!< SD6 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
	uint32_t SD7_Alarm_Static;	/*!< SD7 static Alarm mode,This parameter can be a value of @ref VBAT_SDx_ALARM_LEVEL*/
}VBAT_SD_InitTypeDef;


///** 
//  * @brief  VBAT VD Init structure definition 
//  */	
//typedef struct
//{
//	uint32_t CheckMode; 		/*!< the VBAT check Mode,This parameter can be a value of @ref VBAT_Check_Mode*/
//	uint32_t LowTemperMode;		/*!< the VD Low TEMPER Alarm Mode,This parameter can be a value of @ref VD_LOW_TEMPER_CLEAR_BAT_RAM*/
//	uint32_t HighTemperMode; 	/*!< the VD high TEMPER Alarm Mode,This parameter can be a value of @ref VD_HIGH_TEMPER_ALARM_CLEAR_BAT_RAM*/
//	
//}VBAT_SD_InitTypeDef;


/** @defgroup VBAT_Check_Mode  VBAT Check Mode
  * @{
  */ 
#define VBAT_TIME_OVERFLOW_CHECK	0
#define VBAT_TIME_ALLWAYS_CHECK		1U<<2										
/**
  * @}
  */

/** @defgroup VBAT_VD_FLAG VBAT VD FLAG
  * @{
  */ 
#define VBAT_HIGH_LEVEL_ALARM				0x02U
#define VBAT_LOW_LEVEL_ALARM				0x01U
										
/**
  * @}
  */

/** @defgroup VBAT_TD_FLAG VBAT TD FLAG
  * @{
  */ 
#define VBAT_HIGH_TEMPER_ALARM				0x02U
#define VBAT_LOW_TEMPER_ALARM				0x01U
										
/**
  * @}
  */
  
/** @defgroup VD_HIGH_LEVEL_ALARM_CLEAR_BAT_RAM VD HIGH LEVEL ALARM CLEAR BAT RAM
  * @{
  */ 
#define VD_HIGH_LEVEL_ALARM_CLEAR_BAT_RAM_ENABLE		(1U<<6)
#define VD_HIGH_LEVEL_ALARM_CLEAR_BAT_RAM_DISABLE		0										
/**
  * @}
  */

/** @defgroup VD_LOW_LEVEL_ALARM_CLEAR_BAT_RAM VD LOW LEVEL ALARM CLEAR BAT RAM
  * @{
  */ 
#define VD_LOW_LEVEL_ALARM_CLEAR_BAT_RAM_ENABLE			(1U<<5)
#define VD_LOW_LEVEL_ALARM_CLEAR_BAT_RAM_DISABLE		0										
/**
  * @}
  */

/** @defgroup LOW_LEVEL_ALARM_VALUE	LOW LEVEL ALARM VALUE
  * @{
  */ 
#define VD_LOW_LEVEL_ALARM_LESS_1_5_V					(1U<<4)
#define VD_LOW_LEVEL_ALARM_LESS_2_2_V					0										
/**
  * @}
  */

/** @defgroup TD_HIGH_TEMPERATURE_ALARM_CLEAR_BAT_RAM VD HIGH TEMPERATURE ALARM CLEAR BAT RAM
  * @{
  */ 
#define TD_HIGH_TEMPERATURE_ALARM_CLEAR_BAT_RAM_ENABLE		(1U<<5)
#define TD_HIGH_TEMPERATURE_ALARM_CLEAR_BAT_RAM_DISABLE		0										
/**
  * @}
  */

/** @defgroup TD_LOW_TEMPERATURE_CLEAR_BAT_RAM VD LOW TEMPERATURE CLEAR BAT RAM
  * @{
  */ 
#define TD_LOW_TEMPERATURE_ALARM_CLEAR_BAT_RAM_ENABLE		(1U<<4)
#define TD_LOW_TEMPERATURE_ALARM_CLEAR_BAT_RAM_DISABLE		0										
/**
  * @}
  */

/** @defgroup VBAT_SDx VBAT SDx
  * @{
  */ 
#define VBAT_SD0	1U
#define VBAT_SD1	2U
#define VBAT_SD2	4U
#define VBAT_SD3	8U
#define VBAT_SD4	16U
#define VBAT_SD5	32U
#define VBAT_SD6	64U
#define VBAT_SD7	128U

/**
  * @}
  */

/** @defgroup VBAT_SDx_ALARM_LEVEL  VBAT SDx ALARM LEVEL
  * @{
  */ 
#define VBAT_SDx_ALARM_LEVEL_IS_HIGH	0U
#define VBAT_SDx_ALARM_LEVEL_IS_LOW		1U

/**
  * @}
  */
/** @defgroup SD_STATIC_ALARM_CLEAR_BAT_RAM SD STATIC ALARM CLEAR BAT RAM
  * @{
  */ 
#define SD_STATIC_ALARM_CLEAR_BAT_RAM_ENABLE		(1U<<8)
#define SD_STATIC_ALARM_CLEAR_BAT_RAM_DISABLE		0										
/**
  * @}
  */
/** @defgroup SD_DYNAMIC_ALARM_CLEAR_BAT_RAM SD DYNAMIC ALARM CLEAR BAT RAM
  * @{
  */ 
#define SD_DYNAMIC_ALARM_CLEAR_BAT_RAM_ENABLE		(1U<<9)
#define SD_DYNAMIC_ALARM_CLEAR_BAT_RAM_DISABLE		0										
/**
  * @}
  */  
/** @defgroup VBAT_SDx_TYPE VBAT SDx TYPE
  * @{
  */ 
#define VBAT_SDX_TYPE_STATIC		1U
#define VBAT_SDX_TYPE_DYNAMIC		0U

/**
  * @}
  */

/** @defgroup BGR2_MODE BGR2 MODE
  * @{
  */ 
#define VBAT_BGR2_MODE_ALWAY_START				1U
#define VBAT_BGR2_MODE_ONLY_SENCER_ENABLE_START	0	

/**
  * @}
  */

/** @defgroup BGR_TEST_MODE BGR TEST MODE
  * @{
  */ 
#define VBAT_BGR2_TEST_OUT 		0U
#define	VBAT_BGR1_TEST_OUT  	2U
#define	VBAT_VDDBAT_TEST_OUT  	4U
#define	VBAT_VBAT_TEST_OUT  	6U

/**
  * @}
  */

/** @defgroup MAIN_SD_Flag MAIN SD Flag
  * @{
  */ 
#define MAIN_SD_FLAG_STATIC		1U
#define MAIN_SD_FLAG_DYNAMIC	1U<<1	

/**
  * @}
  */

/** @defgroup MAIN_SD_IT MAIN SD IT
  * @{
  */ 
#define MAIN_SD_IT_STATIC		1U
#define MAIN_SD_IT_DYNAMIC		1U<<1	

/**
  * @}
  */
  
/** @defgroup MAIN_TD_Flag MAIN TD Flag
  * @{
  */ 
#define MAIN_TD_FLAG_LOW_TEMPERATURE		1U
#define MAIN_TD_FLAG_HIGH_TEMPERATURE		1U<<1	

/**
  * @}
  */

/** @defgroup MAIN_TD_IT MAIN TD IT
  * @{
  */ 
#define MAIN_TD_IT_LOW_TEMPERATURE		1U
#define MAIN_TD_IT_HIGH_TEMPERATURE		1U<<1	

/**
  * @}
  */

/** @defgroup MAIN_TD_Flag MAIN TD Flag
  * @{
  */ 
#define MAIN_VD_FLAG_LOW_LEVEL		1U
#define MAIN_VD_FLAG_HIGH_LEVEL		1U<<1	

/**
  * @}
  */

/** @defgroup MAIN_TD_IT MAIN TD IT
  * @{
  */ 
#define MAIN_VD_IT_LOW_LEVEL		1U
#define MAIN_VD_IT_HIGH_LEVEL		1U<<1	

/**
  * @}
  */

/**
  * @}
  */ 
 
/** @addtogroup BAT_exported_function BAT exported function
  * @{
  */
/**
  * @brief  Sensor Timer Start.
  * @param[in] NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void VBAT_SensorTimeStart(FunctionalState NewState);
/**
  * @brief  Sensor Timer Start.
  * @param[in] NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void VBAT_ConfigSensorTimeValue(uint16_t Value);
/**
  * @brief  Get Sensor Timer Status.
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@ARG RESET
  * @note  none
  */
FlagStatus VBAT_GetSensorTimeStatus(void);

/**
  * @brief  Get Sensor Timer Status.
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@ARG RESET
  * @note  none
  */
void VBAT_ClearSensorTimeStatus(void);

/**
  * @brief  Initializes the VBAT VD peripheral according to 
  *   the specified parameters in the VBAT_VD_InitTypeDef.
  * @param[in] VD_InitStruct: pointer to a VBAT_VD_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
void VBAT_VD_Init(VBAT_VD_InitTypeDef *VD_InitStruct);
/**
  * @brief Enable or Disable VD High Level Check 
  * @param[in] NewState: enable or disable VD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_VD_HighLevelCmd(FunctionalState NewState);
/**
  * @brief Enable or Disable VD Low Level Check 
  * @param[in] NewState: enable or disable VD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_VD_LowLevelCmd(FunctionalState NewState);
/**
  * @brief  Get the Status of VBAT VD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_LEVEL_ALARM
  *		@arg VBAT_LOW_LEVEL_ALARM
  * @retval none
  * @note  
  */
FlagStatus VBAT_VD_GetStatus(uint32_t Flag);
/**
  * @brief  Clear the Status of VBAT VD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_LEVEL_ALARM
  *		@arg VBAT_LOW_LEVEL_ALARM
  * @retval none
  * @note  
  */
void VBAT_VD_ClearStatus(uint32_t Flag);
/**
  * @brief  Initializes the VBAT TD peripheral according to 
  *   the specified parameters in the VBAT_TD_InitTypeDef.
  * @param[in] TD_InitStruct: pointer to a VBAT_TD_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
void VBAT_TD_Init(VBAT_TD_InitTypeDef *TD_InitStruct);
/**
  * @brief Enable or Disable TD Check 
  * @param[in] NewState: enable or disable TD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_TD_Cmd(FunctionalState NewState);
/**
  * @brief  Get the Status of VBAT TD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_TEMPER_ALARM
  *		@arg VBAT_LOW_TEMPER_ALARM
  * @retval none
  * @note  
  */
FlagStatus VBAT_TD_GetStatus(uint32_t Flag);
/**
  * @brief  Clear the Status of VBAT TD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_TEMPER_ALARM
  *		@arg VBAT_LOW_TEMPER_ALARM
  * @retval none
  * @note  
  */
void VBAT_TD_ClearStatus(uint32_t Flag);
/**
  * @brief  VBAT SD0 ENABLE or DISABLE
  * @param[in] SDx: 
  *		@arg VBAT_SD0
  *		@arg VBAT_SD1
  *		@arg VBAT_SD2
  *		@arg VBAT_SD3
  *		@arg VBAT_SD4
  *		@arg VBAT_SD5
  *		@arg VBAT_SD6
  *		@arg VBAT_SD7
  * @param[in] NewState: enable or disable SD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_SDx_Cmd(uint32_t SDx, FunctionalState NewState);
/**
  * @brief  Config SDx is Alarm,when the Sdx's Pin is High Level or Low Level
  * @param[in] SDx: 
  *		@arg VBAT_SD0
  *		@arg VBAT_SD1
  *		@arg VBAT_SD2
  *		@arg VBAT_SD3
  *		@arg VBAT_SD4
  *		@arg VBAT_SD5
  *		@arg VBAT_SD6
  *		@arg VBAT_SD7
  * @param[in] AlarmLevel: the Alarm Level 
  *		@arg VBAT_ALARM_LEVEL_IS_HIGH
  *		@arg VBAT_ALARM_LEVEL_IS_LOW
  * @retval none
  * @note  
  */
void VBAT_ConfigSDxStaticLevelAlarm(uint32_t SDx, uint32_t AlarmLevel);
/**
  * @brief  Get the Status of VBAT SDx
  * @param[in] SDx: 
  *		@arg VBAT_SD0
  *		@arg VBAT_SD1
  *		@arg VBAT_SD2
  *		@arg VBAT_SD3
  *		@arg VBAT_SD4
  *		@arg VBAT_SD5
  *		@arg VBAT_SD6
  *		@arg VBAT_SD7
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note  
  */
FlagStatus VBAT_SDx_GetAlarmStatus(uint32_t SDx);
/**
  * @brief  Get the Status of VBAT SDx
  * @param[in] SDx: 
  *		@arg VBAT_SD0
  *		@arg VBAT_SD1
  *		@arg VBAT_SD2
  *		@arg VBAT_SD3
  *		@arg VBAT_SD4
  *		@arg VBAT_SD5
  *		@arg VBAT_SD6
  *		@arg VBAT_SD7
  * @retval none
  * @note  none
  */
void VBAT_SDx_ClearAlarmStatus(uint32_t SDx);
/**
  * @brief  Config the type of VBAT SDx
  * @param[in] SDx: 
  *		@arg VBAT_SD0
  *		@arg VBAT_SD1
  *		@arg VBAT_SD2
  *		@arg VBAT_SD3
  * @param[in] Type: 
  *		@arg VBAT_SDX_TYPE_STATIC
  *		@arg VBAT_SDX_TYPE_DYNAMIC
  * @retval none
  * @note  none
  */
void VBAT_SDx_ConfigType(uint32_t SDx,uint32_t Type);
/**
  * @brief  Config the Rounds of one time DYNAMIC  Detection,and config
  * @param[in] Round:
  *		@arg 0: one Round of one time
  *		@arg 1: two Round of one time
  *		@arg 2: three Round of one time
  *		@arg 3: four Round of one time
  *		@arg 4: five Round of one time
  *		@arg 5: six Round of one time
  *		@arg 6: sever Round of one time
  *		@arg 7: eight Round of one time
  * @param[in] TimeOut:input interface the Receive delay time of Out interface Completed send
  *			   the Range of TimeOut is 0-0xFF (the delay time is TimeOut*4+1 BVAT clock)
  * @retval none
  * @note  none
  */
void VBAT_SDx_ConfigDynamicRoundsAndDelayTime(uint8_t Round,uint8_t TimeOut);
/**
  * @brief  Write Dynamic Detection of Random Number Seeds
  * @param[in] Seed: Dynamic Detection of Random Number Seeds(the Range of Seed is 0-0xFF)	
  * @retval none
  * @note  none
  */
void VBAT_SDx_WriteSeed(uint8_t Seed);
/**
  * @brief  Config  SD Static Fault Tolerance
  * @param[in] TotalTimes:  Total Numbers of Fault Tolerance with one times Static SD check(
  *	  the default value is 7. the Range of TotalTimes is 0-0x0f 0:1 time 0x0f:16 times )
  * @param[in] FaultNumber:  FaultNumber : allow Numbers of Fault Tolerance with one times Static SD check
  *		   (the Range of FaultNumber is 0-0x07,the default value is 1, 0:not allow Fault Tolerance,7:allow 7 times Fault Tolerance)
  * @param[in] NewState
  *		@arg ENABLE : ENANLE TotalTimes and FaultNumber 
  *		@arg DISABLE : DISABLE TotalTimes and FaultNumber 
  * @retval none
  * @note  In order to avoid static SD false alarm, when Every Time static SD check,will be config the FaultNumber and FaultNumber,
  */
void VBAT_SDx_ConfigStaticFaultTolerance(uint8_t TotalNumber ,uint8_t FaultNumber, FunctionalState NewState);
/**
  * @brief  Config  SD Dynamic Fault Tolerance
  * @param[in] TotalTimes:  Total Numbers of Fault Tolerance with one times Dynamic SD check(
  *	  the default value is 7. the Range of TotalTimes is 0-0x0f 0:1 time 0x0f:16 times )
  * @param[in] FaultNumber:  FaultNumber : allow Numbers of Fault Tolerance with one times Dynamic SD check
  *		   (the Range of FaultNumber is 0-0x07,the default value is 1, 0:not allow Fault Tolerance,7:allow 7 times Fault Tolerance)
  * @param[in] NewState
  *		@arg ENABLE : ENANLE TotalTimes and FaultNumber 
  *		@arg DISABLE : DISABLE TotalTimes and FaultNumber 
  * @retval none
  * @note  In order to avoid Dynamic SD false alarm, when Every Time Dynamic SD check,will be config the FaultNumber and FaultNumber,
  */
void VBAT_SDx_ConfigDynamicFaultTolerance(uint8_t TotalNumber ,uint8_t FaultNumber, FunctionalState NewState);

/**
  * @brief  Initializes the VBAT SD peripheral according to the specified parameters in the VBAT_SD_InitTypeDef.
  * @param[in] SD_Init: pointer to a VBAT_SD_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
void VBAT_SDx_Init(VBAT_SD_InitTypeDef *SD_Init);

/**
  * @brief SDx's Alarm whether to Clear BuckUpReg 
  * @param[in] StaticConfig: Static alarm
  *		@arg SD_STATIC_ALARM_CLEAR_BAT_RAM_ENABLE
  *		@arg SD_STATIC_ALARM_CLEAR_BAT_RAM_DISABLE
  * @param[in] DynamicConfig: Dynamic alarm
  *		@arg SD_DYNAMIC_ALARM_CLEAR_BAT_RAM_ENABLE
  *		@arg SD_DYNAMIC_ALARM_CLEAR_BAT_RAM_DISABLE
  * @retval none
  * @note  
  */
void VBAT_SD_CLearBuckUpReg(uint32_t StaticConfig,uint32_t DynamicConfig);

/**
  * @brief  Get the SD Status of MAIN Power domain
  * @param[in] Flag: 
  *		@arg MAIN_SD_FLAG_STATIC
  *		@arg MAIN_SD_FLAG_DYNAMIC
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note if want to clear the status, can call Function of VBAT_SDx_ClearAlarmStatus to Clear which Set Bits
  */
FlagStatus MAIN_SD_GetAlarmStatus(uint32_t Flag);
/**
  * @brief  Config the MAIN Power domain SD IT MASK
  * @param[in] Flag: 
  *		@arg MAIN_SD_IT_STATIC
  *		@arg MAIN_SD_IT_DYNAMIC
  * @param[in] NewState: 
*		@arg ENABLE: ENABLE the IT of SD
  *		@arg DISABLE: DISABLE the IT of SD
  * @retval none
  * @note none
  */
void MAIN_SD_ConfigIT(uint32_t IT,FunctionalState NewState);
/**
  * @brief  Get the TD Status of MAIN Power domain
  * @param[in] Flag: 
  *		@arg MAIN_TD_FLAG_LOW_TEMPERATURE
  *		@arg MAIN_TD_FLAG_HIGH_TEMPERATURE
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note if want to clear the status, can call Function of VBAT_SDx_ClearAlarmStatus to Clear which Set Bits
  */
FlagStatus MAIN_TD_GetAlarmStatus(uint32_t Flag);
/**
  * @brief  Config the MAIN Power domain SD IT MASK
  * @param[in] Flag: 
  *		@arg MAIN_TD_IT_LOW_TEMPERATURE
  *		@arg MAIN_TD_IT_HIGH_TEMPERATURE
  * @param[in] NewState: 
*		@arg ENABLE: ENABLE the IT of TD
  *		@arg DISABLE: DISABLE the IT of TD
  * @retval none
  * @note none
  */
void MAIN_TD_ConfigIT(uint32_t IT,FunctionalState NewState);
/**
  * @brief  Get the TD Status of MAIN Power domain
  * @param[in] Flag: 
  *		@arg MAIN_VD_FLAG_LOW_LEVEL
  *		@arg MAIN_VD_FLAG_HIGH_LEVEL
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note if want to clear the status, can call Function of VBAT_SDx_ClearAlarmStatus to Clear which Set Bits
  */
FlagStatus MAIN_VD_GetAlarmStatus(uint32_t Flag);

/**
  * @brief  Config the MAIN Power domain SD IT MASK
  * @param[in] Flag: 
  *		@arg MAIN_VD_IT_LOW_LEVEL
  *		@arg MAIN_VD_IT_HIGH_LEVEL
  * @param[in] NewState: 
*		@arg ENABLE: ENABLE the VD of TD
  *		@arg DISABLE: DISABLE the VD of TD
  * @retval none
  * @note none
  */
void MAIN_VD_ConfigIT(uint32_t IT,FunctionalState NewState);



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
#ifdef __cplusplus
}
#endif

#endif 

/************************ (C) COPYRIGHT TMC *****END OF FILE***********************/


