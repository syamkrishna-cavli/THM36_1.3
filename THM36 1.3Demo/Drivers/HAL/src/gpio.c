/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: gpio.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/

  
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"

#include "RCC.h"
#include "GPIO.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */

/** @defgroup GPIO 
  * @brief GPIO HAL modules driver
  * @{
  */
  
 /* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup GPIO_Private_Constants GPIO Private Constants
  * @{
  */
  
 /** @defgroup GPIO_Reigster_RSTVAL  GPIO register reset value
  * @brief define gpio default reset register value
  * @{
  */ 
/* ---------------------- GPIO registers reset value **************** ------------------------ */
#define GPIO_MODER_RSTVAL		(0x00000000UL)		/* GPIO_MODER reigster reset value */
#define GPIO_ODEN_RSTVAL		(0x00000000UL)		/* GPIO_ODEN reigster reset value */
#define GPIO_OSPEEDR_RSTVAL		(0x00000000UL)		/* GPIO_OSPEEDR reigster reset value */
#define GPIO_PUPDR_RSTVAL		(0x55555555UL)		/* GPIO_PUPDR reigster reset value */
#define GPIO_IDR_RSTVAL			(0x0000FFFFUL)		/* GPIO_IDR reigster reset value */
#define GPIO_ODR_RSTVAL			(0x00000000UL)		/* GPIO_ODR reigster reset value */
#define GPIO_BSRR_RSTVAL		(0x00000000UL)		/* GPIO_BSRRR reigster reset value */
#define GPIO_AFR_RSTVAL			(0x00000000UL)		/* GPIO_AFR reigster reset value */
#define GPIO_SMIT_RSTVAL		(0x0000FFFFUL)		/* GPIO_SMITR reigster reset value */
#define GPIO_INTC_RSTVAL		(0x00000000UL)		/* GPIO_INTC reigster reset value */
#define GPIO_INTMSK_RSTVAL		(0x0000FFFFUL)		/* GPIO_INTMSK reigster reset value */
#define GPIO_INTSTS_RSTVAL		(0x00000000UL)		/* GPIO_INTSTS reigster reset value */

/**
  * @}
  */
  
 /** @defgroup GPIO_Legacy_Define  GPIO legacy define
  * @brief define gpio function macro value
  * @{
  */ 
#define GPIO_NUMBER				(16UL)				/* GPIO group pin numbers */
/******************  Bits definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODER0		(0x00000003UL)		/* macro for gpio mode  configuration */
/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_OSPEEDR0	(0x00000003UL)		/* macro for gpio speed  configuration */
/******************  Bits definition for GPIO_PUPDR register  *****************/
#define GPIO_PUPDR_PUPDR0		(0x00000003UL)		/* macro for gpio pullup/pulldown configuration */ 
/* Old GPIO_ODR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_ODR_0		(0x00000001UL)		/* macro for gpio od function configuration */
/* Old GPIO_SMIT register bits definition, maintained for legacy purpose */
#define GPIO_SMIT_SMIT_0		(0x00000001UL)		/* macro for gpio schmit function configuration */
 
/**
  * @}
  */ 
 
/** @defgroup GPIO_alternatefunction_define GPIO alternate function define
  * @brief  GPIO alternate function define 
  * @{
  */
#define GPIO_AFR_Function1		(0x00000000UL)  
#define GPIO_AFR_Function2		(0x00000001UL)  
#define GPIO_AFR_Function3		(0x00000002UL)  
#define GPIO_AFR_Function4		(0x00000003UL) 
  
/**
  * @}
  */  

/**
  * @}
  */ 

/** @defgroup GPIO_Exported_Functions GPIO Exported Functions 
  * @{
  */  

