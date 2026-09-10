/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mpu.h 
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MPU_H
#define _MPU_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup MPU
  * @brief MPU HAL modules driver 
  * @{
  */ 
/** @defgroup MPU_Exported_Constants MPU Exported Constants
  * @{
  */
/** 
  * @brief  MPU Init structure definition
  * @note   This sturcture is used with  MPU   
  */	 
typedef struct
{
	uint32_t MPU_Region0_Strat_Add;
	uint32_t MPU_Region0_End_Add;	
	uint32_t MPU_Region1_Strat_Add;
	uint32_t MPU_Region1_End_Add;
	uint32_t MPU_Region2_Strat_Add;
	uint32_t MPU_Region2_End_Add;	
	uint32_t MPU_Region3_Strat_Add;
	uint32_t MPU_Region3_End_Add;		
	uint32_t MPU_Region0_Access;
	uint32_t MPU_Region1_Access;
	uint32_t MPU_Region2_Access;
	uint32_t MPU_Region3_Access;
} MPU_InitTypeDef;


/** 
  * @brief MPU_Access_Config MPU Access Config
  */
#define	MPU_SYS_RW_APP_RW					0
#define	MPU_SYS_RW_APP_R					1UL
#define	MPU_SYS_RW_APP_NO_ACCESS			2UL
#define	MPU_SYS_NO_ACCESS_APP_NO_ACCESS		3UL
/**
  * @}
  */

/**
  * @}
  */
/** @addtogroup MPU_Exported_Function 
  * @{  
  */   

void MPU_StructInit(MPU_InitTypeDef *MPU_InitStruct);
void MPU_ConfigIntoSystermMode(void);
void MPU_ConfigIntoAppMode(void);
void MPU_Region0Cmd(FunctionalState NewState);
void MPU_Region1Cmd(FunctionalState NewState);
void MPU_Region2Cmd(FunctionalState NewState);
void MPU_Region3Cmd(FunctionalState NewState);
void MPU_Initial(MPU_InitTypeDef *MPU_InitStruct);


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
#endif /* __TMC_MPU_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
