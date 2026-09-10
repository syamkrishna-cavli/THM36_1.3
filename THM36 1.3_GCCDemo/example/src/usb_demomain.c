/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: usb_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "USB.h"
#include "usb_demomain.h"
#include "string.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "vbat_demomain.h"

void USB_Init(void)
{
	//uint8_t timeout = 0;
	int i;
	RCC_Security2PeriphClockCmd(RCC_SEC2Periph_USB, ENABLE);


	USBPHYCKCON1 |= Bit1_En;

	USBPHYCKCON2 |= Bit13_En;

	USBPHYCKCON2 |= Bit16_En;

	//RCC_USBClockConfig(RCC_USB_CLKSRC_PLL_L, ENABLE);
	RCC_USBClockConfig(RCC_USB_CLKSRC_USBPHY, ENABLE);

	for(i=0;i<100;i++);

	USBDCON |= Bit20_En;

	for(i=0;i<10;i++)
	{
		__NOP();
		__NOP();
		__NOP();
	}

	USBPHYCON = 0x07;

	usb_Init();

}


void USB_DemoMain(void)
{
	/*Here provide Three kind of USB protocol:
		HID
		CCID
		VSP
	The users can switch the USB protocol by change the MACRO in usb.h file.
	*/
	NVIC_ClearPendingIRQ(USB_IRQn);
	NVIC_EnableIRQ(USB_IRQn);
	USB_Init();
	while(1)
	{
		USB_main();
	}
}

