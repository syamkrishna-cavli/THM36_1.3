/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: VSP_main.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


#include "thm36_1.3.h"
#include "USB.h"  
#include "VSP.h"
#include "VSP_demo.h"

/***************************************************************************
* Function: usbVspSendChars
* Description: Send data
* Input:  buf 
* Input:  len 
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbVspSendChars(uint8_t *buf, uint32_t len)
{
	uint32_t offset=0;
	while(len>=VSPTXLEN){
		memcpy(g_abUsbEp2Buf, buf+offset, VSPTXLEN);
		usbEpnTxAll(2,g_abUsbEp2Buf,VSPTXLEN);
		USB_CDC_Delay(1);
		offset += VSPTXLEN;
		len -= VSPTXLEN;
	}
	if (len > 0){
		memcpy(g_abUsbEp2Buf, buf+offset, len);
		usbEpnTxAll(2,g_abUsbEp2Buf,len);
	}
}

/***************************************************************************
* Function: usbVspRecvChars
* Description: Receive data
* Input:  buf
* Input:  bufSize
* Return: The length of receive
* Other:  NULL
**************************************************************************/
uint32_t usbVspRecvChars(uint8_t *buf, uint32_t bufSize)
{
	uint32_t rcvlen = 0;
	
	if (USBEP1STS & Bit19_En)
	{
		rcvlen = (USBEP1STS >> 8) & 0x000003FF;
		rcvlen = rcvlen > bufSize ? bufSize:rcvlen;
		memcpy(buf, g_abUsbEp1Buf, rcvlen);
		
		USBEP1CON |= Bit9_En;											//	clear buf 控制端点是bit10  其他端点是bit9
	}
	return rcvlen;
}

