/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB_enum.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "USB.h"	
#include "CCID.h"
#include "HID.h"
#include "VSP.h"
/***************************************************************************
* Const Name :  usbDeviceDescriptor
* Description:  Device Descriptor for get descriptor standard request
**************************************************************************/
const uint8_t usbDeviceDescriptor[] = {
0x12,			//	length
0x01,			//	Fixed device 
0x10,0x01,		//	USB 1.1
0x00,
0x00,
0x00,
EP0PKTSIZE,		//	Default max ep0 packet length
#if HID
0x25,0x09,		//	VID
0x29,0x90,		//	PID
#else
0x25,0x09,		//	VID
0x26,0x90,		//	PID
//0xE6,0x08,
//0x37,0x34,
#endif	
0x00,0x01,		//	Device release number
0x01,			//	Index of string descriptor describing manufacturer
0x02,			//	Index of string descriptor describing product
0x00,			//	Index of string descriptor describing the device’s serial number
0x01,			//	Number of possible configurations
};

/***************************************************************************
* Const Name :  usbConfigDescriptor
* Description:  Config Descriptor for get descriptor standard request
**************************************************************************/
const uint8_t usbConfigDescriptor[] = {
0x09,			//	length
0x02,			//	Fixed config
#if HID
HID_CFG_LEN,0x00,
0x01,			//	Only support 1 interface  by this configuratio
#else	
0x56,0x00,
0x01,
#endif
0x01,			//	Configuration Value is 1
0x00,			//	Index of string descriptor describing this configuration
0x80,			//	Doesn't support remote wake up		
0x32,			//	100mA
};

/***************************************************************************
* Const Name :  usbManufacturerStringDescriptor
* Description:  String Descriptor for get descriptor standard request
**************************************************************************/
const uint8_t usbManufacturerStringDescriptor[] = {
0x08,			//	bLength
0x03,			//	bDescriptorType
0x54,0x00,		//	T
0x4D,0x00,		//	M
0x43,0x00,		//	C
};

/***************************************************************************
* Const Name :  usbProductStringDescriptor
* Description:  String Descriptor for get descriptor standard request
**************************************************************************/
const uint8_t usbProductStringDescriptor[] = {
0x12,
0x03,
0x20,0x00,		//	
'T',0x00,		//	U
'H',0x00,	   //	3
'M',0x00,	   //	0
'3',0x00,	   //	8
'6',0x00,	   //	8
0x20,0x00,	   //
0x20,0x00,	   //	
};

