/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: VBAT.h.
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BAT_H
#define __BAT_H

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
  * @brief  RTC Init structure definition 
  */	
typedef struct
{
	uint8_t Sec; 	/*!< the RTC of sec. */
	uint8_t Min; 	/*!< the RTC of Min. */
	uint8_t Hour;	/*!< the RTC of Hour. */
	uint8_t Day;	/*!< the RTC of Day. */
	uint8_t Month;	/*!< the RTC of Month. */
	uint8_t year;	/*!< the RTC of year. */
	uint8_t Week;	/*!< the RTC of Week. */
}RTC_InitTypeDef;
	 
/** 
  * @brief  RTC Alarm Init structure definition 
  */	
typedef struct
{
	uint8_t Sec; 	/*!< the Alarm of sec. */
	uint8_t Min; 	/*!< the Alarm of Min. */
	uint8_t Hour;	/*!< the Alarm of Hour. */
	uint8_t Day;	/*!< the Alarm of Day. */
	uint8_t Month;	/*!< the Alarm of Month. */
	uint8_t year;	/*!< the Alarm of year. */
	uint8_t Week;	/*!< the Alarm of Week. */
	FunctionalState Sec_Alarm;
	FunctionalState Min_Alarm;
	FunctionalState Hour_Alarm;
	FunctionalState Day_Alarm;
	FunctionalState Month_Alarm;
	FunctionalState year_Alarm;
	FunctionalState Week_Alarm;
}RTC_AlarmInitTypeDef;

/** 
  * @brief  IWDT Init structure definition 
  */	
typedef struct
{
	uint16_t InitialValue; 	/*!< Initial Value of IWDT*/
	uint8_t Prescaler;		/*!< the Perscaler Value of IWDT clock*/
	uint8_t RestMode;		/*!< the IWDT Alarm whether Rest chip, This parameter can be a value of @ref IWDT_ALARM_REST_MODE*/
}IWDT_InitTypeDef;


/** 
  * @brief  IWDT Init structure definition 
  */	
typedef struct
{
	uint32_t PINx; 	
	uint32_t Mode;
	uint32_t Speed;
	uint32_t Pull;
	uint32_t SMIT;
	uint32_t INT_MODE;	/*!<  VBAT GPIO This parameter can be a value of @ref BAT_GPIO_INIT_MODE*/
	uint32_t INT_MSK;	/*!<  VBAT GPIO This parameter can be a value of @ref BAT_GPIO_INIT_MODE*/
}VBAT_GPIO_InitTypeDef;


///** 
//  * @brief  RTC Init structure definition 
//  */	
//typedef struct
//{
//	uint8_t FD3_CheckClock;	/*!< the clock of FD3 check 
//									This parameter can be a value of @ref FD3_Check_Clock */
//	uint8_t FD3_ReferenceClock;		/*!< the clock of FD3 Reference check 
//									This parameter can be a value of @ref FD3_Reference_Clock */
//	uint8_t FD3_Divsion;
//}BAT_FD3InitTypeDef;

///** @defgroup FD3_Check_Clock FD3 Check Clock
//  * @{
//  */ 
//#define INSIDE_32K_CLOCK			(1U<<4)	
//#define EXTERNAL_32K_CLOCK			~(1U<<4)														
///**
//  * @}
//  */

///** @defgroup FD3_Reference_Clock FD3 Reference Clock 
//  * @{
//  */ 
//#define REFERENCE_CLOCK_HSE			(1U<<4)	
//#define REFERENCE_CLOCK_PLL_L		(0)															
///**
//  * @}
//  */

///** @defgroup FD3_Reference_Divsion FD3 Reference Divsion
//  * @{
//  */ 
//#define REFERENCE_CLOCK_DIVSION_NO	0
//#define REFERENCE_CLOCK_DIVSION_2	(1U<<5)
//#define REFERENCE_CLOCK_DIVSION_4	(1U<<6)
//#define REFERENCE_CLOCK_DIVSION_8	((1U<<6)|(1U<<5))
///**
//  * @}
//  */

/** @defgroup IWDT_ALARM_REST_MODE IWDT ALARM REST MODE
  * @{
  */ 
#define IWDT_ALARM_REST_CHIP			1U		
#define IWDT_ALARM_NOT_REST_CHIP		0												
/**
  * @}
  */

