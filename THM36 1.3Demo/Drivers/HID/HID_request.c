/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: HID_request.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

#include "thm36_1.3.h"
#include "USB.h"	
#include "ccid.h"
#include "HID.h"
/***************************************************************************
* Function: hidSendReportDscrpt
* Description:HID Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void	hidSendReportDscrpt(void)
{
	memcpy(USBbuf,(uint8_t *)hidReportDescriptor,HID_RPT_LEN);
	usbEpnTxAll(0,USBbuf,HID_RPT_LEN);	
}

/***************************************************************************
* Function: Hid_SendReportDscrpt
* Description:HID Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void	HID_Request(uint8_t req)
{	
	switch(req)
	{
		case GET_INTERFACE:
			fGet_Interface();
			break;
		
		case SET_REPORT:
			hidSetReportRequest();
			break;
		
		case GET_REPORT:
			hidGetReportRequest();
			break;
		
		default:
			break;
	}
}

/***************************************************************************
* Function: Hid_SendReportDscrpt
* Description:HID Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void	hidGetReportRequest(void)
{
	g_bUsbEp0TxIntFlag = 2;	
}

/***************************************************************************
* Function: fSet_Report
* Description:HID Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void	hidSetReportRequest(void)
{
	usbEp0RxAll(g_abUsbEp1Buf, HIDRXLEN);
	g_bUsbEp0RcvDataFlag = 1;
	
}

void fGet_Interface(void)
{	uint32_t TimeCounter = 0;	
	while(!(EP0STSCXSTS))												//	Status stage,CXSTS interrupt	
	{
		//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}	
	EP0CXSTSCLR;													//	Write 1 to clear CXSTS interrupt
	EP0SENDACK;		
}

