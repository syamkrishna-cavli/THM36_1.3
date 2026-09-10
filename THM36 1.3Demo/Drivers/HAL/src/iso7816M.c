/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ISO7816_EMV_L1.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "RCC.h"
#include "ISO7816M.h"

extern void delay_ms_static(unsigned int ms);
/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup 7816M 
  * @brief 7816M HAL modules driver 
  * @{
  */ 

/** @defgroup ISO7816M_Private_Defines ISO7816M Private Defines
  * @{
  */
const uint32_t FD[10][14]={
							0,		0,		0,		0,		0,		0,		0,		0,	0,	0,		0,		0,		0,		0,		
							31*12,	31*12,	31*18,	31*24,	31*36,	31*48,	31*60,	0,	0,	32*16,	32*24,	32*32,	32*48,	32*64,
							31*6,	31*6,	31*9,	31*12,	31*18,	31*24,	31*30,	0,	0,	32*8,	32*12,	32*16,	32*24,	32*32,
							31*3,	31*3,	0,		31*6,	31*9,	31*12,	31*15,	0,	0,	32*4,	32*6,	32*8,	32*12,	32*16,
							0,		0,		0,		31*3,	0,		31*6,	0,		0,	0,	32*2,	32*3,	32*4,	32*6,	32*8,
							0,		0,		0,		0,		0,		31*3,	0,		0,	0,	32*1,	0,		32*2,	32*3,	32*4,
							0,		0,		0,		0,		0,		0,		0,		0,	0,	0,		0,		32,		0,		32*2,
							0,		0,		0,		0,		0,		0,		0,		0,	0,	0,		0,		0,		0,		0,
							31,		31,		0,		31*2,	31*3,	31*4,	31*5,	0,	0,	0,		32*2,	0,		32*4,	0,	
							0,		0,		0,		0,		0,		0,		31*3,	0,	0,	0,		0,		0,		0,		0
						};
	


#define check_7816Mx_param(x,y) ((x&y) ? 1 : 0)

#define M7816x_CLK_OUTPUT			1UL<<4;
#define M7816x_CLK_NOT_OUTPUT		~(1UL<<4);

#define M7816x_SEND_NAK				1UL<<7;
#define M7816x_NOT_SEND_NAK			~(1UL<<7);
						
#define  M7816X_TRANSMIT_LAST_BYTE	(1UL<<2)
/**
  * @}
  */
/**
  * @brief   Initializes the InitStruct of M7816x
  * @param[in]	M7816x_InitStruct: pointer to a M7816x_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
__weak void M7816x_StructInit(M7816x_InitTypeDef* M7816x_InitStruct)
{
	M7816x_InitStruct->ATR_Code_Mode = Hardware_Mode;
	M7816x_InitStruct->BaudRate_FD = 0x11;
	M7816x_InitStruct->DoubleBaudRate = DISABLE;
	M7816x_InitStruct->CLK_CPOL = CLK_STOP_LOW;
	M7816x_InitStruct->Eorrer_Retrans_times = 2;
	M7816x_InitStruct->protol = T0;
	M7816x_InitStruct->Trans_GTR = 0;		/*  GTR = 2 guard time is 3 ETU*/
	M7816x_InitStruct->ATR_SoftCode_Mode = Reverse_coding;
}  
/**
  * @brief  Initializes the M7816x peripheral according to the specified
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  M7816x_InitStruct: pointer to a M7816x_InitTypeDef structure
  * 		that contains the configuration information for the specified M7816x peripheral.
  * @retval none
  * @note
  */
