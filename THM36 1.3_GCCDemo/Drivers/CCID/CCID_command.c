/********************************************************************************
* Copyright (c) 2012, Beijing Tongfang Microelectroics Co->, Ltd->
* All rights reserved->
* Module: CCID
* Author: Yang Song
* Version: V1.0
* History:
*   2012-09-24 Original version
********************************************************************************/
 
#include "string.h"
#include "global.h"
#include "CCID.h"
#include "USB.h"


const u8 ccidMessageType[] = {PC_TO_RDR_ICC_POWER_ON,PC_TO_RDR_ICC_POWER_OFF,PC_TO_RDR_GET_SLOT_STATUS,PC_TO_RDR_XFR_BLOCK,
							  PC_TO_RDR_GET_PARAMETERS,PC_TO_RDR_RESET_PARAMETERS,PC_TO_RDR_SET_PARAMETERS,PC_TO_RDR_ESCAPE,PC_TO_RDR_ICC_CLOCK,
							  PC_TO_RDR_T0_APDU,PC_TO_RDR_SECURE,PC_TO_RDR_MECHANICAL,PC_TO_RDR_ABORT,PC_TO_RDR_SET_DATE_RATE_AND_CLOCK_FREQ,};

const CCID_CMD_FUNC	ccidPCtoRDRHandle[] = {ccidPCtoRDRIccPowerOn,ccidPCtoRDRIccPowerOff,ccidPCtoRDRGetSlotStatus,ccidPCtoRDRXfrBlock,
										ccidPCtoRDRGetParameters,ccidPCtoRDRResetParameters,ccidPCtoRDRSetParameters,ccidPCtoRDREscape,ccidPCtoRDRIccClock,
										ccidPCtoRDRT0APDU,ccidPCtoRDRSecure,ccidPCtoRDRMechanical,ccidPCtoRDRAbort,ccidPCtoRDRSetDataRateAndClockFrequency,};							
 
//====================================================
u16	ccidPCtoRDRIccPowerOn(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf )
{
	u8	bPowerSelect;
	u16 returnlen = 0;
	
	bPowerSelect = outputbuf[7];											//	5->0V,3->0V

	if(st_cmdmessage->bSlot == 0)											//	usbccid->sys specifies bPowerSelect == 0x03
	{
		returnlen = ccidPowerOnProcess(st_cmdmessage->readerid,bPowerSelect,outputbuf+CCID_HEADER_LENGTH);
		if(returnlen == 0)
		{
			returnlen = ccidRDRtoPCTimeOutHandle(RDR_TO_PC_DATA_BLOCK,st_cmdmessage->bSlot,st_cmdmessage->bSeq,outputbuf);
			st_cmdmessage->bIccPowerOn = ICCPWROFF;
		}
		else
		{
			outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_BLOCK;		//	RDR_to_PC_DataBlock
			outputbuf[CCID_DWLENGTH_ADDR] = returnlen;						//	Little endian
			outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
			outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
			outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
			outputbuf[7] = 0x00;											//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
			outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
			outputbuf[9] = 0x00;											//	bChainParameter
	
			returnlen += CCID_HEADER_LENGTH;	
			
			st_cmdmessage->bIccPowerOn = ICCPWRON;
		}							
	}
	else																	//	Slot erroe or voltage error,no ICC
	{	
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_DATA_BLOCK,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0, outputbuf);					//	RDR_to_PC_DataBlock
	}
	return returnlen;
}

