/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: exti.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "rcc.h"
#include "exti.h"

/** @addtogroup thm36_1.3
  * @{
  */
	
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
	
/** @defgroup EXTI
  * @brief EXTI driver modules 
  * @{
  */ 
		

	
/**
  * @brief  Deinitializes the EXTI peripheral registers to  reset values.
  * @param[in] none 
  * @retval none
  * @note
  */
void EXTI_Reset(void)
{
	RCC_Security1PeriphSoftReset(RCC_SEC1Periph_INT);
	while(RCC_Security1PeriphGetResetFlag(RCC_SEC1Periph_INT) == RESET);
}
  
  
/**
  * @brief  enable or disable the EXTIx wake up mode is Rising,and Trigger a Interrupt 
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_GPIOA
  *		@arg EXTI_GPIOB
  *		@arg EXTI_GPIOC
  *		@arg EXTI_GPIOD
  *		@arg EXTI_GPIOE
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_PIN_0
  *		@arg EXTI_PIN_1
  *		@arg EXTI_PIN_2
  *		@arg EXTI_PIN_3
  *		@arg EXTI_PIN_4
  *		@arg EXTI_PIN_5
  *		@arg EXTI_PIN_6
  *		@arg EXTI_PIN_7
  *		@arg EXTI_PIN_8
  *		@arg EXTI_PIN_9
  *		@arg EXTI_PIN_10
  *		@arg EXTI_PIN_11
  *		@arg EXTI_PIN_12
  *		@arg EXTI_PIN_13
  *		@arg EXTI_PIN_14
  *		@arg EXTI_PIN_15
  * @param[in] NewStatus
  * This parameter can be one of the following values:
  *	@arg ENABLE				 
  *	@arg DISABLE   
  * @retval none
  * @note
  */
void EXTI_ConfigRising(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus)
{
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	if(NewStatus == ENABLE)
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->REEN0|=EXTI_Line;break;
			case EXTI_GPIOB: EXTI->REEN0|=(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->REEN1|=EXTI_Line;break;
			case EXTI_GPIOD: EXTI->REEN1|=(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->REEN2|=(EXTI_Line);break;
			default:break;
		}
	}
	else
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->REEN0&=~EXTI_Line;break;
			case EXTI_GPIOB: EXTI->REEN0&=~(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->REEN1&=~EXTI_Line;break;
			case EXTI_GPIOD: EXTI->REEN1&=~(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->REEN2&=~(EXTI_Line);break;
			default:break;
		}
	}
}

///**
//  * @brief  Config EXTI80-EXTI85 wake up mode is Rising,and Trigger a Interrupt 
//  * @param[in] EXTI_Line:
//  *		@arg EXTI_80_USB
//  *		@arg EXTI_81_RTC_ALARM
//  *		@arg EXTI_82_IWDT_ALARM
//  *		@arg EXTI_83_WAKE_UP_PIN_0
//  *		@arg EXTI_84_WAKE_UP_PIN_1
//  * @param[in] NewStatus:
//  *		@arg ENABLE
//  *		@arg DISABLE
//  * @retval none
//  * @note none
//  */
//void EXTI_ConfigOtherRising(uint32_t EXTI_Line,FunctionalState NewStatus)
//{
//	assert_param(IS_EXTI_OTHER_CHECK(EXTI_Line)); 
//	if(NewStatus == ENABLE)
//	{
//		EXTI->REEN3|=EXTI_Line;
//	}
//	else
//	{
//		EXTI->REEN3&=~EXTI_Line;
//	}
//}