StatusTypeDef M7816x_Init(M7816_TypeDef* M7816x, M7816x_InitTypeDef* M7816x_InitStruct)
{
	M7816x->UCON1 = 0;
	M7816x->UCON2 = 0;
	M7816x->UCON2|= M7816x_InitStruct->ATR_Code_Mode;
	M7816x->UCON1|= M7816x_InitStruct->protol;
	M7816x->UCON1|= (M7816x_InitStruct->ATR_Code_Mode>>1);
	M7816x->UCON1|= M7816x_InitStruct->ATR_SoftCode_Mode;
	M7816x->FFCON = (M7816x_InitStruct->Eorrer_Retrans_times<<4);
	M7816x->GTR = M7816x_InitStruct->Trans_GTR;
	if(M7816x_ConfigBaudRate(M7816x,M7816x_InitStruct->BaudRate_FD)!=STATUS_OK)
	{
		return STATUS_ERROR;
	}
	M7816x_SetDoubleBaudRate(M7816x,M7816x_InitStruct->DoubleBaudRate);
	return STATUS_OK;
} 

/**
  * @brief  DeInitializes the M7816x peripheral 
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @retval none
  * @note none
  */
void M7816x_DeInit(M7816_TypeDef* M7816x)
{
	if(M7816x == ISO7816M1)
	{
		RCC_APB2PeriphSoftReset(RCC_APB2Periph_7816M1);
		while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_7816M1)==RESET);
	}
	if(M7816x == ISO7816M2)
	{
		RCC_APB3PeriphSoftReset(RCC_APB3Periph_7816M2);
		while(RCC_APB3PeriphGetResetFlag(RCC_APB3Periph_7816M2)==RESET);
	}
	if(M7816x == ISO7816M3)
	{
		RCC_APB2PeriphSoftReset(RCC_APB2Periph_7816M3);
		while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_7816M3)==RESET);
	}
} 

/**
  * @}
  */ 
/**
  * @brief  config the M7816x's BaudRate 
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  BaudRate: the value of M7816x's BaudRate,This parameter can be a value of @ref FD,example BaudRate is 0x94, M7816x 
  * @retval none
  * @note none
  */
StatusTypeDef M7816x_ConfigBaudRate(M7816_TypeDef* M7816x, uint8_t BaudRate)
{
	uint32_t Temp;
	assert_param(IS_M7816_SELECT(M7816x));
	if(FD[BaudRate&0x0F][BaudRate>>4] == 0)
	{
		return STATUS_ERROR;
	}
	if(FD[BaudRate&0x0F][BaudRate>>4]%32)
	{
		Temp = (FD[BaudRate&0x0F][BaudRate>>4])/31;
		M7816x->UCON2 &= M7816x_PSC_31;
		M7816x->PDR = Temp;
	}
	else
	{
		Temp = (FD[BaudRate&0x0F][BaudRate>>4])/32;
		M7816x->UCON2 |= M7816x_PSC_32;
		M7816x->PDR = Temp;
	}
	return STATUS_OK;
}
/**
  * @brief  Double the M7816x's BaudRate or not Double M7816x's BaudRate 
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */
void M7816x_SetDoubleBaudRate(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->UCON2|= 1<<1;
	}
	else
	{
		M7816x->UCON2&= ~(1<<1);
	}
}

/**
  * @brief  M7816x PowerOn
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */
void M7816x_SoftReset(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->CSEL &= (~8UL);
	}
	else
	{
		M7816x->CSEL |= 8UL;
	}
}
/**
  * @brief  Enable or Disable M7816x
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */
void M7816x_Cmd(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->CSEL |=M7816x_REST_IO_UART;
		M7816x->CSEL |=M7816x_ENABLE_IO_UART;
	}
	else
	{
		M7816x->CSEL &=(~M7816x_ENABLE_IO_UART);
	}
}
/**
  * @brief  Enable or Disable M7816x syncard
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */
void M7816x_SynCardCmd(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	if(NewState != DISABLE)
		M7816x->UCON2 |= Bit3_En | Bit1_En;					//	Enable 同步卡模式 CKU
	
	else
		M7816x->UCON2 &= Bit3_Dis | Bit1_Dis;				//	Disable 同步卡模式 CKU
	
}
/**
  * @brief  Set M7816x syncard to soft mode
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @retval none
  * @note none
  */
