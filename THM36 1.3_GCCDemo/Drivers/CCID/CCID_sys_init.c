/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: CCID_sys_init.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


#include "thm36_1.3.h"
#include "USB.h"  
#include "CCID.h"


uint8_t g_Icc1PowerOn;

uint8_t g_bUsbEp1RcvDataFlag;
uint16_t	g_sUsbEp2TxLen;	

#ifdef __GNUC__ 
//#pragma pack (4)
#if	DYNAMICFIFO
	uint8_t g_abUsbEp1Buf[USB_BUF_LEN] __attribute__((section(".usbram")));										//	USB Ep1 data trans buf
	uint8_t g_abUsbEp2Buf[USB_BUF_LEN] __attribute__((section(".usbram")));										//	USB Ep2 data trans bufv
#else
	uint8_t g_abUsbEp1Buf[USB_BUF_LEN] __attribute__((aligned (4)));										
	uint8_t g_abUsbEp2Buf[USB_BUF_LEN] __attribute__((aligned (4)));	
#endif
//#pragma pack ()
#else

//	Length of Ep2 response data to be transmitted
#if	DYNAMICFIFO
__align(4) uint8_t g_abUsbEp1Buf[USB_BUF_LEN] __attribute__((at(USBFIFO_ADDR_START+USB_BUF_LEN*3)));										//	USB Ep1 data trans buf
__align(4) uint8_t g_abUsbEp2Buf[USB_BUF_LEN] __attribute__((at(USBFIFO_ADDR_START+USB_BUF_LEN*2)));										//	USB Ep2 data trans bufv
#else
__align(4) uint8_t g_abUsbEp1Buf[USB_BUF_LEN] ;										
__align(4) uint8_t g_abUsbEp2Buf[USB_BUF_LEN] ;	
#endif
#endif


CCID_COMMAND_MESSAGE st_ccidCmdMessage = {0,};


//CCID½Ó¿ÚÃèÊö·û
const uint8_t ccidInterfaceDescriptor[] = {
0x09, 					//	length
0x04,					//	Fixed interface	
0x00,					//	Interface  Number is 0
0x00,					//	Alternate Setting is 0
//0x03,					//	Ep number is 3,they are 1.2.5
0x02,					//	Ep number is 3,they are 1.2
0x0b,					//	Smart Card Device Class
0x00,					//	Subclass code
0x00,					//	CCID Protocol
0x04,					//	Index of string descriptor describing this interface
};

//CCIDÉè±¸ÀàÃèÊö·û
const uint8_t ccidSmartCardDeviceClassDescriptor[] = {
0x36,		   			//	length
0x21,		   			//	Functional S/C Device Class descriptor type
0x00,0x01,	   			//	CCID1.0
0x00,		   			//	1 Slot numbered 0x00
0x07,		   			//	01H-5.0V,02H-3.0V,03H-1.8V
0x03,0x00,0x00,0x00,	//	Little Endian,Upper uint16_t is RFU = 0x0000,Lower uint16_t:0001H-T=0,0002H-T=1
0xa0,0x0f,0x00,0x00,	//	3.58MHz=3580KHz=0x0DFC
0xa0,0x0f,0x00,0x00,	//	14.32MHz
0x00,
0x00,0x2a,0x00,0x00,	//	9600bps
0x16,0x40,0x05,0x00,	//	115200bps
0x00,
0x00,0x00,0x01,0x00,	//	IFSD (T=1)
0x00,0x00,0x00,0x00,	//	Upper uint16_t is RFU = 0x0000,Lower uint8_t encodes the supported protocol type
0x00,0x00,0x00,0x00,	//	No spefcial characteristics
0x80,0x00,0x04,0x00,
//0x0f,0x01,0x00,0x00,	//	271B = 10B Header + 5B CMD + 256B data
0x00,0x00,0x01,0x00,	//	0x10000B = 10B Header + CMD + data
0x00,
0x00,
0x00,0x00,				//	No LCD
0x00,
0x01, 					//	1 slot can be simultaneously busy
};

//CCID¶ËµãÃèÊö·û
const uint8_t ccidEndpointDescriptor[] = {
0x0E,					//	total length
						//	ep2 setting
0x07,
0x05, 					//	Fixed endpoint
(USBEPnDIR^0x80)|0x01,	//	DIR,Ep1
0x02,					//	BULK
EP1PKTSIZE,0x00,		//	max packet length is BULKPktSize
0x00,	   				//	Don't care
						//	ep2 setting
0x07,
0x05,					//	Fixed endpoint
USBEPnDIR|0x02,			//	DIR Ep2
0x02,					//	BULK
EP2PKTSIZE,0x00,		//	max packet length is BULKPktSize
0x00,					//	Don't care	

};

//contact reader
const uint8_t ccidReaderStringDescriptor[] = {
34,		   			//	length
0x03,		   			//	fixed string
0x55,0x00,	   			//	U
0x53,0x00,	   			//	S
0x42,0x00,	   			//	B 
0x20,0x00,	   			//	
0x43,0x00,	   			//	C
0x43,0x00,	   			//	C
0x49,0x00,	   			//	I
0x44,0x00,	   			//	D
0x20,0x00,				//
'R',0x00,	   			//	R
'e',0x00,	   			//	e
'a',0x00,	   			//	a
'd',0x00,	   			//	d
'e',0x00,	   			//	e
'r',0x00,	   			//	r
0x20,0x00,				//	
	
};


