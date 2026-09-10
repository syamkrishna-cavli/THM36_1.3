/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: adc.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADC_H
#define _ADC_H

#ifdef __cplusplus
 extern "C" {
#endif
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
/** @defgroup ADC_Exported_Constants SPI Exported Constants
  * @{
  */
/** 
  * @brief ADC_Channel_Select 
  */
typedef enum {
				ADC_Channel0 = 1,
				ADC_Channel1 = 2,
				ADC_Channel2 = 4,
				ADC_Channel3 = 8,
				ADC_Channel4 = 16,
				ADC_Channel5 = 32,
				ADC_Channel6 = 64,
				ADC_Channel7 = 128,
			 } ADC_Channelx;

typedef enum {
	ADC_ConvertingChannel0 = 1,
	ADC_ConvertingChannel1,
	ADC_ConvertingChannel2,
	ADC_ConvertingChannel3,
	ADC_ConvertingChannel4,
	ADC_ConvertingChannel5,
	ADC_ConvertingChannel6,
	ADC_ConvertingChannel7,
} ADC_ConvertingChannelx;
			 
/** 
  * @brief  ADC Init structure definition
  * @note   This sturcture is used with  ADC   
  */	 
typedef struct
{
	uint32_t		ADC_Channelx;	 /*!< the Number of ADC_Channelx Enable,the Max can enable 8 channel,This parameter can be a value of @ref ADC_Channel_Select */
	uint8_t			Divsion;		 /*!< the Clock Divsion of Samping. the range is form 0 to 255 ,please refer Date sheet*/
	uint8_t			SAMPLEWIDTH;  	 /*the value of SAMPLEWIDTH is the clock of ADC Analog Sampling, the min value of Buad - SAMPLEWIDTH is 13*/
	uint16_t		Buad;			 /*the number of  samping clock required for a single ADC Samp the range is form 16 to 65535 ,please refer Date sheet*/
	uint32_t		CMP_Mode;			 /*!<the mode of ADC,This parameter can be a value of @ref ADC_Mode*/
	uint32_t		ADC_Mode;			 /*!<the mode of ADC,This parameter can be a value of @ref ADC_Mode*/
} ADC_InitTypeDef;


typedef struct
{
	uint32_t ADC_AMP_Select;			/*!< Select ADC AMP,This parameter can be a value of @ref ADC_AMP_Mode*/
	uint32_t ADC_AMP_Mode;				/*!< select ADC AMP mode,This parameter can be a value of @ref ADC_AMP_Mode*/
	uint32_t ADC_AMP_CMP;				/*!< Central Level Regulation,This parameter can be a value of @ref ADC_AMP_CMP_Mode*/
	uint32_t ADC_AMP_ADJ;				/*!< ADC Magnification,This parameter can be a value of @ref ADC_AMP_Magnification*/
} ADC_AMP_InitTypeDef;


/** 
  * @brief ADC_AMP_Select ADC AMP Select
  */
#define		ADC_AMP0		(1UL)
#define		ADC_AMP1	 	(1UL<<1)
#define		ADC_AMP2 		(1UL<<2)
/**
  * @}
  */

/** 
  * @brief ADC_AMP_Mode ADC AMP Mode
  */
#define		ADC_AMP_MODE_BYPASS				 (1UL<<2)
#define		ADC_AMP_MODE_AMPLIFICATION	 	~(1UL<<2)
/**
  * @}
  */
/** 
  * @brief ADC_AMP_CMP_Mode ADC AMP CMP Mode
  */
#define	ADC_AMP_CENTRAL_LEVEL_40_PERCENT	(3UL<<4)	
#define	ADC_AMP_CENTRAL_LEVEL_45_PERCENT	(1UL<<5)
#define ADC_AMP_CENTRAL_LEVEL_55_PERCENT	(1UL<<4)
#define ADC_AMP_CENTRAL_LEVEL_50_PERCENT	(0UL)
/**
  * @}
  */

/** 
  * @brief ADC_AMP_Magnification ADC AMP Magnification
  */
#define	ADC_AMP_MAG_2		(0UL)
#define	ADC_AMP_MAG_3		(1UL)
#define	ADC_AMP_MAG_5		(2UL)
#define	ADC_AMP_MAG_8		(3UL)
#define	ADC_AMP_MAG_12		(4UL)
#define	ADC_AMP_MAG_21		(5UL)
#define	ADC_AMP_MAG_29		(6UL)
#define	ADC_AMP_MAG_44		(7UL)
/**
  * @}
  */

/** 
  * @brief CMP_Mode CMP Mode
  */
#define		ADC_CMP			(1UL<<5)
#define		ADC_NOT_CMP	 	~(1UL<<5)
/**
  * @}
  */

/** 
  * @brief ADC_Mode ADC Mode
  */
#define		ADC5_CHOISE_ADC		0
#define 	ADC5_CHOISE_MAG		(1UL<<16)

#define		ADC6_CHOISE_ADC		0
#define 	ADC6_CHOISE_MAG		(1UL<<17)

#define		ADC7_CHOISE_ADC		0
#define 	ADC7_CHOISE_MAG		(1UL<<18)
/**
  * @}
  */

/** 
  * @brief ADC_Flag ADC Flag
  */
#define		ADC_STS_RBF				(1UL)
#define		ADC_STS_OVER_FLOW		(2UL)
/**
  * @}
  */

/** 
  * @brief ADC_IT ADC IT
  */
#define		ADC_IT_RBF				(1UL)
#define		ADC_IT_OVER_FLOW		(2UL)
/**
  * @}
  */

/** 
  * @brief ADC_DMA ADC DMA
  */
#define	ADC_DMA_CHANNEL1	(1UL<<12)
#define	ADC_DMA_CHANNEL2	(1UL<<13)
#define	ADC_DMA_CHANNEL3	(1UL<<14)
/** 
  * @brief ADC_CHANNELx	 ADC CHANNELx
  */
#define ADC_CONFIG_DMA_DISABLE			0
#define ADC_DMA_CONFIG_ADC_CHANNEL0		1U
#define ADC_DMA_CONFIG_ADC_CHANNEL1		2U
#define ADC_DMA_CONFIG_ADC_CHANNEL2		3U
#define ADC_DMA_CONFIG_ADC_CHANNEL3		4U
#define ADC_DMA_CONFIG_ADC_CHANNEL4		5U
#define ADC_DMA_CONFIG_ADC_CHANNEL5		6U
#define ADC_DMA_CONFIG_ADC_CHANNEL6		7U	
#define ADC_DMA_CONFIG_ADC_CHANNEL7		8U
/**
  * @}
  */
/**
  * @}
  */
/** @defgroup ADC_Private_Macros ADC Private Macros
  * @{
  */
#define  IS_CHANNEL_CHECK(x)  ((x&ADC_Channel0)||(x&ADC_Channel1)||(x&ADC_Channel2)||(x&ADC_Channel3)||(x&ADC_Channel4)|| \
							   (x&ADC_Channel5)||(x&ADC_Channel6)||(x&ADC_Channel7))