/**
  * @brief  enable or disable the EXTI wake up mode is Falling,and Trigger a Interrupt 
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_GPIOA
  *		@arg EXTI_GPIOB
  *		@arg EXTI_GPIOC
  *		@arg EXTI_GPIOD
  *		@arg EXTI_GPIOE
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_PIN_0
  *		@arg EXTI_PIN_1
  *		@arg EXTI_PIN_2
  *		@arg EXTI_PIN_3
  *		@arg EXTI_PIN_4
  *		@arg EXTI_PIN_5
  *		@arg EXTI_PIN_6
  *		@arg EXTI_PIN_7
  *		@arg EXTI_PIN_8
  *		@arg EXTI_PIN_9
  *		@arg EXTI_PIN_10
  *		@arg EXTI_PIN_11
  *		@arg EXTI_PIN_12
  *		@arg EXTI_PIN_13
  *		@arg EXTI_PIN_14
  *		@arg EXTI_PIN_15
  * @param[in] NewStatus
  * This parameter can be one of the following values:
  *	@arg ENABLE				 
  *	@arg DISABLE   
  * @retval none
  * @note
  */
void EXTI_ConfigFalling(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus)
{
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	if(NewStatus == ENABLE)
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->FEEN0|=EXTI_Line;break;
			case EXTI_GPIOB: EXTI->FEEN0|=(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->FEEN1|=EXTI_Line;break;
			case EXTI_GPIOD: EXTI->FEEN1|=(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->FEEN2|=(EXTI_Line);break;
			default:break;
		}
	}
	else
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->FEEN0&=~EXTI_Line;break;
			case EXTI_GPIOB: EXTI->FEEN0&=~(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->FEEN1&=~EXTI_Line;break;
			case EXTI_GPIOD: EXTI->FEEN1&=~(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->FEEN2&=~(EXTI_Line);break;
			default:break;
		}
	}
}

///**
//  * @brief  Config EXTI80-EXTI85 wake up mode is Falling,and Trigger a Interrupt 
//  * @param[in] EXTI_Line:
//  *		@arg EXTI_80_USB
//  *		@arg EXTI_81_RTC_ALARM
//  *		@arg EXTI_82_IWDT_ALARM
//  *		@arg EXTI_83_WAKE_UP_PIN_0
//  *		@arg EXTI_84_WAKE_UP_PIN_1
//  *		@arg EXTI_85_SWP
//  * @param[in] NewStatus:
//  *		@arg ENABLE
//  *		@arg DISABLE
//  * @retval none
//  * @note none
//  */
//void EXTI_ConfigOtherFalling(uint32_t EXTI_Line,FunctionalState NewStatus)
//{
//	assert_param(IS_EXTI_OTHER_CHECK(EXTI_Line)); 
//	if(NewStatus == ENABLE)
//	{
//		EXTI->FEEN3|=EXTI_Line;
//	}
//	else
//	{
//		EXTI->FEEN3&=~EXTI_Line;
//	}
//}

/**
  * @brief  enable or disable the EXTI wake up mode is HighLevel,and Trigger a Interrupt 
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_GPIOA
  *		@arg EXTI_GPIOB
  *		@arg EXTI_GPIOC
  *		@arg EXTI_GPIOD
  *		@arg EXTI_GPIOE
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_PIN_0
  *		@arg EXTI_PIN_1
  *		@arg EXTI_PIN_2
  *		@arg EXTI_PIN_3
  *		@arg EXTI_PIN_4
  *		@arg EXTI_PIN_5
  *		@arg EXTI_PIN_6
  *		@arg EXTI_PIN_7
  *		@arg EXTI_PIN_8
  *		@arg EXTI_PIN_9
  *		@arg EXTI_PIN_10
  *		@arg EXTI_PIN_11
  *		@arg EXTI_PIN_12
  *		@arg EXTI_PIN_13
  *		@arg EXTI_PIN_14
  *		@arg EXTI_PIN_15
  * @param[in] NewStatus
  * This parameter can be one of the following values:
  *	@arg ENABLE				 
  *	@arg DISABLE   
  * @retval none
  * @note
  */
