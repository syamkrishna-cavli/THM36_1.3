/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB_main.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 



#include "thm36_1.3.h"
#include "USB.h"	  
#include "CCID.h"
#include "HID.h"
#include "VSP.h"


#ifndef SIMULATOR

uint8_t USB_Time;
uint8_t g_bCcidReader;
							
/***************************************************************************
* Function: usbmain
* Description: USB main process
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void	USB_main(void)
{
		

#if HID
	usbEpHandle();
	HID_main();

#elif VSP
	usbEnumerate();
	VSP_main();
#else
	usbEpHandle();
#if (USBEPnDIR == IN)	
	if(g_bUsbEp1RcvDataFlag == 1 )
		g_bCcidReader = 1;

	else 
 		g_bCcidReader = 0;
#else
	if(g_bUsbEp2RcvDataFlag == 1 )
		g_bCcidReader = 1;

	else 
 		g_bCcidReader = 0;
#endif

	CCID_main(g_bCcidReader);

#if (USBEPnDIR == IN)	
	if(g_bCcidReader == 1)	
		g_bUsbEp1RcvDataFlag = 2;

#else
	if(g_bCcidReader == 1)	
		g_bUsbEp2RcvDataFlag = 2;

#endif
#endif
			
}

/***************************************************************************
* Function: usbReqHandle
* Description: USB standard request process
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbReqHandle(void)
{
	if(st_usbDeviceRequest.bStdRqstFlag)									//	Standard
	{
		usbStdReq[st_usbDeviceRequest.bRequest]();
		st_usbDeviceRequest.bStdRqstFlag = 0;
	}
	else if(st_usbDeviceRequest.bClsRqstFlag)								//	Class
	{
	#if HID
		HID_Request(st_usbDeviceRequest.bRequest);
	#elif CCID
		ccidRequest(st_usbDeviceRequest.bRequest);
	#elif VSP
		vspClassRequest(st_usbDeviceRequest.bRequest);
	#endif
		st_usbDeviceRequest.bClsRqstFlag = 0;
	}
}

							
/***************************************************************************
* Function: USB_Init
* Description: usb Init
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usb_Init(void)
{
/* USB parameter init */
	USB_Time = 0;		
	st_usbDeviceRequest.bStdRqstFlag = 0;
	st_usbDeviceRequest.bClsRqstFlag = 0;
	st_usbDeviceRequest.bVdrRqstFlag = 0;	
	g_sUsbEp0TxLen = 0;
	g_Icc1PowerOn = 0;
	
#if (USBEPnDIR == IN)
	g_bUsbEp1RcvDataFlag = 0;
	g_sUsbEp2TxLen = 0;

#else
	g_bUsbEp2RcvDataFlag = 0;
	g_sUsbEp1TxLen = 0;

#endif
	usbEpInit();															//	USB Eps initialize	

//	USBPCON	= 0x00000002;													//	USB physical layer power on and no suspend 
//	USBPHYCON = 0x00000001;													//	Enable DP PU
}


#endif
