/********************************************************************************
* Copyright (c) 2012, Beijing Tongfang Microelectroics Co., Ltd.
* All rights reserved.
* Module: CCID
* Author: Yang Song
* Version: V1.0
* History:
*   2012-09-24 Original version
********************************************************************************/

 
#include "global.h"
#include "CCID.h"
#include "USB.h"

//====================================================
//	CCID ABORT Class request
void ccidRequestAbort(void)
{
/*	if(st_ccidCmdMessage.bMessageType == PC_TO_RDR_ABORT)						//	The previous command is PC_to_RDR_Abort
	{
		if((st_ccidCmdMessage.bSeq == (st_usbDeviceRequest.wValue >> 8)) && (st_ccidCmdMessage.bSeq == (u8)st_usbDeviceRequest.wValue))
			g_bUsbCcidRcvDataFlag = 0;											//	Indicates no Bulu-OUT message received

		USBbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;					//	RDR_to_PC_SlotStatus
		USBbuf[CCID_DWLENGTH_ADDR] = 0x00;										//	Little endian
		USBbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		USBbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		USBbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		USBbuf[CCID_SLOT_ADDR] = st_ccidCmdMessage.bSlot;
		USBbuf[CCID_SEQ_ADDR] = st_ccidCmdMessage.bSeq;
		USBbuf[7] = 0x00;														//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		USBbuf[8] = 0x00;														//	bError = 0
		USBbuf[9] = 0x00;														//	bClockStatus= 0x00,Clock is running
	//	DMA_MemCpy(USBbuf,USBbuf,CCID_HEADER_LENGTH);
		g_sUsbCcidTxLen = CCID_HEADER_LENGTH;
	}
	else																		//	If PC_to_RDR_Abort was not received,all commands will be failed until PC_to_RDR_Abort is received
	{
		st_ccidCmdMessage.bWaitForAbort = 1;									//	Waiting for PC_to_RDR_Abort
		USBbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;					//	RDR_to_PC_SlotStatus
		USBbuf[CCID_DWLENGTH_ADDR] = 0x00;										//	Little endian
		USBbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		USBbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		USBbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		USBbuf[CCID_SLOT_ADDR] = st_ccidCmdMessage.bSlot;
		USBbuf[CCID_SEQ_ADDR] = st_ccidCmdMessage.bSeq;
		USBbuf[7] = 0x40;														//	bStatus,bmICCStatus = 0,bmCommandStatus = 1
		USBbuf[8] = 0x00;														//	bError = 0,command not supported
		USBbuf[9] = 0x00;														//	bClockStatus= 0x00,Clock is running
	//	DMA_MemCpy(USBbuf,USBbuf,CCID_HEADER_LENGTH);
		g_sUsbCcidTxLen = CCID_HEADER_LENGTH;
	}
*/
}

//====================================================
//	CCID ABORT Class request
void ccidRequestGetClockFrequencies(void)
{																				//	bNumClockSupported = 0x01
	u32 clock_freq = 0x00000DFC;												//	3.58MHz

	usbEpnTxAll(0,(u8*)&clock_freq,4);
}

//====================================================
//	CCID ABORT Class request
void ccidRequestGetDataRates(void)
{																				//	bNumDataRatesSupported = 0x05
	u32 data_rates[5];

	data_rates[0] = 0x00002580;											//	9600bps
	data_rates[1] = 0x00004B00;											//	19200bps
	data_rates[2] = 0x00009600;											//	38400bps
	data_rates[3] = 0x0000E100;											//	57600bps
	data_rates[4] = 0x0001C200;											//	115200bps

	usbEpnTxAll(0,(u8*)data_rates,20);
} 

//====================================================
//	Handle 3-type CCID1.1 specific request
void	ccidRequest(u8 req)
{
	switch(req)
	{
		case ABORT:					ccidRequestAbort();					break;
		case GET_CLOCK_FREQUENCIES:	ccidRequestGetClockFrequencies();	break;
		case GET_DATA_RATES:		ccidRequestGetDataRates();			break;	
		default:					return;										//	To avoid program sets global variables like WaitforAbort
	}
}
