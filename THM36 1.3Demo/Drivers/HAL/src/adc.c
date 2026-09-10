/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: adc.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "adc.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup ADC
  * @brief ADC HAL modules driver 
  * @{
  */
/** @defgroup ADC_private_defines ADC private defines
  * @{
  */
#define ADC_DISABLE					~(1UL)
#define ADC_ENABLE					(1UL)
#define ADC_FIXES_TIME_SAMPLING		~(1UL<<1)

/**
  * @}
  */
/** @defgroup ADC_exported_function ADC exported function
  * @{  
  */
/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @param[in] ADC_InitStruct: pointer to a ADC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  the value of SAMPLEWIDTH is the clock of ADC Analog Sampling, the min value of Buad - SAMPLEWIDTH is 13
  */
__weak void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
	ADC_InitStruct->CMP_Mode = ADC_NOT_CMP;
	ADC_InitStruct->Divsion = 2;
	ADC_InitStruct->SAMPLEWIDTH = 16;
	ADC_InitStruct->Buad = 32;
	ADC_InitStruct->ADC_Channelx = ADC_Channel2|ADC_Channel5|ADC_Channel6;
	ADC_InitStruct->ADC_Mode = ADC5_CHOISE_ADC|ADC6_CHOISE_ADC;
}

__weak void ADC_AMP_StructInit(ADC_AMP_InitTypeDef *ADC_AMP_InitStruct)
{
	ADC_AMP_InitStruct->ADC_AMP_Select = ADC_AMP0|ADC_AMP1;
	ADC_AMP_InitStruct->ADC_AMP_Mode = ADC_AMP_MODE_AMPLIFICATION;
	ADC_AMP_InitStruct->ADC_AMP_CMP = ADC_AMP_CENTRAL_LEVEL_45_PERCENT;
	ADC_AMP_InitStruct->ADC_AMP_ADJ = ADC_AMP_MAG_5;
}


/**
  * @brief Deinitializes the ADC peripheral registers to their default reset values.
  * @param[in] none
  * @retval none
  * @note  
  */ 
void ADC_DeInitial(void)
{
	ADC_Cmd(DISABLE);
	RCC_APB2PeriphSoftReset(RCC_APB2Periph_ADC);
	while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_ADC) == RESET);
}
/**
  * @brief Initializes the ADC peripheral according to 
  *   the specified parameters in the ADC_InitTypeDef.
  * @param[in] ADC_InitStruct: pointer to a ADC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */  
void ADC_Initial(ADC_InitTypeDef *ADC_InitStruct) 
{
	ADC_AnalogCmd(ENABLE);
	/*Fixes time sampling*/
	ADC->ADCCON&= ADC_FIXES_TIME_SAMPLING;
	ADC_Channel_Select(ADC_InitStruct->ADC_Channelx);
	if(ADC_InitStruct->CMP_Mode == ADC_CMP)
	{
		ADC_ConfigCMPMode(ENABLE);
	}
	else
	{
		ADC_ConfigCMPMode(DISABLE);
	}
	ADC_ConfigPSC(ADC_InitStruct->Divsion);
	ADC_ConfigMag(ADC_InitStruct->ADC_Mode);
	ADC_ConfigBuad(ADC_InitStruct->Buad); 
	ADC_ConfigSampleWidth(ADC_InitStruct->SAMPLEWIDTH); 
	//ADC_Cmd(ENABLE);
}  

/**
  * @brief config ADC channel5 channel6 channeil7 to MAG or ADC
  * @param[in] MagMode: mode of ADC
  * @retval none
  * @note none
  */  
void ADC_ConfigMag(uint32_t MagMode) 
{
	assert_param(IS_MAG(MagMode));
	if(MagMode&ADC5_CHOISE_MAG)
	{
		ADC->ADCIOSEL|=ADC5_CHOISE_MAG;
	}
	else
	{
		ADC->ADCIOSEL&=~ADC5_CHOISE_MAG;
	}
	if(MagMode&ADC6_CHOISE_MAG)
	{
		ADC->ADCIOSEL|=ADC6_CHOISE_MAG;
	}
	else
	{
		ADC->ADCIOSEL&=~ADC6_CHOISE_MAG;
	}
	if(MagMode&ADC7_CHOISE_MAG)
	{
		ADC->ADCIOSEL|=ADC7_CHOISE_MAG;
	}
	else
	{
		ADC->ADCIOSEL&=~ADC7_CHOISE_MAG;
	}
}  

