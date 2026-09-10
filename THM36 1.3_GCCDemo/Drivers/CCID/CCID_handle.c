/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: CCID_handle.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


#include "thm36_1.3.h"
#include "USB.h"  
#include "CCID.h"
#include "cmd_handle.h"
#define TOS_VERSION	0x01


const uint8_t Reader1ATR[7]={0x3B,0x14,0x96,0x30,0x88,0x00,TOS_VERSION};
//====================================================
//readerid: 0x01-contact reader, 0x02-contaceless reader
//PowerSelect: 00h 每 Automatic Voltage Selection
//			 01h 每 5.0 volts
//			 02h 每 3.0 volts
//			 03h 每 1.8 volts
//uint8_t * cmddatabuf: OUT: ATR
//return value:	0:	  time out or failed
//				others: the total length of ATR
uint8_t	ccidPowerOnProcess(uint8_t readerid, uint8_t PowerSelect, uint8_t * atrbuf)
{
	memcpy(atrbuf,(uint8_t *)Reader1ATR,7);
	g_Icc1PowerOn = 1;
	return 7;	
}
//====================================================
//readerid: 0x01-contact reader, 0x02-contaceless reader
//return value:	0: 		failed
//				others: bit7:   1-correct 
//					  bit4:   1-ICC is present, 0-No ICC is present
//						bit0-1: 00b-Clock running, 01b-Clock stopped in state L, 10b-Clock stopped in state H,11b-Clock stopped in an unknown state							  
uint8_t	ccidPowerOffProcess(uint8_t readerid)
{
	g_Icc1PowerOn = 0;	

	return 0x90;
}
//====================================================
//readerid: 0x01-contact reader, 0x02-contaceless reader
//protocol: 0x00: T=0  0x01: T=1
//uint8_t * ProtocolDataStructure: IN:ProtocolDataStructure to be set. 
//				  			 OUT:ProtocolDataStructure have been set or not set
//		  ProtocolDataStructure is as follow. For detail,please refer to CCID spec 6.1.7 PC_to_RDR_SetParameters
//		   T=0: bmFindexDindex,bmTCCKST0,bGuardTimeT0,WaitingIntegerT0,bClockStop
//			 T=1: bmFindexDindex,bmTCCKST1,bGuardTimeT1,bmWaitingIntegersT1,bClockStop,bIFSC,bNadValue
//uint16_t len: the total length ProtocolDataStructure
//return value:	0:	  time out or failed
//				others: correct
//note: if thers are some parameters that users does not need ,please do not change it
uint8_t	ccidSetParametersProcess(uint8_t readerid,uint8_t protocol, uint8_t * ProtocolDataStructure, uint8_t len)
{
	return 1;
}
//====================================================
//readerid: 0x01-contact reader, 0x02-contaceless reader
//clkfreq_datarate:ICC clock frequency in KHz  and  ICC data rate in bps
//					dwClockFrequency: 4 bytes
//					dwDataRate:	   4 bytes
//return value:	0:	  time out or failed
//				others: correct
//note: if thers are some parameters that users does not need ,please do not change it
uint8_t	ccidDataRateAndClockFrequencyProcess(uint8_t readerid,uint8_t *clkfreq_datarate)
{
	return 1;
}
//====================================================
//readerid: 0x01-contact reader, 0x02-contaceless reader
//uint8_t * cmddatabuf: IN:first 5 bytes is cmd, others is data
//				  OUT:data for response
//uint16_t len: the total length of cmd and data
//return value:	0:	  time out or failed
//				others: the length of data for response
uint16_t	ccidCommandProcess(uint8_t readerid, uint8_t * cmddatabuf, uint16_t len)
{
	uint16_t return_len = 0;	
	memcpy(g_abComBuf, cmddatabuf, 5);
	memcpy(IOBuf, cmddatabuf+5, len-5);

	if(CLA == 0xFF && INS == 0x10)				   //	PPS
	{
		return 4;
	}
	else
	{
		Foffset = P1*0x100 + P2;				   //	Start address of flash reading/writing
		SWptr = SUCCESS;						   //	9000 as default
		CMD_Handle();						   	   //	ISO7816 CMD Handler
	}
	
	if(((INS == GETRDMN)|| ((INS == ALGOPER) && (((P2&0xB0) == 0x10)|| ((P2&0xB0) == 0x90)))) && ((SWptr == SUCCESS)))
	{		
		if(P3 == 0x00)
		{
			memcpy(cmddatabuf,IOBuf,0x100);
			return_len = 0x100;
		}		
		else
		{
			return_len = P3;
			memcpy(cmddatabuf,IOBuf,P3);
		} 
	}

	cmddatabuf[return_len++] = SW[SWptr];			//	SW1
	cmddatabuf[return_len++] = SW[SWptr+1];			//	SW2

	return return_len;	
}
