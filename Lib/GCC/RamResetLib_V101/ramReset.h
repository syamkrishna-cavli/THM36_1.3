/**
  ******************************************************************************
  * @file    RamReset.h
  * @author  TMC Terminal Team
  * @version V1.0.0
  * @date    04/21/2023
  * @brief   This file provides the RAM reset functions.
  * History:
  *          2023-04-21 Original version
  ******************************************************************************  
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, TMC SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2023 TMC</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RAM_RESET_H_
#define _RAM_RESET_H_

/**
* @brief  Get the version of RamReset Library.
* @param[in] Version: Two bytes buffer.
* @retval none
* @note	none
*/
void _GetRamResetLibVersion(unsigned char *Version);

/**
  * @brief  Ram reset api
  * @param  none
  * @retval none
  */
void _Thm36RamReset(void);


#endif //_RAM_RESET_H_