void EXTI_ConfigHighLevel(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus)
{
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	if(NewStatus == ENABLE)
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->HLEN0|=EXTI_Line;break;
			case EXTI_GPIOB: EXTI->HLEN0|=(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->HLEN1|=EXTI_Line;break;
			case EXTI_GPIOD: EXTI->HLEN1|=(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->HLEN2|=(EXTI_Line);break;
			default:break;
		}
	}
	else
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->HLEN0&=~EXTI_Line;break;
			case EXTI_GPIOB: EXTI->HLEN0&=~(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->HLEN1&=~EXTI_Line;break;
			case EXTI_GPIOD: EXTI->HLEN1&=~(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->HLEN2&=~(EXTI_Line);break;
			default:break;
		}
	}
}

///**
//  * @brief  Config EXTI80-EXTI85 wake up mode is HighLevel,and Trigger a Interrupt 
//  * @param[in] EXTI_Line:
//  *		@arg EXTI_80_USB
//  *		@arg EXTI_81_RTC_ALARM
//  *		@arg EXTI_82_IWDT_ALARM
//  *		@arg EXTI_83_WAKE_UP_PIN_0
//  *		@arg EXTI_84_WAKE_UP_PIN_1
//  *		@arg EXTI_85_SWP
//  * @param[in] NewStatus:
//  *		@arg ENABLE
//  *		@arg DISABLE
//  * @retval none
//  * @note none
//  */
//void EXTI_ConfigOtherHighLevel(uint32_t EXTI_Line,FunctionalState NewStatus)
//{
//	assert_param(IS_EXTI_OTHER_CHECK(EXTI_Line)); 
//	if(NewStatus == ENABLE)
//	{
//		EXTI->HLEN3|=EXTI_Line;
//	}
//	else
//	{
//		EXTI->HLEN3&=~EXTI_Line;
//	}
//}

/**
  * @brief  enable or disable the EXTI wake up mode is LowLevel,and Trigger a Interrupt 
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_GPIOA
  *		@arg EXTI_GPIOB
  *		@arg EXTI_GPIOC
  *		@arg EXTI_GPIOD
  *		@arg EXTI_GPIOE
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_PIN_0
  *		@arg EXTI_PIN_1
  *		@arg EXTI_PIN_2
  *		@arg EXTI_PIN_3
  *		@arg EXTI_PIN_4
  *		@arg EXTI_PIN_5
  *		@arg EXTI_PIN_6
  *		@arg EXTI_PIN_7
  *		@arg EXTI_PIN_8
  *		@arg EXTI_PIN_9
  *		@arg EXTI_PIN_10
  *		@arg EXTI_PIN_11
  *		@arg EXTI_PIN_12
  *		@arg EXTI_PIN_13
  *		@arg EXTI_PIN_14
  *		@arg EXTI_PIN_15
  * @param[in] NewStatus
  * This parameter can be one of the following values:
  *	@arg ENABLE				 
  *	@arg DISABLE   
  * @retval none
  */
void EXTI_ConfigLowLevel(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus)
{
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	if(NewStatus == ENABLE)
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->LLEN0|=EXTI_Line;break;
			case EXTI_GPIOB: EXTI->LLEN0|=(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->LLEN1|=EXTI_Line;break;
			case EXTI_GPIOD: EXTI->LLEN1|=(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->LLEN2|=(EXTI_Line);break;
			default:break;
		}
	}
	else
	{
		switch(EXTIx)
		{
			case EXTI_GPIOA: EXTI->LLEN0&=~EXTI_Line;break;
			case EXTI_GPIOB: EXTI->LLEN0&=~(EXTI_Line<<16);break;
			case EXTI_GPIOC: EXTI->LLEN1&=~EXTI_Line;break;
			case EXTI_GPIOD: EXTI->LLEN1&=~(EXTI_Line<<16);break;
			case EXTI_GPIOE: EXTI->LLEN2&=~(EXTI_Line);break;
			default:break;
		}
	}
}

