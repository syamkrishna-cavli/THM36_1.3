/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ISO7816M.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _7816M_H
#define _7816M_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @addtogroup 7816M 
  * @brief 7816M driver modules
  * @{
  */ 
/** @defgroup 7816M_Exported_Types 7816M Exported Types 
  * @{
  */
	 
/** @defgroup M7816_TOC_MODE M7816 TOC MODE
  * @{
  */
#define M7816x_TIM_STOP_TO1_TO2_TO3	 	0x00	/*STOP TM1,TM2 TM3*/
#define M7816x_TIM_STOP_TO2_TO3	 		0x05	/*STOP TM2 TM3*/
	 
#define M7816x_TIM_MODE2				0x05	/*STOP TM2 and TM3,TM1 working,auto Load*/
#define M7816x_TIM_MODE3				0x61	/*TM1 STOP Count,TIM2+TIM3 make up to 16Bit Timer,write value to TOR2+TOR3 ,the Timer start count,
											  the Timer count to 0,make a interrupt,TOC3 Flag Set.before write new value to TOR2+TOR3,write 0 to TOC*/
#define M7816x_TIM_MODE4				0x65	/*TIM1 make up 8Bit Timer,auto,load,TIM2+TIM3 make up to 16Bit Timer,when get Frist bit form IO(receive and transmit),
											  the TIM1 start count,TIM1 count to 0,make a interrupt of TO1,and auto Load. TIM2+TIM3 start count,when the value write to Timer,
											  the Timer is counted to 0,make a interrupt.write 0 to TOC,STOP TIM1+TIM2+TIM3.write 0x05 to TOC,STOP TIM2+TIM3.if write new value to TIM2+TIM3,need to stop TIM2+TIM3*/
#define M7816x_TIM_MODE5				0x68	/*TIM1+TIM2+TIM2 make a 24Bit Timer,when write a new value to Timer,the Timer start count. write 0 to stop timer*/
#define M7816x_TIM_MODE6				0x71	/*TIM1 STOP Count,TIM2+TIM3 make up 16BIT Timer.write value to Timer,when get start bit of IO(Receive and Transmit),the Timer start count,when get next start bit, Restart Count of inital value 
											  write 0 to TOC,Stop Timer*/
#define M7816x_TIM_MODE7				0x75	/*TIM1 is 8Bit Timer,atuo Laod.TIM2+TIM3 make up 16BIT Timer,when get frist start bit of IO(receive and Transmit), the TIM1 start count,count to 0,make a interrupt,auto load inital value,
											 when get frist start bit of IO(receive and Transmit),TIM2+TIM3 start count.if get a new start bit of IO,TIM2+TIM3 Restart count*/
#define M7816x_TIM_MODE8				0x7C	/*TIM1+TIM2+TIM3 make up a 16Bit Timer,write the value to TOR1+TOR2+TOR3,when get a strt bit of IO(receive and Transmit),start to count,when get a new start bit,Restart count*/
#define M7816x_TIM_MODE9				0x85	/*it is same as M7816x_TIM_MODE2,the different of M7816x_TIM_MODE2 is when get a start bit and after 12ETU,the Timer is over */
#define M7816x_TIM_MODE10				0xE5	/*it is same as M7816x_TIM_MODE4,the different of M7816x_TIM_MODE4 is when get a start bit and after 12ETU,the Timer is over*/
#define M7816x_TIM_MODE11				0xF1	/*it is same as M7816x_TIM_MODE6,the different of M7816x_TIM_MODE6 is when get a start bit and after 12ETU,the Timer is over*/
#define M7816x_TIM_MODE12				0xF5	/*it is same as M7816x_TIM_MODE7,the different of M7816x_TIM_MODE7 is when get a start bit and after 12ETU,the Timer is over*/
	 
/**
  * @}
  */

/** @defgroup M7816_protol M7816 protol
  * @{
  */
typedef enum 
{
	T0 = 0,
	T1 = 1<<4
} M7816_protol;

/**
  * @}
  */

/** @defgroup M7816_ATR_Coding_Mode M7816 ATR Coding Mode
  * @{
  */
typedef enum 
{
	Hardware_Mode = 0,
	Software_Mode = 1<<2
} M7816_ATR_Code_Mode;

/**
  * @}
  */

/** @defgroup M7816_ATR_Software_Mode M7816 ATR Software Mode
  * @{
  */
typedef enum 
{
	Forward_coding	= 1,
	Reverse_coding	= 0
} M7816_ATR_Software_Mode;

/**
  * @}
  */

/** 
  * @brief  7816M Init Structure definition  
  */
	 
typedef struct
{ 
	uint8_t BaudRate_FD;						/*!< This member configures the 7816M communication baud rate.BaudRate_FD = F<<4+D
													This parameter can be a value of @ref M7816_BaudRate_F and @ref M7816_BaudRate_D*/	
	FunctionalState DoubleBaudRate;				/*!< whether Double 7816Mx's BaudRate,example BaudRate_FD =0x11, buadrate = 31*12(1 ETU time == 376 clock), if DoubleBaudRate is ENABLE, buadrate = 31*6(1 ETU time == 188 clock)*/
	uint8_t Eorrer_Retrans_times;   			/*!< This Error retrans Times,Including receive and Transmit ,the range of value is 0 to 8*/
	uint32_t Trans_GTR;							/*!< Transmit guard time  the GTR form 0 to 0xFE the GTR time is 1 to */
	M7816_protol protol;						/*!< 7816Mx's protol is T = 0 or T = 1 
													This parameter can be a value of @ref M7816_protol*/
	M7816_ATR_Code_Mode ATR_Code_Mode;			/*!< ATR_Code_Mode is ATR Receive mode ,This parameter can be a value of @ref M7816_ATR_Coding_Mode*/
	M7816_ATR_Software_Mode ATR_SoftCode_Mode;	/*!< when ATR_Code_Mode is config Software_Mode,This parameter can be a value of @ref M7816_ATR_Software_Mode*/
	uint8_t CLK_CPOL;							/*!<the CLK's CPOL ,This parameter can be a value of @ref M7816_CLK_CPOL*/
	
} M7816x_InitTypeDef;	 	 

/** @defgroup 7816M_BaudRate_F 7816M BaudRate F
  * @{
  */
typedef enum 
{
	F_0 = 0,
	F_1 = 1,
	F_2 = 2,
	F_3 = 3,
	F_4 = 4,
	F_5 = 5,
	F_6 = 6,
	F_7 = 7,
	F_8 = 8,
	F_9 = 9,
	F_A = 10,
	F_B = 11,
	F_C = 12,
	F_D = 13
} M7816_BaudRate_F;

/**
  * @}
  */

/** @defgroup 7816M_BaudRate_D
  * @{
  */
typedef enum 
{
	D_0 = 0,
	D_1 = 1,
	D_2 = 2,
	D_3 = 3,
	D_4 = 4,
	D_5 = 5,
	D_6 = 6,
	D_7 = 7,
	D_8 = 8,
	D_9 = 9
} M7816_BaudRate_D;

/**
  * @}
  */
/** @defgroup 7816M_CLK_CPOL 7816M CLK CPOL
  * @{
  */
typedef enum 
{
	CLK_STOP_LOW = 0,
	CLK_STOP_HIGH= 1<<5,
} M7816_CLK_CPOL;

/**
  * @}
  */

/** @defgroup M7816x_PSC  M7816x PSC
  * @{
  */
#define M7816x_PSC_32	1UL
#define M7816x_PSC_31	~(1UL)
/**
  * @}
  */
/** @defgroup M7816x_STATUS M7816x STATUS
  * @{
  */
#define M7816x_PULL_UP_AFTER_PARITY		(1UL<<1)
/**
  * @}
  */

/** @defgroup M7816x_STATUS M7816x STATUS
  * @{
  */
#define M7816x_HSTS_PRTL		(1UL<<5)
#define M7816x_HSTS_PRL			((1UL)<<2)
#define M7816x_HSTS_PRES		(1UL)

#define M7816x_MSTS_BGT			(1UL<<5)	
#define	M7816x_MSTS_RBF			(2UL)		
#define	M7816x_MSTS_TBE			(1UL)	

#define	M7816x_USTS_RETRY		((1UL)<<8)
#define	M7816x_USTS_TO3			((1UL)<<7)
#define	M7816x_USTS_TO2			((1UL)<<6)
#define	M7816x_USTS_TO1			((1UL)<<5)
#define	M7816x_USTS_EA			((1UL)<<4)
#define	M7816x_USTS_PE			((1UL)<<3)
#define	M7816x_USTS_OVR			((1UL)<<2)
#define	M7816x_USTS_FER			((1UL)<<1)
#define	M7816x_USTS_TACT		(1UL)

/**
  * @}
  */
/** @defgroup M7816x_IT_Mask M7816x IT Mask
  * @{
  */
#define M7816x_HSTS_Mask_PRTL			(1UL<<5)
#define M7816x_HSTS_Mask_PRL			((1UL)<<2)
#define M7816x_HSTS_Mask_PRES			(1UL)
#define M7816x_MSTS_Mask_BGT			(1UL<<5)
#define	M7816x_MSTS_Mask_RBF			(1UL<<1)
#define	M7816x_MSTS_Mask_TBE			(1UL)

#define	M7816x_USTS_Mask_TO3			((1UL)<<7)
#define	M7816x_USTS_Mask_TO2			((1UL)<<6)
#define	M7816x_USTS_Mask_TO1			((1UL)<<5)
#define	M7816x_USTS_Mask_EA				((1UL)<<4)
#define	M7816x_USTS_Mask_PE				((1UL)<<3)
#define	M7816x_USTS_Mask_OVR			((1UL)<<2)
#define	M7816x_USTS_Mask_FER			((1UL)<<1)
#define	M7816x_USTS_Mask_TACT			(1UL)

/**
  * @}
  */  
  
/** @defgroup M7816x_Power_Secection M7816x Power Secection
  * @{
  */
#define M7816x_POWER_UP		1UL
#define M7816x_POWER_DOWN	~(1UL)
/**
  * @}
  */ 
  
/** @defgroup M7816x_RST_LEVEL M7816x RST LEVEL
  * @{
  */
#define M7816x_RST_HIGH_LEVEL		(1UL<<2)
#define M7816x_RST_LOW_LEVEL		~(1UL<<2)

#define M7816x_REST_IO_UART (1UL<<3)
#define M7816x_ENABLE_IO_UART 1
/**
  * @}
  */ 
  
/** @defgroup M7816x_IO_Direction   M7816x IO Direction
  * @{
  */
#define M7816x_CONV_FORWARD	(1UL<<0)   
#define M7816x_CONV_REVERSE	~(1UL<<0) 
#define M7816x_RECEIVE_MODE	   ~(1UL<<3)
#define M7816x_SEND_MODE	   (1UL<<3)
#define M7816x_AUTOCONV_SOFT   (1UL<<2)
#define M7816x_AUTOCONV_HARD   ~(1UL<<2)
/**
  * @}
  */ 
  
/** @defgroup M7816x_Protocol_Type   M7816x Protocol Type
  * @{
  */
#define M7816x_PROTOCOL_T0		~(1UL<<4)
#define M7816x_PROTOCOL_T1		(1UL<<4)
/**
  * @}defgroup EMV7816M2 
  */ 
#define EMV7816M2_TIMING_HARDWARE   ~(1UL) 
#define EMV7816M2_TIMING_SOFTWARE	(1UL) 
#define EMV7816M2_IO_ELECTRONIC	(1UL<<4) 
#define EMV7816M2_IO_NORMAL		~(1UL<<4) 
#define EMV7816M2_VOLTAGE_3V	   (1UL<<1)
#define EMV7816M2_VOLTAGE_5V	   ~(1UL<<1)
#define EMV7816M2_VOLTAGE_1V8	   (1UL<<3) 
/**
  * @}
  */ 
  
 
  
/** @defgroup 7816Mx_Private_Macros 7816Mx Private Macros
  * @{
  */


#define IS_M7816_SELECT(x)			((x == ISO7816M1)||(x == ISO7816M2)||(x == ISO7816M3))  
#define IS_M7816x_POWER(x)			((x == M7816x_POWER_UP)||(x == M7816x_POWER_DOWN))						
#define IS_M7816x_RST(x)			((x == M7816x_RST_HIGH_LEVEL)||(x == M7816x_RST_LOW_LEVEL))	
#define IS_M7816x_TOC(x)			((x == M7816x_TIM_MODE2)||(x == M7816x_TIM_MODE3)||(x == M7816x_TIM_MODE4)||(x == M7816x_TIM_MODE5) || \
									(x == M7816x_TIM_MODE6)||(x == M7816x_TIM_MODE7)||(x == M7816x_TIM_MODE8)||(x == M7816x_TIM_MODE9)||(x == M7816x_TIM_MODE10) || \
									(x == M7816x_TIM_MODE11)||(x == M7816x_TIM_MODE12))	
