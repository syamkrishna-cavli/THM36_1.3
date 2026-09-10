/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwr.h 
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PWR_H
#define _PWR_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup PWR
  * @brief PWR HAL modules driver 
  * @{
  */ 
/** @defgroup PWR_Exported_Constants PWR Exported Constants
  * @{
  */

/** @defgroup PWR_WAKE_UP_Mode PWR WAKE UP Mode
  * @{
  */
#define IWDT_WAKE_UP					(1UL<<7)
#define RTC_WAKE_UP						(1UL<<6)
	 
#define	BAT_IO2_RISING_WAKE_UP			((1UL<<3)|(1UL<<4)&(~(1UL<<5)))
#define	BAT_IO2_FALLING_WAKE_UP			((~(1UL<<3))|(1UL<<4)&(~(1UL<<5)))
#define	BAT_IO2_HIGH_LEVEL_WAKE_UP		((1UL<<3)|(~(1UL<<4))&(~(1UL<<5)))
#define	BAT_IO2_LOW_LEVEL_WAKE_UP		((~(1UL<<3))&(~(1UL<<4))&(~(1UL<<5)))
#define BAT_IO2_NOT_WAKE_UP				((1UL<<3)|(1UL<<4)|(1UL<<5))

#define	BAT_IO1_RISING_WAKE_UP			((1UL<<0)|(1UL<<1)&(~(1UL<<2)))
#define	BAT_IO1_FALLING_WAKE_UP			((~(1UL<<0))|(1UL<<1)&(~(1UL<<2)))
#define	BAT_IO1_HIGH_LEVEL_WAKE_UP		((1UL<<0)|(~(1UL<<1))&(~(1UL<<2)))
#define	BAT_IO1_LOW_LEVEL_WAKE_UP		((~(1UL<<0))&(~(1UL<<1))&(~(1UL<<2)))
#define BAT_IO1_NOT_WAKE_UP				((1UL<<0)|(1UL<<1)|(1UL<<2))
/**
  * @}
  */

/** @defgroup PWR_WAKE_UP_Source PWR WAKE UP Source
  * @{
  */

#define PWR_STS_BAT_READY				(1UL<<6)		/*BAT Ready Status*/
#define PWR_STS_SAND_BY					(1UL<<7)		/*when CHIP wake up,whether into SANDBY*/
#define PWR_STS_NREST					(1UL<<4)		/*NRST WAKE UP CHIP*/
#define PWR_STS_IWDT_ALM				(1UL<<3)		/*IWDT WAKE UP CHIP*/
#define PWR_STS_RTC_ALM					(1UL<<2)		/*RTC WAKE UP CHIP*/
#define PWR_STS_WAKE_UP_IO2_ALM			(1UL<<1)		/*WAKE_UP_IO2 WAKE UP CHIP*/
#define PWR_STS_WAKE_UP_IO1_ALM			(1UL)			/*WAKE_UP_IO1 WAKE UP CHIP*/

/**
  * @}
  */

/** @defgroup PWR_STOP_MODE_CONFIG_MODULE PWR STOP MODE CONFIG MODULE
  * @{
  */
#define PWR_STOP_MODE_HSI_PWR_DOWN		(1UL<<10)
#define PWR_STOP_MODE_FLASH_PWR_DOWN	(1UL<<9)
#define PWR_STOP_MODE_LDO_PWR_DOWN		(1UL<<8)

/**
  * @}
  */
  
/** @defgroup PWR_STOP_PWOE_DWON PWR STOP PWOE DWON
  * @{
  */
#define PWOR_DWON_SOFTWARE	(1UL<<31)
#define PWOR_DWON_HARDWARE	~(1UL<<31)

/**
  * @}
  */
  
/** @defgroup PWR_PWOR_DOWN_Mode  PWR PWOR DOWN Mode
  * @{
  */
#define PWOR_DWON_STOP_MODE			~(0x03UL)
#define PWOR_DWON_SANDBY_MODE		0x01UL

/**
  * @}
  */  
  
  
/** @defgroup PWR_Idle_Mode PWR Idle Mode  
  * @{
  */ 
#define PWR_STOPEntry_WFI			   ((uint8_t)0x01)
#define PWR_STOPEntry_WFE			   ((uint8_t)0x02)

/**
  * @}
  */

/** @defgroup PWR_Private_Macros PWR Private Macros
  * @{
  */
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPEntry_WFI) || ((ENTRY) == PWR_STOPEntry_WFE))