/**
  * @brief	 De-initializes the GPIO peripheral registers to their default reset values.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 devices
  * @retval	None
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	GPIOx->MODE	  = GPIO_MODER_RSTVAL;
	GPIOx->ODEN	  = GPIO_ODEN_RSTVAL;
	GPIOx->OSPEED	= GPIO_OSPEEDR_RSTVAL;
	GPIOx->PUPD	  = GPIO_PUPDR_RSTVAL;
	GPIOx->ID		= GPIO_IDR_RSTVAL;
	GPIOx->OD		= GPIO_ODR_RSTVAL;
	GPIOx->BSRR	  = GPIO_BSRR_RSTVAL;
	GPIOx->AF		= GPIO_AFR_RSTVAL;
	GPIOx->SMIT	  = GPIO_SMIT_RSTVAL;
	GPIOx->INTC	  = GPIO_INTC_RSTVAL;
	GPIOx->INTMSK	= GPIO_INTMSK_RSTVAL;
	GPIOx->INTSTS	= GPIO_INTSTS_RSTVAL;
}

/**
  * @brief	 Initializes the GPIOx peripheral according to the specified parameters in the GPIO_InitStruct.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @param[in] GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that contains
  *			the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{	
	uint32_t pinposition = 0x00, ioposition = 0x00 , iocurrentpin = 0x00;	

	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_MODE(GPIO_InitStruct->Mode));
	assert_param(IS_GPIO_SPEED(GPIO_InitStruct->Speed));
	assert_param(IS_GPIO_PUPD(GPIO_InitStruct->Pull));	
	assert_param(IS_GPIO_OTYPE(GPIO_InitStruct->Otype));	
	assert_param(IS_GPIO_SMIT(GPIO_InitStruct->SMIT));	
	/* ------------------------- Configure the port pins ---------------- */
	for(pinposition = 0x00; pinposition < GPIO_NUMBER; pinposition++)
	{
		/* Get the IO position */
		ioposition = ((uint32_t)0x01) << pinposition;
		/* Get the current IO position */
		iocurrentpin = (GPIO_InitStruct->Pin) & ioposition;

		if (iocurrentpin == ioposition)
		{
			/* GPIO_MODER register configuration */			
			GPIOx->MODE  &= ~(GPIO_MODER_MODER0 << (pinposition * 2));
			GPIOx->MODE |= (((uint32_t)GPIO_InitStruct->Mode) << (pinposition * 2));
			
			if((GPIO_InitStruct->Mode == GPIO_MODE_OUT) || (GPIO_InitStruct->Mode == GPIO_MODE_AF))
			{
				/* GPIO_OSPEEDR register configurati on */
				GPIOx->OSPEED &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pinposition * 2));
				GPIOx->OSPEED |= ((uint32_t)(GPIO_InitStruct->Speed) << (pinposition * 2));
 			}
			/* GPIO_PUPD resistor configuration */
			GPIOx->PUPD &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinposition * 2));
			GPIOx->PUPD |= (((uint32_t)GPIO_InitStruct->Pull) << (pinposition * 2));	
			/* OD resistor configuration */
			GPIOx->ODEN  &= ~((GPIO_OTYPER_ODR_0) << ((uint16_t)pinposition)) ;
			GPIOx->ODEN |= (uint16_t)(((uint16_t)GPIO_InitStruct->Otype) << ((uint16_t)pinposition));

			/* GPIO_SMIT resistor configuration */
			GPIOx->SMIT  &= ~((GPIO_SMIT_SMIT_0) << ((uint16_t)pinposition)) ;
			GPIOx->SMIT |= (uint16_t)(((uint16_t)GPIO_InitStruct->SMIT) << ((uint16_t)pinposition));			
		}
	}	
}
/**
  * @brief	 Initializes the InitStruct of GPIO
  * @param[in] GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure which will be initialized.
  * @retval	None
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
	/* Reset GPIO init structure parameters values */
	GPIO_InitStruct->Pin	   = GPIO_PIN_All;
	GPIO_InitStruct->Mode	  = GPIO_MODE_OUT; 
	GPIO_InitStruct->Speed	 = GPIO_MEDIUM_SPEED;	
	GPIO_InitStruct->Pull	  = GPIO_PULLUP;
	GPIO_InitStruct->Otype	 = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct->SMIT	  = GPIO_INPUTSCHMIT_ENABLE;
}