///**
//  * @brief  Config EXTI80-EXTI85 wake up mode is LowLevel(,and Trigger a Interrupt 
//  * @param[in] EXTI_Line:
//  *		@arg EXTI_80_USB
//  *		@arg EXTI_81_RTC_ALARM
//  *		@arg EXTI_82_IWDT_ALARM
//  *		@arg EXTI_83_WAKE_UP_PIN_0
//  *		@arg EXTI_84_WAKE_UP_PIN_1
//  *		@arg EXTI_85_SWP
//  * @param[in] NewStatus:
//  *		@arg ENABLE
//  *		@arg DISABLE
//  * @retval none
//  * @note none
//  */
//void EXTI_ConfigOtherLowLevel(uint32_t EXTI_Line,FunctionalState NewStatus)
//{
//	assert_param(IS_EXTI_OTHER_CHECK(EXTI_Line)); 
//	if(NewStatus == ENABLE)
//	{
//		EXTI->LLEN3|=EXTI_Line;
//	}
//	else
//	{
//		EXTI->LLEN3&=~EXTI_Line;
//	}
//}

/**
  * @brief  enable or disable the EXTI wake up mode is Other Mode (not GPIO Pinbx)
  * @param[in] Mode
  *		@arg EXTI_USB_WAKE_UP
  *		@arg EXTI_RTC_ALM_WAKE_UP
  *		@arg EXTI_IWDT_ALM_WAKE_UP
  * @param[in] NewStatus
  * This parameter can be one of the following values:
  *	@arg ENABLE				 
  *	@arg DISABLE   
  * @retval none
  */
void EXTI_ConfigOtherMode(uint32_t Mode,FunctionalState NewStatus)
{
	assert_param(IS_EXTI_OTHER(Mode)); 
	if(NewStatus == ENABLE)
	{
		if(Mode&EXTI_USB_WAKE_UP)
		{
			EXTI->REEN3|= 0x01;
		}
		if(Mode&EXTI_RTC_ALM_WAKE_UP)
		{
			EXTI->REEN3|= 0x02;
		}
		if(Mode&EXTI_IWDT_ALM_WAKE_UP)
		{
			EXTI->REEN3|= 0x04;
		}
		if(Mode&EXTI_BAT_PIN0_RSING_WAKE_UP)
		{
			EXTI->REEN3|= 0x08;
		}
		if(Mode&EXTI_BAT_PIN1_RSING_WAKE_UP)
		{
			EXTI->REEN3|= 0x10;
		}
		if(Mode&EXTI_BAT_PIN0_FALLING_WAKE_UP)
		{
			EXTI->FEEN3|= 0x08;
		}
		if(Mode&EXTI_BAT_PIN1_FALLING_WAKE_UP)
		{
			EXTI->FEEN3|= 0x10;
		}
		if(Mode&EXTI_BAT_PIN0_HIGH_LEVEL_WAKE_UP)
		{
			EXTI->HLEN3|= 0x08;
		}
		if(Mode&EXTI_BAT_PIN1_HIGH_LEVEL_WAKE_UP)
		{
			EXTI->HLEN3|= 0x10;
		}
		if(Mode&EXTI_BAT_PIN0_LOW_LEVEL_WAKE_UP)
		{
			EXTI->LLEN3|= 0x08;
		}
		if(Mode&EXTI_BAT_PIN1_LOW_LEVEL_WAKE_UP)
		{
			EXTI->LLEN3|= 0x10;
		}
	}
	else
	{
		if(Mode&EXTI_USB_WAKE_UP)
		{
			EXTI->REEN3&= ~(0x01);
		}
		if(Mode&EXTI_RTC_ALM_WAKE_UP)
		{
			EXTI->REEN3&= ~(0x02);
		}
		if(Mode&EXTI_IWDT_ALM_WAKE_UP)
		{
			EXTI->REEN3&= ~(0x04);
		}
		if(Mode&EXTI_BAT_PIN0_RSING_WAKE_UP)
		{
			EXTI->REEN3&= ~(0x08);
		}
		if(Mode&EXTI_BAT_PIN1_RSING_WAKE_UP)
		{
			EXTI->REEN3&= ~(0x10);
		}
		if(Mode&EXTI_BAT_PIN0_FALLING_WAKE_UP)
		{
			EXTI->FEEN3&= ~(0x08);
		}
		if(Mode&EXTI_BAT_PIN1_FALLING_WAKE_UP)
		{
			EXTI->FEEN3&= ~(0x10);
		}
		if(Mode&EXTI_BAT_PIN0_HIGH_LEVEL_WAKE_UP)
		{
			EXTI->HLEN3&= ~(0x08);
		}
		if(Mode&EXTI_BAT_PIN1_HIGH_LEVEL_WAKE_UP)
		{
			EXTI->HLEN3&= ~(0x10);
		}
		if(Mode&EXTI_BAT_PIN0_LOW_LEVEL_WAKE_UP)
		{
			EXTI->LLEN3&= ~(0x08);
		}
		if(Mode&EXTI_BAT_PIN1_LOW_LEVEL_WAKE_UP)
		{
			EXTI->LLEN3&= ~(0x10);
		}
	}
}
///**
//  * @brief  Deinitializes the EXTI peripheral registers to their default reset values.
//  * @param	none
//  * @retval	none
//  * @note	none
//  */
//void EXTI_DeInit(void)
//{
//	EXTI->FEEN0 = 0;
//	EXTI->FEEN1 = 0;
//	EXTI->FEEN2 = 0;
//	EXTI->FEEN3 = 0;
//	
//	EXTI->REEN0 = 0;
//	EXTI->REEN1 = 0;
//	EXTI->REEN2 = 0;
//	EXTI->REEN3 = 0;
//	
//	EXTI->HLEN0 = 0;
//	EXTI->HLEN1 = 0;
//	EXTI->HLEN2 = 0;
//	EXTI->HLEN3 = 0;
//	
//	EXTI->LLEN0 = 0;
//	EXTI->LLEN1 = 0;
//	EXTI->LLEN2 = 0;
//	EXTI->LLEN3 = 0;
//	
//	EXTI->MCUINTCON0 = 0xFFFFFFFF;
//	EXTI->MCUINTCON1 = 0xFFFFFFFF;
//	EXTI->MCUINTCON2 = 0xFFFFFFFF;
//	
//	EXTI->MCUEVCON0 = 0;
//	EXTI->MCUEVCON1 = 0;
//	EXTI->MCUEVCON2 = 0;
//}