void M7816x_SynSoftMode(M7816_TypeDef* M7816x)
{
	M7816x->SXCON &= Bit4_Dis;
}
/**
  * @brief  Start M7816x syncard sending and waiting to start finished
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @retval none
  * @note none
  */
void M7816x_SynStartSend(M7816_TypeDef* M7816x)
{
	M7816x->UCON1 |= M7816x_SEND_MODE |M7816x_CONV_FORWARD; 
	M7816x->UCON2 |= M7816x_AUTOCONV_SOFT; 
	while((M7816x->UCON1& M7816x_SEND_MODE) == 0);
}

/**
  * @brief  Start M7816x syncard receiving
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @retval none
  * @note none
  */
void M7816x_SynStartRecv(M7816_TypeDef* M7816x)
{
	M7816x->UCON1 &= (~M7816x_SEND_MODE); 
	while(M7816x->UCON1& M7816x_SEND_MODE);
}

/**
  * @brief  Set M7816x syncard IO to corresponding level
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  level: which can be 1(high level) or 0(low level)	 
  * @retval none
  * @note none
  */
void M7816x_SynSoftIO(M7816_TypeDef* M7816x,bool level)
{
	if(level)
		M7816x->SYNCBUF |= Bit0_En;
	else
		M7816x->SYNCBUF &= Bit0_Dis;
}
/**
  * @brief  Set M7816x syncard CLOCK to corresponding level
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  level: which can be 1(high level) or 0(low level)	 
  * @retval none
  * @note none
  */
void M7816x_SynSoftClk(M7816_TypeDef* M7816x,bool level)
{
	if(level)
		M7816x->SXCON |= Bit5_En;
	else
		M7816x->SXCON &= Bit5_Dis;
		
}
/**
  * @brief  Set M7816x syncard RESET to corresponding level
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  level: which can be 1(high level) or 0(low level)	 
  * @retval none
  * @note none
  */
void M7816x_SynSoftRst(M7816_TypeDef* M7816x,bool  level)
{
	if(level)
		M7816x->PWCON |= Bit2_En;
	else
		M7816x->PWCON &= Bit2_Dis;
}
/**
  * @brief  Read the level of M7816x syncard IO
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 	 
  * @retval 1:High level 0:Low level
  * @note none
  */
bool M7816x_SynSoftReadIO(M7816_TypeDef* M7816x)
{
	return (bool)(M7816x->SYNCBUF & Bit0_En); 
}
/**
  * @brief  get 7816Mx HSTS register's Status
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  Flag: the Flag of 7816Mx 
  *	 @arg M7816x_HSTS_PRTL
  *	 @arg M7816x_HSTS_PRL
  *	 @arg M7816x_HSTS_PRES
  * @retval none
  * @note none
  */
FlagStatus M7816x_GetHSTS(M7816_TypeDef* M7816x,uint32_t Flag)
{
	uint32_t temp_Hsts;
	assert_param(IS_M7816_SELECT(M7816x));
	temp_Hsts = M7816x->HSTS;
	if(temp_Hsts&Flag)
	{
		return SET;
	}
	return RESET;
}



/**
  * @brief  get 7816Mx MSTS register's Status
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  Flag: the Flag of 7816Mx 
  *	 @arg M7816x_MSTS_BGT
  *	 @arg M7816x_MSTS_RBF
  *	 @arg M7816x_MSTS_TBE
  * @retval none
  * @note none
  */
FlagStatus M7816x_GetMSTS(M7816_TypeDef* M7816x,uint32_t Flag)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(Flag&M7816x->MSTS)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  get 7816Mx USTS register's Status
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  Flag: the Flag of 7816Mx 
  *	 @arg M7816x_USTS_RETRY
  *	 @arg M7816x_USTS_TO3
  *	 @arg M7816x_USTS_TO2
  *	 @arg M7816x_USTS_TO1
  *	 @arg M7816x_USTS_EA
  *	 @arg M7816x_USTS_PE
  *	 @arg M7816x_USTS_OVR
  *	 @arg M7816x_USTS_FER
  *	 @arg M7816x_USTS_TACT
  * @retval none
  * @note none
  */
