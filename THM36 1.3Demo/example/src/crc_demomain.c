/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: crc_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36x2.h"
#include "RCC.h"
#include "crc.h"
CRC_InitTypeDef CRC_InitStruct;
uint8_t CRC_Bufer[10] = {2,2,3,4,5,6,7,8,9,10};
uint16_t data;
void CRC_Test(void)
{
	CRC_InitStruct.CRC_Mode = CRC_MODE_CCITT;
	CRC_InitStruct.CRC_Read = CRC_BIG_ENDIAN;
	CRC_InitStruct.CRC_Result_Reverberate = DISABLE;
	CRC_InitStruct.CRC_Data_Reverberate = DISABLE;
	CRC_Init(&CRC_InitStruct);
	CRC_16_Calculate(1,CRC_Bufer,1,&data);
	
	
}