/** @defgroup RTC_Time_Config RTC Time Config
  * @{
  */ 
#define SUNDAY			(1U)	
#define MONDAY			(1U<<1)		
#define TUESDAY			(1U<<2)													
#define WEDNESDAY		(1U<<3)	
#define THURSDAY		(1U<<4)	
#define FRIDAY			(1U<<5)	
#define SATURDAY		(1U<<6)	

/**
  * @}
  */

/** @defgroup RTC_ADJUSTMENT_MODE_DAY RTC ADJUSTMENT MODE
  * @{
  */ 
#define RTC_DAILY_INCREASE			2U		
#define RTC_DAILY_DECREASE			~(2U)													
/**
  * @}
  */
/** @defgroup RTC_ADJUSTMENT_MODE_SEC RTC ADJUSTMENT MODE
  * @{
  */ 
#define RTC_SEC_INCREASE			2U		
#define RTC_SEC_DECREASE			~(2U)													
/**
  * @}
  */
/** @defgroup BAT_CLOCK_DIVISON BAT CLOCK DIVISON
  * @{
  */ 
#define BAT_CLOCK_DIV_2	
												
/**
  * @}
  */

/** @defgroup BAT_GPIO_INIT_MODE BAT GPIO INIT MODE
  * @{
  */ 
#define BAT_GPIO_INIT_MODE_RASING		3U
#define BAT_GPIO_INIT_MODE_FALLING		2U
#define BAT_GPIO_INIT_MODE_HIGH_LEVEL	1U	
#define BAT_GPIO_INIT_MODE_LOW_LEVEL	0U	
/**
  * @}
  */

/** @defgroup BAT_GPIO_INIT_MSK_MODE BAT GPIO INIT MSK MODE
  * @{
  */ 
#define BAT_GPIO0_INIT_ENABLE			0U
#define BAT_GPIO3_INIT_DISABLE			1U

/**
  * @}
  */

/** @defgroup BAT_Private_Macros BAT Private Macros
  * @{
  */
#define	IS_RTC_ADJUSTMENT_SEC(x)	(x<=15)
#define	IS_RTC_ADJUSTMENT_MODE(x)	((x==RTC_DAILY_INCREASE)||(x==RTC_DAILY_DECREASE))
#define IS_RTC_WEEK_CHECK(x)				((x == MONDAY)||(x == TUESDAY)||(x == WEDNESDAY)|| \
											 (x == THURSDAY)||(x == FRIDAY)||(x == SATURDAY)|| \
											 (x == SUNDAY))
#define IS_RTC_YEAR_CHECK(x)		(x<=127)
#define IS_RTC_MON_CHECK(x)			((x>0)&&(x<=12))
#define IS_RTC_DAY_CHECK(x)			((x>0)&&(x<=31))
#define IS_RTC_HOUR_CHECK(x)		(x<=23)
#define IS_RTC_MIN_CHECK(x)			((x<=59))
#define IS_RTC_SEC_CHECK(x)			((x<=59))

#define IS_IWDT_INIT_VALUE_CHECK(x)	(x<=0xFFFF)
#define IS_IWDT_PRESCALER_CHECK(x)	(x<=127)

#define IS_VBAT_GPIO_PINX_CHECK(x)	((x & GPIO_PIN_0)||(x & GPIO_PIN_1)||(x & GPIO_PIN_2)||(x & GPIO_PIN_3))
#define IS_VBAT_GPIO_MODE_CHECK(x)	((x == GPIO_MODE_IN)||(x == GPIO_MODE_OUT))
#define IS_VBAT_GPIO_SPEED_CHECK(x)	((x == GPIO_LOW_SPEED)||(x == GPIO_FAST_SPEED)||(x == GPIO_MEDIUM_SPEED)||(x == GPIO_HIGH_SPEED))
#define IS_VBAT_GPIO_PULL_CHECK(x)	((x == GPIO_NOPULL)||(x == GPIO_PULLUP)||(x == GPIO_PULLDOWN))
#define IS_VBAT_GPIO_SMIT_CHECK(x)	((x == GPIO_INPUTSCHMIT_DISABLE)||(x == GPIO_INPUTSCHMIT_ENABLE))
#define IS_VBAT_GPIO_INIT_MODE_CHECK(x)	((x == BAT_GPIO_INIT_MODE_RASING)||(x == BAT_GPIO_INIT_MODE_FALLING)||(x == BAT_GPIO_INIT_MODE_HIGH_LEVEL)||(x == BAT_GPIO_INIT_MODE_LOW_LEVEL))
#define IS_IWDT_REST_MODE_CHECK(x)	((x==IWDT_ALARM_REST_CHIP)||(x == IWDT_ALARM_NOT_REST_CHIP))
//#define IS_RTC_MINI_FREQ_CHECK(x)	((x>8)&&(x<32000))
//#define INSIDE_32K_CLOCK			(1U<<4)	
//#define EXTERNAL_32K_CLOCK			~(1U<<4)

