/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB_std_req.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 



#include "thm36_1.3.h"
#include "USB.h"
#include "HID.h"

uint8_t	g_bUsbRemoteWakeUp;											//	0: REMOTE WAKEUP is not supported,1:supported
uint8_t	g_bUsbEpHalt;												//	0: no ep halted,others:Number of ep to be halted 

const voidfunc usbStdReq[0x0D]= {usbStdReqGetStatus,usbStdReqClearFeature,usbStdReqRfu1,usbStdReqSetFeature,
								usbStdReqRfu2,usbStdReqSetAddress,usbStdReqGetDescriptor,usbStdReqSetDescriptor,
								usbStdReqGetConfiguration,usbStdReqSetConfiguration,usbStdReqGetInterface,
								usbStdReqSetInterface,usbStdReqSynchFrame};		 
		
//******Handling 11 Standard Request******//

/***************************************************************************
* Function: usbStdReqGetStatus
* Description: Get Status request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetStatus(void)
{	   
	if(((USBSTS0 & 0x0000007F) > 0) && (USBSTS0 & Bit21_En)) 	//	Cofig state									
	{	
		if(st_usbDeviceRequest.bmRequestType == 0x80)			//	1000 0000B,device
		{	
		//	if(g_bUsbRemoteWakeUp)
		//		USBbuf[0] = 0x02;								//	Remote wakeup is enabled
		//	else
			USBbuf[0] = 0x00;									//	Does not support Remote Wakeup and Self-Powered
			USBbuf[1] = 0x00;
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x81)		//	&& (st_usbDeviceRequest.wIndex == 0))									//	1000 0001B,interface 0
		{	
			USBbuf[0] = 0x00;
			USBbuf[1] = 0x00;
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x82 && (st_usbDeviceRequest.wIndex <= 0x88))   //	1000 0010B,ep0~ep4									
		{	
			if(!g_bUsbEpHalt)
			{	
				USBbuf[0] = 0x00;								//	Bit0=0,ep not halt
				USBbuf[1] = 0x00;
			}
			else if(g_bUsbEpHalt == st_usbDeviceRequest.wIndex)
			{	
				USBbuf[0] = 0x01;								//	Bit0=1,specified ep halt
				USBbuf[1] = 0x00;
			}
		}
		else
		{	
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
			return;
		}
		g_sUsbEp0TxLen = 2;
		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
	}
	else if((USBSTS0 & 0x0000007F) > 0)							//	Address state
	{	
		if(st_usbDeviceRequest.wIndex > 0)						//	Not interface 0 or ep0
		{
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
			return;
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x80)		//	1000 0000B,device
		{	
		//	if(g_bUsbRemoteWakeUp)
		//	USBbuf[0] = 0x02;									//	Remote wakeup is enabled
		//	else
			USBbuf[0] = 0x00;									//	Does not support Remote Wakeup and Self-Powered
			USBbuf[1] = 0x00;
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x81)		//	1000 0001B,interface 0
		{
			USBbuf[0] = 0x00;
			USBbuf[1] = 0x00;
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x82)		//	1000 0010B,ep0
		{
			USBbuf[0] = 0x00;									//	Bit0=0,ep not halt
			USBbuf[1] = 0x00;
		}
		g_sUsbEp0TxLen = 2;
		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
	}
}	   

/***************************************************************************
* Function: usbStdReqClearFeature
* Description: Clear Feature request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqClearFeature(void)
{	   
	if(((USBSTS0 & 0x0000007F) > 0) && (USBSTS0 & Bit21_En))	//	Cofig state										
	{
		if(st_usbDeviceRequest.bmRequestType == 0x00)			//	1000 0000B,device
		{
			g_bUsbRemoteWakeUp = 0;
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,clear remote wakeup and self-powered
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x01)		//	&& (st_usbDeviceRequest.wIndex == 0))	//	0000 0001B,interface 0										
		{
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x02 && (st_usbDeviceRequest.wIndex <= 0x88))	//	0000 0010B,ep0~ep4									
		{
			g_bUsbEpHalt = 0x00;								//	Clear ep halt
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,clear ep halt
		}
		else
		{
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
		}
	}	
	else if((USBSTS0 & 0x0000007F) > 0)							//	Address state
	{
		if(st_usbDeviceRequest.wIndex > 0)						//	Not interface 0 or ep0
		{
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x00)		//	0000 0000B,device
		{
			g_bUsbRemoteWakeUp = 0;
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,clear remote wakeup
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x01)		//	0000 0001B,interface 0
		{
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK
		}
		else if(st_usbDeviceRequest.bmRequestType == 0x02)		//	0000 0010B,ep0
		{
			g_bUsbEpHalt = 0;
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,clear ep halt
		}
	} 
	
	//	Setting registers for implementing the specified USB feature
	if(!g_bUsbRemoteWakeUp)
		USBDCON &= Bit28_Dis;									//	Disables Remote Wakeup
}

/***************************************************************************
* Function: usbStdReqRfu1
* Description: RFU 
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqRfu1(void)
{}

/***************************************************************************
* Function: usbStdReqSetFeature
* Description:  Set Feature request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqSetFeature(void)
{
	if(((USBSTS0 & 0x0000007F) > 0) && (USBSTS0 & Bit21_En))	//	Cofig state												
	{
		if(st_usbDeviceRequest.bmRequestType == 0x00 && st_usbDeviceRequest.wValue == 1)	//	1000 0000B,device,g_bUsbRemoteWakeUp
		{	
			g_bUsbRemoteWakeUp = 1;
		//	USBCECON &= Bit9_Dis;
		//	USBCECON |= Bit8_En;								//	ACK,set remote wakeup
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
		}
//		else if(bmRequestType == 0x01)// && (wIndex == 0))		//	0000 0001B,interface 0												
//		{
//			USBCECON &= Bit9_Dis;
//			USBCECON |= Bit8_En;								//	ACK
//		}
		else if(st_usbDeviceRequest.bmRequestType == 0x02 && (st_usbDeviceRequest.wValue == 0)) //	0000 0010B,ENDPOINT_HALT										
		{
			g_bUsbEpHalt = st_usbDeviceRequest.wIndex;			//	Halt specified ep
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,set ep halted
		}
		else
		{
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
		}
	}
	else if((USBSTS0 & 0x0000007F) > 0)							//	Address state
	{
		if(st_usbDeviceRequest.wIndex > 0)						//	Not interface 0 or ep0
		{
			USBCECON |= Bit9_En;
			USBCECON &= Bit8_Dis;								//	STALL
		}
		else if((st_usbDeviceRequest.bmRequestType == 0x00) && (st_usbDeviceRequest.wValue == 1))	 //	0000 0000B,device,g_bUsbRemoteWakeUp										
		{
			g_bUsbRemoteWakeUp = 1;
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,set remote wakeup,self-powered is not supported
		}
//		else if(st_usbDeviceRequest.bmRequestType = 0x01)		//	0000 0001B,interface 0
//		{
//			USBCECON &= Bit9_Dis;
//			USBCECON |= Bit8_En;								//	ACK
//		}
		else if((st_usbDeviceRequest.bmRequestType == 0x02) && (st_usbDeviceRequest.wValue == 0))	//	0000 0010B,ep0,EPPOINT_HALT										
		{
			g_bUsbEpHalt = st_usbDeviceRequest.wIndex;			//	Halt specified ep
			USBCECON &= Bit9_Dis;
			USBCECON |= Bit8_En;								//	ACK,set ep halted
		}
	}

	if(g_bUsbRemoteWakeUp)
		USBDCON |= Bit28_En;									//	Enables Remote Wakeup
}

/***************************************************************************
* Function: usbStdReqRfu2
* Description:  RFU
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqRfu2(void)
{}

/***************************************************************************
* Function: usbStdReqSetAddress
* Description:  Set Address request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqSetAddress(void)
{
	uint32_t TimeCounter = 0;
	USBCECON &= Bit9_Dis;
	USBCECON |= Bit8_En;										//	Send ACK (0-length data packet) first, Then set the new addr after the host sends Status Stage flag pkt,
	USBSTS0 &= 0xFFFFFF00;										//	Set address
	USBSTS0 |= st_usbDeviceRequest.wValue;
	while(!(USBCESTS & Bit2_En))
	{	
		{	//TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
	}	
	USBCESTS = Bit2_En;											//	Write 1 to clear CXSTS interrupt 
}

/***************************************************************************
* Function: usbStdReqGetDescriptor
* Description:  Get Descriptor request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetDescriptor(void)
{
	uint8_t	DscptType;
		
	DscptType = st_usbDeviceRequest.wValue >> 8;				//	High uint8_t specifies the descriptor type
	
	switch(DscptType)
	{
		case DEVICE			:	usbStdReqGetDeviceDescriptor();		break;
		case CONFIGURATION	:	usbStdReqGetConfigDescriptor();		break;
		case STRING			:	usbStdReqGetStringDescriptor();		break;
		case INTERFACE		:										break;
		case ENDPOINT	   :										break;
		#if HID
		case HIDRREPORT		:	hidSendReportDscrpt();				break;
		#endif
		default				:	EP0SENDSTALL;						break;
	}
}

/***************************************************************************
* Function: usbStdReqSetDescriptor
* Description:  Set Descriptor request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqSetDescriptor(void)
{
	USBCECON |= Bit9_En;
	USBCECON &= Bit8_Dis;										//	STALL,does not support Set_Descriptor
}

/***************************************************************************
* Function: usbStdReqGetConfiguration
* Description:  Get Configuration request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetConfiguration(void)
{
	if(((USBSTS0 & 0x0000007F) > 0) && (USBSTS0 & Bit21_En))	//	Configured,send config descriptor										
	{
		USBbuf[0] = 0x01;										//	Return 0x01 after configured
		g_sUsbEp0TxLen = 0x01;
		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
	}
	else if((USBSTS0 & 0x0000007F) > 0)
	{	
		USBbuf[0] = 0;											//	Return 0 if not configured but addressed
		g_sUsbEp0TxLen = 1;
		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
	}
}

/***************************************************************************
* Function: usbStdReqSetConfiguration
* Description:  Set Configuration request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqSetConfiguration(void)
{
	uint32_t TimeCounter = 0;
	if((st_usbDeviceRequest.wValue & 0x00FF) == 1)				//	Low uint8_t of wValue == 1,default config
	{	
		USBSTS0 |= Bit21_En;									//	DVC Status Reg.CFG = 1
		while(!(USBCESTS & Bit2_En))						//	add by ys 20121010
		{	//TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
		USBCESTS = Bit2_En;										//	Write 1 to clear CXSTS interrupt 
		USBCECON &= Bit9_Dis;									//	ACK	
		USBCECON |= Bit8_En;				
	}
	else if((st_usbDeviceRequest.wValue & 0x00FF) == 0)			//	Low uint8_t of wValue == 0,return addess state
	{	
		USBSTS0 &= Bit21_Dis;									//	DVC Status Reg.CFG = 1
		TimeCounter = 0;
		while(!(USBCESTS & Bit2_En))							//	add by ys 20121010
		{	//TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
		USBCESTS = Bit2_En;										//	Write 1 to clear CXSTS interrupt 
		USBCECON &= Bit9_Dis;									//	ACK
		USBCECON |= Bit8_En;			
	}
	else														//	Low uint8_t of wValue == 0 or config is not supported
	{
		USBCECON |= Bit9_En;
		USBCECON &= Bit8_Dis;									//	STALL
	}
}

/***************************************************************************
* Function: usbStdReqGetInterface
* Description:  Get Interface request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqGetInterface(void)
{	
	if((USBSTS0 & Bit21_En)&&(st_usbDeviceRequest.wIndex == 0))	//	Support only 1 interface
	{	
		USBbuf[0] = 0;					
		g_sUsbEp0TxLen = 1;
		usbEpnTxAll(0, USBbuf, g_sUsbEp0TxLen);
	}
	else
	{	
		USBCECON |= Bit9_En;
		USBCECON &= Bit8_Dis;									//	STALL,does not support Set_Descriptor
	}
}

/***************************************************************************
* Function: usbStdReqSetInterface
* Description:  Set Interface request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqSetInterface(void)
{
	if((USBSTS0 & Bit21_En)&&(st_usbDeviceRequest.wValue == 0)&&(st_usbDeviceRequest.wIndex==0))//	Support the default Alternative Setting	for only 1 interface
	{
		USBCECON &= Bit9_Dis;
		USBCECON |= Bit8_En;									//	ACK
	}
	else
	{
		USBCECON |= Bit9_En;									//	STALL,does not support Set_Descriptor
		USBCECON &= Bit8_Dis;			
	}
}

/***************************************************************************
* Function: usbStdReqSynchFrame
* Description:  Synch Frame request
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbStdReqSynchFrame(void)
{
	if((USBSTS0 & 0x0000007F) > 0)								//	Address
	{	
		USBCECON |= Bit9_En;
		USBCECON &= Bit8_Dis;									//	STALL,does not support Set_Descriptor
	}
	else if(USBSTS0 & Bit21_En)									//	Configured,.CFG=1
	{;}															//	To be completed,ISO transfer
}

//************End************//
