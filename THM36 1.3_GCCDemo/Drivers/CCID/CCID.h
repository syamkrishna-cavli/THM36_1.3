/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: CCID.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __CCID_H__
#define __CCID_H__



//	Macros for indicating ICC status in the CCID
#define	ICCPWRON								0xA5
#define	ICCPWROFF								0x5A

//	CCID Class Specific Request
#define	ABORT									0x01
#define	GET_CLOCK_FREQUENCIES					0x02
#define	GET_DATA_RATES							0x03

//	Command Pipe,Bulk-OUT Messages
#define	PC_TO_RDR_ICC_POWER_ON					0x62
#define	PC_TO_RDR_ICC_POWER_OFF					0x63
#define	PC_TO_RDR_GET_SLOT_STATUS				0x65
#define	PC_TO_RDR_XFR_BLOCK						0x6F
#define	PC_TO_RDR_GET_PARAMETERS				0x6C
#define	PC_TO_RDR_RESET_PARAMETERS				0x6D
#define	PC_TO_RDR_SET_PARAMETERS				0x61
#define	PC_TO_RDR_ESCAPE						0x6B
#define	PC_TO_RDR_ICC_CLOCK						0x6E
#define	PC_TO_RDR_T0_APDU						0x6A
#define	PC_TO_RDR_SECURE						0x69
#define	PC_TO_RDR_MECHANICAL					0x71
#define	PC_TO_RDR_ABORT							0x72
#define	PC_TO_RDR_SET_DATE_RATE_AND_CLOCK_FREQ	0x73

//	Response Pipe,Bulk-IN Messages
#define	RDR_TO_PC_DATA_BLOCK					0x80
#define	RDR_TO_PC_SLOT_STATUS					0x81
#define	RDR_TO_PC_PARAMETERS					0x82
#define	RDR_TO_PC_ESCAPE						0x83
#define	RDR_TO_PC_DATA_RATE_AND_CLOCK_FREQ		0x84

//	bError definition
#define	CMD_ABORTED								0xFF

//	Length of header
#define	CCID_HEADER_LENGTH						0x0A

#define	CCID_MESSAGE_TYPE_ADDR					0	//	bMessageType
#define	CCID_DWLENGTH_ADDR						1	//	st_ccidCmdMessage.dwLength
#define	CCID_SLOT_ADDR							5	//	bSlot
#define	CCID_SEQ_ADDR							6	//	bSeq

//	Variables and functions defined in CCID
typedef struct{
uint32_t dwLength;
uint8_t	bMessageType;
uint8_t	bSlot;
uint8_t	bSeq;
uint8_t	bLockCard;
uint8_t	bIccPowerOn;
uint8_t	bWaitForAbort;									//	Flag indicates ABORT request has been received,but PC_to_RDR_Abort is not received
uint8_t	readerid;
}CCID_COMMAND_MESSAGE;

extern CCID_COMMAND_MESSAGE st_ccidCmdMessage;

typedef	uint16_t (* CCID_CMD_FUNC) (CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);

