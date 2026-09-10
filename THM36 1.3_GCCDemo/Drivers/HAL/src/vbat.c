/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: vbat.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/

#include "thm36_1.3.h"
#include "rcc.h"
#include "vbat.h"
#include "vbat_sensor.h"
#include "gpio.h"
#include "sensor.h"
#include "romTable.h"
/** @addtogroup thm36_1.3
  * @{
  */
  
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */

/** @defgroup VBAT 
  * @brief VBAT HAL modules driver 
  * @{
  */ 
  
/** @defgroup VBAT_Private_Defines VBAT Private Defines
  * @{
  */
#define RTC_ENABLE					0x01U
#define RTC_DISABLE					0x00U
#define RTC_ALL_ALARM_DISABLE		0x00U
#define RTC_WEEK_ALARM_ENABLE		(1U<<7)
#define RTC_WEEK_ALARM_DISABLE		~(1U<<7)
#define RTC_YEAR_ALARM_ENABLE		(1U<<6)
#define RTC_YEAR_ALARM_DISABLE		~(1U<<6)
#define RTC_MON_ALARM_ENABLE		(1U<<5)
#define RTC_MON_ALARM_DISABLE		~(1U<<5)
#define RTC_DAY_ALARM_ENABLE		(1U<<4)
#define RTC_DAY_ALARM_DISABLE		~(1U<<4)
#define RTC_HOUR_ALARM_ENABLE		(1U<<3)
#define RTC_HOUR_ALARM_DISABLE		~(1U<<3)
#define RTC_MIN_ALARM_ENABLE		(1U<<2)
#define RTC_MIN_ALARM_DISABLE		~(1U<<2)
#define RTC_SEC_ALARM_ENABLE		(1U<<1)
#define RTC_SEC_ALARM_DISABLE		~(1U<<1)
#define RTC_ALARM_ENABLE			(1U)
#define RTC_ALARM_DISABLE			~(1U)
#define LOW_FREQ_ENABLE				1U
#define LOW_FREQ_DISABLE			~(1U)
#define HIGH_FREQ_ENABLE			2U
#define HIGH_FREQ_DISABLE			~(2U)

#define IWDT_LOAD_VALUE				0xAA
#define IWDT_START					0xCC
#define IWDT_PROTECT				0x55
#define IWDT_STOP					0x00

#define VBAT_SENSOR_TIME_OPEN	1U
#define VBAT_SENSOR_TIME_CLOSE	0

#define VBAT_SENSOR_HVD_ENABLE	1U<<1
#define VBAT_SENSOR_HVD_DISABLE	~(1U<<1)

#define VBAT_SENSOR_LVD_ENABLE	1U
#define VBAT_SENSOR_LVD_DISABLE	~(1U)

#define VBAT_SENSOR_TD_ENABLE	1U
#define VBAT_SENSOR_TD_DISABLE	~(1U)

#define VBAT_SENSOR_SD0_ENABLE	1U
#define VBAT_SENSOR_SD0_DISABLE	~(1U)

#define VBAT_SENSOR_SD1_ENABLE	1U<<1
#define VBAT_SENSOR_SD1_DISABLE	~(1U<<1)

#define VBAT_SENSOR_SD2_ENABLE	1U<<2
#define VBAT_SENSOR_SD2_DISABLE	~(1U<<2)

#define VBAT_SENSOR_SD3_ENABLE	1U<<3
#define VBAT_SENSOR_SD3_DISABLE	~(1U<<3)

#define VBAT_SENSOR_SD4_ENABLE	1U<<4
#define VBAT_SENSOR_SD4_DISABLE	~(1U<<4)

#define VBAT_SENSOR_SD5_ENABLE	1U<<5
#define VBAT_SENSOR_SD5_DISABLE	~(1U<<5)

#define VBAT_SENSOR_SD6_ENABLE	1U<<6
#define VBAT_SENSOR_SD6_DISABLE	~(1U<<6)

#define VBAT_SENSOR_SD7_ENABLE	1U<<7
#define VBAT_SENSOR_SD7_DISABLE	~(1U<<7)
/**
  * @}
  */
  
/** @defgroup VBAT_Exported_Functions VBAT Exported Functions
  * @{
  */

/**
  * @brief  Enable or Disable the VBAT Register Write Permission
  * @param[in] NewState: ENABLE OR DISABLE 
  * @retval none
  * @note when call Function PWR_DeInit,VBAT Write ENABLE will be Close
  */
void BAT_WriteEnable(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		PWR_VBAT->BWP = 0x3355AACC;
	}
	else
	{
		PWR_VBAT->BWP = 0;
	}
	
}



/**
  * @brief  Enable or Disable the RTC
  * @param[in] NewState: ENABLE OR DISABLE RTC
  * @retval none
  * @note 
  */
