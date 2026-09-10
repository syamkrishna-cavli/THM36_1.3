/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: VSP.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef VSP_H_
#define VSP_H_

//#if VSP



#define VSPINTPKTSIZE		0x40

#define	GET_LINE_CODING 	0x21
#define SET_LINE_CODING	 0x20
#define SET_CONTROL_LINE_STATE  0x22


#define VSPLINECODINGLEN	0x07


#define VSPRXMAXLENL 0x0B
#define VSPRXMAXLENH 0x08

#define VSPTXMAXLENL 0x1F
#define VSPTXMAXLENH 0x00

#define VSP_CFG_LEN		 0x43


#define	VSPRXLEN	0x40
#define	VSPTXLEN	0x40

extern __attribute__ ((aligned (4))) uint8_t g_abUsbEp1Buf[];				//	USB Ep1 data trans buf
extern __attribute__ ((aligned (4))) uint8_t g_abUsbEp2Buf[];				//	USB Ep2 data trans bufv

extern uint8_t g_bVspLineCoding[7];

extern const uint8_t usbVspProductStringDescriptor[];

extern void	vspGetLineCoding(void);
extern void	vspSetLineCoding(void);
extern void	vspSetCtrlLineState(void);

extern void	vspClassRequest(uint8_t req);


extern void VSP_main(void);

extern const uint8_t usbVspDeviceDescriptor[];
extern const uint8_t usbVspConfigDescriptor[];
extern const uint8_t vspLineCoding[];

extern uint32_t usbVspRecvChars(uint8_t *buf, uint32_t bufSize);
extern void usbVspSendChars(uint8_t *buf, uint32_t len);

extern void usbEnumerate(void);
//#endif

#endif
