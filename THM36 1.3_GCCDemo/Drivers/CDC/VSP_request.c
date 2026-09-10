/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: VSP_request.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

#include "thm36_1.3.h"
#include "USB.h"  
#include "VSP.h"
uint8_t g_bVspLineCoding[7];
/***************************************************************************
* Function: vspClassRequest
* Description:VSP Data processing
* Input: req(AT command)
* Return: NULL	
* Other:  NULL
**************************************************************************/
void vspClassRequest(uint8_t req)
{	
	switch(req)
	{
		case SET_LINE_CODING:
			vspSetLineCoding();	 
			break;//0x20
		
		case GET_LINE_CODING:
			vspGetLineCoding();
			break;//0x21
		
		case SET_CONTROL_LINE_STATE:
			vspSetCtrlLineState();
			break;//0x22
		
		default:
			break;
	}
}

/***************************************************************************
* Function: vspGetLineCoding
* Description:VSP Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void vspGetLineCoding(void)
{
	
	memcpy(g_abUsbEp0Buf, (uint8_t*)g_bVspLineCoding, 7);
	
	usbEpnTx(0,g_abUsbEp0Buf,7);
}
/***************************************************************************
* Function: vspSetLineCoding
* Description:VSP Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void vspSetLineCoding(void)
{
	uint32_t TimeCounter = 0;
	while(!EP0STSCXDPKT)
	{ //TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
	}
	memcpy(g_bVspLineCoding, g_abUsbEp0Buf, 7);
}
/***************************************************************************
* Function: vspSetCtrlLineState
* Description:VSP Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void vspSetCtrlLineState(void)
{
	if (g_abUsbEp0Buf[2] == 0x00)
	{
	}
	else if (g_abUsbEp0Buf[2] == 0x03) //Open VSP
	{
	}
	else if (g_abUsbEp0Buf[2] == 0x02) //Close VSP
	{
	}
}
