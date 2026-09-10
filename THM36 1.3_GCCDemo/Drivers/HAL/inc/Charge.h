/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: Charge.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
/**
  * @brief  Get the Charge Status Detect
  * @param  none
  * @retval uint8_t
  *		@arg 0x0A Charging not completed
  *		@arg 0x0B Charging completed
  */
uint8_t ChargeStatusDetect(void);

/**
  * @brief  THM3622 must call this Function before into Stop mode,it will be reduced low power Current
  * @param  none
  * @retval none
  */
void ChargeConfigStop(void);