extern uint16_t	ccidPCtoRDRIccPowerOn(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRIccPowerOff(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRGetSlotStatus(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRXfrBlock(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRGetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRResetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRSetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDREscape(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRIccClock(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRT0APDU(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRSecure(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRMechanical(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRAbort(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);
extern uint16_t	ccidPCtoRDRSetDataRateAndClockFrequency(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * inputbuf,uint8_t * outputbuf);

extern const uint8_t ccidMessageType[];
extern const CCID_CMD_FUNC	ccidPCtoRDRHandle[];

extern uint16_t	ccidWaitForAbort(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * outputbuf);
extern uint16_t	ccidSendProcedureu8(uint8_t slot, uint8_t seq, uint8_t pb, uint8_t * outputbuf);
extern uint16_t	ccidRDRtoPCTimeOutHandle(uint8_t messagetype,uint8_t slot,uint8_t seq, uint8_t * outputbuf);
extern uint16_t	ccidRDRtoPCSlotDoesNotExistHandle(uint8_t messagetype,uint8_t slot,uint8_t seq,uint8_t field9, uint8_t * outputbuf );

extern void	ccidRequest(uint8_t req);
extern void	CCID_main(uint8_t ccidreader);
extern void	ccidDataSeparate(CCID_COMMAND_MESSAGE *st_cmdmessage,uint8_t * ccidheaderbuf);
extern uint16_t	ccidCmdHandler(CCID_COMMAND_MESSAGE *st_cmdmessage, uint8_t* inputbuf, uint8_t* outputbuf);


//
//extern uint8_t g_Icc1PowerOn;
//
//extern uint8_t g_bUsbEp1RcvDataFlag;
//extern uint8_t g_sUsbEp5TxLen;
//extern uint16_t	g_sUsbEp2TxLen;									
extern uint8_t g_abUsbEp1Buf[];									
extern uint8_t g_abUsbEp2Buf[];									
//extern uint8_t g_abUsbEp5Buf[];
//
//
//
//#define g_bUsbCcidRcvDataFlag	g_bUsbEp1RcvDataFlag
//#define g_sUsbCcidTxLen			g_sUsbEp2TxLen		
//#define g_abUsbCcidRxBuf 		g_abUsbEp1Buf
//#define g_abUsbCcidTxBuf		g_abUsbEp2Buf
//#define g_abUsbCcidCmdHeader	g_abUsbCcidTxBuf
//
//
//extern const uint8_t ccidInterfaceDescriptor[];
//extern const uint8_t ccidSmartCardDeviceClassDescriptor[];
//extern const uint8_t ccidEndpointDescriptor[];
//extern const uint8_t ccidReaderStringDescriptor[];

extern uint8_t g_Icc1PowerOn;

extern uint8_t g_abUsbEp1Buf[];									
extern uint8_t g_abUsbEp2Buf[];

#if (USBEPnDIR == IN)
	extern uint8_t g_bUsbEp1RcvDataFlag;
	extern uint16_t	g_sUsbEp2TxLen;	
	#define g_bUsbCcidRcvDataFlag	g_bUsbEp1RcvDataFlag
	#define g_sUsbCcidTxLen			g_sUsbEp2TxLen
	#define g_abUsbCcidRxBuf 		g_abUsbEp1Buf
	#define g_abUsbCcidTxBuf		g_abUsbEp2Buf
	#define g_abUsbCcidCmdHeader	g_abUsbCcidTxBuf
#else
	extern uint8_t g_bUsbEp2RcvDataFlag;
	extern uint16_t	g_sUsbEp1TxLen;	
	#define g_bUsbCcidRcvDataFlag	g_bUsbEp2RcvDataFlag
	#define g_sUsbCcidTxLen			g_sUsbEp1TxLen
	#define g_abUsbCcidRxBuf 		g_abUsbEp2Buf
	#define g_abUsbCcidTxBuf		g_abUsbEp1Buf
	#define g_abUsbCcidCmdHeader	g_abUsbCcidTxBuf
#endif
								

extern const uint8_t ccidInterfaceDescriptor[];
extern const uint8_t ccidSmartCardDeviceClassDescriptor[];
extern const uint8_t ccidEndpointDescriptor[];
extern const uint8_t ccidReaderStringDescriptor[];

extern uint8_t	ccidPowerOnProcess(uint8_t readerid, uint8_t PowerSelect, uint8_t * atrbuf);
extern uint8_t	ccidPowerOffProcess(uint8_t readerid);
extern uint8_t	ccidSetParametersProcess(uint8_t readerid,uint8_t protocol, uint8_t * ProtocolDataStructure, uint8_t len);
extern uint8_t	ccidDataRateAndClockFrequencyProcess(uint8_t readerid,uint8_t *clkfreq_datarate);
extern uint16_t	ccidCommandProcess(uint8_t readerid, uint8_t * cmddatabuf, uint16_t len);


#endif //__CCID_H__
