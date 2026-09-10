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
//	CCID system main process
void	CCID_main(u8 ccidreader)
{	
#if CCID
	if(ccidreader == 1)
	{
		ccidDataSeparate(&st_ccidCmdMessage,g_abUsbCcidRxBuf);
		st_ccidCmdMessage.readerid = 1;
		if((st_ccidCmdMessage.bWaitForAbort == 1) && (st_ccidCmdMessage.bMessageType != PC_TO_RDR_ABORT))		//	Send a response with bmCommandStatus=Failed and bError=CMD_ABORTED to each Bulk-OUT message													
			g_sUsbCcidTxLen = ccidWaitForAbort(&st_ccidCmdMessage,g_abUsbCcidTxBuf);
		else if((st_ccidCmdMessage.bWaitForAbort == 0) || (st_ccidCmdMessage.bMessageType == PC_TO_RDR_ABORT))	//	Call CCID PC to RDR CMD Handler
			g_sUsbCcidTxLen = ccidCmdHandler(&st_ccidCmdMessage,g_abUsbCcidRxBuf,g_abUsbCcidTxBuf);					
	}
#endif

#if CCID2
	if(ccidreader == 2)
	{
		ccidDataSeparate(&st_ccid2CmdMessage,g_abUsbCcid2RxBuf);
		st_ccid2CmdMessage.readerid = 2;
		if((st_ccid2CmdMessage.bWaitForAbort == 1) && (st_ccid2CmdMessage.bMessageType != PC_TO_RDR_ABORT))		//	Send a response with bmCommandStatus=Failed and bError=CMD_ABORTED to each Bulk-OUT message													
			g_sUsbCcid2TxLen = ccidWaitForAbort(&st_ccid2CmdMessage,g_abUsbCcid2TxBuf);
		else if((st_ccid2CmdMessage.bWaitForAbort == 0) || (st_ccid2CmdMessage.bMessageType == PC_TO_RDR_ABORT))	//	Call CCID PC to RDR CMD Handler
			g_sUsbCcid2TxLen = ccidCmdHandler(&st_ccid2CmdMessage,g_abUsbCcid2RxBuf,g_abUsbCcid2TxBuf);					
	}
#endif
#if CCID3
	if(ccidreader == 3)
	{
		ccidDataSeparate(&st_ccid3CmdMessage,g_abUsbCcid3RxBuf);
		st_ccid3CmdMessage.readerid = 3;
		if((st_ccid3CmdMessage.bWaitForAbort == 1) && (st_ccid3CmdMessage.bMessageType != PC_TO_RDR_ABORT))		//	Send a response with bmCommandStatus=Failed and bError=CMD_ABORTED to each Bulk-OUT message													
			g_sUsbCcid3TxLen = ccidWaitForAbort(&st_ccid3CmdMessage,g_abUsbCcid3TxBuf);
		else if((st_ccid3CmdMessage.bWaitForAbort == 0) || (st_ccid3CmdMessage.bMessageType == PC_TO_RDR_ABORT))	//	Call CCID PC to RDR CMD Handler
			g_sUsbCcid3TxLen = ccidCmdHandler(&st_ccid3CmdMessage,g_abUsbCcid3RxBuf,g_abUsbCcid3TxBuf);	
	}
#endif
	else
	{}
}
//====================================================
//	Separate received data
void	ccidDataSeparate(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * ccidheaderbuf)
{
	st_cmdmessage->bMessageType = ccidheaderbuf[CCID_MESSAGE_TYPE_ADDR];

	st_cmdmessage->dwLength = ccidheaderbuf[4];
	st_cmdmessage->dwLength <<= 8;
	st_cmdmessage->dwLength += ccidheaderbuf[3];
	st_cmdmessage->dwLength <<= 8;
	st_cmdmessage->dwLength += ccidheaderbuf[2];
	st_cmdmessage->dwLength <<= 8;
	st_cmdmessage->dwLength += ccidheaderbuf[1];
	
	st_cmdmessage->bSlot = ccidheaderbuf[CCID_SLOT_ADDR];
	st_cmdmessage->bSeq = ccidheaderbuf[CCID_SEQ_ADDR];
}

//====================================================
//	Initialize the PC_to_RDR CMD Handlers
u16	ccidCmdHandler(CCID_COMMAND_MESSAGE *st_cmdmessage, u8* inputbuf, u8* outputbuf)
{
	u8	count;	

	for(count=0;count<14;count++)
	{
		if(ccidMessageType[count] == st_cmdmessage->bMessageType)															//	Polling function pointer ID with messagetype
		{
			return ccidPCtoRDRHandle[count](st_cmdmessage,inputbuf,outputbuf);																		//	Run target ccid command handler
		}
	}
	return 0;
}