#define  IS_DIVSION(x)  	(x<=255)

#define  IS_BAUD(x)			((x>=0x10)&&(x<=0xFFFF))
#define  IS_MODE(x)			((x== ADC_CMP)||(x== ADC_NOT_CMP))
#define  IS_DTHH(x)			(x<=0xFFF)
#define  IS_DTHL(x)			(x<=0xFFF)
#define  IS_DMA_ENABLE(x)	((x == ADC_DMA_CHANNEL1)||(x == ADC_DMA_CHANNEL2)||(x == ADC_DMA_CHANNEL3))

#define  IS_AMP_MODE_CHECK(x)	 ((x == ADC_AMP_MODE_BYPASS)||(x == ADC_AMP_MODE_AMPLIFICATION))
#define  IS_AMP_CHECK(x)	 	 (((x &ADC_AMP0)== ADC_AMP0)||((x &ADC_AMP1) == ADC_AMP1)||((x &ADC_AMP2) == ADC_AMP2))
#define  IS_AMP_CMP_CHECK(x) ((x == ADC_AMP_CENTRAL_LEVEL_40_PERCENT)|| \
							  (x == ADC_AMP_CENTRAL_LEVEL_45_PERCENT)|| \
							  (x == ADC_AMP_CENTRAL_LEVEL_50_PERCENT)|| \
							  (x == ADC_AMP_CENTRAL_LEVEL_55_PERCENT))
#define  IS_AMP_ADJ_CHECK(x) ((x == ADC_AMP_MAG_2)|| \
							  (x == ADC_AMP_MAG_3)|| \
							  (x == ADC_AMP_MAG_5)|| \
							  (x == ADC_AMP_MAG_8)|| \
							  (x == ADC_AMP_MAG_12)|| \
							  (x == ADC_AMP_MAG_21)|| \
							  (x == ADC_AMP_MAG_29)|| \
							  (x == ADC_AMP_MAG_44))
#define  IS_MAG(x)  		  ((x&ADC5_CHOISE_MAG )|| \
							   (x&ADC6_CHOISE_MAG )|| \
							   (x&ADC7_CHOISE_MAG )|| \
							   (x==0))
/**
  * @}
  */
/** @addtogroup ADC_Exported_Function 
  * @{  
  */   
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct);
void ADC_Initial(ADC_InitTypeDef *ADC_InitStruct);
void ADC_DeInitial(void);
void ADC_AnalogCmd(FunctionalState NewState);
void ADC_Cmd(FunctionalState NewState);
void ADC_Channel_Select(uint32_t Channelx);
void ADC_ConfigCMPMode(FunctionalState NewState);
void ADC_ConfigPSC(uint8_t div);
void ADC_ConfigSampleWidth(uint8_t Samp);
void ADC_ConfigBuad(uint16_t Buad);
void ADC_ConfigCmpHighData(uint16_t DTHH);
void ADC_ConfigCmpLowData(uint16_t DTHL);
uint16_t ADC_GetData(void);
FlagStatus ADC_GetStatus(uint32_t Flag);
void ADC_ClearStatus(uint32_t Flag);
void ADC_ConfigIT(uint32_t Flag,FunctionalState NewState);
void ADC_Channel_CMP(ADC_Channelx Channel);
void ADC_DMA_Cmd(uint32_t DMA_Channel,FunctionalState NewState);
void ADC_Config_DMA_Channel1(uint32_t DMA_Channel);
void ADC_Config_DMA_Channel2(uint32_t DMA_Channel);
void ADC_Config_DMA_Channel3(uint32_t DMA_Channel);
void ADC_AMP_Cmd(uint32_t ADC_Amp,FunctionalState NewState);
void ADC_AMP_Initial(ADC_AMP_InitTypeDef *ADC_AMP_InitStruct);
void ADC_AMP_StructInit(ADC_AMP_InitTypeDef *ADC_AMP_InitStruct);
void ADC_ConfigMag(uint32_t MagMode);
//void ADC_ConfigDMA(uint32_t DMA_Channel,uint32_t ADC_Channel,FunctionalState NewState);

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
#endif /* __TMC_ADC_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