/**
  * @brief	Generates a Software interrupt.
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_GPIOA
  *		@arg EXTI_GPIOB
  *		@arg EXTI_GPIOC
  *		@arg EXTI_GPIOD
  *		@arg EXTI_GPIOE
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_PIN_0
  *		@arg EXTI_PIN_1
  *		@arg EXTI_PIN_2
  *		@arg EXTI_PIN_3
  *		@arg EXTI_PIN_4
  *		@arg EXTI_PIN_5
  *		@arg EXTI_PIN_6
  *		@arg EXTI_PIN_7
  *		@arg EXTI_PIN_8
  *		@arg EXTI_PIN_9
  *		@arg EXTI_PIN_10
  *		@arg EXTI_PIN_11
  *		@arg EXTI_PIN_12
  *		@arg EXTI_PIN_13
  *		@arg EXTI_PIN_14
  *		@arg EXTI_PIN_15
  * @retval none
  * @note write resgiter of PRx to Clear the EVENT
  */
void EXTI_GenerateSWInterrupt(EXTI_Groupx EXTIx,uint32_t EXTI_Line)
{	
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	switch(EXTIx)
	{
		case EXTI_GPIOA: EXTI->SWIER0|=EXTI_Line;break;
		case EXTI_GPIOB: EXTI->SWIER0|=(EXTI_Line<<16);break;
		case EXTI_GPIOC: EXTI->SWIER1|=EXTI_Line;break;
		case EXTI_GPIOD: EXTI->SWIER1|=(EXTI_Line<<16);break;
		case EXTI_GPIOE: EXTI->SWIER2|=(EXTI_Line);break;
		default:break;
	}
}

