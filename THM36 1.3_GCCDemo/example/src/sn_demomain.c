/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: sn_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

#include "thm36_1.3.h"
#include "sn_demomain.h"




SN_StructTypedef SN_Data;


/**
  * @brief Get the chip serial number.The complete information are 16 bytes.
  * @param  None
  * @retval None
  */
void SN_DemoMain(void)
{
	memcpy((uint8_t *)&SN_Data, (uint8_t *)OTP_SN_ADDR, 16);
}