/**
  * @brief Initializes the ADC AMP peripheral according to 
  *   the specified parameters in the ADC_AMP_InitTypeDef.
  * @param[in] ADC_AMP_InitStruct pointer to a ADC_AMP_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note before call ADC_AMP_Initial,must be call ADC_Initial
  */  
void ADC_AMP_Initial(ADC_AMP_InitTypeDef *ADC_AMP_InitStruct) 
{
	assert_param(IS_AMP_CHECK(ADC_AMP_InitStruct->ADC_AMP_Select)); 
	assert_param(IS_AMP_MODE_CHECK(ADC_AMP_InitStruct->ADC_AMP_Mode)); 
	assert_param(IS_AMP_CMP_CHECK(ADC_AMP_InitStruct->ADC_AMP_CMP));
	assert_param(IS_AMP_ADJ_CHECK(ADC_AMP_InitStruct->ADC_AMP_ADJ));
	/*Close ADC AMP0 AMP1 AMP2*/
	ADC_AMP->AMP0CON = 0;
	ADC_AMP->AMP1CON = 0;
	ADC_AMP->AMP2CON = 0;
	ADC_AMP->AMP1PARA = 0;
	ADC_AMP->AMP2PARA = 0;
	ADC_AMP->AMP2PARA = 0;
	if(ADC_AMP_InitStruct->ADC_AMP_Select&ADC_AMP0) 
	{
		ADC_AMP->AMP0CON|=ADC_AMP_InitStruct->ADC_AMP_Mode;
		ADC_AMP->AMP0PARA|=ADC_AMP_InitStruct->ADC_AMP_CMP;
		ADC_AMP->AMP0PARA|=ADC_AMP_InitStruct->ADC_AMP_ADJ;
		if(ADC_AMP_InitStruct->ADC_AMP_Mode == ADC_AMP_MODE_AMPLIFICATION)
		{
			ADC_AMP_Cmd(ADC_AMP0,ENABLE);
		}
		else
		{
			ADC_AMP_Cmd(ADC_AMP0,DISABLE);
		}
	}
	if(ADC_AMP_InitStruct->ADC_AMP_Select&ADC_AMP1) 
	{
		ADC_AMP->AMP1CON|=ADC_AMP_InitStruct->ADC_AMP_Mode;
		ADC_AMP->AMP1PARA|=ADC_AMP_InitStruct->ADC_AMP_CMP;
		ADC_AMP->AMP1PARA|=ADC_AMP_InitStruct->ADC_AMP_ADJ;
		if(ADC_AMP_InitStruct->ADC_AMP_Mode == ADC_AMP_MODE_AMPLIFICATION)
		{
			ADC_AMP_Cmd(ADC_AMP1,ENABLE);
		}
		else
		{
			ADC_AMP_Cmd(ADC_AMP1,DISABLE);
		}

	}
	if(ADC_AMP_InitStruct->ADC_AMP_Select&ADC_AMP2) 
	{
		ADC_AMP->AMP2CON|=ADC_AMP_InitStruct->ADC_AMP_Mode;
		ADC_AMP->AMP2PARA|=ADC_AMP_InitStruct->ADC_AMP_CMP;
		ADC_AMP->AMP2PARA|=ADC_AMP_InitStruct->ADC_AMP_ADJ;
		if(ADC_AMP_InitStruct->ADC_AMP_Mode == ADC_AMP_MODE_AMPLIFICATION)
		{
			ADC_AMP_Cmd(ADC_AMP2,ENABLE);
		}
		else
		{
			ADC_AMP_Cmd(ADC_AMP2,DISABLE);
		}
	}
}  