//====================================================
u16	ccidPCtoRDRIccPowerOff(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u16 returnlen = 0;

	if(st_cmdmessage->bSlot == 0)											//	Slot #0
	{
		returnlen = ccidPowerOffProcess(st_cmdmessage->readerid);
		if(returnlen & 0x80)
		{
			outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
			outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
			outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
			outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot; 
			outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		//	if((st_cmdmessage->bIccPowerOn == ICCPWRON)&&(returnlen&0x10))
		//		outputbuf[7] = 0x00;										//	bStatus,an ICC is present and active
			if(returnlen&0x10)
				outputbuf[7] = 0x01;										//	bStatus,an ICC is present and inactive
			else
				outputbuf[7] = 0x02;										//	bStatus,an No ICC is present
			outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
			outputbuf[9] = returnlen&0x03;									//	bClockStatus,Clock is ->.->	
		}
		else
		{  
			outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
			outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
			outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
			outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot; 
			outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
			outputbuf[7] = 0x40;											//	Fail - Command Not Supported
			outputbuf[8] = 0x00;											//	
			outputbuf[9] = 0x00;	
		}
		st_cmdmessage->bIccPowerOn = ICCPWROFF;						
	}
	else																	//	Not Slot #0
	{	
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_SLOT_STATUS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0x03, outputbuf);		 		//	RDR_to_PC_SlotStatus->  bClockStatus= 0x03,Clock stopped in an unknown state	
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRGetSlotStatus(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	if(st_cmdmessage->bSlot == 0)											//	Slot #0
	{
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;			//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;								//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		if(st_cmdmessage->bIccPowerOn != ICCPWROFF)
		{
			outputbuf[7] = 0x00;											//	bmICCStatus,0 - ICC is present and active
			outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
			outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running
		}
		else
		{
			outputbuf[7] = 0x01;											//	bmICCStatus,0 - ICC is present but inactive
			outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
			outputbuf[9] = 0x01;											//	bClockStatus= 0x00, Clock stopped in state L
		}
		
	}
	else																	//	Not Slot #0
	{
		ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_SLOT_STATUS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0x03,outputbuf);				//	RDR_to_PC_SlotStatus/  bClockStatus= 0x03,Clock stopped in an unknown state
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRXfrBlock(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{	
//	static u8	LcDataFlag;
//	u8 RcvCCIDAPDU = 0;
//	u16	wLevelParameter;
	u16 returnlen = 0;

	if(st_cmdmessage->bSlot != 0)										//	Slot error
	{	
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_DATA_BLOCK,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);		   			//	RDR_to_PC_DataBlock
		return returnlen;
	}

	returnlen = ccidCommandProcess(st_cmdmessage->readerid, inputbuf+CCID_HEADER_LENGTH,st_cmdmessage->dwLength);
	
	if((returnlen != 0))
	{
		memcpy(outputbuf+CCID_HEADER_LENGTH,inputbuf+CCID_HEADER_LENGTH,returnlen);
		
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_BLOCK;		//	RDR_to_PC_DataBlock
		outputbuf[CCID_DWLENGTH_ADDR] = returnlen;						//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = returnlen >> 8;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = returnlen >> 16;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = returnlen >> 24;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;					//	If multi-block is transfered,the current st_cmdmessage->bSeq is used
		outputbuf[7] = 0x00;											//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
		outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
		outputbuf[9] = 0x00;											//	bChainParameter

		returnlen += CCID_HEADER_LENGTH;								//	First 10B reserved for CCID Header
	}
	else 																// time out
	{
		returnlen = ccidRDRtoPCTimeOutHandle(RDR_TO_PC_DATA_BLOCK,st_cmdmessage->bSlot,st_cmdmessage->bSeq,outputbuf);
	}
	return returnlen;
//	LcDataFlag = 0;														//	Clear LcData flag
}

//====================================================
//	The response is the same as ResetParameters if the parameters haven't been set->
u16	ccidPCtoRDRGetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u16 returnlen = 0;
	if(st_cmdmessage->bSlot == 0)										//	Slot #0
	{
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_PARAMETERS;		//	RDR_to_PC_Parameters
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
		outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
		outputbuf[9] = 0x00;											//	bProtocolNum,T = 0
		//DMA_MemCpy(outputbuf,outputbuf,CCID_HEADER_LENGTH);
		returnlen = CCID_HEADER_LENGTH;
	}
	else																//	Not Slot #0
	{	
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_PARAMETERS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);					//	RDR_to_PC_Parameters
	}
	return returnlen;
}