FlagStatus M7816x_GetUSTS(M7816_TypeDef* M7816x,uint32_t Flag)
{
	uint32_t temp_Usts;
	assert_param(IS_M7816_SELECT(M7816x));
	temp_Usts = M7816x->USTS;
	if(Flag&temp_Usts)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clear 7816Mx HSTS register's Status
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  Flag: the Flag of 7816Mx 
  *	 @arg M7816x_HSTS_PRL
  *		@arg M7816x_HSTS_PRES
  * @retval none
  * @note none
  */
void M7816x_ClearHStatus(M7816_TypeDef* M7816x,uint32_t Flag)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->HSTS |= Flag;
}

/**
  * @brief  Clear 7816Mx USTS register's Status
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  Flag: the Flag of 7816Mx 
  *	 @arg M7816x_USTS_RETRY
  *	 @arg M7816x_USTS_TO3
  *	 @arg M7816x_USTS_TO2
  *	 @arg M7816x_USTS_TO1
  *	 @arg M7816x_USTS_EA
  *	 @arg M7816x_USTS_PE
  *	 @arg M7816x_USTS_OVR
  *	 @arg M7816x_USTS_FER
  *	 @arg M7816x_USTS_TACT
  * @retval none
  * @note none
  */
void M7816x_ClearUStatus(M7816_TypeDef* M7816x,uint32_t Flag)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->USTS = Flag;
}

/**
  * @brief  Enable or disable 7816Mx interrupt
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	M7816x_IT: specifies the M7816x interrupt status.
  *	 @arg M7816x_HSTS_Mask_PRTL
  *	 @arg M7816x_HSTS_Mask_PRL
  * @param[in]	NewState
  *		@arg DISABLE :Disable interrupt
  *	 @arg ENABLE  :Enable interrupt
  * @retval none
  * @note	none
  */ 
void M7816x_ITConfigHMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT, FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->HMSK &= ~(M7816x_IT);
	}
	else
	{
		M7816x->HMSK |= M7816x_IT;
	}	

}
/**
  * @brief  Get 7816Mx interrupt whether SET
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	M7816x_IT: specifies the M7816x interrupt status.
  *	 @arg M7816x_HSTS_Mask_PRTL
  *	 @arg M7816x_HSTS_Mask_PRL
  * @retval ITStatus
  *		@ARG SET
  *		@ARG RESET
  * @note	none
  */ 

ITStatus M7816x_GetIT_HMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(M7816x->HMSK&M7816x_IT)
	{
		return SET;
	}
	else
	{
		return RESET;
	}	
}
/**
  * @brief  Enable or disable 7816Mx interrupt
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	M7816x_IT: specifies the M7816x interrupt status.
  *	 @arg M7816x_MSTS_Mask_BGT
  *	 @arg M7816x_MSTS_Mask_RBF
  *	 @arg M7816x_MSTS_Mask_TBE
  * @param[in]	NewState
  *		@arg DISABLE :Disable interrupt
  *	 @arg ENABLE  :Enable interrupt
  * @retval none
  * @note	none
  */ 
void M7816x_ITConfigMMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT, FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->MMSK &= ~(M7816x_IT);
	}
	else
	{
		M7816x->MMSK |= M7816x_IT;
	}	

}

/**
  * @brief  Get 7816Mx MMSK cinterrupt whether SET
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	M7816x_IT: specifies the M7816x interrupt status.
 *	 @arg M7816x_MSTS_Mask_BGT
  *	 @arg M7816x_MSTS_Mask_RBF
  *	 @arg M7816x_MSTS_Mask_TBE
  * @retval ITStatus
  *		@ARG SET
  *		@ARG RESET
  * @note	none
  */ 

