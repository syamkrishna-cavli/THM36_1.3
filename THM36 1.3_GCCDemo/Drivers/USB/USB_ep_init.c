/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB_ep_init.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "USB.h"
#include "CCID.h"
#include "VSP.h"


#ifndef SIMULATOR


static void usbSetMaxPktSize(uint8_t epn,uint16_t maxlength);


/***************************************************************************
* Function: usbEpInit
* Description: Initialize ep
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/	
void	usbEpInit(void)
{
	#if VSP
	USBCECON = 0x00010C07;
 //	BULK OUT,double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP1CON = 0x00230600;
	//	BULK IN, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP2CON = 0x00330600;
	//	INT IN, double buffer disable,maxpkt=8B,clr buf 0 & 1,not STALL
	USBEP3CON = 0x00500600;
	#else
	//	maxpkt=16B,clr buf 0 & 1,not complete CXSTS,CXDPKT,CXS enable
	//USBCECON = 0x00010C05;								//	default endpoint
	USBCECON = 0x00010C01;								//	default endpoint
	//	BULK OUT,double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP1CON = 0x00230600;								//	used for BULK OUT
	//	BULK IN, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP2CON = 0x00330600;								//	used for BULK IN
	//	BULK OUT,double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP3CON = 0x00230600; 							//	used for BULK OUT
	//	BULK IN, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP4CON = 0x00330600;	 							//	used for BULK IN
	//	INT IN, double buffer disable,maxpkt=8B,clr buf 0 & 1,not STALL
	USBEP5CON = 0x00500600;	 							//	used for INTERRUPT IN
	//	INT IN, double buffer disable,maxpkt=8B,clr buf 0 & 1,not STALL
	USBEP6CON = 0x00500600;	 							//	used for INTERRUPT IN
	//	BULK OUT, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP7CON = 0x00230600;	 							//	used for INTERRUPT IN
	//	BULK IN, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP8CON = 0x00330600;
	//	BULK OUT, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP9CON = 0x00230600;	 							//	used for INTERRUPT IN
	//	BULK IN, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP10CON = 0x00330600;	//	used for INTERRUPT IN
	//	BULK OUT, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP11CON = 0x00230600;	 							//	used for INTERRUPT IN
	//	BULK IN, double buffer disable,maxpkt=64B,clr buf 0 & 1,not STALL
	USBEP12CON = 0x00330600;	//	used for INTERRUPT IN
	#endif
	usbSetMaxPktSize(0,EP0PKTSIZE);
	#if VSP==0
	usbSetMaxPktSize(1,EP1PKTSIZE);
	usbSetMaxPktSize(2,EP2PKTSIZE);
	usbSetMaxPktSize(3,EP3PKTSIZE);
	usbSetMaxPktSize(4,EP4PKTSIZE);
	usbSetMaxPktSize(5,EP5PKTSIZE);
	usbSetMaxPktSize(6,EP6PKTSIZE);
	usbSetMaxPktSize(7,EP7PKTSIZE);
	usbSetMaxPktSize(8,EP8PKTSIZE);
	usbSetMaxPktSize(9,EP9PKTSIZE);
	usbSetMaxPktSize(10,EP10PKTSIZE);
	usbSetMaxPktSize(11,EP11PKTSIZE);
	usbSetMaxPktSize(12,EP12PKTSIZE);

#if (USBEPnDIR == IN)
	SetEndpointOUT(1)
	SetEndpointIN(2)
	SetEndpointOUT(3)
	SetEndpointIN(4)
	SetEndpointOUT(5)
	SetEndpointIN(6)
	SetEndpointOUT(7)
	SetEndpointIN(8)
	SetEndpointOUT(9)
	SetEndpointIN(10)
	SetEndpointOUT(11)
	SetEndpointIN(12)
#else
	SetEndpointOUT(2)
	SetEndpointIN(1)
	SetEndpointOUT(4)
	SetEndpointIN(3)
	SetEndpointOUT(6)
	SetEndpointIN(5)
	SetEndpointOUT(8)
	SetEndpointIN(7)
	SetEndpointOUT(10)
	SetEndpointIN(9)
	SetEndpointOUT(12)
	SetEndpointIN(11)
#endif	
#endif

#if	DYNAMICFIFO
	USBBFCON |= Bit7_En;

	USBCEBF = (uint32_t)g_abUsbEp0Buf;
	USBEP1BF = (uint32_t)g_abUsbEp1Buf;
	USBEP2BF = (uint32_t)g_abUsbEp2Buf;

#else 
	USBBFCON &= Bit7_Dis;
	USBBFCON = (((uint32_t)USBFIFO_ADDR_START) - SRAM1_ADDR_START) >> 6;	//	set USB buf start address
#endif
	
	USBDCON |= 0x00000001;								//	Enable EP0 interrupt
//	USBEP1CON |= Bit0_En;								//	Enable EP1 interrupt
//	USBEP2CON |= Bit0_En;								//	Enable EP2 interrupt
//	USBEP3CON |= Bit0_En;					
//	Enable EP3 interrupt
//	USBEP4CON |= Bit0_En;								//	Enable EP4 interrupt
//	USBDCON |= Bit16_En;								//	Enable RST interrupt for debugging
//	USBDCON |= Bit17_En;								//	Enable Suspend interrupt for debugging
//	USBDCON |= Bit18_En;								//	Enable SOF interrupt for debugging
//	USBDCON = 0x7FFFF;									//	Enable all interrupts
//	USBDCON &= Bit16_Dis;								//	Disable RST interrupt for debugging
//	USBDCON &= Bit17_Dis;								//	Disable Suspend interrupt for debugging
//	USBDCON &= Bit18_Dis;								//	Disable SOF interrupt for debugging
}

/***************************************************************************
* Function: usbSetMaxPktSize
* Description: Set all EP Max packet size
* Input:  epn	   :  the number of ep,form 0 to 8
		  maxlength	:  the max packet length, the value can be set to 16,32,64
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
static void usbSetMaxPktSize(uint8_t epn,uint16_t maxlength)
{
	uint32_t tempUSBEPCON;
	tempUSBEPCON = USBEPCON(epn);
	tempUSBEPCON &= 0xFFF8FFFF;
	if(maxlength == 16)
	{
		tempUSBEPCON |= 0x00010000;
	}
	else if(maxlength == 64)
	{
		tempUSBEPCON |= 0x00030000;	
	}
	else if(maxlength == 32)
	{
		tempUSBEPCON |= 0x00020000;	
	}
	USBEPCON(epn) = tempUSBEPCON;
}

#endif
