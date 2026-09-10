/********************************************************************************
* Copyright (c) 2012, Beijing Tongfang Microelectroics Co., Ltd.
* All rights reserved.
* Module: CCID.h
* Author: Yang Song
* Version: V1.0
* History:
*   2012-09-24 Original version
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
unsigned int 	dwLength;
unsigned char	bMessageType;
unsigned char	bSlot;
unsigned char	bSeq;
unsigned char	bLockCard;
unsigned char	bIccPowerOn;
unsigned char	bWaitForAbort;									//	Flag indicates ABORT request has been received,but PC_to_RDR_Abort is not received
unsigned char	readerid;
}CCID_COMMAND_MESSAGE;

extern CCID_COMMAND_MESSAGE st_ccidCmdMessage;

typedef	unsigned short (* CCID_CMD_FUNC) (CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);

extern unsigned short	ccidPCtoRDRIccPowerOn(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRIccPowerOff(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRGetSlotStatus(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRXfrBlock(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRGetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRResetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRSetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDREscape(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRIccClock(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRT0APDU(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRSecure(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRMechanical(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRAbort(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);
extern unsigned short	ccidPCtoRDRSetDataRateAndClockFrequency(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * inputbuf,unsigned char * outputbuf);

extern const unsigned char ccidMessageType[];
extern const CCID_CMD_FUNC	ccidPCtoRDRHandle[];

extern unsigned short	ccidWaitForAbort(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * outputbuf);
extern unsigned short	ccidSendProcedureu8(unsigned char slot, unsigned char seq, unsigned char pb, unsigned char * outputbuf);
extern unsigned short	ccidRDRtoPCTimeOutHandle(unsigned char messagetype,unsigned char slot,unsigned char seq, unsigned char * outputbuf);
extern unsigned short	ccidRDRtoPCSlotDoesNotExistHandle(unsigned char messagetype,unsigned char slot,unsigned char seq,unsigned char field9, unsigned char * outputbuf );

extern void	ccidRequest(unsigned char req);
extern void	CCID_main(unsigned char ccidreader);
extern void	ccidDataSeparate(CCID_COMMAND_MESSAGE *st_cmdmessage,unsigned char * ccidheaderbuf);
extern unsigned short	ccidCmdHandler(CCID_COMMAND_MESSAGE *st_cmdmessage, unsigned char* inputbuf, unsigned char* outputbuf);



extern unsigned char g_Icc1PowerOn;

extern unsigned char g_bUsbEp1RcvDataFlag;
extern unsigned char g_sUsbEp5TxLen;
extern unsigned short	g_sUsbEp2TxLen;									
extern unsigned char g_abUsbEp1Buf[];									
extern unsigned char g_abUsbEp2Buf[];									
extern unsigned char g_abUsbEp5Buf[];

#define	CCIDBULKPKTSIZE			0x40
#define	CCIDINTTPKTSIZE			0x08

#define g_bUsbCcidRcvDataFlag	g_bUsbEp1RcvDataFlag
#define g_sUsbCcidTxLen			g_sUsbEp2TxLen		
#define g_abUsbCcidRxBuf 		g_abUsbEp1Buf
#define g_abUsbCcidTxBuf		g_abUsbEp2Buf
#define g_abUsbCcidCmdHeader	g_abUsbCcidTxBuf


extern const unsigned char ccidInterfaceDescriptor[];
extern const unsigned char ccidSmartCardDeviceClassDescriptor[];
extern const unsigned char ccidEndpointDescriptor[];
extern const unsigned char ccidReaderStringDescriptor[];

extern unsigned char	ccidPowerOnProcess(unsigned char readerid, unsigned char PowerSelect, unsigned char * atrbuf);
extern unsigned char	ccidPowerOffProcess(unsigned char readerid);
extern unsigned char	ccidSetParametersProcess(unsigned char readerid,unsigned char protocol, unsigned char * ProtocolDataStructure, unsigned char len);
extern unsigned char	ccidDataRateAndClockFrequencyProcess(unsigned char readerid,unsigned char *clkfreq_datarate);
extern unsigned short	ccidCommandProcess(unsigned char readerid, unsigned char * cmddatabuf, unsigned short len);


#endif //__CCID_H__
