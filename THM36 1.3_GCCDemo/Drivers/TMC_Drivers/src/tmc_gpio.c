/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_gpio.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_gpio.h"
#include "rcc.h"

/** @addtogroup thm36_1.3
  * @{
  */
  
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */

/** @defgroup TMC_GPIO 
  * @brief GPIO TMC modules driver
  * @{
  */ 
/* Exported functions ---------------------------------------------------------*/
/** @defgroup TMC_GPIO_Exported_Functions TMC GPIO Exported Functions
  * @{
  */
  
/** @defgroup TMC_GPIO_Exported_Functions_Group1 TMC GPIO Initialization and de-initialization functions 
  *  @brief	Initialization and Configuration functions 
  *
@verbatim	
 ===============================================================================
			  ##### Initialization and de-initialization functions #####
 ===============================================================================
  [..]
	This section provides functions allowing to initialize and de-initialize the GPIOs
	to be ready for use.

@endverbatim
  * @{
  */ 

/**
  * @brief	 Clear the Struct of GPIOx_Init
  * @param[in] GPIO_Init: pointer to a GPIO_InitTypeDef structure that contains
  *			the configuration information for the specified GPIO peripheral.
  * @retval	None
  */
void TMC_GPIO_StructInit(GPIO_InitTypeDef* GPIOx_Init)
{		
	GPIOx_Init->Mode = GPIO_MODE_IN;
	GPIOx_Init->Otype = 0;
	GPIOx_Init->Pin= 0;
	GPIOx_Init->Pull = GPIO_PULLUP;
	GPIOx_Init->SMIT = 1;
	GPIOx_Init->Speed = 0;
}
/**
  * @brief	 Initializes the TMC GPIOx peripheral.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @param[in] GPIO_Init: pointer to a GPIO_InitTypeDef structure that contains
  *			the configuration information for the specified GPIO peripheral.
  * @retval	None
  */
void TMC_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIOx_Init)
{		
	GPIO_Init(GPIOx, GPIOx_Init);
}


/**
  * @brief	 De-initializes the GPIOx peripheral registers to their default reset values.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 devices
  * @retval	None
  */
void TMC_GPIO_DeInit(GPIO_TypeDef* GPIOx)
{	
	GPIO_DeInit(GPIOx);			 
}

/**
  * @}
  */

/** @defgroup TMC_GPIO_Exported_Functions_Group2 TMC GPIO operation functions 
  *  @brief GPIO Read and Write
  *
@verbatim   
  ==============================================================================
					  ##### TMC GPIO operation functions #####
  ==============================================================================  
	  
@endverbatim
  * @{
  */	
 
/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @param  GPIO_Pin specifies the port bit to read.
  *		 This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval The input port pin value.
  */
GPIO_PinState TMC_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{	
	if(Bit_RESET == GPIO_ReadInputDataBit(GPIOx, GPIO_Pin))
		return GPIO_PIN_RESET;
	else	
		return GPIO_PIN_SET;
}

/**
  * @brief  Sets or clears the selected data port bit.
  *
  * @note   This function uses GPIOx_BSRR register to allow atomic read/modify
  *		 accesses. In this way, there is no risk of an IRQ occurring between
  *		 the read and the modify access.
  *
  * @param  GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @param  GPIO_Pin specifies the port bit to be written.
  *		  This parameter can be one of GPIO_PIN_x where x can be (0..15).
  * @param  PinState specifies the value to be written to the selected bit.
  *		  This parameter can be one of the GPIO_PinState enum values:
  *			@arg GPIO_PIN_RESET: to clear the port pin
  *			@arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void TMC_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState Pin_State)
{	
	if(Pin_State != GPIO_PIN_RESET)
		GPIO_SetBits(GPIOx, GPIO_Pin);
	else 
		GPIO_ResetBits(GPIOx, GPIO_Pin);
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
/************************ (C) COPYRIGHT TMC *****END OF FILE****/