ITStatus M7816x_GetIT_MMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(M7816x->MMSK&M7816x_IT)
	{
		return SET;
	}
	else
	{
		return RESET;
	}	
}
/**
  * @brief  Enable or disable 7816Mx interrupt
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	M7816x_IT: specifies the M7816x interrupt status.
  *	 @arg M7816x_USTS_Mask_RETRY
  *	 @arg M7816x_USTS_Mask_TO3
  *	 @arg M7816x_USTS_Mask_TO1
  *	 @arg M7816x_USTS_Mask_EA
  *	 @arg M7816x_USTS_Mask_PE
  *	 @arg M7816x_USTS_Mask_OVR
  *	 @arg M7816x_USTS_Mask_FER
  *	 @arg M7816x_USTS_Mask_TACT
  * @param[in]	NewState
  *		@arg DISABLE :Disable interrupt
  *	 @arg ENABLE  :Enable interrupt
  * @retval none
  * @note	none
  */ 

void M7816x_ITConfigUMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT, FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->UMSK &= ~(M7816x_IT);
	}
	else
	{
		M7816x->UMSK |= M7816x_IT;
	}	

}

/**
  * @brief  Get 7816Mx UMSK cinterrupt whether SET
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	M7816x_IT: specifies the M7816x interrupt status.
  *	 @arg M7816x_USTS_Mask_RETRY
  *	 @arg M7816x_USTS_Mask_TO3
  *	 @arg M7816x_USTS_Mask_TO1
  *	 @arg M7816x_USTS_Mask_EA
  *	 @arg M7816x_USTS_Mask_PE
  *	 @arg M7816x_USTS_Mask_OVR
  *	 @arg M7816x_USTS_Mask_FER
  *	 @arg M7816x_USTS_Mask_TACT
  * @retval ITStatus
  *		@ARG SET
  *		@ARG RESET
  * @note	none
  */ 
ITStatus M7816x_GetIT_UMSK(M7816_TypeDef* M7816x, uint32_t M7816x_IT)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(M7816x->UMSK&M7816x_IT)
	{
		return SET;
	}
	else
	{
		return RESET;
	}	
}
/**
  * @brief  Enable or Disable PullUp after parity bit of M7816x.
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral	 
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */
void M7816x_ConfigPullUpAfterParity(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));

	if(NewState == ENABLE)
	{
		M7816x->CSEL |= M7816x_PULL_UP_AFTER_PARITY;
	}
	else
	{
		M7816x->CSEL &= ~M7816x_PULL_UP_AFTER_PARITY;
	}
}
/**
  * @brief  Power up or Power down 7816EMV
  * @param[in]  M7816x: M78162 peripheral  
  * @param[in]	Power_Swith:Power Selection
  *		@arg M7816x_POWER_UP : M7816x Power up 
  *	 @arg M7816x_POWER_DOWN : M7816x Power down  
  * @retval none
  * @note	none
  */ 