/**
  * @brief Get the status of EXTI Pinx
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_GPIOA
  *		@arg EXTI_GPIOB
  *		@arg EXTI_GPIOC
  *		@arg EXTI_GPIOD
  *		@arg EXTI_GPIOE
  * @param[in] EXTIx: the Groupx of EXTIx
  *		@arg EXTI_PIN_0
  *		@arg EXTI_PIN_1
  *		@arg EXTI_PIN_2
  *		@arg EXTI_PIN_3
  *		@arg EXTI_PIN_4
  *		@arg EXTI_PIN_5
  *		@arg EXTI_PIN_6
  *		@arg EXTI_PIN_7
  *		@arg EXTI_PIN_8
  *		@arg EXTI_PIN_9
  *		@arg EXTI_PIN_10
  *		@arg EXTI_PIN_11
  *		@arg EXTI_PIN_12
  *		@arg EXTI_PIN_13
  *		@arg EXTI_PIN_14
  *		@arg EXTI_PIN_15
  * @retval The new state of EXTI_Line (SET or RESET).
  */
FlagStatus EXTI_GetPINxStatus(EXTI_Groupx EXTIx,uint32_t EXTI_Line)
{
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	switch(EXTIx)
	{
		case EXTI_GPIOA: if((EXTI->PR0)&EXTI_Line)
						{	
							return SET;
						}
						break;
		case EXTI_GPIOB:if((EXTI->PR0)&(EXTI_Line<<16))
						{	
							return SET;
						}
						break; 
		case EXTI_GPIOC:if((EXTI->PR1)&EXTI_Line)
						{	
							return SET;
						}
						break;  
			
		case EXTI_GPIOD:if((EXTI->PR1)&(EXTI_Line<<16))
						{	
							return SET;
						}
						break;  
						
		case EXTI_GPIOE:if((EXTI->PR2)&EXTI_Line)
						{	
							return SET;
						}
						break; 
		default:break;
	}
	return RESET;
}
/**
  * @brief Get the status of EXTI Pinx
  * @param[in] Mode
  *		@arg EXTI_USB_WAKE_UP
  *		@arg EXTI_RTC_ALM_WAKE_UP
  *		@arg EXTI_IWDT_ALM_WAKE_UP
  * @retval The new state of EXTI_Line (SET or RESET).
  */
FlagStatus EXTI_GetOtherStatus(uint32_t Flag)
{
	if(Flag&EXTI_USB_WAKE_UP)
	{
		if(EXTI->PR0&0x01)
		return SET;
	}
	if(Flag&EXTI_RTC_ALM_WAKE_UP)
	{
		if(EXTI->PR0&0x02)
		return SET;
	}
	if(Flag&EXTI_IWDT_ALM_WAKE_UP)
	{
		if(EXTI->PR0&0x04)
		return SET;
	}
	if((Flag&EXTI_BAT_PIN0_RSING_WAKE_UP)||(Flag&EXTI_BAT_PIN0_FALLING_WAKE_UP)||(Flag&EXTI_BAT_PIN0_HIGH_LEVEL_WAKE_UP)||(Flag&EXTI_BAT_PIN0_LOW_LEVEL_WAKE_UP))
	{
		if(EXTI->PR0&0x08)
		return SET;
	}
	if((Flag&EXTI_BAT_PIN1_RSING_WAKE_UP)||(Flag&EXTI_BAT_PIN1_FALLING_WAKE_UP)||(Flag&EXTI_BAT_PIN1_HIGH_LEVEL_WAKE_UP)||(Flag&EXTI_BAT_PIN1_LOW_LEVEL_WAKE_UP))
	{
		if(EXTI->PR0&0x10)
		return SET;
	}
	return RESET;
}

