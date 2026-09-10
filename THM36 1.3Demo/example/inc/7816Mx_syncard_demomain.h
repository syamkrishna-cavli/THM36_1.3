 /******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: 7816Mx_syncard_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

#ifndef __7816Mx_SYNCARD_DEMOMAIN_H_
#define __7816Mx_SYNCARD_DEMOMAIN_H_

#define		CMD_RD_MAIN_MEM			0x30
#define		CMD_WR_MAIN_MEM			0x38 
#define		CMD_RD_PROTECT_MEM		0x34
#define		CMD_WR_PROTECT_MEM		0x3C

#define		NO_EFFECT_CHAR		0xFF


#define		CMD_RD_PSC_MEM		0x31
#define		CMD_WR_PSC_MEM		0x39	
#define		CMD_COMP_PSC_MEM	0x33


#define		SOFT_MODE	 1
#define		HARD_MODE	 0

#define		SE_EMV_PORT 		1	
#define		CURRENT_MODE		SOFT_MODE

#define		SYN_TO_ETU			(9600)		

#define		TM_E2W_CLOCK		255
#define		TM_NE2W_CLOCK		124
#define		TM_E2NW_CLOCK		124

#define		TYPE_UPDATE_E2W		    01			//Erase and Write
#define		TYPE_UPDATE_NE2W		02			//Write only
#define		TYPE_UPDATE_E2NW		03			//Erase only

void SLE4442Demo(void);
uint32_t ISLE_Reset(unsigned char* AtrBuffer);
int  ISLE_CheckPass_SLE4442 (unsigned char* Password,unsigned char *SMBuffer);
uint16_t ISLE_ChangePass_SLE4442 (unsigned char* Password);
uint16_t ISLE_ReadPass_SLE4442 (unsigned char *SMBuffer);
void ISLE_WriteProtection (uint16_t Offset, uint16_t len,unsigned char *ProtBuffer);//00-1F
void ISLE_ReadProtection (unsigned char* ProtBuffer);//32 protection bits
void ISLE_Write_MainMem(uint16_t Offset,uint16_t len, unsigned char* DataBuffer);
void ISLE_Read_MainMem(uint16_t Offset,uint16_t len, unsigned char * DataBuffer);
void ISLE_SetPortMode(uint8_t interface,uint8_t  enhard,uint8_t pulsewidth);
void ISLE_InitSynCard(void);
void ISO7816M2_SynDemoMain(void);
#endif //__7816Mx_SYNCARD_DEMOMAIN_H_




