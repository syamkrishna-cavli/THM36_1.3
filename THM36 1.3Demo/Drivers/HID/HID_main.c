/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: HID_main.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "USB.h"    
#include "ccid.h"
#include "HID.h"
#include "cmd_handle.h"

uint16_t	hidCommandProcess(uint8_t * cmddatabuf, uint16_t len, uint8_t* responsedatabuf);

/***************************************************************************
* Function: HID_main
* Description:HID Data processing
* Input: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void HID_main(void)
{
	uint16_t bufoffset;

	if((g_bUsbEp1RcvDataFlag == 1) ||(g_bUsbEp0RcvDataFlag ==1))
	{
		g_sUsbEp2TxLen = hidCommandProcess(g_abUsbEp1Buf, HIDRXLEN, g_abUsbEp2Buf);
	}
	if(g_bUsbEp1RcvDataFlag == 1)
		g_bUsbEp1RcvDataFlag = 2;

	if(g_bUsbEp0RcvDataFlag == 1)					
	{
		bufoffset = 0;
		while(g_sUsbEp2TxLen > 0)
		{
			while(g_bUsbEp0TxIntFlag != 2);
		
			if(g_sUsbEp2TxLen > HIDTXLEN)
			{	
				g_sUsbEp2TxLen -= HIDTXLEN;
			}
			else
			{
				g_sUsbEp2TxLen = 0;
			}
			usbEpnTxAll(0,g_abUsbEp2Buf+bufoffset,HIDTXLEN);
			g_bUsbEp0TxIntFlag = 0;
			bufoffset += HIDTXLEN;
		}
		g_sUsbEp2TxLen = 0;
		g_bUsbEp0RcvDataFlag = 0;		
	}
}


/***************************************************************************
* Function: HID_main
* Description:HID Data processing
* Input: cmddatabuf: first 5 bytes is cmd, others is data
 		 len: the total length of cmd and data
* output: responsedatabuf : responsed data
* Return: 0:      time out or failed
		  others: the length of data for response	
* Other:  NULL
**************************************************************************/
uint16_t	hidCommandProcess(uint8_t * cmddatabuf, uint16_t len, uint8_t* responsedatabuf)
{
	uint16_t return_len = 0;	
	memcpy(g_abComBuf, cmddatabuf, 5);
	memcpy(IOBuf, cmddatabuf+5, len-5);

	if(CLA == 0xFF && INS == 0x10)				   //	PPS
	{
		return 4;
	}
	else
	{
		Foffset = P1*0x100 + P2;				   //	Start address of flash reading/writing
		SWptr = SUCCESS;						   //	9000 as default
		CMD_Handle();					       	   //	ISO7816 CMD Handler
	}
	
	if(((INS == GETRDMN)|| ((INS == ALGOPER) && (((P2&0xB0) == 0x10)|| ((P2&0xB0) == 0x90)))) && ((SWptr == SUCCESS)))
	{		
		if(P3 == 0x00)
		{
			memcpy(responsedatabuf,IOBuf,0x100);
			return_len = 0x100;
		}		
		else
		{
			return_len = P3;
			memcpy(responsedatabuf,IOBuf,P3);
		} 
	}

	responsedatabuf[return_len++] = SW[SWptr];					//	SW1
	responsedatabuf[return_len++] = SW[SWptr+1];				//	SW2

	return return_len;	
}