#define IS_M7816x_STOP_TOC(x)		((x == M7816x_TIM_STOP_TO1_TO2_TO3)||(x == M7816x_TIM_STOP_TO2_TO3))	
#define IS_M7816_EMV_VOLTAGE(x)		((x == EMV7816M2_VOLTAGE_3V)||(x == EMV7816M2_VOLTAGE_5V)||(x == EMV7816M2_VOLTAGE_1V8))
#define IS_M7816_EMV_IO_TYPE(x)		((x == EMV7816M2_IO_ELECTRONIC)||(x == EMV7816M2_IO_NORMAL))  
#define IS_M7816_EMV_TIMING(x)		((x == EMV7816M2_TIMING_SOFTWARE)||(x == EMV7816M2_TIMING_HARDWARE))  
/**
  * @}
  */
  
/** @addtogroup M7816x_Exported_Functions
  * @{  
  */
void M7816x_StructInit(M7816x_InitTypeDef* M7816x_InitStruct);
StatusTypeDef M7816x_Init(M7816_TypeDef* M7816x, M7816x_InitTypeDef* M7816x_InitStruct);
void M7816x_DeInit(M7816_TypeDef* M7816x);
StatusTypeDef M7816x_ConfigBaudRate(M7816_TypeDef* M7816x, uint8_t BaudRate);
void M7816x_SetDoubleBaudRate(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_SoftReset(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_Cmd(M7816_TypeDef* M7816x,FunctionalState NewState);
FlagStatus M7816x_GetHSTS(M7816_TypeDef* M7816x,uint32_t Flag);
FlagStatus M7816x_GetMSTS(M7816_TypeDef* M7816x,uint32_t Flag);
FlagStatus M7816x_GetUSTS(M7816_TypeDef* M7816x,uint32_t Flag);
void M7816x_ClearHStatus(M7816_TypeDef* M7816x,uint32_t Flag);
void M7816x_ClearUStatus(M7816_TypeDef* M7816x,uint32_t Flag);
void M7816x_ITConfigHMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT, FunctionalState NewState);
void M7816x_ITConfigMMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT, FunctionalState NewState);
void M7816x_ITConfigUMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT, FunctionalState NewState);
//void M7816x_ConfigPower(M7816_TypeDef* M7816x,uint32_t Power_Swith,uint32_t RST_Level);
void M7816x_ConfigPullUpAfterParity(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_ConfigActiveCard(M7816_TypeDef* M7816x,uint32_t Power_Swith);
uint32_t M7816x_GetActiveCardStatus(M7816_TypeDef* M7816x);
void M7816x_ConfigReset(M7816_TypeDef* M7816x,uint32_t RST_Level);
void M7816x_ConfigRetry(M7816_TypeDef* M7816x,uint32_t num);
void M7816x_ConfigClkOutput(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_ConfigClkCpol(M7816_TypeDef* M7816x,uint32_t CLK_Level);
void M7816x_ConfigAtrFirstByteError(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_SendData(M7816_TypeDef* M7816x,uint8_t Data);
uint8_t M7816x_ReciveData(M7816_TypeDef* M7816x);
void M7816x_ConfigDMA(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_ConfigTOR(M7816_TypeDef* M7816x,uint32_t TimerValue);
void M7816x_ConfigTOC(M7816_TypeDef* M7816x,uint8_t StartTOC);
void M7816x_ConfigIODirection(M7816_TypeDef* M7816x,uint32_t Dir);
void M7816x_StopTOC(M7816_TypeDef* M7816x,uint8_t StopTOC);
void M7816x_ConfigGuardTime(M7816_TypeDef* M7816x,uint8_t Guard);
void M7816x_ConfigProtocol(M7816_TypeDef* M7816x, uint32_t Type);
void M7816x_ConfigEncodingRuleDetectionEnable(M7816_TypeDef* M7816x, FunctionalState NewState);
ITStatus M7816x_GetIT_UMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT);
ITStatus M7816x_GetIT_MMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT);
ITStatus M7816x_GetIT_HMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT);

void M7816x_ConfigLCT(M7816_TypeDef* M7816x);
void EMV7816M2_ConfigVoltage( uint32_t voltage);
void EMV7816M2_ConfigElectronic(uint32_t IOType, uint32_t TimingMode);
void EMV_ConfigActiveCard(M7816_TypeDef* M7816x,uint32_t Power_Swith);

void M7816x_SynCardCmd(M7816_TypeDef* M7816x,FunctionalState NewState);
void M7816x_SynSoftMode(M7816_TypeDef* M7816x);
void M7816x_SynStartSend(M7816_TypeDef* M7816x);
void M7816x_SynStartRecv(M7816_TypeDef* M7816x);
void M7816x_SynSoftIO(M7816_TypeDef* M7816x,bool level);	
void M7816x_SynSoftClk(M7816_TypeDef* M7816x,bool level);
void M7816x_SynSoftRst(M7816_TypeDef* M7816x,bool level);
bool M7816x_SynSoftReadIO(M7816_TypeDef* M7816x);
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
#endif	 
	 
	 
/************************ (C) COPYRIGHT TMC *****END OF FILE****/	 