//====================================================
//	Reset the slot parameters to their default values
u16	ccidPCtoRDRResetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u16 returnlen = 0;

	if(st_cmdmessage->bSlot == 0)										//	Slot #0
	{
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_PARAMETERS;		//	RDR_to_PC_Parameters
		outputbuf[CCID_DWLENGTH_ADDR] = 0x05;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
		outputbuf[8] = 0x00;											//	bError,don't care when bmCommandStatus is 0
		outputbuf[9] = 0x00;											//	bProtocolNum,T = 0

		returnlen = CCID_HEADER_LENGTH;
		outputbuf[returnlen++] = 0x11;									//	bmFindexDindex,FI/DI
		outputbuf[returnlen++] = 0x00;									//	bmTCCKST0,direct convention
		outputbuf[returnlen++] = 0x02;									//	bGuardTimeT0,GuardTime = 2etu
		outputbuf[returnlen++] = 0x80;									//	bWaitingIntegerT0
		outputbuf[returnlen++] = 0x03;									//	bClockStop,with Clock either H or L
	}
	else																//	Not Slot #0
	{
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_PARAMETERS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);					//	RDR_to_PC_Parameters
	}
	return returnlen;
}

//====================================================
u16	ccidPCtoRDRSetParameters(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u8	protocol;
	u16 returnlen = 0;

	protocol = outputbuf[7];
	
	if(st_cmdmessage->bSlot != 0)
	{	
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_PARAMETERS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);					//	RDR_to_PC_Parameters
	}
	else
	{
		returnlen = ccidSetParametersProcess(st_cmdmessage->readerid,protocol,inputbuf+CCID_HEADER_LENGTH,st_cmdmessage->dwLength);
		if(returnlen == 0x00)											
		{
			returnlen = ccidRDRtoPCTimeOutHandle(RDR_TO_PC_PARAMETERS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,outputbuf);	
		}
		else															//	Successful
		{
			outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_PARAMETERS;	//	RDR_to_PC_Parameters
			outputbuf[CCID_DWLENGTH_ADDR] = 0x05;						//	Little endian
			outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
			outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
			outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
			outputbuf[7] = 0x00;										//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
			outputbuf[8] = 0x00;										//	bError,don't care when bmCommandStatus is 0
			outputbuf[9] = protocol;									//	bProtocolNum,T = 0

			memcpy(outputbuf+CCID_HEADER_LENGTH,inputbuf+CCID_HEADER_LENGTH,st_cmdmessage->dwLength);
			returnlen = CCID_HEADER_LENGTH + st_cmdmessage->dwLength;
		}
	}
	return returnlen;
}

//====================================================
u16	ccidPCtoRDREscape(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u16 returnlen = 0;
	if(st_cmdmessage->bSlot != 0)
	{	
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_ESCAPE,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);						//	RDR_to_PC_Escape
	}
	else																//	The current response is just for testing Ep1 BULK IN
	{	
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_ESCAPE;			//	RDR_to_PC_Escape
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x02;
		outputbuf[8] = 0x00;
		outputbuf[9] = 0x00;											//	RFU

		returnlen = CCID_HEADER_LENGTH;	
		if((st_cmdmessage->dwLength == 0x01)	&& (inputbuf[CCID_HEADER_LENGTH] == 0x02))	//	Defined by CCID manufacture
		{
			outputbuf[CCID_DWLENGTH_ADDR] = 0x12;						//	The length of data
			outputbuf[returnlen++] = 0x47;
			outputbuf[returnlen++] = 0x65;
			outputbuf[returnlen++] = 0x6D;
			outputbuf[returnlen++] = 0x54;
			outputbuf[returnlen++] = 0x77;
			outputbuf[returnlen++] = 0x69;
			outputbuf[returnlen++] = 0x6E;
			outputbuf[returnlen++] = 0x2D;
			outputbuf[returnlen++] = 0x56;
			outputbuf[returnlen++] = 0x32;
			outputbuf[returnlen++] = 0x2E;
			outputbuf[returnlen++] = 0x30;
			outputbuf[returnlen++] = 0x30;
			outputbuf[returnlen++] = 0x2D;
			outputbuf[returnlen++] = 0x47;
			outputbuf[returnlen++] = 0x54;
			outputbuf[returnlen++] = 0x30;
			outputbuf[returnlen++] = 0x30;			
		}
	}
	return returnlen;
}