//#define IS_FD3_SOURCE_CLOCK_CHECK(x) 	((x==INSIDE_32K_CLOCK)||(x==EXTERNAL_32K_CLOCK))
//#define IS_FD3_REFERENCE_CLOCK_CHECK(x) ((x==REFERENCE_CLOCK_HSE)||(x==REFERENCE_CLOCK_PLL_L))
//#define IS_FD3_REFERENCE_DIVSION_CHECK(x)	((x==REFERENCE_CLOCK_DIVSION_NO)||(x==REFERENCE_CLOCK_DIVSION_2)||(x==REFERENCE_CLOCK_DIVSION_4)||(x==REFERENCE_CLOCK_DIVSION_8))
/**
  * @}
  */  
/**
  * @}
  */ 
/** @addtogroup BAT_exported_function BAT exported function
  * @{
  */
void BAT_RTC_Cmd(FunctionalState NewState);
void BAT_RTC_StructInit(RTC_InitTypeDef *RTC_InitStruct);
void BAT_RTC_Init(RTC_InitTypeDef *RTC_InitStruct);
void BAT_RTC_DeInit(void);
void BAT_RTC_AlarmStructInit(RTC_AlarmInitTypeDef *Alarm_InitStruct);
void BAT_RTC_AlarmInit(RTC_AlarmInitTypeDef *Alarm_InitStruct);
void BAT_RTC_AllAlarm_Cmd(FunctionalState NewState);
void BAT_RTC_WeekAlarm(FunctionalState NewState);
void BAT_RTC_YearAlarm(FunctionalState NewState);
void BAT_RTC_MonthAlarm(FunctionalState NewState);
void BAT_RTC_DayAlarm(FunctionalState NewState);
void BAT_RTC_MinAlarm(FunctionalState NewState);
void BAT_RTC_SecAlarm(FunctionalState NewState);
FlagStatus  BAT_RTC_GetAlarmStatus(void);
void BAT_RTC_ClearAlarmStatus(void);
FlagStatus  BAT_RTC_GetStatus(void);
void BAT_RTC_ClearStatus(void);
void BAT_RTC_configAdjustment(uint32_t Sec,uint32_t mode);
void BAT_RTC_GetCalendar(RTC_InitTypeDef *RTC_CurrentValue);
void BAT_RTC_ConfigAdjustmentPerSec(uint32_t value,uint32_t mode);
void BAT_RTC_HourAlarm(FunctionalState NewState);
void BAT_IWDT_StructInit(IWDT_InitTypeDef *IWDT_InitStruct);
void BAT_IWDT_Init(IWDT_InitTypeDef *IWDT_InitStruct);
void BAT_IWDT_DeInit(void);
void BAT_IWDT_Cmd(FunctionalState NewState);
void BAT_IWDT_FeedDog(void);
void BAT_WriteRam(uint32_t *Data);
void BAT_ReadRam(uint32_t *Data);
void BAT_WriteEnable(FunctionalState NewState);
void BAT_GPIO_Init(VBAT_GPIO_InitTypeDef *GPIO_Init);
void BAT_GPIO_DeInit(void);
void BAT_GPIO_ConfigIT_Mode(uint32_t VBAT_GPIO_PINX,uint32_t mode);
void BAT_GPIO_ITConfig(uint32_t VBAT_GPIO_PINX,FunctionalState NewState);
FlagStatus BAT_GPIO_GetITStatus(uint32_t VBAT_GPIO_PINX);
void BAT_GPIO_ClearStatus(uint32_t VBAT_GPIO_PINX);
uint32_t BAT_GPIO_GetData(void);

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