void EMV_ConfigActiveCard(M7816_TypeDef* M7816x,uint32_t Power_Swith)
{
//	assert_param(IS_M7816_SELECT(M7816x));
//	assert_param(IS_M7816x_POWER(Power_Swith));  
//   
//	if(Power_Swith == M7816x_POWER_UP)
//	{
//		M7816x->PWCON |= M7816x_POWER_UP;		  //VCC on
//		delay_ms(1);
//		M7816x->CKCON |= M7816x_CLK_OUTPUT;		 //CLK on 
//		EMV7816M2->EMVCON |= EMV7816M2_IO_ELECTRONIC;		  //SIO on,EMVCON reg will be 0x19 after this instruction
//		delay_ms(9);
//		M7816x->PWCON |= M7816x_RST_HIGH_LEVEL; //RST on
////		delay_ms(1);	  
//	}
//	else
//	{
//		M7816x->PWCON &= M7816x_RST_LOW_LEVEL;  //RST off
//		delay_ms(1);
//		EMV7816M2->EMVCON &= EMV7816M2_IO_NORMAL;		//SIO off,EMVCON will be 0x09 after this instruction
//		M7816x->CKCON &= M7816x_CLK_NOT_OUTPUT;		//CLK off
//		delay_ms(1);
//		M7816x->PWCON &= M7816x_POWER_DOWN;	 //VCC off
//	}
//TMC(20240328)-zt:添加CLK和RST操作-同步已过检代码
	assert_param(IS_M7816_SELECT(M7816x));
	assert_param(IS_M7816x_POWER(Power_Swith));
	if(Power_Swith == M7816x_POWER_UP)
	{
		M7816x->PWCON |= M7816x_POWER_UP;
		delay_ms_static(1);
    M7816x->CKCON |= M7816x_CLK_OUTPUT;         //CLK on 
    EMV7816M2->EMVCON |= EMV7816M2_IO_ELECTRONIC;          //SIO on,EMVCON reg will be 0x19 after this instruction
    delay_ms_static(8);
    M7816x->PWCON |= M7816x_RST_HIGH_LEVEL; //RST on
	}
	else
	{
		M7816x->PWCON &= M7816x_RST_LOW_LEVEL;
		EMV7816M2->EMVCON &= EMV7816M2_IO_NORMAL;        //SIO off,EMVCON will be 0x09 after this instruction
    M7816x->CKCON &= M7816x_CLK_NOT_OUTPUT;        //CLK off
    delay_ms_static(1);
    M7816x->PWCON &= M7816x_POWER_DOWN;     //VCC off
	}
}
/**
  * @brief  Power up or Power down 7816Mx
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	Power_Swith:Power Selection
  *		@arg M7816x_POWER_UP : M7816x Power up 
  *	 @arg M7816x_POWER_DOWN : M7816x Power down  
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigActiveCard(M7816_TypeDef* M7816x,uint32_t Power_Swith)
{
	assert_param(IS_M7816_SELECT(M7816x));
	assert_param(IS_M7816x_POWER(Power_Swith));
	if(Power_Swith == M7816x_POWER_UP)
	{
		M7816x->PWCON |= M7816x_POWER_UP;
	}
	else
	{
		M7816x->PWCON &= M7816x_POWER_DOWN;
	}
}

/**
  * @brief  Get card activated status
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  *		@arg M7816x_POWER_UP : M7816x Power up 
  *	 @arg M7816x_POWER_DOWN : M7816x Power down  
  * @retval Card active status
  *		@arg SET
  *		@arg RESET
  */ 
uint32_t M7816x_GetActiveCardStatus(M7816_TypeDef* M7816x)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(M7816x->PWCON & Bit0_En)
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Config the level on the reset pin when the card is activated
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	RST_Level:RST Level
  *		@arg M7816x_RST_HIGH_LEVEL : when M7816x Power up, RST PIN is HIGH LEVEL  
  *	 @arg M7816x_RST_LOW_LEVEL : when M7816x Power up, RST PIN is Low LEVEL  
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigReset(M7816_TypeDef* M7816x,uint32_t RST_Level)
{
	assert_param(IS_M7816x_RST(RST_Level));
	assert_param(IS_M7816_SELECT(M7816x));
	if(RST_Level == M7816x_RST_HIGH_LEVEL)
	{
		M7816x->PWCON |= M7816x_RST_HIGH_LEVEL;
	}
	else
	{
		M7816x->PWCON &= M7816x_RST_LOW_LEVEL;
	}
}

