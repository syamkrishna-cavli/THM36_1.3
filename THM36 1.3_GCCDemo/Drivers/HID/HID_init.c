/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: HID_init.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

#include "thm36_1.3.h"
#include "USB.h"	
#include "CCID.h"
#include "HID.h"

uint8_t g_bUsbEp0RcvDataFlag;
volatile uint8_t	g_bUsbEp0TxIntFlag;

const uint8_t hidInterfaceDescriptor[] = {
0x09,0x04,0x00,0x00,0x02,0x03,0x00,0x00,0x00,
};

const uint8_t hidDeviceClassDescriptor[] = {
0x09,0x21,0x10,0x01,0x00,0x01,0x22,HID_RPT_LEN,0x00,
};

const uint8_t hidEndpointDescriptor[] = {
14,
0x07,0x05,0x01,0x03,HIDINTPKTSIZE,0x00,0x01,
0x07,0x05,0x82,0x03,HIDINTPKTSIZE,0x00,0x01,
};

//====================================================
//Report Descriptor(input report/output report)
const uint8_t hidReportDescriptor[HID_RPT_LEN] =
{
	0x06, 0xA0, 0xFF, 	// Usage Page (vendor defined FFA0)  (Global)
	0x09, 0x01,			// Usage (vendor defined)   (Local)
	0xA1, 0x01,			// Collection (Application)  (Main)

	0x06, 0xA1, 0xFF,	// Usage Page (vendor defined FFA1)  (Global)

	//The input report
	0x09, 0x03,			// usage - vendor defined	(Local)
	0x15, 0x00,			// logical minimum (0)		(Local)
	0x25, 0xff,			// logical maximum (255)	  (Local)
	0x75, 0x08,			// Report Size (8)  (bits)   (Local)
	0x96, HIDTXMAXLENL, HIDTXMAXLENH,  // Report Count (255)  (fields)  (Local)
	0x81, 0x02,			// Input (Data, Variable, Absolute)  (Main)

	//The output report
	0x09, 0x04,			// usage - vendor defined	  (Local)
	0x15, 0x00,			// logical minimum (0)		  (Local)
	0x25, 0xff,			// logical maximum (255)		 (Local)
	0x75, 0x08,			// Report Size (8)  (bits)	   (Local)
	0x96, HIDRXMAXLENL, HIDRXMAXLENH,  // Report Count (255)  (fields)   (Local)
	0x91, 0x02,			// Output (Data, Variable, Absolute)  (Main)

	0xC0				// End Collection   (Main)
};