/**
  * @brief  Enable or Disable ADC Analog circuit
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
void ADC_AnalogCmd(FunctionalState NewState) 
{
	if(NewState == ENABLE)
	{
		ADC->ADCPCON = 1;
	}
	else
	{
		ADC->ADCPCON = 0;
	}
}

/**
  * @brief Enable or Disable ADC AMP 
  * @param[in] ADC_Amp
  *		@arg ADC_AMP0
  *		@arg ADC_AMP1
  *		@arg ADC_AMP2
  * @param[in] NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note none
  */  
void ADC_AMP_Cmd(uint32_t ADC_Amp,FunctionalState NewState) 
{
	assert_param(IS_AMP_CHECK(ADC_Amp)); 
	if(NewState ==ENABLE)
	{
		if(ADC_Amp&ADC_AMP0)
		{
			ADC_AMP->AMP0CON|=1UL;
		}
		if(ADC_Amp&ADC_AMP1)
		{
			ADC_AMP->AMP1CON|=1UL;
		}
		if(ADC_Amp&ADC_AMP2)
		{
			ADC_AMP->AMP2CON|=1UL;
		}
	}
	else
	{
		if(ADC_Amp&ADC_AMP0)
		{
			ADC_AMP->AMP0CON&=~(1UL);
		}
		if(ADC_Amp&ADC_AMP1)
		{
			ADC_AMP->AMP1CON&=~(1UL);
		}
		if(ADC_Amp&ADC_AMP2)
		{
			ADC_AMP->AMP2CON&=~(1UL);
		}
	}
}

/**
  * @brief  Enable or Disable ADC 
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
void ADC_Cmd(FunctionalState NewState) 
{
	if(NewState == ENABLE)
	{
		ADC->ADCCON |= ADC_ENABLE;
	}
	else
	{
		ADC->ADCCON &= ADC_DISABLE;
	}
}

/**
  * @brief Select ADC Channel
  * @param[in] Channel: where x can be 0 to 7 to select the ADC Channel. 
  * @retval none
  * @note none
  */  
void ADC_Channel_Select(uint32_t Channelx) 
{
	assert_param(IS_CHANNEL_CHECK(Channelx)); 
	ADC->ADCIOSEL&= ~(255UL);
	ADC->ADCIOSEL = Channelx;
}

/**
  * @brief Select ADC Channel
  * @param[in] Channel: where x can be 0 to 7 to select the ADC Channel. 
  * @retval none
  * @note none
  */  
void ADC_Channel_CMP(ADC_Channelx Channel) 
{
	switch(Channel)
	{
		case ADC_Channel0:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<8;
						  break;
		case ADC_Channel1:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<9;
						  break;
		case ADC_Channel2:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<10;
						  break;
		case ADC_Channel3:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<11;
						  break;
		case ADC_Channel4:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<12;
						  break;
		case ADC_Channel5:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<13;
						  break;
		case ADC_Channel6:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<14;
						  break;
			
		case ADC_Channel7:ADC->ADCIOSEL&= ~(255UL<<8);
						  ADC->ADCIOSEL|=1<<15;
						  break;
	}
}

/**
  * @brief  Enable or Disable ADC CMP mode
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
void ADC_ConfigCMPMode(FunctionalState NewState) 
{
	if(NewState == ENABLE)
	{
		ADC->ADCCON|= ADC_CMP;
	}
	else
	{
		ADC->ADCCON&= ADC_NOT_CMP;
	}
}

/**
  * @brief  Config the divsion of ADC source clock
  * @param[in] div: the vaule of divsion, 0-div_2,1-div_4,2-div_6,...FF-div_512
  * @retval none
  * @note none
  */  
void ADC_ConfigPSC(uint8_t div) 
{
	assert_param(IS_DIVSION(div)); 
	ADC->ADCPSC = div;
}

/**
  * @brief  Config the Cycles of once ADC  
  * @param[in] Cycle: the vaule of Cycle
  * @retval none
  * @note none
  */  
void ADC_ConfigSampleWidth(uint8_t samp) 
{
	//assert_param(IS_CYCLE(Cycle));
	assert_param(samp<(ADC->ADCBAUD&0xFFFFUL));
	ADC->ADCBAUD &=~(255<<16);
	ADC->ADCBAUD |= samp<<16;
}