/**
  * @brief  Config retry times.
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	num: where num can be 0 to 7 to config retry times 
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigRetry(M7816_TypeDef* M7816x,uint32_t num)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->FFCON = (num << 4);
}
/**
  * @brief  Power up or Power down 7816Mx
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	Power_Swith:Power Selection
  *		@arg M7816x_POWER_UP : M7816x Power up 
  *	 @arg M7816x_POWER_DOWN : M7816x Power down  
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigClkOutput(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->CKCON|=  M7816x_CLK_OUTPUT;
	}
	else
	{
		M7816x->CKCON&=  M7816x_CLK_NOT_OUTPUT;
	}
}

/**
  * @brief  Config 7816Mx's CLK cpol 
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]	CLK_Level:
  *		@arg M7816x_RST_HIGH_LEVEL : M7816x's CLK is stop,the Clk cpol is Low Level
  *	 @arg M7816x_RST_LOW_LEVEL :  M7816x's CLK is stop,the Clk cpol is High Level 
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigClkCpol(M7816_TypeDef* M7816x,uint32_t CLK_Level)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(CLK_Level == M7816x_RST_HIGH_LEVEL)
	{
		M7816x->CKCON|=  M7816x_RST_HIGH_LEVEL;
	}
	else
	{
		M7816x->CKCON&= M7816x_RST_LOW_LEVEL;
	}
}


/**
  * @brief  Config 7816Mx's First Byte of ATR is not 0x3B or 0x3F,IO wether send a NAK
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigAtrFirstByteError(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->UCON1|= M7816x_SEND_NAK
	}
	else
	{
		M7816x->UCON1&= M7816x_NOT_SEND_NAK;
	}
}

/**
  * @brief  Send one Byte by M7816x's IO,the byte is not Last Byte
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  Data: the send data of M7816x
  * @retval none
  * @note	none
  */ 
void M7816x_SendData(M7816_TypeDef* M7816x,uint8_t Data)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->TBUF = Data;
}

/**
  * @brief  Send one Byte by M7816x's IO
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  Data: the send data of M7816x
  * @retval none
  * @note	none
  */ 
uint8_t M7816x_ReciveData(M7816_TypeDef* M7816x)
{
	assert_param(IS_M7816_SELECT(M7816x));
	return M7816x->RBUF;
}

/**
  * @brief Config M7816x's DMA Mode
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  NewState: new state of the M7816x peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigDMA(M7816_TypeDef* M7816x,FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->DMACON = 1;
	}
	else
	{
		M7816x->DMACON = 0;
	}
}

/**
  * @brief Config M7816x's TOR(value of Tim1,Tim2,Tim3)
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  TimerValue: the value of Timer
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigTOR(M7816_TypeDef* M7816x,uint32_t TimerValue)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->TOR = TimerValue;
}

/**
  * @brief Config M7816x's TOC(value of Time Mode)
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  TOC: the value of Timer mode, value of @ref M7816_TOC_MODE
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigTOC(M7816_TypeDef* M7816x,uint8_t StartTOC)
{
	assert_param(IS_M7816x_TOC(StartTOC));
	//M7816x->TOC = StopTOC;
	M7816x->TOC = StartTOC;
}

/**
  * @brief Stop M7816x's TOC
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  StopTOC: the value of Stop Timer mode, value of @ref M7816_TOC_MODE
  * @retval none
  * @note	none
  */ 
