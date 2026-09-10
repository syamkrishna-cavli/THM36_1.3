/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#ifndef _USB_H_
#define _USB_H_
typedef	void (* voidfunc) (void);
// VSP(CDC) HID CCID Only one macro can be true.
#define VSP					0
#define HID 				0
#define CCID 				1

#ifndef SIMULATOR	
extern uint8_t	USB_Time;
//	Standard Request Codes (bRequest)
#define	GET_STATUS			0
#define	CLEAR_FEATURE		1
#define USBSTDREQRFU1		2
#define	SET_FEATURE			3
#define USBSTDREQRFU2		4
#define	SET_ADDRESS			5
#define	GET_DESCRIPTOR		6
#define	SET_DESCRIPTOR		7
#define	GET_CONFIGURATION	8
#define	SET_CONFIGURATION	9
#define	GET_INTERFACE		10
#define	SET_INTERFACE		11
#define	SYNCH_FRAME			12
//	Descriptor Type
#define	DEVICE				1
#define	CONFIGURATION		2
#define	STRING				3
#define	INTERFACE			4
#define	ENDPOINT			5

//	Define transfer type
//#define CTRL				0x01
#define	BULK				0x02
#define	INTT				0x03
#define	ISO					0x04

//	Define pkt size
#define MAXPKTSIZE			0x40
#define	EP0PKTSIZE			MAXPKTSIZE
#define	EP1PKTSIZE			MAXPKTSIZE
#define	EP2PKTSIZE			MAXPKTSIZE
#define	EP3PKTSIZE			MAXPKTSIZE
#define	EP4PKTSIZE			MAXPKTSIZE
#define	EP5PKTSIZE			MAXPKTSIZE
#define	EP6PKTSIZE			MAXPKTSIZE
#define	EP7PKTSIZE			MAXPKTSIZE
#define	EP8PKTSIZE			MAXPKTSIZE
#define	EP9PKTSIZE			MAXPKTSIZE
#define	EP10PKTSIZE			MAXPKTSIZE
#define	EP11PKTSIZE			MAXPKTSIZE
#define	EP12PKTSIZE			MAXPKTSIZE

//	Define transfer direction
#define IN  				0x80
#define OUT 				0x00
#define USBEPnDIR			IN

//	Define transfer over time
#define	USBOVERTIME			0x3000		

//	Define type of status
#define	ACK					0x00
#define	NAK					0x01

//	Define USB buffer length
#define USB_BUF_LEN			0x120

//	Enable or disable Dynamic FIFO,related configuration
#define	DYNAMICFIFO	1	

//USBOVERTIME_3MS:K88V20 = 8100 ; THM1.3 = 96700 ; THM36 = 82475 ; THM36 1.1 = 58000
#define USBOVERTIME_3MS  96700	// ~ 3 mS @ 192MHz

//	Variables and functions defined in USB_ep.c
typedef struct	{
uint8_t	bmRequestType;
uint8_t	bRequest;
uint16_t	wValue;
uint16_t	wIndex;
uint16_t	wLength;
uint8_t	bStdRqstFlag;						//	To indicate standard request is received
uint8_t	bClsRqstFlag;						//	To indicate class request is received
uint8_t	bVdrRqstFlag;						//	To indicate vendor request is received
}USB_DEVICE_REQUEST;


extern uint16_t	g_sUsbEp0TxLen;				//	Length of Ep0 data to be transmitted
extern uint8_t g_abUsbEp0Buf[];

#if	!DYNAMICFIFO
extern uint8_t g_abUsbBuf[];
#endif


#define USBbuf	g_abUsbEp0Buf

extern uint8_t g_bCcidReader;
extern 	USB_DEVICE_REQUEST st_usbDeviceRequest;


extern void usbStdReqGetDeviceDescriptor(void);
extern void usbStdReqGetConfigDescriptor(void);
extern void usbStdReqGetStringDescriptor(void);

extern void usbStdReqGetDescriptor(void);
extern void usbStdReqGetStatus(void);
extern void usbStdReqRfu1(void);
extern void usbStdReqClearFeature(void);
extern void usbStdReqRfu2(void);
extern void usbStdReqSetFeature(void);
extern void usbStdReqSetAddress(void);
extern void usbStdReqSetDescriptor(void);
extern void usbStdReqGetConfiguration(void);
extern void usbStdReqSetConfiguration(void);
extern void usbStdReqGetInterface(void);
extern void usbStdReqSetInterface(void);
extern void usbStdReqSynchFrame(void);

extern const voidfunc usbStdReq[0x0D];



extern void USB_main(void);
extern void usbEpHandle(void);
extern void usbReqHandle(void);
extern void	usbEpInit(void);
extern void USB_Init(void);
extern void USB_PowerOn(void);
extern void USB_ClockCfg(uint8_t clk);
extern void usb_Init(void);

extern void usbEp0RxTx(void);
extern void	usbEp0RxAll(uint8_t * buf,uint16_t len);
extern void usbEpnTx(uint8_t ep, uint8_t * buf,uint8_t len);
extern void usbEpnTxAll(uint8_t ep,uint8_t * buf,uint16_t len);
extern uint8_t   usbEpnRxFirstPacket(uint8_t ep,uint8_t * buf);
extern uint16_t	usbEpnRxLastAllData(uint8_t ep,uint8_t * buf,uint16_t len);



#endif



#endif //_USB_H_
