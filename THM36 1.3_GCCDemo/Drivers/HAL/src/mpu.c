/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mpu.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "mpu.h"
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
/** @defgroup MPU_private_defines MPU private defines
  * @{
  */
#define MPU_SYS_MODE	0
#define MPU_APP_MODE	1UL

#define MPU_START		1UL
#define MPU_STOP		(~1UL)
/**
  * @}
  */
/** @defgroup MPU_exported_function MPU exported function
  * @{  
  */
  
/**
  * @brief  Fills each MPU_InitStruct member with its default value.
  * @param[in] ADC_InitStruct: pointer to a MPU_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
__weak void MPU_StructInit(MPU_InitTypeDef *MPU_InitStruct)
{
	MPU_InitStruct->MPU_Region0_Access = MPU_SYS_RW_APP_RW;
	MPU_InitStruct->MPU_Region0_End_Add = 0;
	MPU_InitStruct->MPU_Region0_Strat_Add = 0;
	
	MPU_InitStruct->MPU_Region1_Access = MPU_SYS_RW_APP_RW;
	MPU_InitStruct->MPU_Region1_End_Add = 0;
	MPU_InitStruct->MPU_Region1_Strat_Add = 0;
	
	MPU_InitStruct->MPU_Region2_Access = MPU_SYS_RW_APP_RW;
	MPU_InitStruct->MPU_Region2_End_Add = 0;
	MPU_InitStruct->MPU_Region2_Strat_Add = 0;
	
	MPU_InitStruct->MPU_Region3_Access = MPU_SYS_RW_APP_RW;
	MPU_InitStruct->MPU_Region3_End_Add = 0;
	MPU_InitStruct->MPU_Region3_Strat_Add = 0;
}

/**
  * @brief  config into the mode of systerm
  * @param[in] none
  * @retval none
  * @note if MPU mode into systerm,while make a NMI innterrupt, must CLR TMC_MPU->MODECON2 in NMI_IRQHandle
  */  
void MPU_ConfigIntoSystermMode(void)
{
	if(TMC_MPU->STS&MPU_APP_MODE)
	{
		TMC_MPU->MODECON2 = 1;
	}
}

/**
  * @brief  config into the mode of APP 
  * @param[in] none
  * @retval none
  * @note none
  */ 
void MPU_ConfigIntoAppMode(void)
{
	if((TMC_MPU->STS&MPU_APP_MODE)== 0)
	{
		TMC_MPU->MODECON1 = 1;
	}
}

/**
  * @brief  ENABLE or DISABLE Region0 
  * @param[in] none
  * @retval none
  * @note none
  */ 
void MPU_Region0Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TMC_MPU->R0STRAT|= MPU_START;
	}
	else
	{
		TMC_MPU->R0STRAT&= MPU_STOP;
	}
}

/**
  * @brief  ENABLE or DISABLE Region1 
  * @param[in] none
  * @retval none
  * @note none
  */ 
void MPU_Region1Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TMC_MPU->R1STRAT|= MPU_START;
	}
	else
	{
		TMC_MPU->R1STRAT&= MPU_STOP;
	}
}

/**
  * @brief  ENABLE or DISABLE Region2 
  * @param[in] none
  * @retval none
  * @note none
  */ 
void MPU_Region2Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TMC_MPU->R2STRAT|= MPU_START;
	}
	else
	{
		TMC_MPU->R2STRAT&= MPU_STOP;
	}
}

/**
  * @brief  ENABLE or DISABLE Region3
  * @param[in] none
  * @retval none
  * @note none
  */ 
void MPU_Region3Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TMC_MPU->R2STRAT|= MPU_START;
	}
	else
	{
		TMC_MPU->R2STRAT&= MPU_STOP;
	}
}

/**
  * @brief  Initial MPU
  * @param[in]  MPU_InitStruct pointer to a MPU_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */ 
void MPU_Initial(MPU_InitTypeDef *MPU_InitStruct) 
{
	MPU_ConfigIntoSystermMode();
	while(TMC_MPU->STS&MPU_APP_MODE);//Wait sys mode.
	/*Disable MPU before set*/
	MPU_Region0Cmd(DISABLE);
	MPU_Region1Cmd(DISABLE);
	MPU_Region2Cmd(DISABLE);
	MPU_Region3Cmd(DISABLE);
	TMC_MPU->R0STRAT  = MPU_InitStruct->MPU_Region0_Strat_Add;
	TMC_MPU->R0End = MPU_InitStruct->MPU_Region0_End_Add;
	TMC_MPU->R1STRAT  = MPU_InitStruct->MPU_Region1_Strat_Add;
	TMC_MPU->R1End = MPU_InitStruct->MPU_Region1_End_Add;
	TMC_MPU->R2STRAT  = MPU_InitStruct->MPU_Region2_Strat_Add;
	TMC_MPU->R2End = MPU_InitStruct->MPU_Region2_End_Add;
	TMC_MPU->R3STRAT  = MPU_InitStruct->MPU_Region3_Strat_Add;
	TMC_MPU->R3End = MPU_InitStruct->MPU_Region3_End_Add;
	TMC_MPU->AP = 0;
	TMC_MPU->AP = (MPU_InitStruct->MPU_Region0_Access)|(MPU_InitStruct->MPU_Region1_Access<<4)| \
	(MPU_InitStruct->MPU_Region2_Access<<8)|(MPU_InitStruct->MPU_Region3_Access<<12);
	MPU_Region0Cmd(ENABLE);
	MPU_Region1Cmd(ENABLE);
	MPU_Region2Cmd(ENABLE);
	MPU_Region3Cmd(ENABLE);
	MPU_ConfigIntoAppMode();
	while(!(TMC_MPU->STS&MPU_APP_MODE));
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