#define IS_PWR_POWER_UP_MODE(x)	 ((x == IWDT_WAKE_UP)||(x == RTC_WAKE_UP)||(x == BAT_IO2_RISING_WAKE_UP) || \
								  (x == BAT_IO2_FALLING_WAKE_UP)||(x == BAT_IO2_HIGH_LEVEL_WAKE_UP)||(x == BAT_IO2_LOW_LEVEL_WAKE_UP) || \
								  (x == BAT_IO2_NOT_WAKE_UP)||(x == BAT_IO1_RISING_WAKE_UP)||(x == BAT_IO1_FALLING_WAKE_UP) || \
								  (x == BAT_IO1_HIGH_LEVEL_WAKE_UP)||(x == BAT_IO1_LOW_LEVEL_WAKE_UP)||(x == BAT_IO1_NOT_WAKE_UP))
								  
#define IS_PWR_POWER_UP(x)	   ((x == PWR_STS_BAT_READY) || (x == PWR_STS_SAND_BY) || (x == PWR_STS_NREST)|| \
								  (x == PWR_STS_IWDT_ALM) || (x == PWR_STS_RTC_ALM) || (x == PWR_STS_WAKE_UP_IO2_ALM)|| \
								  (x == PWR_STS_WAKE_UP_IO1_ALM))
								  
#define IS_PWR_STOP_MODE(x)	  ((x == PWR_STOP_MODE_HSI_PWR_DOWN) || (x == PWR_STOP_MODE_FLASH_PWR_DOWN)||(x == PWR_STOP_MODE_LDO_PWR_DOWN)||\
								  (x == PWR_STOP_MODE_HSI_PWR_DOWN|PWR_STOP_MODE_FLASH_PWR_DOWN) ||\
								  (x == PWR_STOP_MODE_HSI_PWR_DOWN|PWR_STOP_MODE_LDO_PWR_DOWN) ||\
								  (x == PWR_STOP_MODE_FLASH_PWR_DOWN|PWR_STOP_MODE_LDO_PWR_DOWN) ||\
								  (x == PWR_STOP_MODE_HSI_PWR_DOWN|PWR_STOP_MODE_FLASH_PWR_DOWN|PWR_STOP_MODE_LDO_PWR_DOWN))
								  
								  
#define IS_PWR_RAMX_MODE(x)	  ((x & (~(PWR_STOP_MODE_RAMRANGE0_DOWN|PWR_STOP_MODE_RAMRANGE1_DOWN|PWR_STOP_MODE_RAMRANGE2_DOWN|PWR_STOP_MODE_RAMRANGE3_DOWN|PWR_STOP_MODE_RAMRANGE4_DOWN|PWR_STOP_MODE_RAMRANGE5_DOWN))) == 0 )
#define IS_PWR_MODE(x)			 ((x == PWOR_DWON_SOFTWARE)||(x == PWOR_DWON_HARDWARE))
/**
  * @}
  */

  
/** @addtogroup PWR_Exported_Function 
  * @{  
  */   
void PWR_DeInit(void);
void PWR_ConfigStandbyWakeUpSource(uint32_t Source);
FlagStatus PWR_GetStandbyStatus(uint32_t flag);
void PWR_ClearStandbyStatus(uint32_t flag);
void PWR_ConfigModuleStateInStopMode(uint32_t Mode,FunctionalState NewState);
void PWR_ConfigRAMPowerDownRange(uint32_t Mode,FunctionalState NewState);
FlagStatus PWR_GetRAMPowerDownFlag(uint32_t Flag);
FlagStatus PWR_GetRAMPowerState(uint32_t State);
void PWR_EnterStopMode(uint8_t StopEntry);
void PWR_EnterStandbyMode(void);
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
/**
  * @}
  */
#ifdef __cplusplus
}
#endif
#endif /* __TMC_PWR_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