/**
  * @brief  Config the clocks of one Cycle 
  * @param[in] Buad: the vaule of Buad ,the Buad must be more than 17
  * @retval none
  * @note none
  */  
void ADC_ConfigBuad(uint16_t Buad) 
{
	assert_param(IS_BAUD(Buad));
	ADC->ADCBAUD &=~(0xFFFFUL);
	ADC->ADCBAUD |= Buad;
}
/**
  * @brief  Config the CMP High value,if ADC input Value Higher than DTHH,the RFB of STS SET
  * @param[in] DTHH: the vaule of compare(range of 0-4095)
  * @retval none
  * @note none
  */ 
void ADC_ConfigCmpHighData(uint16_t DTHH)
{
	assert_param(IS_DTHH(DTHH));
	ADC->ADCDCMP&=~(0xFFFUL<<16);
	ADC->ADCDCMP|= DTHH<<16;
}

/**
  * @brief  Config the CMP Low value,if ADC input Value Lower than DTHL,the RFB of STS SET
  * @param[in] DTHL: the vaule of compare(range of 0-4095)
  * @retval none
  * @note none
  */ 
void ADC_ConfigCmpLowData(uint16_t DTHL)
{
	assert_param(IS_DTHL(DTHL));
	ADC->ADCDCMP&=~(0xFFFUL);
	ADC->ADCDCMP|= DTHL;
}
/**
  * @brief  Get ADC's Data
  * @param[in] none
  * @retval uint16_t : ADC return Value(range of 0-4095)
  * @note none
  */
uint16_t ADC_GetData(void)
{
	return ADC->ADCDAT;
}
/**
  * @brief  Get ADC's Status
  * @param[in] Flag: the Flag of Status
  *	  @arg ADC_STS_RBF
  *	  @arg ADC_STS_OVER_FLOW
  * @retval FlagStatus SET or RESET
  * @note none
  */
FlagStatus ADC_GetStatus(uint32_t Flag)
{
	if(ADC->ADCSTS&Flag)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clear ADC's Status
  * @param[in] Flag: the Flag of Status
  *	  @arg ADC_STS_RBF
  *	  @arg ADC_STS_OVER_FLOW
  * @retval FlagStatus 
  * @note none
  */
void ADC_ClearStatus(uint32_t Flag)
{
	ADC->ADCSTS = Flag;
}

/**
  * @brief  Config ADC's interrupt 
  * @param[in] Flag: the IT of ADC
  *	  @arg ADC_IT_RBF
  *	  @arg ADC_IT_OVER_FLOW
  * @retval FlagStatus 
  * @note none
  */
void ADC_ConfigIT(uint32_t Flag,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		ADC->ADCMSK &= ~Flag;
	}
	else
	{
		ADC->ADCMSK |= Flag;
	}
}

/**
  * @brief  config ADC's DMA Enable or Disable
  * @param[in]  DMA_Channel : DMA Channel request
  *		@arg	ADC_DMA_CHANNEL1
  *		@arg	ADC_DMA_CHANNEL2
  *		@arg	ADC_DMA_CHANNEL3
  * @param[in]  NewState: new state of the ADC peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */ 
void ADC_DMA_Cmd(uint32_t DMA_Channel,FunctionalState NewState)
{
	assert_param(IS_DMA_ENABLE(DMA_Channel));
	if(NewState == ENABLE)
	{
		ADC->ADCDMACFG|=DMA_Channel;
	}
	else
	{
		ADC->ADCDMACFG&=~DMA_Channel;
	}
}

/**
  * @brief  config the setting of Channel0 of DMA
  * @param[in]  DMA_Channel : 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6
  * @param[in]  NewState: new state of the ADC peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */ 
void ADC_Config_DMA_Channel1(uint32_t DMA_Channel)
{
	assert_param(IS_CHANNEL_CHECK(DMA_Channel));
	ADC->ADCDMACFG&=~(0x0FU);
	ADC->ADCDMACFG|=DMA_Channel;
}

