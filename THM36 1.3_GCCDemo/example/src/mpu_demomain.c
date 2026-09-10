/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mpu_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Includes ------------------------------------------------------------------*/

#include "thm36_1.3.h"
#include "mpu.h"
MPU_InitTypeDef MPU_InitStruc;
void MPU_StructInit(MPU_InitTypeDef *MPU_InitStruct)
{
	/*The configure MPU protect area,All of the Start Address and End Address is 24 bits.*/
	MPU_InitStruct->MPU_Region0_Access = MPU_SYS_RW_APP_NO_ACCESS;
	MPU_InitStruct->MPU_Region0_End_Add =   0x10011000;
	MPU_InitStruct->MPU_Region0_Strat_Add = 0x10010000;	
	
	MPU_InitStruct->MPU_Region1_Access = MPU_SYS_RW_APP_NO_ACCESS;
	MPU_InitStruct->MPU_Region1_End_Add = 0x10012000;
	MPU_InitStruct->MPU_Region1_Strat_Add = 0x10011100;	
	
	MPU_InitStruct->MPU_Region2_Access = MPU_SYS_RW_APP_NO_ACCESS;
	MPU_InitStruct->MPU_Region2_End_Add = 0x10013000;
	MPU_InitStruct->MPU_Region2_Strat_Add = 0x10012100;	
	
	MPU_InitStruct->MPU_Region3_Access = MPU_SYS_RW_APP_NO_ACCESS;
	MPU_InitStruct->MPU_Region3_End_Add = 0x10014000;
	MPU_InitStruct->MPU_Region3_Strat_Add = 0x10013100;	
}

void MPU_DemoMain(void)
{	
	volatile uint32_t temp;
	MPU_StructInit(&MPU_InitStruc);
	MPU_Initial(&MPU_InitStruc); 
	temp = *((uint32_t*)(0x10010004));
	
}