void BAT_RTC_Cmd(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->CON = RTC_ENABLE;
	}
	else
	{
		VBAT_RTC->CON = RTC_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param[in] RTC_InitTypeDef: pointer to a RTC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
__weak void BAT_RTC_StructInit(RTC_InitTypeDef *RTC_InitStruct)
{
	RTC_InitStruct->Sec = 0;
	RTC_InitStruct->Min = 0;
	RTC_InitStruct->Hour = 0;
	RTC_InitStruct->Day = 1;
	RTC_InitStruct->Month = 11;
	RTC_InitStruct->year= 18;
	RTC_InitStruct->Week = 	THURSDAY;
}
/**
  * @brief Initializes the RTC peripheral according to 
  *   the specified parameters in the RTC_InitTypeDef.
  * @param[in] RTC_InitStruct: pointer to a RTC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 

void BAT_RTC_Init(RTC_InitTypeDef *RTC_InitStruct)
{
	assert_param(IS_RTC_WEEK_CHECK(RTC_InitStruct->Week));
	assert_param(IS_RTC_YEAR_CHECK(RTC_InitStruct->year));
	assert_param(IS_RTC_MON_CHECK(RTC_InitStruct->Month));
	assert_param(IS_RTC_DAY_CHECK(RTC_InitStruct->Day));
	assert_param(IS_RTC_HOUR_CHECK(RTC_InitStruct->Hour));
	assert_param(IS_RTC_MIN_CHECK(RTC_InitStruct->Min));
	assert_param(IS_RTC_SEC_CHECK(RTC_InitStruct->Sec));
	BAT_RTC_Cmd(DISABLE);
	VBAT_RTC->SEC = RTC_InitStruct->Sec;
	VBAT_RTC->MIN = RTC_InitStruct->Min;
	VBAT_RTC->HOUR = RTC_InitStruct->Hour;
	VBAT_RTC->DAY = RTC_InitStruct->Day;
	VBAT_RTC->MON = RTC_InitStruct->Month;
	VBAT_RTC->YEAR = RTC_InitStruct->year;
	VBAT_RTC->WEEK = RTC_InitStruct->Week;
	BAT_RTC_Cmd(ENABLE);
}
/**
  * @brief DeInitializes the RTC peripheral 
  * @param[in] none
  * @retval none
  * @note none
  */ 
void BAT_RTC_DeInit(void)
{
	RCC_BATSoftResetCmd(RCC_BAT_RSTEN_RTC);
}

/**
  * @brief  Fills each Alarm_InitStruct member with its default value.
  * @param[in] Alarm_InitStruct: pointer to a RTC_AlarmInitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
__weak void BAT_RTC_AlarmStructInit(RTC_AlarmInitTypeDef *Alarm_InitStruct)
{
	Alarm_InitStruct->Sec = 0;
	Alarm_InitStruct->Min = 1;
	Alarm_InitStruct->Hour = 0;
	Alarm_InitStruct->Day = 1;
	Alarm_InitStruct->Month = 11;
	Alarm_InitStruct->year= 18;
	Alarm_InitStruct->Sec_Alarm = DISABLE;
	Alarm_InitStruct->Min_Alarm = ENABLE;
	Alarm_InitStruct->Hour_Alarm = DISABLE;
	Alarm_InitStruct->Day_Alarm = DISABLE;
	Alarm_InitStruct->Month_Alarm = DISABLE;
	Alarm_InitStruct->year_Alarm = DISABLE;
	//Alarm_InitStruct->Week = THURSDAY;
}

/**
  * @brief Initializes the ADC peripheral according to 
  *   the specified parameters in the RTC_InitTypeDef.
  * @param[in] RTC_InitStruct: pointer to a RTC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 
void BAT_RTC_AlarmInit(RTC_AlarmInitTypeDef *Alarm_InitStruct)
{
	//assert_param(IS_RTC_WEEK_CHECK(Alarm_InitStruct->Week));
	assert_param(IS_RTC_YEAR_CHECK(Alarm_InitStruct->year));
	assert_param(IS_RTC_MON_CHECK(Alarm_InitStruct->Month));
	assert_param(IS_RTC_DAY_CHECK(Alarm_InitStruct->Day));
	assert_param(IS_RTC_HOUR_CHECK(Alarm_InitStruct->Hour));
	assert_param(IS_RTC_MIN_CHECK(Alarm_InitStruct->Min));
	assert_param(IS_RTC_SEC_CHECK(Alarm_InitStruct->Sec));
	//VBAT_RTC->ALMSEC = Alarm_InitStruct->Sec;
	VBAT_RTC->ALMMIN = Alarm_InitStruct->Min;
	VBAT_RTC->ALMHOUR = Alarm_InitStruct->Hour;
	VBAT_RTC->ALMDAY = Alarm_InitStruct->Day;
	VBAT_RTC->ALMMON = Alarm_InitStruct->Month;
	VBAT_RTC->ALMYEAR = Alarm_InitStruct->year;
	BAT_RTC_WeekAlarm(Alarm_InitStruct->Week_Alarm);
	BAT_RTC_YearAlarm(Alarm_InitStruct->year_Alarm);
	BAT_RTC_MonthAlarm(Alarm_InitStruct->Month_Alarm);
	BAT_RTC_DayAlarm(Alarm_InitStruct->Day_Alarm);
	BAT_RTC_MinAlarm(Alarm_InitStruct->Min_Alarm);
	BAT_RTC_SecAlarm(Alarm_InitStruct->Sec_Alarm);
	BAT_RTC_HourAlarm(Alarm_InitStruct->Hour_Alarm);
//	if((Alarm_InitStruct->Week_Alarm == ENABLE)||(Alarm_InitStruct->year_Alarm == ENABLE)||
//	   (Alarm_InitStruct->Month_Alarm == ENABLE)||(Alarm_InitStruct->Day_Alarm == ENABLE)||
//	   (Alarm_InitStruct->Min_Alarm == ENABLE)||(Alarm_InitStruct->Hour_Alarm == ENABLE)||(Alarm_InitStruct->Sec_Alarm == ENABLE))
//	{
//		BAT_RTC_AllAlarmDisable(ENABLE);
//	}
	//VBAT_RTC->ALMWEEK = Alarm_InitStruct->Week;	
}

/**
  * @brief  Disable or Enable RTC Alarm
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_AllAlarm_Cmd(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState ==ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_ALARM_DISABLE;
	}
//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Enable or Disable RTC of Week Alarm 
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_WeekAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_WEEK_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_WEEK_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Enable or Disable RTC of Year Alarm 
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_YearAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_YEAR_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_YEAR_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}


/**
  * @brief  Enable or Disable RTC of Month Alarm 
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_MonthAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_MON_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_MON_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Enable or Disable RTC of Month Alarm 
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_DayAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_DAY_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_DAY_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Enable or Disable RTC of MIN Alarm 
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_HourAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_HOUR_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_HOUR_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}
/**
  * @brief  Enable or Disable RTC of MIN Alarm 
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_RTC_MinAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_MIN_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_MIN_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Enable or Disable RTC of SEC Alarm 
  * @param[in] none.
  * @retval none
  * @note  none
  */
void BAT_RTC_SecAlarm(FunctionalState NewState)
{
	volatile uint32_t temp,i;
	if(NewState == ENABLE)
	{
		VBAT_RTC->ALMCON |= RTC_SEC_ALARM_ENABLE;
	}
	else
	{
		VBAT_RTC->ALMCON &= RTC_SEC_ALARM_DISABLE;
	}
	//由于主电源域启动操作RTC使能，到VBAT的RTC使能，跨时钟域，导致必须等待2个VBAT时钟才能真正启动，所以在此位置增加一个延时操作
	temp = RCC_GetSysClocksFreq();//获取CPU时钟
	for(i=0;i<temp*3/32000;i++);  //由于VBAT的时钟可能存在偏差，所以在此延时等待3个vbat时钟
}

/**
  * @brief  Get the Status of RTC Alarm Status
  * @param[in] none.
  * @retval FlagStatus SET or RESET
  * @note  none
  */
FlagStatus  BAT_RTC_GetAlarmStatus(void)
{
	if(VBAT_RTC->ALM)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief Clear RTC Alarm Status
  * @param[in] none.
  * @retval FlagStatus SET or RESET
  * @note  none
  */
void  BAT_RTC_ClearAlarmStatus(void)
{
	VBAT_RTC->ALM = 1;
}

/**
  * @brief  Get the Status of RTC  Status
  * @param[in] none.
  * @retval FlagStatus SET or RESET
  * @note  none
  */
FlagStatus  BAT_RTC_GetStatus(void)
{
	if(VBAT_RTC->STS)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief Clear RTC Alarm Status
  * @param[in] none.
  * @retval FlagStatus SET or RESET
  * @note  none
  */
void  BAT_RTC_ClearStatus(void)
{
	VBAT_RTC->STS = 1;
}

/**
  * @brief config RTC Adjustment every Day 
  * @param[in] mode: Daily Adjustment mode.
  *		@arg RTC_DAILY_INCREASE					
  *		@arg RTC_DAILY_DECREASE	
  * @param[in] Sec: Daily Adjustment Sec (0s~15s)
  * @retval void
  * @note  none
  */
void  BAT_RTC_configAdjustment(uint32_t Sec,uint32_t mode)
{
	assert_param(IS_RTC_ADJUSTMENT_SEC(Sec));
	assert_param(IS_RTC_ADJUSTMENT_MODE(mode));
	VBAT_RTC->ADJ = 0;
	VBAT_RTC->ADJ = Sec<<4;
	if(mode == RTC_DAILY_INCREASE)
	{
		VBAT_RTC->ADJ|=RTC_DAILY_INCREASE;
	}
	VBAT_RTC->ADJ|=1;
}

/**
  * @brief config RTC Adjustment every Sec 
  * @param[in] value: the Value of Per SEC Adjustment value
  * @param[in] mode: Daily Adjustment mode.
  *		@arg RTC_SEC_INCREASE					
  *		@arg RTC_SEC_DECREASE	
  * @retval void
  * @note  none
  */
void  BAT_RTC_ConfigAdjustmentPerSec(uint32_t value,uint32_t mode)
{
	assert_param(IS_RTC_ADJUSTMENT_MODE(mode));
	VBAT_RTC->REFCLKADJ = 0;
	VBAT_RTC->REFCLKADJ = value<<8;
	if(mode == RTC_DAILY_INCREASE)
	{
		VBAT_RTC->REFCLKADJ|=RTC_DAILY_INCREASE;
	}
	VBAT_RTC->REFCLKADJ|=1;
}

/**
  * @brief Get RTC Current Calendar
  * @param[in] RTC_CurrentValue: pointer to a RTC_InitTypeDef structure,Current Calendar save in RTC_CurrentValue
  * @retval none
  * @note  
  */
void BAT_RTC_GetCalendar(RTC_InitTypeDef *RTC_CurrentValue)
{
	BAT_RTC_ClearStatus();
	while(BAT_RTC_GetStatus() == RESET);
	RTC_CurrentValue->Sec   =  VBAT_RTC->SEC;
	RTC_CurrentValue->Min   =  VBAT_RTC->MIN;
	RTC_CurrentValue->Hour  =  VBAT_RTC->HOUR;
	RTC_CurrentValue->Day   =  VBAT_RTC->DAY;
	RTC_CurrentValue->Month =  VBAT_RTC->MON;
	RTC_CurrentValue->year  =  VBAT_RTC->YEAR;
	RTC_CurrentValue->Week  =  VBAT_RTC->WEEK; 
}

/**
  * @brief  Fills each IWDT_InitTypeDef member with its default value.
  * @param[in] IWDT_InitStruct: pointer to a IWDT_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
__weak void BAT_IWDT_StructInit(IWDT_InitTypeDef *IWDT_InitStruct)
{
	IWDT_InitStruct->InitialValue = 5000;
	IWDT_InitStruct->Prescaler = 0x1F;
	IWDT_InitStruct->RestMode = IWDT_ALARM_NOT_REST_CHIP;
}


/**
  * @brief Initializes the ADC peripheral according to 
  *   the specified parameters in the RTC_InitTypeDef.
  * @param[in] RTC_InitStruct: pointer to a RTC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 

void BAT_IWDT_Init(IWDT_InitTypeDef *IWDT_InitStruct)
{
	assert_param(IS_IWDT_INIT_VALUE_CHECK(IWDT_InitStruct->InitialValue));
	assert_param(IS_IWDT_PRESCALER_CHECK(IWDT_InitStruct->Prescaler));
	assert_param(IS_IWDT_REST_MODE_CHECK(IWDT_InitStruct->RestMode));
	IWDT->CON = IWDT_PROTECT;
	IWDT->VALUL = (uint8_t)IWDT_InitStruct->InitialValue;
	IWDT->VALUH = (uint8_t)(IWDT_InitStruct->InitialValue>>8);
	IWDT->PRE = IWDT_InitStruct->Prescaler;
	if(IWDT_InitStruct->RestMode == IWDT_ALARM_REST_CHIP)
	{
		RCC_WDTResetSystemCmd(RCC_ALARM_RSTEN_IWDT,ENABLE);
	}
	else
	{
		RCC_WDTResetSystemCmd(RCC_ALARM_RSTEN_IWDT,DISABLE);
	}
}

/**
  * @brief DeInitializes the IWDT peripheral 
  * @param[in] none
  * @retval none
  * @note none
  */ 

void BAT_IWDT_DeInit(void)
{
	RCC_BATSoftResetCmd(RCC_BAT_RSTEN_IWDT);
}

/**
  * @brief  Enable or Disable IWDT
  * @param[in] NewState.
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void BAT_IWDT_Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		IWDT->CON = IWDT_LOAD_VALUE;
		IWDT->CON = IWDT_START;
	}
	else
	{
		IWDT->CON = IWDT_STOP;
	}
}
/**
  * @brief  Feed watch dog
  * @param[in]  none
  * @retval none
  * @note	none
  */
void BAT_IWDT_FeedDog(void)
{
	IWDT->CON = IWDT_LOAD_VALUE;
	IWDT->CON = IWDT_START;
}



/**
  * @brief  Write VBAT RAM
  * @param[in]  Data: the Point to Write address
  * @retval none
  * @note	none
  */
void BAT_WriteRam(uint32_t *Data)
{
 	uint8_t i;
	uint32_t *pBackRam=(uint32_t *)BAT_RAM_BASE_ADDR;
	
	for(i = 0;i < 32;i++)
	{
		pBackRam[i] = Data[i];
	}
//	BAT_RAM->BKPMEM1 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM2 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM3 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM4 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM5 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM6 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM7 = *Data;
//	Data++;
//	BAT_RAM->BKPMEM8 = *Data;

}


/**
  * @brief  Read VBAT RAM
  * @param[in]  Data: the Point to read address
  * @retval none
  * @note	none
  */
void BAT_ReadRam(uint32_t *Data)
{
	uint8_t i;
	uint32_t *pBackRam=(uint32_t *)BAT_RAM_BASE_ADDR;
	
	for(i = 0;i < 32;i++)
	{
		Data[i] = pBackRam[i];
	}
//	*Data = BAT_RAM->BKPMEM1;
//	Data++;
//	*Data = BAT_RAM->BKPMEM2;
//	Data++;
//	*Data = BAT_RAM->BKPMEM3;
//	Data++;
//	*Data = BAT_RAM->BKPMEM4;
//	Data++;
//	*Data = BAT_RAM->BKPMEM5;
//	Data++;
//	*Data = BAT_RAM->BKPMEM6;
//	Data++;
//	*Data = BAT_RAM->BKPMEM7;
//	Data++;
//	*Data = BAT_RAM->BKPMEM8;

}

/**
  * @brief Initializes the BAT GPIO peripheral according to 
  *   the specified parameters in the VBAT_GPIO_InitTypeDef.
  * @param[in] GPIO_Init: pointer to a VBAT_GPIO_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 
void BAT_GPIO_Init(VBAT_GPIO_InitTypeDef *GPIO_Init)
{
	volatile uint32_t Temp;
	assert_param(IS_VBAT_GPIO_PINX_CHECK(GPIO_Init->PINx));
	assert_param(IS_VBAT_GPIO_MODE_CHECK(GPIO_Init->Mode));
	assert_param(IS_VBAT_GPIO_SPEED_CHECK(GPIO_Init->Speed));
	assert_param(IS_VBAT_GPIO_PULL_CHECK(GPIO_Init->Pull));
	assert_param(IS_VBAT_GPIO_SMIT_CHECK(GPIO_Init->SMIT));
	assert_param(IS_VBAT_GPIO_INIT_MODE_CHECK(GPIO_Init->INT_MODE));
	for(Temp = 0;Temp<4;Temp++)
	{
		if(GPIO_Init->PINx&(1<<Temp))
		{
			if(GPIO_Init->Mode==GPIO_MODE_IN)
			{
				VBAT_GPIO->MODER&=~(3U<<(Temp*2));
			}
			else
			{
				VBAT_GPIO->MODER&=~(2U<<(Temp*2));
				VBAT_GPIO->MODER|= 1U<<(Temp*2);
			}
			switch(GPIO_Init->Pull)
			{
				case GPIO_NOPULL:VBAT_GPIO->PUPD&=~(3U<<(Temp*2));break;
				case GPIO_PULLUP:VBAT_GPIO->PUPD&=~(2U<<(Temp*2));
								 VBAT_GPIO->PUPD|=(1U<<(Temp*2));
								 break;	
				case GPIO_PULLDOWN:	VBAT_GPIO->PUPD|=(2U<<(Temp*2));
									VBAT_GPIO->PUPD&=~(1U<<(Temp*2));
									break;
				default:break;
			}
			switch(GPIO_Init->Speed)
			{
				case GPIO_LOW_SPEED: 	VBAT_GPIO->OSPEED&=~(3U<<(Temp*2)); break;
				case GPIO_MEDIUM_SPEED:	VBAT_GPIO->OSPEED&=~(2U<<(Temp*2));
										VBAT_GPIO->OSPEED|=(1U<<(Temp*2));
										break;
				case GPIO_FAST_SPEED:	VBAT_GPIO->OSPEED|=(2U<<(Temp*2));
										VBAT_GPIO->OSPEED&=~(1U<<(Temp*2));
										break;
				case GPIO_HIGH_SPEED:	VBAT_GPIO->OSPEED|=(2U<<(Temp*2));
										VBAT_GPIO->OSPEED|=(1U<<(Temp*2));
										break;
				default:break;
			}
			switch(GPIO_Init->SMIT)
			{
				case GPIO_INPUTSCHMIT_DISABLE: 	VBAT_GPIO->SMIT&=~(1U<<Temp);break;
				case GPIO_INPUTSCHMIT_ENABLE:	VBAT_GPIO->SMIT|=1U<<Temp;break;
				default:break;
			}
			switch(GPIO_Init->INT_MODE)
			{
				case BAT_GPIO_INIT_MODE_RASING:VBAT_GPIO->INTC|=3<<(Temp*2);break;
				case BAT_GPIO_INIT_MODE_FALLING:VBAT_GPIO->INTC|=2<<(2*Temp);VBAT_GPIO->INTC&=~(1<<(2*Temp));break;
				case BAT_GPIO_INIT_MODE_HIGH_LEVEL:VBAT_GPIO->INTC&=~(2<<(2*Temp));VBAT_GPIO->INTC|=(1<<(2*Temp));break;
				case BAT_GPIO_INIT_MODE_LOW_LEVEL:VBAT_GPIO->INTC&=~(2<<(2*Temp));VBAT_GPIO->INTC&=~(1<<(2*Temp));break;
				default:break;
			}
			if(GPIO_Init->INT_MSK == BAT_GPIO0_INIT_ENABLE)
			{
				VBAT_GPIO->INTMSK&=~(1<<Temp);
			}
			else if(GPIO_Init->INT_MSK == BAT_GPIO3_INIT_DISABLE)
			{
				VBAT_GPIO->INTMSK |= 1<<Temp;
			}
		}
	}	
}

/**
  * @brief DEInitializes the BAT GPIO peripheral a
  * @param[in] none
  * @retval none
  * @note none
  */ 
void BAT_GPIO_DeInit(void)
{
	RCC_BATSoftResetCmd(RCC_BAT_RSTEN_VBATGPIO);
}

/**
  * @brief config VBAT's GPIO INIT MODE 
  * @param[in] VBAT_GPIO_PINX: 
  *		@arg GPIO_PIN_0
  *		@arg GPIO_PIN_1
  *		@arg GPIO_PIN_2
  *		@arg GPIO_PIN_3
  * @param[in] mode: 
  *		@arg BAT_GPIO_INIT_MODE_RASING
  *		@arg BAT_GPIO_INIT_MODE_FALLING
  *		@arg BAT_GPIO_INIT_MODE_HIGH_LEVEL
  *		@arg BAT_GPIO_INIT_MODE_LOW_LEVEL
  * @retval none
  * @note none
  */ 

void BAT_GPIO_ConfigIT_Mode(uint32_t VBAT_GPIO_PINX,uint32_t mode)
{
	volatile uint32_t Temp =0 ;
	assert_param(IS_VBAT_GPIO_PINX_CHECK(VBAT_GPIO_PINX));
	assert_param(IS_VBAT_GPIO_INIT_MODE_CHECK(mode));
	for(Temp = 0;Temp<4;Temp++)
	{
		if(VBAT_GPIO_PINX&(1<<Temp))
		{
			switch(mode)
			{
				case BAT_GPIO_INIT_MODE_RASING: VBAT_GPIO->INTC|=(0x03U<<(Temp*2));break;
				case BAT_GPIO_INIT_MODE_FALLING:VBAT_GPIO->INTC|=(0x02U<<(Temp*2));VBAT_GPIO->INTC&=~(0x01U<<(Temp*2));break;
				case BAT_GPIO_INIT_MODE_HIGH_LEVEL:VBAT_GPIO->INTC&=~(0x02U<<(Temp*2));VBAT_GPIO->INTC|=(0x01U<<(Temp*2));break;
				case BAT_GPIO_INIT_MODE_LOW_LEVEL:VBAT_GPIO->INTC&=~(0x03U<<(Temp*2));break;
				default:break;
			}
		}
	}
}

/**
  * @brief ENABLE or DISABLE VBAT GPIO Interrupt 
  * @param[in] VBAT_GPIO_PINX: 
  *		@arg GPIO_PIN_0
  *		@arg GPIO_PIN_1
  *		@arg GPIO_PIN_2
  *		@arg GPIO_PIN_3
  * @param[in] NewState: 
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */ 

void BAT_GPIO_ITConfig(uint32_t VBAT_GPIO_PINX,FunctionalState NewState)
{
	assert_param(IS_VBAT_GPIO_PINX_CHECK(VBAT_GPIO_PINX));
	if(NewState == ENABLE)
	{
		VBAT_GPIO->INTMSK &= ~(VBAT_GPIO_PINX);
	}
	else
	{
		VBAT_GPIO->INTMSK |= VBAT_GPIO_PINX;
	}
}

/**
  * @brief  Get Interrupt VBAT GPIO Status
  * @param[in] VBAT_GPIO_PINX: 
  *		@arg GPIO_PIN_0
  *		@arg GPIO_PIN_1
  *		@arg GPIO_PIN_2
  *		@arg GPIO_PIN_3
  * @retval none
  * @note none
  */ 
FlagStatus BAT_GPIO_GetITStatus(uint32_t VBAT_GPIO_PINX)
{
	assert_param(IS_VBAT_GPIO_PINX_CHECK(VBAT_GPIO_PINX));
	if(VBAT_GPIO->INTSTS&VBAT_GPIO_PINX)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clear Interrupt VBAT GPIO Status
  * @param[in] VBAT_GPIO_PINX: 
  *		@arg GPIO_PIN_0
  *		@arg GPIO_PIN_1
  *		@arg GPIO_PIN_2
  *		@arg GPIO_PIN_3
  * @retval none
  * @note none
  */ 
void BAT_GPIO_ClearITStatus(uint32_t VBAT_GPIO_PINX)
{
	assert_param(IS_VBAT_GPIO_PINX_CHECK(VBAT_GPIO_PINX));
	VBAT_GPIO->INTSTS = VBAT_GPIO_PINX;
}

/**
  * @brief  Read BAT GPIO Data,when the GPIO mode is input
  * @param[in] none 
  * @retval uint32_t:
  *		0bit: 0- BAT_GPIO0 Input Low Level
  *			  1- BAT_GPIO0 Input High Level
  *		1bit: 0- BAT_GPIO1 Input Low Level
  *			  1- BAT_GPIO1 Input High Level	
  *		2bit: 0- BAT_GPIO2 Input Low Level
  *			  1- BAT_GPIO2 Input High Level
  *		3bit: 0- BAT_GPIO3 Input Low Level
  *			  1- BAT_GPIO3 Input High Level	
  * @note none
  */ 
uint32_t BAT_GPIO_GetData(void)
{
	return VBAT_GPIO->IDR;
}



/**
  * @brief  Sensor Timer Start.
  * @param[in] NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  none
  */
void VBAT_SensorTimeStart(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		VBAT_SENSOR->SENTIMERCON = VBAT_SENSOR_TIME_OPEN;
	}
	else
	{
		VBAT_SENSOR->SENTIMERCON = VBAT_SENSOR_TIME_CLOSE;
	}
}

/**
  * @brief  Config Sensor Timer Value.
  * @param[in] Value :Timer inital Value
  * @retval none
  * @note  none
  */
void VBAT_ConfigSensorTimeValue(uint16_t Value)
{
	VBAT_SENSOR->SENTIMEREVAL = Value;
}

/**
  * @brief  Get Sensor Timer Status.
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@ARG RESET
  * @note  none
  */
FlagStatus VBAT_GetSensorTimeStatus(void)
{
	if(VBAT_SENSOR->SENTIMESTS)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Get Sensor Timer Status.
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@ARG RESET
  * @note  none
  */
void VBAT_ClearSensorTimeStatus(void)
{
	VBAT_SENSOR->SENTIMESTS = 1;
}



/**
  * @brief  Initializes the VBAT VD peripheral according to 
  *   the specified parameters in the VBAT_VD_InitTypeDef.
  * @param[in] VD_InitStruct: pointer to a VBAT_VD_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
void VBAT_VD_Init(VBAT_VD_InitTypeDef *VD_InitStruct)
{
	volatile uint32_t temp = 0;
	temp |= VD_InitStruct->CheckMode|VD_InitStruct->HighAlarmMode|VD_InitStruct->LowAlarmMode|VD_InitStruct->LowLevelMode;
	VBAT_SENSOR->VBATVDCON = temp;
}


/**
  * @brief Enable or Disable VD High Level Check 
  * @param[in] NewState: enable or disable VD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_VD_HighLevelCmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		VBAT_SENSOR->VBATVDCON |=VBAT_SENSOR_HVD_ENABLE;
	}
	else
	{
		VBAT_SENSOR->VBATVDCON&= VBAT_SENSOR_HVD_DISABLE;
	}
}


/**
  * @brief Enable or Disable VD Low Level Check 
  * @param[in] NewState: enable or disable VD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_VD_LowLevelCmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		VBAT_SENSOR->VBATVDCON |=VBAT_SENSOR_LVD_ENABLE;
	}
	else
	{
		VBAT_SENSOR->VBATVDCON&= VBAT_SENSOR_LVD_DISABLE;
	}
}

/**
  * @brief  Get the Status of VBAT VD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_LEVEL_ALARM
  *		@arg VBAT_LOW_LEVEL_ALARM
  * @retval none
  * @note  
  */
FlagStatus VBAT_VD_GetStatus(uint32_t Flag)
{
	if(VBAT_SENSOR->VBATVDSTS&Flag)
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Clear the Status of VBAT VD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_LEVEL_ALARM
  *		@arg VBAT_LOW_LEVEL_ALARM
  * @retval none
  * @note  
  */
void VBAT_VD_ClearStatus(uint32_t Flag)
{
	VBAT_SENSOR->VBATVDSTS = Flag;
}

/**
  * @brief  Initializes the VBAT TD peripheral according to 
  *   the specified parameters in the VBAT_TD_InitTypeDef.
  * @param[in] TD_InitStruct: pointer to a VBAT_TD_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
void VBAT_TD_Init(VBAT_TD_InitTypeDef *TD_InitStruct)
{
	volatile uint32_t temp = 0;
	temp |= TD_InitStruct->CheckMode|TD_InitStruct->HighTemperatureMode|TD_InitStruct->LowTemperatureMode;
	VBAT_SENSOR->VBATTDCON = temp;
}


/**
  * @brief Enable or Disable TD Check 
  * @param[in] NewState: enable or disable VD
  *		@arg Enable
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void VBAT_TD_Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		VBAT_SENSOR->VBATTDCON |=VBAT_SENSOR_TD_ENABLE;
	}
	else
	{
		VBAT_SENSOR->VBATTDCON&= VBAT_SENSOR_TD_DISABLE;
	}
}

/**
  * @brief  Get the Status of VBAT TD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_TEMPER_ALARM
  *		@arg VBAT_LOW_TEMPER_ALARM
  * @retval none
  * @note  
  */
FlagStatus VBAT_TD_GetStatus(uint32_t Flag)
{
	if(VBAT_SENSOR->VBATTDSTS&Flag)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clear the Status of VBAT TD
  * @param[in] Flag: 
  *		@arg VBAT_HIGH_TEMPER_ALARM
  *		@arg VBAT_LOW_TEMPER_ALARM
  * @retval none
  * @note  
  */
void VBAT_TD_ClearStatus(uint32_t Flag)
{
	VBAT_SENSOR->VBATTDSTS = Flag;
}

/**
  * @brief  Initializes the VBAT SD peripheral according to the specified parameters in the VBAT_SD_InitTypeDef.
  * @param[in] SD_Init: pointer to a VBAT_SD_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
void VBAT_SDx_Init(VBAT_SD_InitTypeDef *SD_Init)
{
	volatile uint8_t buf;
	/*SD0 type choise and config SD0 Alarm mode*/
	if(SD_Init->SD0_ConfigMode ==VBAT_SDX_TYPE_STATIC)
	{
		VBAT_SDx_ConfigType(VBAT_SD0,VBAT_SDX_TYPE_STATIC);
		VBAT_ConfigSDxStaticLevelAlarm(VBAT_SD0,SD_Init->SD0_Alarm_Static);
	}
	else
	{
		VBAT_SDx_ConfigType(VBAT_SD0,VBAT_SDX_TYPE_DYNAMIC);
	}
	/*SD1 type choise and config SD1 Alarm mode*/
	if(SD_Init->SD1_ConfigMode ==VBAT_SDX_TYPE_STATIC)
	{
		VBAT_SDx_ConfigType(VBAT_SD1,VBAT_SDX_TYPE_STATIC);
		VBAT_ConfigSDxStaticLevelAlarm(VBAT_SD1,SD_Init->SD1_Alarm_Static);
	}
	else
	{
		VBAT_SDx_ConfigType(VBAT_SD1,VBAT_SDX_TYPE_DYNAMIC);
	}
	/*SD2 type choise and config SD2 Alarm mode*/
	if(SD_Init->SD2_ConfigMode ==VBAT_SDX_TYPE_STATIC)
	{
		VBAT_SDx_ConfigType(VBAT_SD2,VBAT_SDX_TYPE_STATIC);
		VBAT_ConfigSDxStaticLevelAlarm(VBAT_SD2,SD_Init->SD2_Alarm_Static);
	}
	else
	{
		VBAT_SDx_ConfigType(VBAT_SD2,VBAT_SDX_TYPE_DYNAMIC);
	}
	/*SD3 type choise and config SD3 Alarm mode*/
	if(SD_Init->SD3_ConfigMode ==VBAT_SDX_TYPE_STATIC)
	{
		VBAT_SDx_ConfigType(VBAT_SD3,VBAT_SDX_TYPE_STATIC);
		VBAT_ConfigSDxStaticLevelAlarm(VBAT_SD3,SD_Init->SD3_Alarm_Static);
	}
	else
	{
		VBAT_SDx_ConfigType(VBAT_SD3,VBAT_SDX_TYPE_DYNAMIC);
	}
	/*config SD5 SD4 Alarm mode*/
	VBAT_ConfigSDxStaticLevelAlarm(VBAT_SD4,SD_Init->SD4_Alarm_Static);
	VBAT_ConfigSDxStaticLevelAlarm(VBAT_SD5,SD_Init->SD5_Alarm_Static);
	/*config Dynamic and staic RoundsAndDelayTime, FaultTolerance*/
	VBAT_SDx_ConfigDynamicRoundsAndDelayTime(SD_Init->Dynamic_Init.Round,SD_Init->Dynamic_Init.ReceiveDelay);
	VBAT_SDx_ConfigDynamicFaultTolerance(SD_Init->Dynamic_Init.TotalNumber,SD_Init->Dynamic_Init.FaultNumber,ENABLE);
	VBAT_SDx_ConfigStaticFaultTolerance(SD_Init->Static_Init.TotalNumber,SD_Init->Static_Init.FaultNumber, ENABLE);
	/*Config SD Static Alarm and Dynamic Alarm clear BuckUpreg*/
	VBAT_SD_CLearBuckUpReg(SD_Init->Static_Init.ClearBuckUpReg,SD_Init->Dynamic_Init.ClearBuckUpReg);
	pkeGetRandomNum((uint8_t*)(&buf),1);
	/*Update dynamic pseudo random number speed*/
	VBAT_SDx_WriteSeed(buf);
}

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
void VBAT_SDx_Cmd(uint32_t SDx, FunctionalState NewState)
{
	
	if(NewState == ENABLE)
	{
		if(SDx&VBAT_SD0)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD0_ENABLE;
		if(SDx&VBAT_SD1)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD1_ENABLE;
		if(SDx&VBAT_SD2)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD2_ENABLE;
		if(SDx&VBAT_SD3)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD3_ENABLE;
		if(SDx&VBAT_SD4)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD4_ENABLE;
		if(SDx&VBAT_SD5)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD5_ENABLE;
		if(SDx&VBAT_SD6)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD6_ENABLE;
		if(SDx&VBAT_SD7)
            VBAT_SENSOR->VBATSDCON|=VBAT_SENSOR_SD7_ENABLE;
	}
	else
	{
		if(SDx&VBAT_SD0)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD0_DISABLE;
		if(SDx&VBAT_SD1)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD1_DISABLE;
		if(SDx&VBAT_SD2)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD2_DISABLE;
		if(SDx&VBAT_SD3)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD3_DISABLE;
		if(SDx&VBAT_SD4)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD4_DISABLE;
		if(SDx&VBAT_SD5)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD5_DISABLE;
		if(SDx&VBAT_SD6)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD6_DISABLE;
		if(SDx&VBAT_SD7)
            VBAT_SENSOR->VBATSDCON&=VBAT_SENSOR_SD7_DISABLE;
	}
}

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
void VBAT_ConfigSDxStaticLevelAlarm(uint32_t SDx, uint32_t AlarmLevel)
{
	if(AlarmLevel&VBAT_SDx_ALARM_LEVEL_IS_LOW)
	{
		VBAT_SENSOR->VBATSDLV|=(SDx);
	}
	else
	{
		VBAT_SENSOR->VBATSDLV&=~(SDx);
	}	
}

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
FlagStatus VBAT_SDx_GetAlarmStatus(uint32_t SDx)
{
	if(VBAT_SENSOR->VBATSDSTS&SDx)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clear the Status of VBAT SDx
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
void VBAT_SDx_ClearAlarmStatus(uint32_t SDx)
{
	VBAT_SENSOR->VBATSDSTS = SDx;
}

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
void VBAT_SDx_ConfigType(uint32_t SDx,uint32_t Type)
{
	if(SDx&VBAT_SD0)
	{
		if(Type == VBAT_SDX_TYPE_STATIC)
		{
			VBAT_SENSOR->VBATSDTYPE |= VBAT_SDX_TYPE_STATIC;
		}
		else if(Type == VBAT_SDX_TYPE_DYNAMIC)
		{
			VBAT_SENSOR->VBATSDTYPE &= (~VBAT_SDX_TYPE_STATIC);
		}
	}
	if(SDx&VBAT_SD1)
	{
		if(Type == VBAT_SDX_TYPE_STATIC)
		{
			VBAT_SENSOR->VBATSDTYPE |= VBAT_SDX_TYPE_STATIC<<1;
		}
		else if(Type == VBAT_SDX_TYPE_DYNAMIC)
		{
			VBAT_SENSOR->VBATSDTYPE &= (~(VBAT_SDX_TYPE_STATIC<<1));
		}
	}
	if(SDx&VBAT_SD2)
	{
		if(Type == VBAT_SDX_TYPE_STATIC)
		{
			VBAT_SENSOR->VBATSDTYPE |= VBAT_SDX_TYPE_STATIC<<2;
		}
		else if(Type == VBAT_SDX_TYPE_DYNAMIC)
		{
			VBAT_SENSOR->VBATSDTYPE &= (~(VBAT_SDX_TYPE_STATIC<<2));
		}
	}
	if(SDx&VBAT_SD3)
	{
		if(Type == VBAT_SDX_TYPE_STATIC)
		{
			VBAT_SENSOR->VBATSDTYPE |= VBAT_SDX_TYPE_STATIC<<3;
		}
		else if(Type == VBAT_SDX_TYPE_DYNAMIC)
		{
			VBAT_SENSOR->VBATSDTYPE &= (~(VBAT_SDX_TYPE_STATIC<<3));
		}
	}
}


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
void VBAT_SDx_ConfigDynamicRoundsAndDelayTime(uint8_t Round,uint8_t TimeOut)
{
	volatile uint32_t temp = 0;
	if(Round>7)
	{
		return;
	}
	temp|= Round<<8;
	temp|= TimeOut;
	VBAT_SENSOR->VBATSDROUND = temp;
}

/**
  * @brief  Write Dynamic Detection of Random Number Seeds
  * @param[in] Seed: Dynamic Detection of Random Number Seeds(the Range of Seed is 0-0xFF)	
  * @retval none
  * @note  none
  */
void VBAT_SDx_WriteSeed(uint8_t Seed)
{
	VBAT_SENSOR->VBATSDSEED = (uint32_t)Seed;
}

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
void VBAT_SDx_ConfigStaticFaultTolerance(uint8_t TotalNumber ,uint8_t FaultNumber, FunctionalState NewState)
{
	volatile uint32_t temp = 0;
	if(TotalNumber>0x0f)
	{
		return;
	}
	if(FaultNumber>0x07)
	{
		return;
	}
	if(NewState == ENABLE)
	{
		temp |= TotalNumber<<4;
		temp |= FaultNumber<<1;
		temp |= 1;
		VBAT_SENSOR->VBATSDSTATOL = temp;
	}
	else
	{
		VBAT_SENSOR->VBATSDSTATOL &= ~(1U);
	}
}

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
void VBAT_SDx_ConfigDynamicFaultTolerance(uint8_t TotalNumber ,uint8_t FaultNumber, FunctionalState NewState)
{
	volatile uint32_t temp = 0;
	if(TotalNumber>0x0f)
	{
		return;
	}
	if(FaultNumber>0x07)
	{
		return;
	}
	if(NewState == ENABLE)
	{
		temp |= TotalNumber<<4;
		temp |= FaultNumber<<1;
		temp |= 1;
		VBAT_SENSOR->VBATSDDYNTOL = temp;
	}
	else
	{
		VBAT_SENSOR->VBATSDDYNTOL &= ~(1U);
	}
}

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
void VBAT_SD_CLearBuckUpReg(uint32_t StaticConfig,uint32_t DynamicConfig)
{
	VBAT_SENSOR->VBATSDCON|=StaticConfig|DynamicConfig;
}

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
FlagStatus MAIN_SD_GetAlarmStatus(uint32_t Flag)
{
	if(VBAT_SENSOR->VBATSDMSTS&Flag)
	{
		return SET;
	}
	return RESET;
}

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
void MAIN_SD_ConfigIT(uint32_t IT,FunctionalState NewState)
{
	if(NewState==ENABLE)
	{
		if(IT == MAIN_SD_IT_STATIC)
		VBAT_SENSOR->VBATSDINTMSK&=~(MAIN_SD_IT_STATIC); 
		if(IT == MAIN_SD_IT_DYNAMIC)
		VBAT_SENSOR->VBATSDINTMSK&=~(MAIN_SD_IT_DYNAMIC);
	}
	else
	{
		if(IT == MAIN_SD_IT_STATIC)
		VBAT_SENSOR->VBATSDINTMSK|=(MAIN_SD_IT_STATIC); 
		if(IT == MAIN_SD_IT_DYNAMIC)
		VBAT_SENSOR->VBATSDINTMSK|=(MAIN_SD_IT_DYNAMIC);
	}
}

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
FlagStatus MAIN_TD_GetAlarmStatus(uint32_t Flag)
{
	if(VBAT_SENSOR->VBATTDMSTS&Flag)
	{
		return SET;
	}
	return RESET;
}

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
void MAIN_TD_ConfigIT(uint32_t IT,FunctionalState NewState)
{
	if(NewState==ENABLE)
	{
		if(IT & MAIN_TD_IT_LOW_TEMPERATURE)
		VBAT_SENSOR->VBATTDINTMSK&=~(MAIN_TD_IT_LOW_TEMPERATURE); 
		if(IT & MAIN_TD_IT_HIGH_TEMPERATURE)
		VBAT_SENSOR->VBATTDINTMSK&=~(MAIN_TD_IT_HIGH_TEMPERATURE);
	}
	else
	{
		if(IT & MAIN_TD_IT_LOW_TEMPERATURE)
		VBAT_SENSOR->VBATTDINTMSK|=(MAIN_TD_IT_LOW_TEMPERATURE); 
		if(IT & MAIN_TD_IT_HIGH_TEMPERATURE)
		VBAT_SENSOR->VBATTDINTMSK|=(MAIN_TD_IT_HIGH_TEMPERATURE);
	}
}

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
FlagStatus MAIN_VD_GetAlarmStatus(uint32_t Flag)
{
	if(VBAT_SENSOR->VBATVDMSTS&Flag)
	{
		return SET;
	}
	return RESET;
}

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
void MAIN_VD_ConfigIT(uint32_t IT,FunctionalState NewState)
{
	if(NewState==ENABLE)
	{
		if(IT &MAIN_VD_IT_LOW_LEVEL)
		VBAT_SENSOR->VBATVDINTMSK&=~(MAIN_VD_IT_LOW_LEVEL); 
		if(IT & MAIN_VD_IT_HIGH_LEVEL)
		VBAT_SENSOR->VBATVDINTMSK&=~(MAIN_VD_FLAG_HIGH_LEVEL);
	}
	else
	{
		if(IT & MAIN_VD_IT_HIGH_LEVEL)
		VBAT_SENSOR->VBATVDINTMSK|=(MAIN_VD_IT_LOW_LEVEL); 
		if(IT & MAIN_VD_IT_HIGH_LEVEL)
		VBAT_SENSOR->VBATVDINTMSK|=(MAIN_VD_FLAG_HIGH_LEVEL);
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
/************************ (C) COPYRIGHT TMC *****END OF FILE***********************/