void M7816x_StopTOC(M7816_TypeDef* M7816x,uint8_t StopTOC)
{
	assert_param(IS_M7816x_STOP_TOC(StopTOC));
	M7816x->TOC = StopTOC;
}
/**
  * @brief Config M7816x's IO Direction
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  Dir: the value of Dir, value of @ref M7816x_IO_Direction
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigIODirection(M7816_TypeDef* M7816x,uint32_t Dir)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(Dir == M7816x_RECEIVE_MODE)
	{
		M7816x->UCON1 &= M7816x_RECEIVE_MODE;
	}
	else if(Dir == M7816x_SEND_MODE)
	{
		M7816x->UCON1 |= M7816x_SEND_MODE;
	}
}
/**
  * @brief Config M7816x's guard time
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  Guard: the value of Guard time, value can be 0x00 to 0xFF
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigGuardTime(M7816_TypeDef* M7816x,uint8_t Guard)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->GTR = Guard;
}
/**
  * @brief Config M7816x's protocol
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  Type: the type of protocol, value of @ref M7816x_Protocol_Type
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigProtocol(M7816_TypeDef* M7816x, uint32_t Type)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(Type == M7816x_PROTOCOL_T0)
	{
		M7816x->UCON1 &= M7816x_PROTOCOL_T0;
	}
	else if(Type == M7816x_PROTOCOL_T1)
	{
		M7816x->UCON1 |= M7816x_PROTOCOL_T1;
	}
}
/**
  * @brief Config M7816x's encoding rule detection.
  * @param[in]  M7816x: where x can be 1 to 3 to select the M7816x peripheral  
  * @param[in]  NewState: new state of the M7816x peripheral.
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigEncodingRuleDetectionEnable(M7816_TypeDef* M7816x, FunctionalState NewState)
{
	assert_param(IS_M7816_SELECT(M7816x));
	if(NewState == ENABLE)
	{
		M7816x->UCON1 |= Bit1_En;
	}
	else
	{
		M7816x->UCON1 &= Bit1_Dis;
	}
}

/**
  * @brief Config M7816x's encoding rule detection.
  * @param[in]  none
  * @param[in]  none
  * @retval none
  * @note	none
  */ 
void M7816x_ConfigLCT(M7816_TypeDef* M7816x)
{
	assert_param(IS_M7816_SELECT(M7816x));
	M7816x->UCON1 |= M7816X_TRANSMIT_LAST_BYTE;
}

/**
  * @brief Config M7816x's Class.
  * @param[in]  voltage:
  *					 EMV7816M2_VOLTAGE_5V
  *					 EMV7816M2_VOLTAGE_3V
  *					 EMV7816M2_VOLTAGE_1V8
  * @param[in]  none
  * @retval none
  * @note	none
  */
void EMV7816M2_ConfigVoltage( uint32_t voltage)
{
	assert_param(IS_M7816_EMV_VOLTAGE(voltage));
	if (voltage == EMV7816M2_VOLTAGE_1V8)
	{
		ISO7816M2->PWCON |= voltage;
	}
	else
	{

		ISO7816M2->PWCON &= (~EMV7816M2_VOLTAGE_1V8);//clear 1.8V.
		if (voltage == EMV7816M2_VOLTAGE_3V)
		{
			ISO7816M2->PWCON |= EMV7816M2_VOLTAGE_3V;
		}
		else
		{
			ISO7816M2->PWCON &= EMV7816M2_VOLTAGE_5V;
		}
	}
}
/**
* @brief  Configure EMV Electronic
* @param[in]	IOType
*		@arg EMV7816M2_IO_ELECTRONIC : ananlog IO
*	   @arg EMV7816M2_IO_NORMAL  : GPIO
* @param[in]	TimingMode
*		@arg EMV7816M2_TIMING_HARDWARE : the timing of VCC&CLOCK&RST&SIO is controlled by HardWare.
*	   @arg EMV7816M2_TIMING_SOFTWARE : the timing of VCC&CLOCK&RST&SIO is controlled by SoftWare.
* @retval none
* @note	none
*/
void EMV7816M2_ConfigElectronic(uint32_t IOType, uint32_t TimingMode)
{
	assert_param(IS_M7816_EMV_IO_TYPE(IOType));
	assert_param(IS_M7816_EMV_TIMING(TimingMode));
	if(TimingMode == EMV7816M2_TIMING_SOFTWARE)
	{
		EMV7816M2->EMVCON |= EMV7816M2_TIMING_SOFTWARE;
	}
	else
	{
		EMV7816M2->EMVCON &= ~EMV7816M2_TIMING_SOFTWARE;
	}

	if (IOType == EMV7816M2_IO_ELECTRONIC)
	{
		EMV7816M2->EMVCON |= EMV7816M2_IO_ELECTRONIC;
	}
	else
	{
		EMV7816M2->EMVCON &= ~(EMV7816M2_IO_ELECTRONIC);
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

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
