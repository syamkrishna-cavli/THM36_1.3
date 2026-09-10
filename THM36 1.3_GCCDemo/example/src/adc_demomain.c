/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: adc_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
#include "thm36_1.3.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "tmc_dma.h"
#include "tmc_adc.h"
#include "tmc_gpio.h"
#include "adc_demomain.h"
#include "adc.h"


ADC_HandleTypeDef TMC_ADC;
ADC_InitTypeDef	  ADC_Init; 
ADC_AMP_InitTypeDef ADC_AMP_Init;
GPIO_InitTypeDef GPIOB_Init;
GPIO_InitTypeDef GPIOC_Init;

DMA_HandleTypeDef ADC_DMA2_Init;
DMA_HandleTypeDef ADC_DMA5_Init;
DMA_HandleTypeDef ADC_DMA6_Init;

uint16_t Adc2Buffer[1024];
uint16_t Adc5Buffer[1024];
uint16_t Adc6Buffer[1024];
uint16_t AdcxBuffer[1024];
uint16_t ADCx_Complete_Flag;
uint16_t ADC2_Complete_Flag;
uint16_t ADC5_Complete_Flag;
uint16_t ADC6_Complete_Flag;


typedef enum 
{
	ADC_Demo_Polling 	= 0,
	ADC_PollingWithChannelSelect	= 1,
	ADC_Demo_IRQ	  	= 2,
	ADC_Demo_DMA	  	= 3
} ADC_Demo_TypefDef;

/**
	* ADC polling: Only configure 3 channels,each channel can be set for each ADC channel of ADC channel 0-7.
	* ADC polling with Channel select: Only configure 3 channels,but only get the data of channel 2.each channel can be set for each ADC channel of ADC channel 0-7.
	* ADC DMA mode: Only support 3 DMA channels,each channel can be set for each ADC channel of ADC channel 0-7.
	* ADC Interrupt mode:Can enable the all 8 channels,the ADC may sample the channels for channel 0 to channel 7.
*/

void ADC_DeomMain(void)
{
	ADC_Demo_TypefDef Flag;
	Flag = ADC_Demo_Polling;

	if(Flag == ADC_Demo_Polling)
	{
		ADC_Polling_Demo();     //ADM Polling.
	}
	if(Flag == ADC_PollingWithChannelSelect)   
	{
		ADC_SelSingleChannel_Demo();    //ADC Select one channel for three active channel.Ignore the data of other two channel.
	}
	if(Flag == ADC_Demo_IRQ)
	{
		ADC_IRQ_Demo();         //ADC Interrupt.
	}
	if(Flag == ADC_Demo_DMA)
	{
		ADC_DMA_Demo();         //ADC DMA.
	}
}


void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
	ADC_InitStruct->CMP_Mode = ADC_NOT_CMP;
	ADC_InitStruct->Divsion = 2;
	ADC_InitStruct->SAMPLEWIDTH = 16;
	ADC_InitStruct->Buad = 32;
	ADC_InitStruct->ADC_Channelx = ADC_Channel2|ADC_Channel5|ADC_Channel6;
	ADC_InitStruct->ADC_Mode = ADC5_CHOISE_ADC|ADC6_CHOISE_ADC;
	
}


void ADC_Polling_Demo(void)
{
	TMC_ADC.Init = &ADC_Init;
	TMC_ADC.AMP_Init = &ADC_AMP_Init;
	ADC_StructInit(TMC_ADC.Init);
	ADC_AMP_StructInit(TMC_ADC.AMP_Init);
	TMC_ADC_StructInit(&TMC_ADC);
	
	TMC_ADC_Init(&TMC_ADC);
	TMC_ADC_Start(&TMC_ADC,AdcxBuffer,1024,4000);
 	TMC_ADC_Stop(&TMC_ADC);
	TMC_ADC_DeInit(&TMC_ADC);
}

void ADC_SelSingleChannel_Demo(void)
{
	TMC_ADC.Init = &ADC_Init;
	TMC_ADC.AMP_Init = &ADC_AMP_Init;
	ADC_StructInit(TMC_ADC.Init);
	ADC_AMP_StructInit(TMC_ADC.AMP_Init);
	TMC_ADC_StructInit(&TMC_ADC);
	
	TMC_ADC_Init(&TMC_ADC);
	TMC_ADC_SingleChannelStart(&TMC_ADC,AdcxBuffer,1024,ADC_ConvertingChannel2,4000);
	TMC_ADC_Stop(&TMC_ADC);
	TMC_ADC_DeInit(&TMC_ADC);
}

