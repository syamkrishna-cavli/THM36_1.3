/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: VSP_demo.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#include "thm36_1.3.h"
#include "USB.h"  
#include "VSP.h"

//uint8_t *g_abUsbEp0Buf = (uint8_t *)USBFIFO_ADDR_START;										//	USB Ep1 data trans buf
//uint8_t *g_abUsbEp1Buf = (uint8_t *)USBFIFO_ADDR_START+0x100;									//	USB Ep1 data trans buf
//uint8_t *g_abUsbEp2Buf = (uint8_t *)USBFIFO_ADDR_START+0xA00;									//	USB Ep2 data trans bufv
//uint8_t *g_abUsbEp3Buf = (uint8_t *)USBFIFO_ADDR_START+0xB00;									//	USB Ep2 data trans bufv
extern uint32_t usbVspRecvChars(uint8_t *buf, uint32_t bufSize);
extern void usbVspSendChars(uint8_t *buf, uint32_t len);
uint8_t CmdBuf[8];
void USB_CDC_Delay(unsigned int ms)
{
	volatile unsigned long  i,j,k;
	k = 22283;
	for(i=0;i<ms;i++)
	{
		for(j=0;j<k;j++)
		{}
	}
}

void usbEnumerate(void)
{
	uint32_t cnt=0;
//	uint8_t *g_abUsbEp0Buf = (uint8_t *)USBFIFO_ADDR_START;										//	USB Ep1 data trans buf
//	uint8_t *g_abUsbEp1Buf = (uint8_t *)USBFIFO_ADDR_START+0x100;									//	USB Ep1 data trans buf
//	uint8_t *g_abUsbEp2Buf = (uint8_t *)USBFIFO_ADDR_START+0xA00;									//	USB Ep2 data trans bufv
//	uint8_t *g_abUsbEp3Buf = (uint8_t *)USBFIFO_ADDR_START+0xB00;									//	USB Ep2 data trans bufv
	while(1)
	{
		usbEpHandle();
		USB_CDC_Delay(1);
		cnt++;
		if(cnt>2000)
			break;
	}
}


void VSP_main(void)
{
	
	while(1)
	{
		if(usbVspRecvChars(CmdBuf,8)==8)
		{
			usbVspSendChars(CmdBuf,8);
		}
	}

}


