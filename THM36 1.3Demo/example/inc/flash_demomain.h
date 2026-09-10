/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: Flash Demo main.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __FLASH_DEMO_H
#define __FLASH_DEMO_H

#include "Flash.h"

extern Flash_EraseTypeDef EraseMode;
extern Flash_ProgramTypeDef Program;
extern void Flash_CacheDemomain(void);
void Flash_Demo_Test(void);
bool TMC_Compare(uint8_t *soure,uint8_t *dest,uint32_t length);

void FLASH_DemoMain(void);
void UserOTP_Demo(void);
void ReturnBL_Test(void);
#endif