void ADC_IRQ_Demo(void)
{
	TMC_ADC.Init = &ADC_Init;
	TMC_ADC.AMP_Init = &ADC_AMP_Init;
	ADC_StructInit(TMC_ADC.Init);
	ADC_AMP_StructInit(TMC_ADC.AMP_Init);
	TMC_ADC_StructInit(&TMC_ADC);
	TMC_ADC_Init(&TMC_ADC);
	TMC_ADC_Start_IT(&TMC_ADC,AdcxBuffer,1024);
	while(TMC_ADC.State == TMC_ADC_STATE_BUSY);
	TMC_ADC_Stop_IT(&TMC_ADC);
	TMC_ADC_DeInit(&TMC_ADC);
}


void ADC_DMA_Demo(void)
{
	TMC_ADC.Init = &ADC_Init;
	TMC_ADC.AMP_Init = &ADC_AMP_Init;
	ADC_StructInit(TMC_ADC.Init);
	ADC_AMP_StructInit(TMC_ADC.AMP_Init);
	
	ADC_DMA2_Init.Instance = DMA_Channel0;
	ADC_DMA2_Init.Init.DestInc = DMA_DEST_INC_ENABLE;
	ADC_DMA2_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_HALFWORD;
	ADC_DMA2_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
	ADC_DMA2_Init.Init.RequestSelect = DMA_REQUEST_ADC1;
	ADC_DMA2_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_HALFWORD;
	ADC_DMA2_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
	ADC_DMA2_Init.Init.IT_Enable = DMA_IT_ENABLE;
	ADC_DMA2_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	ADC_DMA2_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
	
	ADC_DMA5_Init.Instance = DMA_Channel1;
	ADC_DMA5_Init.Init.DestInc = DMA_DEST_INC_ENABLE;
	ADC_DMA5_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_HALFWORD;
	ADC_DMA5_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
	ADC_DMA5_Init.Init.RequestSelect = DMA_REQUEST_ADC2;
	ADC_DMA5_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_HALFWORD;
	ADC_DMA5_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
	ADC_DMA5_Init.Init.IT_Enable = DMA_IT_ENABLE;
	ADC_DMA5_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	ADC_DMA5_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
	
	ADC_DMA6_Init.Instance = DMA_Channel2;
	ADC_DMA6_Init.Init.DestInc = DMA_DEST_INC_ENABLE;
	ADC_DMA6_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_HALFWORD;
	ADC_DMA6_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
	ADC_DMA6_Init.Init.RequestSelect = DMA_REQUEST_ADC3;
	ADC_DMA6_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_HALFWORD;
	ADC_DMA6_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
	ADC_DMA6_Init.Init.IT_Enable = DMA_IT_ENABLE;
	ADC_DMA5_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	ADC_DMA5_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;

	TMC_ADC_StructInit(&TMC_ADC);
	TMC_ADC_Init(&TMC_ADC);
	ADC_Cmd(ENABLE);
	/*ADC Channel 0 DMA mode.*/
	TMC_ADC.hdma0= &ADC_DMA2_Init;
	ADC2_Complete_Flag = 0;
	TMC_DMA_Init(TMC_ADC.hdma0);
	TMC_ADC_Start_HDMA0_REQUEST_ADC1_IT(&TMC_ADC,ADC_DMA_CONFIG_ADC_CHANNEL2,Adc2Buffer, 1024,0);
	/*ADC Channel 1 DMA mode.*/
	TMC_ADC.hdma1= &ADC_DMA5_Init;
	ADC5_Complete_Flag = 0;
	TMC_DMA_Init(TMC_ADC.hdma1);
	TMC_ADC_Start_HDMA1_REQUEST_ADC2_IT(&TMC_ADC,ADC_DMA_CONFIG_ADC_CHANNEL5,Adc5Buffer, 1024,0);
	/*ADC Channel 2 DMA mode.*/
	TMC_ADC.hdma2 = &ADC_DMA6_Init;
	ADC6_Complete_Flag = 0;
	TMC_DMA_Init(TMC_ADC.hdma2);
	TMC_ADC_Start_HDMA2_REQUEST_ADC3_IT(&TMC_ADC,ADC_DMA_CONFIG_ADC_CHANNEL6,Adc6Buffer, 1024,0);
	
	while(!ADC2_Complete_Flag);
	while(!ADC5_Complete_Flag);
	while(!ADC6_Complete_Flag);
	ADC_Cmd(DISABLE);
	TMC_ADC_Stop_HDMA0_REQUEST_ADC1_IT(&TMC_ADC);
	TMC_ADC_Stop_HDMA1_REQUEST_ADC2_IT(&TMC_ADC);
	TMC_ADC_Stop_HDMA2_REQUEST_ADC3_IT(&TMC_ADC);
	TMC_ADC_DeInit(&TMC_ADC);
}


