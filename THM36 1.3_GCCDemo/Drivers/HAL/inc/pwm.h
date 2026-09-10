/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwm.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PWM_H
#define _PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup PWM
  * @brief PWM HAL modules driver 
  * @{
  */ 
/** @defgroup PWM_Exported_Constants PWM Exported Constants
  * @{
  */
/** 
  * @brief  PWM Init structure definition
  * @note   This sturcture is used with  PWM   
  */	 
typedef struct
{
	uint32_t PWM_Start_Level; /*!< PWM's Start Level, This parameter can be a value of @ref PWM_START_LEVEL*/
	uint32_t PWM_Stop_Level;  /*!< PWM's STOP Level, This parameter can be a value of @ref PWM_STOP_LEVEL*/
	uint8_t PWM_Div;		  /*!< PWM CLK DIV, This parameter can be a value of @ref PWM_CLK_DIV*/
	uint8_t PWM_PSC;		  /*!< PWM's PSC, the Value is 0-0xFF*/
	uint32_t PWM_DTY;		  /*!< PWM's Phase*/
	uint32_t PWM_PER;		  /*!< PWM's cycle*/
} PWM_InitTypeDef;


/** 
  * @brief PWM_START_LEVEL PWM START LEVEL
  */
#define		START_LEVEL_HIGH	(1UL)
#define		START_LEVEL_LOW		~(1UL)
/**
  * @}
  */

/** 
  * @brief PWM_STOP_LEVEL PWM STOP LEVEL
  */
#define		STOP_LEVEL_IS_NOW		(~2UL)
#define		STOP_LEVEL_IS_START		(2UL)
/**
  * @}
  */

/** 
  * @brief PWM_CLK_DIV	PWM CLK DIV
  */
#define		PWM_CLK_DIV1	0
#define		PWM_CLK_DIV2	1
#define		PWM_CLK_DIV3	2
#define		PWM_CLK_DIV4	3
#define		PWM_CLK_DIV5	4
#define		PWM_CLK_DIV6	5
#define		PWM_CLK_DIV7	6
#define		PWM_CLK_DIV8	7
/**
  * @}
  */

/** @defgroup PWM_Private_Macros PWM Private Macros
  * @{
  */
#define  IS_PWM_START_LEVEL(x)	((x == START_LEVEL_HIGH)||(x == START_LEVEL_LOW))
#define  IS_PWM_STOP_LEVEL(x)	((x == STOP_LEVEL_IS_NOW)||(x == STOP_LEVEL_IS_START))

#define  IS_PWM_DIV(x)			((x == PWM_CLK_DIV1)||(x == PWM_CLK_DIV2)|| \
								(x == PWM_CLK_DIV3)||(x == PWM_CLK_DIV4) || \
								(x == PWM_CLK_DIV5)||(x == PWM_CLK_DIV6) || \
								(x == PWM_CLK_DIV7)||(x == PWM_CLK_DIV8))
								
#define IS_PWM_PSC(x)			(x <=255)
#define IS_PWM_DTY(x)			((x >=1)&&(x <=0xFFFF))
#define IS_PWM_PER(x)			((x >=1)&&(x <=0xFFFF))
/**
  * @}
  */
/** 
  * @brief PWM_Flag PWM Flag
  */
#define	 PWM_OUT_HIGH				(4UL)
#define		PWM_STS_DTY_FINISH			(2UL)
#define		PWM_STS_PER_FINISH			(1UL)
/**
  * @}
  */
  
/** @addtogroup PWM_Exported_Function 
  * @{  
  */   
void PWM_StructInit(PWM_InitTypeDef *PWM_InitStruct);
void PWM_Initial(PWM_TypeDef* PWMx,PWM_InitTypeDef *PWM_InitStruct);
void PWM_DeInitial(PWM_TypeDef* PWMx);
void PWM_ConfigStartLevel(PWM_TypeDef* PWMx,uint32_t Level);
void PWM_ConfigStopLevel(PWM_TypeDef* PWMx,uint32_t Level);
void PWM_Cmd(PWM_TypeDef* PWMx,FunctionalState NewState);
void PWM_ConfigIT(PWM_TypeDef* PWMx,uint32_t Flag,FunctionalState NewState);
FlagStatus PWM_GetStatus(PWM_TypeDef* PWMx,uint32_t Flag);
void PWM_ClearStatus(PWM_TypeDef* PWMx,uint32_t Flag);
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
#endif /* __TMC_PWM_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