/**
  * @brief	 Reads the specified GPIO input port pin.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @param[in] GPIO_Pin:  specifies the PIN to read.
  *					   This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval GPIO input port pin value.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	/* Check the parameters */	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	
	   
	if ((GPIOx->ID & GPIO_Pin) != (uint32_t)Bit_RESET)
	{
		bitstatus = (uint8_t)Bit_SET;
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	return bitstatus;
}

/**
  * @brief	 Reads the specified GPIO input data port.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @retval	GPIO input data port value.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
	/* Check the parameters */	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx)); 
	return ((uint16_t)GPIOx->ID);
}


/**
  * @brief	 Reads the specified output data port bit.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @param[in] GPIO_Pin:  specifies the port bit to read.
  *					   This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval	GPIO output port pin value.
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;	
	/* Check the parameters */	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	
	
  
	if ((GPIOx->OD & GPIO_Pin) != (uint32_t)Bit_RESET)
	{
		bitstatus = (uint8_t)Bit_SET;
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	return bitstatus;
}

/**
  * @brief	 Reads the specified GPIO output data port.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral for thm36_1.3 device.
  * @retval	GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
	/* Check the parameters */	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	
	return ((uint16_t)GPIOx->OD);
}


/**
  * @brief	 Sets the selected data port bits.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral  for thm36_1.3 device.
  * @param[in] GPIO_Pin:  specifies the port bit to read.
  *					   This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval	None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->OD |= GPIO_Pin;
}

/**
  * @brief	 Clears the selected data port bits.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral  for thm36_1.3 device.
  * @param[in] GPIO_Pin:  specifies the port bit to read.
  *					   This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval	None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->OD &= ~GPIO_Pin;
}


/**
  * @brief	 Configures GPIO pin with alternate function.
  * @param[in] GPIOx: where x can be (A..E) to select the GPIO peripheral  for thm36_1.3 device.
  * @param[in] GPIO_Pin: specifies the port bits to be written.
  *					  This parameter can be any combination of GPIO_PIN_Sourcex where x can be (0..15).
  * @param[in] Mode: gpio alternate function value.
  * 				 This parameter can be one of the values @ref GPIO_Alternat_function_selection_define
  * @retval	None
  */
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Mode)
{
		uint32_t Temp;
	Temp = GPIO_Pin*2;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN_SOURCE(GPIO_Pin));
//	assert_param(IS_GPIO_AF(Mode));	
	

	switch(Mode)
	{
		case GPIO_AFR_Function1: GPIOx->AF &= (~(1<<Temp)); 
								 GPIOx->AF &=(~(1<<(Temp+1)));
		break;
		case GPIO_AFR_Function2: GPIOx->AF |=(1<<Temp); 
								 GPIOx->AF &=(~(1<<(Temp+1))); 
		break;
		case GPIO_AFR_Function3: GPIOx->AF &=(~(1<<Temp)); 
								 GPIOx->AF |=(1<<(Temp+1)); 
		break;
		case GPIO_AFR_Function4: GPIOx->AF |=(1<<Temp); 
								 GPIOx->AF |=(1<<(Temp+1)); 
		break;
	
	}		
}