/**
  * @brief  Initialize the ADC MSP.
  * @param  hspi pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
void TMC_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	RCC_HSICmd(ENABLE);
	/*Enable ADC clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC,ENABLE);
	/*Enable ADC sampling clock*/
	RCC_ADCClockConfig(RCC_ADC_CLKSRC_PLL_L, ENABLE);
	
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA,ENABLE);

	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);
	
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
}

/**
  * @brief  DeInitialize the ADC MSP.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
void TMC_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
	/*Enable ADC clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC,DISABLE);
	/*Enable ADC sampling clock*/
	//RCC_ADCClockConfig(RCC_ADC_CLKSRC_PLL_L, DISABLE);
		
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_DisableIRQ(ADC_IRQn);
}

/**
  * @brief  TMC ADC0 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
void TMC_ADCx_RxCpltCallback(ADC_HandleTypeDef *hadc)
{	
	ADCx_Complete_Flag = 1;
}


/**
  * @brief TMC ADC0 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
void TMC_ADCx_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	TMC_ADC_StructInit(hadc);
	TMC_ADC_DeInit(hadc);
	ADCx_Complete_Flag = 2;

}

/**
  * @brief  TMC ADC0 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
void TMC_ADC_HDMA0_RxCpltCallback(ADC_HandleTypeDef *hadc)
{
	ADC2_Complete_Flag = 1;
}


/**
  * @brief TMC ADC0 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
void TMC_ADC_HDMA0_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	TMC_ADC_StructInit(hadc);
	TMC_ADC_DeInit(hadc);
	ADC2_Complete_Flag = 2;

}

/**
  * @brief  TMC ADC1 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
void TMC_ADC_HDMA1_RxCpltCallback(ADC_HandleTypeDef *hadc)
{	
	ADC5_Complete_Flag = 1;
}


/**
  * @brief TMC ADC1 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
void TMC_ADC_HDMA1_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	TMC_ADC_StructInit(hadc);
	TMC_ADC_DeInit(hadc);
	ADC5_Complete_Flag = 2;
}

/**
  * @brief  TMC ADC2 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
void TMC_ADC_HDMA2_RxCpltCallback(ADC_HandleTypeDef *hadc)
{

	ADC6_Complete_Flag = 1;
}


/**
  * @brief TMC ADC2 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
void TMC_ADC_HDMA2_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	TMC_ADC_StructInit(hadc);
	TMC_ADC_DeInit(hadc);
	ADC6_Complete_Flag = 2;
	
}

void ADC_DMA_Handle(void)
{
	if(DMA_GetITStatus(DMA_IT_GL0) == SET)
	{
		TMC_DMA_IRQHandler(TMC_ADC.hdma0);
	}
	if(DMA_GetITStatus(DMA_IT_GL1) == SET)
	{
		TMC_DMA_IRQHandler(TMC_ADC.hdma1);
	}
	if(DMA_GetITStatus(DMA_IT_GL2) == SET)
	{
		TMC_DMA_IRQHandler(TMC_ADC.hdma2);
	}
}

void ADC_InterruptHandle(void)
{

	TMC_ADC_IRQHandler(&TMC_ADC);
}

void ADC_Deom_Main(void)
{
	ADC_DeomMain();
}
