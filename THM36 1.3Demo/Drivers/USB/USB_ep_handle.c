/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB_ep_handle.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "USB.h"	
#include "ccid.h"
#include "HID.h"


void usbCcidEpnRxAllData(uint8_t ep, uint8_t maxpktsize, uint8_t * buf)
{
	uint16_t	receivelen = 0;
	receivelen = usbEpnRxFirstPacket(ep, buf);
	if(receivelen == 0)
		return ;
	else if(receivelen < maxpktsize)
	{
		EPBUF0CLR(ep);
	}
	else
	{
		receivelen = buf[2];	
		receivelen = (receivelen<<8)+buf[1]+10 - maxpktsize;
		usbEpnRxLastAllData(ep, buf+maxpktsize, receivelen);	
	}
}

/***************************************************************************
* Function: usbDataEpStatus
* Description: Deal with USB endpoint receive and send data
* Input:  ep  : the number of ep,form 1 to 8
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/	
void usbDataEpStatus(uint8_t ep)
{
	
#if HID
	uint16_t	receivelen = 0;
	uint16_t bufoffset;
	if(ep == 1)															//	Ep1, CCID RX
	{
		usbEpnRxFirstPacket(1,g_abUsbEp1Buf);	
		receivelen = HIDRXLEN - HIDINTPKTSIZE;
		usbEpnRxLastAllData(1,g_abUsbEp1Buf+HIDINTPKTSIZE, receivelen);
		g_bUsbEp1RcvDataFlag = 1;
	}
	else if(ep == 2)													//	Ep2, CCID TX			
	{	 
		if(g_bUsbEp1RcvDataFlag == 2)
		{
			bufoffset = 0;
			while(g_sUsbEp2TxLen > 0)
			{
				if(g_sUsbEp2TxLen > HIDTXLEN)
				{	
					g_sUsbEp2TxLen -= HIDTXLEN;
				}
				else
				{
					g_sUsbEp2TxLen = 0;
				}
				usbEpnTxAll(2,g_abUsbEp2Buf+bufoffset,HIDTXLEN);
				bufoffset += HIDTXLEN;
			}
			g_sUsbEp2TxLen = 0;
			g_bUsbEp1RcvDataFlag = 0;	
			g_bUsbEp0TxIntFlag=0;	
		}
	}
#else
	if(ep == 1)															//	Ep1, CCID RX
	{
		usbCcidEpnRxAllData(1, EP1PKTSIZE, g_abUsbEp1Buf);
		g_bUsbEp1RcvDataFlag = 1;
	} 
	else if(ep == 2)													//	Ep2, CCID TX			
	{	 		
		if(g_bUsbEp1RcvDataFlag == 2)
		{
			usbEpnTxAll(2, g_abUsbEp2Buf, g_sUsbEp2TxLen);
			g_sUsbEp2TxLen = 0;	
			g_bUsbEp1RcvDataFlag = 0;
		}

	}
#endif	
}

/***************************************************************************
* Function: usbEpHandle
* Description: check which USB EP need to be handled
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbEpHandle(void)
{	
	uint32_t	intstatus;
	uint32_t	backup_USBDCON;
	uint8_t  i;
	uint8_t  epid = 0xFF;
	
	if(USBMISTS & Bit16_En)													//	USB reset
	{	
		USBMISTS |= Bit16_En;												//	Clear USB reset pending bit
		if(USB_Time != 0)													//	If it is not the 1st time for entering USB mode 
		{
			backup_USBDCON = USBDCON;										//	Back up USBDCON
			USBDCON |= Bit20_En;											//	S/W bit,H/W reset the USB controller
			for(i=0;i<5;i++)												//	Waiting for USB controller reset,16 cycles needed
			{
				__nop();
				__nop();
				__nop();	
			}
			USBDCON |= backup_USBDCON;										//	Reset the register to enable USB interrupts
			USB_Init();
		}
		return;					
	}

	else if((intstatus = (USBMISTS & 0x0000FFFF)) >= 0x00000001)			//	At least one Ep interrupt is generated
	{
		for(epid=0;epid<13;epid++)
		{
			if(intstatus & (0x0001 << epid))
			{			
				if((epid == 2)&&(g_bUsbEp1RcvDataFlag == 2))
					break;

				else
					break;
			}
		}

	}
//	if(epid != 0)
//	{
//		USB_Time = 1;
//	}	
	if(epid < 13)
	{
		/* USB soft test mode */
		usbDataEpStatus(epid);
	}	
}

