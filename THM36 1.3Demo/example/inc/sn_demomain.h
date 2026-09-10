/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: sn_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
#ifndef __SN_DEMOMAIN_H_
#define __SN_DEMOMAIN_H_

#define OTP_SN_ADDR 	0x1F000020


typedef struct 
{
   //LotID formart example:S5QH50
   uint8_t LotID[6];
   //WaferID format example:23
   uint8_t WaferID[2];
   //DieAddress format example:(0x40£¬0x4F)
   uint8_t DieAddress[4];
   //Data format example:(0x7D7,0xC,0x1F)means 2007,12,31
   uint8_t Year[2];
   uint8_t Month;
   uint8_t Day;
} SN_StructTypedef;

void SN_DemoMain(void);

#endif  //__SN_DEMOMAIN_H_

