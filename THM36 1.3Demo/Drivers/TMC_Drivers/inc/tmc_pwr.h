/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_pwr.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMC_TIM_H
#define __TMC_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "PWR.h"
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_TIM
  * @brief TIM TMC modules driver 
  * @{
  */
/** @defgroup TMC_TIM_exported_Types TIM exported types
  * @{
  */
/** 
  * @brief PWR_RAM_RANG PWR RAM RANG
  */
#define SRAM_RANGE5PD   (1UL<<21) //0x2003C000 - 0x20040000 
#define SRAM_RANGE4PD	(1UL<<20) //0x20034000 - 0x2003C000 
#define SRAM_RANGE3PD	(1UL<<19) //0x20030000 - 0x20034000
#define SRAM_RANGE2PD	(1UL<<18) //0x20028000 - 0x20030000
#define SRAM_RANGE1PD	(1UL<<17) //0x20018000 - 0x20028000
#define SRAM_RANGE0PD	(1UL<<16) //0x20000000 - 0x20018000
/**
  * @}
  */
/**
  * @}
  */
/** @addtogroup TMC_PWR_Exported_Function TMC PWR Exported Function
  * @{ 
  */
ErrorStatus TMC_PWR_EnterStopMode(uint32_t Sotp_Mode,uint8_t Cpu_Mode,uint32_t RamRang);
ErrorStatus TMC_PWR_EnterStandByMode(void);
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

#endif /* __thm36_13_TMC_PWR_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