/**
  * @brief  outPut clock form GPIO interface
  * @param[in] num: the num of GPIO interface
  * 	This parameter can be one of the following values: 
  *		@arg 0 :outPut clock form GPIO3
  *	 @arg 1 :outPut clock form GPIO4
  * @param[in] source the source of OutPut clock
  *		@arg OSC_SOURCE_CLOCK_PLL_H :
  *	 @arg OSC_SOURCE_CLOCK_LSE:
  *	 @arg OSC_SOURCE_CLOCK_USBPHY:
  *	 @arg OSC_SOURCE_CLOCK_PLL_L:
  *	 @arg OSC_SOURCE_CLOCK_HSE:
  *	 @arg OSC_SOURCE_CLOCK_HSI:
  * @param[in] division 
  *   the division  of OutPut clock 
  *	 @arg 0: f(out)=f(source)/1
  *	 @arg 1: f(out)=f(source)/2
  *	 @arg 2: f(out)=f(source)/4
  *	 @arg 3: f(out)=f(source)/8
  *	 @arg 4: f(out)=f(source)/16
  *	 @arg 5: f(out)=f(source)/32
  * @retval none
  * @note
  */ 
void GPIO_MonitorClkOut(uint8_t num,uint8_t source,uint8_t division)
{
	uint8_t i;
	if(IS_MONITOR_DIV(division))
	{
		if(num == 0)
		{
			GPIOA->MODE &=Bit6_Dis;
			GPIOA->MODE |=Bit7_En;
			GPIOA->AF  &=Bit6_Dis;
			GPIOA->AF   |=Bit7_En;
		}
		else
		{
			GPIOA->MODE &=Bit8_Dis;
			GPIOA->MODE |=Bit9_En;
			GPIOA->AF   &=Bit8_Dis;
			GPIOA->AF   |=Bit9_En;
		}
		switch(source)
		{
			case OSC_SOURCE_CLOCK_PLL_H:
										RCC_CLK->CLKMONITORSEL&= Bit2_Dis;
										RCC_CLK->CLKMONITORSEL|= Bit1_En;	
										RCC_CLK->CLKMONITORSEL|= Bit0_En;
										break;
			case OSC_SOURCE_CLOCK_LSE:
										RCC_CLK->CLKMONITORSEL|= Bit2_En;	
										RCC_CLK->CLKMONITORSEL|= Bit1_En;
										RCC_CLK->CLKMONITORSEL&= Bit0_Dis;
										break;
			case OSC_SOURCE_CLOCK_LSI: 
										RCC_CLK->CLKMONITORSEL|= Bit2_En;	
										RCC_CLK->CLKMONITORSEL&= Bit1_Dis;
										RCC_CLK->CLKMONITORSEL|= Bit0_En;
										break;
			case OSC_SOURCE_CLOCK_USBPHY:
										RCC_CLK->CLKMONITORSEL|= Bit2_En;	
										RCC_CLK->CLKMONITORSEL&= Bit1_Dis;
										RCC_CLK->CLKMONITORSEL&= Bit0_Dis;
										break;
			case OSC_SOURCE_CLOCK_PLL_L:
										RCC_CLK->CLKMONITORSEL&= Bit2_Dis;	
										RCC_CLK->CLKMONITORSEL|= Bit1_En;	
										RCC_CLK->CLKMONITORSEL&= Bit0_Dis;
										break;
			case OSC_SOURCE_CLOCK_HSE:
										RCC_CLK->CLKMONITORSEL&= Bit2_Dis;	
										RCC_CLK->CLKMONITORSEL&= Bit1_Dis;	
										RCC_CLK->CLKMONITORSEL|= Bit0_En;
										break;
			case OSC_SOURCE_CLOCK_HSI:
										RCC_CLK->CLKMONITORSEL&= Bit2_Dis;	
										RCC_CLK->CLKMONITORSEL&= Bit1_Dis;	
										RCC_CLK->CLKMONITORSEL&= Bit0_Dis;
										break;
		}
		for(i=0;i<3;i++)
		{
			RCC_CLK->CLKMONITORSEL&= (~(1<<(4+i)));	
		}
		RCC_CLK->CLKMONITORSEL |=(division<<4);
		RCC_CLK->CLKMONITORSEL|= Bit3_En;
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