const uint8_t usbVspDeviceDescriptor[] = { //虚拟串口
0x12,			//	length
0x01,			//	Fixed device 
0x10,0x01,		//	USB 1.1
0x02,		   //  CDC 通信类设备
0x00,
0x00,
EP0PKTSIZE,		//	Default max ep0 packet length 
//0xE6,0x08,		//	VID
//0x37,0x34,		//	CCID PID
0xf1,0x26,		//	VID?
0x03,0x88,		//	CCID3 PID?
0x00,0x01,		//	Device release number
0x01,			//	Index of string descriptor describing manufacturer
0x02,			//	Index of string descriptor describing product
0x03,			//	Index of string descriptor describing the device’s serial number
0x01,			//	Number of possible configurations
};
/***************************************************************************
* Function: usbStdReqGetDeviceDescriptor
* Description:  Deal with get device descriptor request 
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetDeviceDescriptor(void)
{
	#if VSP
	memcpy(USBbuf,(uint8_t *)usbVspDeviceDescriptor,usbDeviceDescriptor[0]);
	#else
	memcpy(USBbuf,(uint8_t *)usbDeviceDescriptor,usbDeviceDescriptor[0]);
	#endif	
	if(st_usbDeviceRequest.wLength > usbDeviceDescriptor[0])
		g_sUsbEp0TxLen = usbDeviceDescriptor[0];
	else
		g_sUsbEp0TxLen = st_usbDeviceRequest.wLength;	

	usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
	#if VSP ==0
	if(g_sUsbEp0TxLen == USBMAXPKTSIZE(0))	
	{
		TMC_Delay(0x2000);
		if(!EP0STSCXSTS)
		{
			usbEpnTxAll(0, USBbuf+USBMAXPKTSIZE(0), usbDeviceDescriptor[0]-USBMAXPKTSIZE(0));		
		}
	}
	#endif
}

/***************************************************************************
* Function: usbStdReqGetDeviceDescriptor
* Description:  Deal with get config descriptor request 
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetConfigDescriptor(void)
{
	uint16_t copy_config_descriptor_len;
#if VSP
	memcpy(USBbuf ,(uint8_t *)usbVspConfigDescriptor, VSP_CFG_LEN);
	copy_config_descriptor_len = VSP_CFG_LEN;
	memcpy(g_bVspLineCoding, (uint8_t*)vspLineCoding, 7);
	if(st_usbDeviceRequest.wLength >= copy_config_descriptor_len)
		g_sUsbEp0TxLen = copy_config_descriptor_len;
	else 
		g_sUsbEp0TxLen = st_usbDeviceRequest.wLength;	

	usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
#else
		//提前取出一部分参数
	memcpy(g_bVspLineCoding, (uint8_t*)vspLineCoding, 7);
	memcpy(USBbuf,(uint8_t *)usbConfigDescriptor,usbConfigDescriptor[0]);
	copy_config_descriptor_len = usbConfigDescriptor[0];

#if HID 
	memcpy(USBbuf + usbConfigDescriptor[0], (uint8_t *)hidInterfaceDescriptor, hidInterfaceDescriptor[0]);
	copy_config_descriptor_len += hidInterfaceDescriptor[0];

	memcpy(USBbuf + copy_config_descriptor_len,(uint8_t *)hidDeviceClassDescriptor, hidDeviceClassDescriptor[0]);
	copy_config_descriptor_len +=hidDeviceClassDescriptor[0]; 

	memcpy(USBbuf + copy_config_descriptor_len,(uint8_t *)(hidEndpointDescriptor+1), hidEndpointDescriptor[0]);
	copy_config_descriptor_len += hidEndpointDescriptor[0];

#else
	memcpy(USBbuf + usbConfigDescriptor[0],(uint8_t *)ccidInterfaceDescriptor,ccidInterfaceDescriptor[0]);
	copy_config_descriptor_len += ccidInterfaceDescriptor[0];

	memcpy(USBbuf + copy_config_descriptor_len,(uint8_t *)ccidSmartCardDeviceClassDescriptor,ccidSmartCardDeviceClassDescriptor[0]);
	copy_config_descriptor_len += ccidSmartCardDeviceClassDescriptor[0];

	memcpy(USBbuf + copy_config_descriptor_len,(uint8_t *)(ccidEndpointDescriptor+1),ccidEndpointDescriptor[0]);
	copy_config_descriptor_len += ccidEndpointDescriptor[0];

#endif

	if(st_usbDeviceRequest.wLength >= copy_config_descriptor_len)
		g_sUsbEp0TxLen = copy_config_descriptor_len;
	else 
		g_sUsbEp0TxLen = st_usbDeviceRequest.wLength;	

	usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
#endif

}

/***************************************************************************
* Function: usbStdReqGetDeviceDescriptor
* Description:  Deal with get string descriptor request 
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetStringDescriptor(void)
{
	if((st_usbDeviceRequest.wValue & 0x00FF) == 0x00)				//	For answering 1st Get_Descriport(String),Language ID,index= 0
	{
		USBbuf[0] = 0x04;											//	bLength
		USBbuf[1] = 0x03;											//	bDescriptorType
		USBbuf[2] = 0x09;											//	wLANGID = 0409,English(US)
		USBbuf[3] = 0x04;
		
		if(st_usbDeviceRequest.wLength >= 4)
			g_sUsbEp0TxLen = 4;
		else 
			g_sUsbEp0TxLen = st_usbDeviceRequest.wLength;

		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);

		return;
	}
	else if((st_usbDeviceRequest.wValue & 0x00FF) == 0x01)			//	index = 1
	{
		memcpy(USBbuf,(uint8_t *)usbManufacturerStringDescriptor,usbManufacturerStringDescriptor[0]);
		
		if((st_usbDeviceRequest.wLength != 0xFF) && (st_usbDeviceRequest.wLength != 0x01F4) && (st_usbDeviceRequest.wLength < usbManufacturerStringDescriptor[0]))
			g_sUsbEp0TxLen = st_usbDeviceRequest.wLength; 		
		else
			g_sUsbEp0TxLen = usbManufacturerStringDescriptor[0];

		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
		return;
	}
	else if((st_usbDeviceRequest.wValue & 0x00FF) == 0x02)			//	index = 2
	{
		#if VSP
		memcpy(USBbuf,(uint8_t *)usbVspProductStringDescriptor,usbProductStringDescriptor[0]);
		#else
		memcpy(USBbuf,(uint8_t *)usbProductStringDescriptor,usbProductStringDescriptor[0]);
		#endif
		if((st_usbDeviceRequest.wLength != 0xFF) && (st_usbDeviceRequest.wLength != 0x01F4) && (st_usbDeviceRequest.wLength < usbProductStringDescriptor[0]))
			g_sUsbEp0TxLen = st_usbDeviceRequest.wLength;
		else
			g_sUsbEp0TxLen = usbProductStringDescriptor[0];

		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
		return;
	}

	else
	{
		memcpy(USBbuf,(uint8_t *)ccidReaderStringDescriptor,ccidReaderStringDescriptor[0]);
		if((st_usbDeviceRequest.wLength < ccidReaderStringDescriptor[0]))
			g_sUsbEp0TxLen = st_usbDeviceRequest.wLength;
		else
			g_sUsbEp0TxLen = ccidReaderStringDescriptor[0];

		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
		return;
	}

}

void delay(uint32_t n)
{
	uint32_t i;
	for(i=0;i<n;i++);
}
