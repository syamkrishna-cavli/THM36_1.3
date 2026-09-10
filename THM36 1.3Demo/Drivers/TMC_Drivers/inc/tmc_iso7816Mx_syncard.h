 /******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_iso7816Mx_Syncard.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#include "stdint.h"

#ifndef __TMC_ISO7816Mx_SYNCARD_H_
#define __TMC_ISO7816Mx_SYNCARD_H_

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif
#define		ST_BP_WITH			20		//Bit width
#define		HF_BP_WITH			(ST_BP_WITH>>1)//Half bit width

void M7816x_SynSoftETU(M7816_TypeDef* M7816x,uint32_t timeout);
void M7816x_SynSoftStart(M7816_TypeDef* M7816x);
void M7816x_SynSoftEnd(M7816_TypeDef* M7816x);
void M7816x_SynSoftStop(M7816_TypeDef* M7816x);
void M7816x_SynSoftClock(M7816_TypeDef* M7816x);
void M7816x_SynSoftOutByte(M7816_TypeDef* M7816x,uint8_t data);
void M7816x_SynSoftSendStart(M7816_TypeDef* M7816x);
void M7816x_SynSoftRecvStart(M7816_TypeDef* M7816x);
uint8_t	M7816x_SynSoftReadByte(M7816_TypeDef* M7816x);
void M7816x_SynSoftBreak(M7816_TypeDef* M7816x);
void M7816x_SynSoftRst2Atr(M7816_TypeDef* M7816x,uint8_t *Atr);
void M7816x_SynSoftProcEnd(M7816_TypeDef* M7816x);
void M7816x_SynSoftCmd(M7816_TypeDef* M7816x,uint8_t ctl,uint8_t  add,uint8_t data);

#endif //__TMC_ISO7816Mx_SYNCARD_H_