/**
  * @brief Clear the status of EXTI Pinx
  * @param[in] Mode
  *		@arg EXTI_USB_WAKE_UP
  *		@arg EXTI_RTC_ALM_WAKE_UP
  *		@arg EXTI_IWDT_ALM_WAKE_UP
  * @retval The new state of EXTI_Line (SET or RESET).
  */
void EXTI_ClearOtherStatus(uint32_t Flag)
{
	if(Flag&EXTI_USB_WAKE_UP)
	{
		EXTI->PR3 = 0x01;
	}
	if(Flag&EXTI_RTC_ALM_WAKE_UP)
	{
		EXTI->PR3 = 0x02;
	}
	if(Flag&EXTI_IWDT_ALM_WAKE_UP)
	{
		EXTI->PR3 = 0x04;
	}
	if(Flag&EXTI_BAT_PIN0_WAKE_UP)
	{
		EXTI->PR3 = 0x08;
	}
	if(Flag&EXTI_BAT_PIN1_WAKE_UP)
	{
		EXTI->PR3 = 0x10;
	}
}

/**
  * @brief  Clears the EXTI line pending flags.
  * @param  EXTIx: specifies the EXTI IO Group.
  *   This parameter can be @ref EXTI_Groupx.
  * @param  EXTI_Line: specifies the EXTI lines.
  *   This parameter can be 0-15.
  * @retval None
  */
void EXTI_ClearITPendingBit(EXTI_Groupx EXTIx,uint32_t EXTI_Line)
{
	assert_param(IS_EXTI_PIN(EXTI_Line)); 
	switch(EXTIx)
	{
		case EXTI_GPIOA: EXTI->PR0 =EXTI_Line;break;
		case EXTI_GPIOB: EXTI->PR0 =EXTI_Line<<16;break;
		case EXTI_GPIOC: EXTI->PR1 =EXTI_Line;break;
		case EXTI_GPIOD: EXTI->PR1 = EXTI_Line<<16;break;				
		case EXTI_GPIOE: EXTI->PR2 = EXTI_Line;break;	
		default:break;
	}
}


void EXTI_ConfigInterruptMode(IRQn_Type IRQn,FunctionalState NewStatus)
{
	if(NewStatus == ENABLE)
	{
		if(IRQn<=31)
		{
			EXTI->MCUINTCON0 |= 1<<IRQn;
		}
		else if((IRQn<=63)&&(IRQn>31))
		{
			EXTI->MCUINTCON1 |= 1<<(IRQn-32);
		}
		else if((IRQn>63))
		{
			EXTI->MCUINTCON2 |= 1<<(IRQn-64);
		}
	}
	else
	{
		if(IRQn<=31)
		{
			EXTI->MCUINTCON0 &= ~(1<<IRQn);
		}
		else if((IRQn<=63)&&(IRQn>31))
		{
			EXTI->MCUINTCON1 &=~(1<<(IRQn-32));
		}
		else if((IRQn>64))
		{
			EXTI->MCUINTCON2 &= ~(1<<(IRQn-64));
		}
	}
}


void EXTI_ConfigEventMode(IRQn_Type IRQn,FunctionalState NewStatus)
{
	if(NewStatus == ENABLE)
	{
		if(IRQn<=31)
		{
			EXTI->MCUEVCON0 |= 1<<IRQn;
		}
		else if((IRQn<=63)&&(IRQn>31))
		{
			EXTI->MCUEVCON1 |= 1<<(IRQn-32);
		}
		else if((IRQn>63))
		{
			EXTI->MCUEVCON2 |= 1<<(IRQn-64);
		}
	}
	else
	{
		if(IRQn<=31)
		{
			EXTI->MCUEVCON0 &= ~(1<<IRQn);
		}
		else if((IRQn<=63)&&(IRQn>31))
		{
			EXTI->MCUEVCON1 &=~(1<<(IRQn-32));
		}
		else if((IRQn>64))
		{
			EXTI->MCUEVCON2 &= ~(1<<(IRQn-64));
		}
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