//====================================================
u16	ccidPCtoRDRIccClock(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u8	bClockCommand;
	
	bClockCommand = inputbuf[7];
	
	if(st_cmdmessage->bSlot != 0)
	{
		ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_SLOT_STATUS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);					//	RDR_to_PC_SlotStatus
	}
	else if(bClockCommand == 0x00)										//	Restart clock
	{
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running
	}
	else if(bClockCommand == 0x03)										//	Stop clock in the state shown in bClockStop field of _SetParameters
	{
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x03;											//	bClockStatus= 0x03,Clock is stopped
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRT0APDU(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u8	bmChanges;
//	u8	bClassGetResponse;
//	u8	bClassEnvelope;
	
	bmChanges = inputbuf[7];
//	bClassGetResponse = inputbuf[8];
//	bClassEnvelope = inputbuf[9];
	
	if(st_cmdmessage->bSlot != 0)
	{
		ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_SLOT_STATUS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);					//	RDR_to_PC_SlotStatus
	}
	else if(bmChanges & 0x01)
	{	
	//	CLA = bClassGetResponse;
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running
	}
	else if(bmChanges & 0x02)
	{	
	//	CLA = bClassEnvelope;
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRSecure(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_BLOCK;			//	RDR_to_PC_DataBlock
	outputbuf[CCID_DWLENGTH_ADDR] = 0x00;								//	Little endian
	outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
	outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
	outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
	outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
	outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;						//	If multi-block is transfered,the current st_cmdmessage->bSeq is used
	outputbuf[7] = 0x40;												//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
	outputbuf[8] = 0x00;												//	bError,don't care when bmCommandStatus is 0
	outputbuf[9] = 0x00;												//	bChainParameter	
	
	return  CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRMechanical(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u8	bFunction;
	
	bFunction = inputbuf[7];
	
	if(st_cmdmessage->bSlot != 0)
	{
		ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_SLOT_STATUS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);					//	RDR_to_PC_SlotStatus
	}
	else if(bFunction == 0x04)
	{
		st_cmdmessage->bLockCard = 1;
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running	
	}
	else if(bFunction == 0x05)
	{
		st_cmdmessage->bLockCard = 0;
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRAbort(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	if(st_cmdmessage->bSlot == 0)
	{	
		if((st_usbDeviceRequest.bRequest == ABORT) && (st_cmdmessage->bWaitForAbort == 1))		//	CCID request ABORT has been received
		{	
			st_cmdmessage->bWaitForAbort = 0;							//	Clear the flag
		}

		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;		//	RDR_to_PC_SlotStatus
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x00;											//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
		outputbuf[8] = 0x00;											//	bError = 0
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running	
	}
	else
	{
		ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_SLOT_STATUS,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
u16	ccidPCtoRDRSetDataRateAndClockFrequency(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * inputbuf,u8 * outputbuf)
{
	u16 returnlen = 0;

	if(st_cmdmessage->bSlot != 0)
	{
		returnlen = ccidRDRtoPCSlotDoesNotExistHandle(RDR_TO_PC_DATA_RATE_AND_CLOCK_FREQ,st_cmdmessage->bSlot,st_cmdmessage->bSeq,0,outputbuf);	
	}
	else
	{
		returnlen = ccidDataRateAndClockFrequencyProcess(st_cmdmessage->readerid,inputbuf+CCID_HEADER_LENGTH);
		if(returnlen != 0)
		{
			outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_RATE_AND_CLOCK_FREQ;
			outputbuf[CCID_DWLENGTH_ADDR] = 0x08;						//	Little endian
			outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
			outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
			outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
			outputbuf[7] = 0x00;										//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
			outputbuf[8] = 0x00;										//	bError = 0
			outputbuf[9] = 0x00;										//	RFU
			//DMA_MemCpy(outputbuf,outputbuf,CCID_HEADER_LENGTH);	
			memcpy(outputbuf+CCID_HEADER_LENGTH,inputbuf+CCID_HEADER_LENGTH,0x08);		
			returnlen = CCID_HEADER_LENGTH + 8;
		}
		else
		{
			outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_RATE_AND_CLOCK_FREQ;
			outputbuf[CCID_DWLENGTH_ADDR] = 0x00;						//	Little endian
			outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
			outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
			outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
			outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
			outputbuf[7] = 0x40;										//	bStatus,bmICCStatus = 0,bmCommandStatus = 0
			outputbuf[8] = 0x00;										//	bError = 0
			outputbuf[9] = 0x00;
			returnlen = CCID_HEADER_LENGTH;		
		}
	}
	return returnlen;
}

//====================================================
//	Waiting for PC_to_RDR_Abort,after ABORT request is received
u16	ccidWaitForAbort(CCID_COMMAND_MESSAGE *st_cmdmessage,u8 * outputbuf)
{
//	PC_to_RDR_IccPowerOn				XfrBlock			Secure
	if((st_cmdmessage->bMessageType==0x62) || (st_cmdmessage->bMessageType==0x6F) || (st_cmdmessage->bMessageType==0x69))
	{	
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_BLOCK;		//	RDR_to_PC_DataBlock
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x41;											//	bStatus,bmICCStatus = 1,bmCommandStatus = 1
		outputbuf[8] = CMD_ABORTED;										//	bError
		outputbuf[9] = 0x00;											//	bChainParameter
	}
//	PC_to_RDR_IccPowerOff				GetSlotStatus			IccClock			T0APDU				Mechanical	
	else if((st_cmdmessage->bMessageType==0x63) || (st_cmdmessage->bMessageType==0x65) || (st_cmdmessage->bMessageType==0x6E) || (st_cmdmessage->bMessageType==0x6A) || (st_cmdmessage->bMessageType==0x71))
	{	
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_SLOT_STATUS;
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x41;											//	bStatus,bmICCStatus = 1,bmCommandStatus = 1
		outputbuf[8] = CMD_ABORTED;										//	bError
		outputbuf[9] = 0x00;											//	bClockStatus= 0x00,Clock is running
	}
//	PC_to_RDR_	GetParameters		ResetParameters			SetParameters	
	else if((st_cmdmessage->bMessageType==0x6C) || (st_cmdmessage->bMessageType==0x6D) || (st_cmdmessage->bMessageType==0x61))
	{	
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_PARAMETERS;
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x41;											//	bStatus,bmICCStatus = 1,bmCommandStatus = 1
		outputbuf[8] = CMD_ABORTED;										//	bError
		outputbuf[9] = 0x00;											//	bProtocolNum,T = 0
	}	
	else if(st_cmdmessage->bMessageType == RDR_TO_PC_ESCAPE)
	{	
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_ESCAPE;
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x41;											//	bStatus,bmICCStatus = 1,bmCommandStatus = 1
		outputbuf[8] = CMD_ABORTED;										//	bError
		outputbuf[9] = 0x00;											//	RFU
	}	
	else if(st_cmdmessage->bMessageType == PC_TO_RDR_SET_DATE_RATE_AND_CLOCK_FREQ)
	{	
		outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_RATE_AND_CLOCK_FREQ;
		outputbuf[CCID_DWLENGTH_ADDR] = 0x00;							//	Little endian
		outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
		outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
		outputbuf[CCID_SLOT_ADDR] = st_cmdmessage->bSlot;
		outputbuf[CCID_SEQ_ADDR] = st_cmdmessage->bSeq;
		outputbuf[7] = 0x41;											//	bStatus,bmICCStatus = 1,bmCommandStatus = 1
		outputbuf[8] = CMD_ABORTED;										//	bError
		outputbuf[9] = 0x00;											//	RFU
	}
	return CCID_HEADER_LENGTH;
}

//====================================================
//	Send Procedure Byte
u16	ccidSendProcedureu8(u8 slot, u8 seq, u8 pb, u8 * outputbuf)
{
	outputbuf[CCID_MESSAGE_TYPE_ADDR] = RDR_TO_PC_DATA_BLOCK;			//	RDR_to_PC_DataBlock
	outputbuf[CCID_DWLENGTH_ADDR] = 0x01;								//	Little endian
	outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
	outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
	outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
	outputbuf[CCID_SLOT_ADDR] = slot;
	outputbuf[CCID_SEQ_ADDR] = seq;
	outputbuf[7] = 0x00;												//	bStatus,Bit1~0:bmICCStatus,Bit7~6:bmCommandStatus
	outputbuf[8] = 0x00;												//	bError,don't care when bmCommandStatus is 0
	outputbuf[9] = 0x00;												//	bChainParameter
	outputbuf[CCID_HEADER_LENGTH] = pb;

	return CCID_HEADER_LENGTH + 1;
}
//====================================================
//	Time out handle
u16	ccidRDRtoPCTimeOutHandle(u8 messagetype,u8 slot,u8 seq, u8 * outputbuf)
{
	outputbuf[CCID_MESSAGE_TYPE_ADDR] = messagetype;					//	RDR_to_PC_DataBlock
	outputbuf[CCID_DWLENGTH_ADDR] = 0;	
	outputbuf[CCID_DWLENGTH_ADDR + 1] = 0;
	outputbuf[CCID_DWLENGTH_ADDR + 2] = 0;
	outputbuf[CCID_DWLENGTH_ADDR + 3] = 0;
	outputbuf[CCID_SLOT_ADDR] = slot;
	outputbuf[CCID_SEQ_ADDR] = seq;										//	If multi-block is transfered,the current st_cmdmessage->bSeq is used
	outputbuf[7] = 0x42;												//	Failed and No ICC is present
	outputbuf[8] = 0xfe;												//	time out
	outputbuf[9] = 0x00;
	
	return CCID_HEADER_LENGTH;											//	First 10B reserved for CCID Header
}

//====================================================
//	Time out handle
u16	ccidRDRtoPCSlotDoesNotExistHandle(u8 messagetype,u8 slot,u8 seq,u8 field9, u8 * outputbuf )
{
	outputbuf[CCID_MESSAGE_TYPE_ADDR] = messagetype;
	outputbuf[CCID_DWLENGTH_ADDR] = 0x00;								//	Little endian
	outputbuf[CCID_DWLENGTH_ADDR + 1] = 0x00;
	outputbuf[CCID_DWLENGTH_ADDR + 2] = 0x00;
	outputbuf[CCID_DWLENGTH_ADDR + 3] = 0x00;
	outputbuf[CCID_SLOT_ADDR] = slot;
	outputbuf[CCID_SEQ_ADDR] = seq;
	outputbuf[7] = 0x42;												//	bStatus,bmICCStatus = 2,bmCommandStatus = 1
	outputbuf[8] = 0x05;												//	bError = 5,st_cmdmessage->bSlot does not exist
	outputbuf[9] = field9;
												
	return CCID_HEADER_LENGTH;
}

