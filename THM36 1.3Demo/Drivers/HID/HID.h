/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: HID.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 



#ifndef HID_H_
#define HID_H_

//#define HID 0


#define HIDINTPKTSIZE		0x40

#define	SET_REPORT			0x09
#define	GET_REPORT			0x01

#define HIDRREPORT  		0x22

#define HID_RPT_LEN 37 

#define HIDRXMAXLENL 0x1C
#define HIDRXMAXLENH 0x01

#define HIDTXMAXLENL 0x1C
#define HIDTXMAXLENH 0x01

#define HID_CFG_LEN 0x29

#define	HIDRXLEN	(((uint16_t)HIDRXMAXLENH<<8)+HIDRXMAXLENL)
#define	HIDTXLEN	(((uint16_t)HIDTXMAXLENH<<8)+HIDTXMAXLENL)


extern uint8_t g_bUsbEp0RcvDataFlag;
extern volatile uint8_t	g_bUsbEp0TxIntFlag;

extern uint8_t g_bUsbEp1RcvDataFlag;
extern uint16_t	g_sUsbEp2TxLen;	
													
extern __align(4) uint8_t g_abUsbEp1Buf[];				//	USB Ep1 data trans buf
extern __align(4) uint8_t g_abUsbEp2Buf[];				//	USB Ep2 data trans bufv

extern const uint8_t hidInterfaceDescriptor[];
extern const uint8_t hidDeviceClassDescriptor[];
extern const uint8_t hidEndpointDescriptor[];
extern const uint8_t hidReportDescriptor[];


extern void	HID_Request(uint8_t req);

extern void	hidSetReportRequest(void);
extern void	hidGetReportRequest(void);
void	fGet_Interface(void);

extern void hidSendReportDscrpt(void);

extern void HID_main(void);

#endif