/**
  * @brief  config the setting of Channel0 of DMA
  * @param[in]  DMA_Channel : 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6
  * @param[in]  NewState: new state of the ADC peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */ 
void ADC_Config_DMA_Channel2(uint32_t DMA_Channel)
{
	assert_param(IS_CHANNEL_CHECK(DMA_Channel));
	ADC->ADCDMACFG&=~(0xF0U);
	ADC->ADCDMACFG|=DMA_Channel<<4;
}

/**
  * @brief  config the setting of Channel0 of DMA
  * @param[in]  DMA_Channel : 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL0
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL1
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL3
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL4
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL7
  * @param[in]  NewState: new state of the ADC peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */ 
void ADC_Config_DMA_Channel3(uint32_t DMA_Channel)
{
	assert_param(IS_CHANNEL_CHECK(DMA_Channel));
	ADC->ADCDMACFG&=~(0x0F00);
	ADC->ADCDMACFG|=DMA_Channel<<8;
}
///**
//  * @brief Config ADC's DMA Mode
//  * @param[in]  DMA_Channel : DMA Channel request
//  *		@arg	ADC_DMA_CHANNEL0
//  *		@arg	ADC_DMA_CHANNEL1
//  *		@arg	ADC_DMA_CHANNEL2
//  * @param[in]  ADC_Channel : DMA Channel request
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL0
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL1
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL3
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL4
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6
//  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL7
//  * @param[in]  NewState: new state of the ADC peripheral.
//  *   This parameter can be: ENABLE or DISABLE.
//  * @retval none
//  * @note	none
//  */ 
//void ADC_ConfigDMA(uint32_t DMA_Channel,uint32_t ADC_Channel,FunctionalState NewState)
//{
//	if(NewState == ENABLE)
//	{
//		if(DMA_Channel == ADC_DMA_CHANNEL0)
//		{
//			ADC->ADCDMACFG|=ADC_DMA_CHANNEL0;
//			ADC->ADCDMACFG&= ~(0x0FU);
//			switch(ADC_Channel)
//			{
//				case ADC_DMA_CONFIG_ADC_CHANNEL0:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL0;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL1:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL1;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL2:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL2;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL3:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL3;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL4:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL5:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL5;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL6:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL6;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL7:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL7;break;
//				default:break;
//			}
//		}
//		else if(DMA_Channel == ADC_DMA_CHANNEL1)
//		{
//			ADC->ADCDMACFG|=ADC_DMA_CHANNEL1;
//			ADC->ADCDMACFG&= ~(15UL<<4);
//			switch(ADC_Channel)
//			{
//				case ADC_DMA_CONFIG_ADC_CHANNEL0:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL0<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL1:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL1<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL2:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL2<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL3:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL3<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL4:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL4<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL5:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL5<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL6:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL6<<4;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL7:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL7<<4;break;
//				default:break;
//			}
//		}
//		else if(DMA_Channel == ADC_DMA_CHANNEL2)
//		{
//			ADC->ADCDMACFG|=ADC_DMA_CHANNEL2;
//			ADC->ADCDMACFG&= ~(15UL<<8);
//			switch(ADC_Channel)
//			{
//				case ADC_DMA_CONFIG_ADC_CHANNEL0:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL0<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL1:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL1<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL2:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL2<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL3:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL3<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL4:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL4<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL5:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL5<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL6:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL6<<8;break;
//				case ADC_DMA_CONFIG_ADC_CHANNEL7:ADC->ADCDMACFG|=ADC_DMA_CONFIG_ADC_CHANNEL7<<8;break;
//				default:break;
//			}
//		}
//	}
//	else
//	{
//		switch(DMA_Channel)
//		{
//			case ADC_DMA_CHANNEL0:ADC->ADCDMACFG&=~ADC_DMA_CHANNEL0;break;
//			case ADC_DMA_CHANNEL1:ADC->ADCDMACFG&=~ADC_DMA_CHANNEL1;break;
//			case ADC_DMA_CHANNEL2:ADC->ADCDMACFG&=~ADC_DMA_CHANNEL2;break;
//			default:break;
//		}
//	}
//}

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

