/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: USB_ep.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "USB.h"	  


#ifndef SIMULATOR


USB_DEVICE_REQUEST st_usbDeviceRequest;

uint16_t	g_sUsbEp0TxLen;														//	Length of Ep0 data to be transmitted

#ifdef __GNUC__ 
//#pragma pack (64)  __attribute__((section(".sram")))
#if	DYNAMICFIFO
 uint8_t g_abUsbEp0Buf[USB_BUF_LEN*2] __attribute__((section(".usbram")));	//	USB Ep0 trans buf
#else 
 uint8_t g_abUsbBuf[MAXPKTSIZE*12] __attribute__((section(".usbram")));
 uint8_t g_abUsbEp0Buf[USB_BUF_LEN*2] __attribute__ ((aligned (64)));
#endif
//#pragma pack ()
#else
#if	DYNAMICFIFO
__attribute__ ((aligned (64))) uint8_t g_abUsbEp0Buf[USB_BUF_LEN*2] __attribute__((at(USBFIFO_ADDR_START)));	//	USB Ep0 trans buf
#else 
__attribute__ ((aligned (64))) uint8_t g_abUsbBuf[MAXPKTSIZE*12] __attribute__((at(USBFIFO_ADDR_START)));
__attribute__ ((aligned (64))) uint8_t g_abUsbEp0Buf[USB_BUF_LEN*2];
#endif
#endif

static void usbEpTx(uint8_t ep, uint8_t * buf,uint8_t len);
/***************************************************************************
* Description: static function
**************************************************************************/
static void usbEpTxAll(uint8_t ep, uint8_t * buf, uint16_t len);
//static void usbEpTx(uint8_t ep, uint8_t * buf,uint8_t len);
void usbEp0TxAll(uint8_t * buf,uint16_t len);
//static void usbEp0Tx(uint8_t * buf,uint8_t len);


/***************************************************************************
* Function: usbEp0RxTx
* Description: USB Ep0 Rx and Tx
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbEp0RxTx(void)
{
	uint32_t	rcvlen;
	uint32_t TimeCounter = 0;
	rcvlen = (USBCESTS >> 8) & 0x000003FF;								//	buf 0 valid enties
	if(EP0STSCXS)														//	SETUP stage,CXS interrupt
	{
		EP0SENDNAK;											
		EP0CXSCLR;														//	Write 1 to clear CXS interrutp
			
		st_usbDeviceRequest.wValue = USBDCC1 >> 16;
		st_usbDeviceRequest.bRequest = USBDCC1 >> 8;
		st_usbDeviceRequest.bmRequestType = USBDCC1;
		st_usbDeviceRequest.wLength = USBDCC2 >> 16;
		st_usbDeviceRequest.wIndex = USBDCC2;

		USBCEBF = (uint32_t)g_abUsbEp0Buf;
		
		if(!(st_usbDeviceRequest.bmRequestType&Bit6_En) && !(st_usbDeviceRequest.bmRequestType&Bit5_En))		//	Bit6~5=00,Standard
			st_usbDeviceRequest.bStdRqstFlag = 1;
		else if(!(st_usbDeviceRequest.bmRequestType&Bit6_En) && (st_usbDeviceRequest.bmRequestType&Bit5_En))	//	Bit6~5=01,Class
			st_usbDeviceRequest.bClsRqstFlag = 1;
		else if((st_usbDeviceRequest.bmRequestType&Bit6_En) && !(st_usbDeviceRequest.bmRequestType&Bit5_En))	//	Bit6~5=10,Vendor
			st_usbDeviceRequest.bVdrRqstFlag = 1;
	}
	else if(EP0STSCXDPKT && (rcvlen > 0))								//	CXDPKT interrupt,Data stage
	{				
		EP0CXDPKTCLR;													//	Write 1 to clear CXDPKT interrutp
		USBCECON |= Bit10_En;											//	clear buf
		while(!(EP0STSCXSTS))  										
		{	//TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
		EP0CXSTSCLR;													//	Write 1 to clear CXSTS interrupt
		EP0SENDACK;
		//g_bUsbEp0RcvDataFlag = 0x01;
	}	
	else if(EP0STSCXDPKT && (g_sUsbEp0TxLen > 0))						//	CXDPKT interrupt,and there is data left for being transmitted
	{
		EP0CXDPKTCLR;													//	Write 1 to clear CXDPKT interrupt
		if(EP0STSCXSTS)													//	Status stage,CXSTS interrupt
		{				
			EP0CXSTSCLR;												// 	Write 1 to clear CXSTS interrutp
			if((!(USBCECON & Bit9_En)) && (USBCECON & Bit8_En))
			{
				EP0SENDACK;												//	Send ACK if 0-length data packet received
			}
			else
			{
				EP0SENDNAK;												//	Send NAK,not complete yet
			}
		}
		usbEpnTxAll(0, USBbuf,g_sUsbEp0TxLen);
	}
	else if(EP0STSCXDPKT)												//	CXDPKT interrupt,no data left
	{					
		EP0CXDPKTCLR;													//	Write 1 to clear CXDPKT interrutp
		EP0SENDACK;														//	Send ACK		
	}
	else if(EP0STSCXSTS)												//	Status stage,CXSTS interrupt
	{																							
		EP0CXSTSCLR;													//	Write 1 to clear CXSTS interrupt
		EP0SENDACK;	
	}
}


/***************************************************************************
* Function: usbEpnTxAll
* Description: USB all Ep Tx all data
* Input:  ep	: the number of ep,form 0 to 8
		  buf   : transmit data memory address 
		  len   : transmit data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbEpnTxAll(uint8_t ep, uint8_t * buf, uint16_t len)
{
	if(ep == 0)
		usbEp0TxAll(buf,len);	
	else
		usbEpTxAll(ep, buf, len);
}

/***************************************************************************
* Function: usbEp0RxAll
* Description: USB Ep0 Rx all data
* Input:  buf   : receive data memory address 
		  len   : receive data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void	usbEp0RxAll(uint8_t * buf,uint16_t len)
{
	uint16_t	rcvlen;
	uint16_t	i;
	uint32_t TimeCounter = 0;
	while(!EP0STSCXDPKT)
	{	//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}
	rcvlen = (USBCESTS >> 8) & 0x000003FF;								//	buf 0 valid enties
	
	if(rcvlen > 0)														//	CXDPKT interrupt,Data stage
	{				
		EP0CXDPKTCLR;													//	Write 1 to clear CXDPKT interrutp

		for(i=0;i<len;i++)						
		{
			buf[i] = g_abUsbEp0Buf[(i & (USBMAXPKTSIZE(0) - 1))];			
			if(i == (len-1))
			{
				EP0SENDNAK;					   							//	send NAK
				break;
			}

			if((i > 0) && ((i + 1) % USBMAXPKTSIZE(0) == 0))
			{
				USBCECON |= Bit10_En;									//	Ep0 Buffer0 cleared
				EP0SENDACK;
				TimeCounter = 0;
				while(!(EP0STSCXDPKT))
				{	//TMC(20240103)-zt: Exit execution in case of timeout
					TimeCounter++;
					if(TimeCounter > USBOVERTIME_3MS)
					{
						TimeCounter = 0;
						return;
					}
				}
				EP0CXDPKTCLR;
				EP0SENDNAK;												//	send NAK
			}
		}
		USBCECON |= Bit10_En;											//	clear buf
		TimeCounter = 0;
		while(!(EP0STSCXSTS))  										
		{	//TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
		EP0CXSTSCLR;													//	Write 1 to clear CXSTS interrupt
		EP0SENDACK;

	}	
	else																//	CXDPKT interrupt,no data left
	{
		EP0CXDPKTCLR;													//	Write 1 to clear CXDPKT interrutp
		EP0SENDACK;														//	Send ACK
	}
}

/***************************************************************************
* Function: usbEpTxAll
* Description: USB Epn except Ep0 Rx first packet data
* Input:  ep  : the number of ep,form 1 to 8
		  buf : Receive data memory address 
* Output: NULL
* Return: Receive data length	
* Other:  NULL
**************************************************************************/	
uint8_t usbEpnRxFirstPacket(uint8_t ep, uint8_t * buf)
{
	uint32_t tempEpStat;
	uint8_t len;
	uint32_t TimeCounter = 0;
#if	!DYNAMICFIFO		
	uint8_t i;			
#endif
	
	while(!(USBEPSTS(ep)&Bit0_En))
	{	//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return 0;
		}
	}
	tempEpStat = USBEPSTS(ep);
	len = (tempEpStat >> 8) & 0x03FF;									//	Buf 0 valid entries,bit 8~17 of USBEP2STS

	EPDPKTCLR(ep);														//	Clear DPKT interrupt
#if	DYNAMICFIFO						
	if((uint32_t)buf != USBEPBF(ep))
		memcpy(buf,(uint8_t*)(USBEPBF(ep)) ,len);
#else			
	for(i=0;i<len;i++)
		buf[i] = USBEPDAT_B(ep);
#endif
//	EP1BUF0CLR;															//	Clear buffer 0, 
	return len;															//	return receive data len
}

/***************************************************************************
* Function: usbEpTxAll
* Description: USB Epn except Ep0 Rx last all data
* Input:  ep  : the number of ep,form 1 to 8
		  buf : Receive data memory address 
		  len : last all data length
* Output: NULL
* Return: Receive data length	
* Other:  NULL
**************************************************************************/	
uint16_t	usbEpnRxLastAllData(uint8_t ep, uint8_t * buf,uint16_t len)
{
#if !DYNAMICFIFO
	uint16_t i;
#endif
	uint16_t	rxlen = 0, pktlen = 0;
	uint32_t	tempEpStat;
	uint16_t maxpacketsize = USBMAXPKTSIZE(ep);
	uint32_t TimeCounter = 0;
	
	if(len == 0) 		
	{
		EPBUF0CLR(ep);
		return 0;
	}
	else 				
	{
	#if DYNAMICFIFO
		USBEPBF(ep) = (uint32_t)buf;
		EPBUF0CLR(ep); 													//	Clear buffer 0
	#endif
		
		while(rxlen != len)
		{						  
			while(!(USBEPSTS(ep)&Bit0_En))
			{	//TMC(20240103)-zt: Exit execution in case of timeout
				TimeCounter++;
				if(TimeCounter > USBOVERTIME_3MS)
				{
					TimeCounter = 0;
					return 0;
				}
			}
			tempEpStat = USBEPSTS(ep);
			pktlen = (tempEpStat >> 8) & 0x03FF;						//	Buf 0 valid entries,bit 8~17 of USBEP2STS		
	
			EPDPKTCLR(ep);												//	Clear DPKT interrupt
			if(pktlen == maxpacketsize)									//	whole packet length 
			{
	#if DYNAMICFIFO		
				USBEPBF(ep) += maxpacketsize;
				EPBUF0CLR(ep);											//	Clear buffer 0
	#else
				for(i=0;i<pktlen;i++)
					buf[rxlen+i] = USBEPDAT_B(ep);
	#endif
			}
			else if(pktlen > 0)
			{
	#if DYNAMICFIFO				
				EPBUF0CLR(ep);											//	Clear buffer 0
	#else
				for(i=0;i<pktlen;i++)
					buf[rxlen+i] = USBEPDAT_B(ep);
	#endif			
			}
			else														//	Zero-length pkt
			{
				EPBUF0CLR(ep);											//	Clear buffer 0
			}
			rxlen += pktlen;
		}
	}
#if DYNAMICFIFO	
	USBEPBF(ep) = (uint32_t)buf;
#endif
	return	rxlen;
}

void usbEp0TxSTSACK(void)
{
	uint32_t TimeCounter = 0;														
	while(!(EP0STSCXSTS))						//	Status stage,CXSTS interrupt	
	{	//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}			
	EP0CXSTSCLR;													//	Write 1 to clear CXSTS interrupt
	EP0SENDACK;	
}
/***************************************************************************
* Function: usbEp0Tx
* Description: USB Ep0 tx one packet data
* Input:  buf : transmit data memory address 
		  len : transmit data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
static void usbEp0Tx(uint8_t * buf,uint8_t len)
{
	uint32_t TimeCounter = 0;
#if	!DYNAMICFIFO
	uint8_t	i;	
#endif

	USBCECON |= Bit11_En;												//	Clear Buffer 1	
#if	DYNAMICFIFO
	USBCEBF = (uint32_t)buf;

	USBCESTS_B2 = (len << 4);											//	BF1VENTRS,low 4-bit
	USBCESTS_B3 = (len >> 4);											//	BF1VENTRS,high 4-bit
#else
	for(i=0;i<len;i++)									
		USBCEDAT_B = buf[i];
#endif
	if(len < EP0PKTSIZE)												//	Short pkt
	{
		EP0SENDACK;														//	send ACK
	}

	while(!(USBCESTS & Bit1_En))															//	Waiting for short pkt is transmitted successfully				
	{	//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}
	EP0CXDPKTCLR;														//	Clear the CXDPKT flag after short pkt is sent
#if	DYNAMICFIFO
	USBCEBF = (uint32_t)g_abUsbEp0Buf;
#endif

}

/***************************************************************************
* Function: usbEp0TxAll
* Description: USB Ep0 Tx all data
* Input:  buf  : transmit data memory address 
		  len  : transmit data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbEp0TxAll(uint8_t * buf,uint16_t len)
{
	if(len == 0)
	{
		usbEp0Tx(buf,0);
	}
	else
	{
		while(len)
		{
			if(len > EP0PKTSIZE)
			{
				usbEp0Tx(buf,EP0PKTSIZE); 
				len -= EP0PKTSIZE;
				buf += EP0PKTSIZE; 
			}
			else if(len == EP0PKTSIZE)
			{
				usbEp0Tx(buf,len);
				TMC_Delay(0x2000);
				if(!EP0STSCXSTS)
					usbEp0Tx(buf,0);
				len = 0;
			}
			else
			{
				usbEp0Tx(buf,len);	
				len = 0;
			}
		}
	}	
}
/***************************************************************************
* Function: usbEpnTx
* Description: USB all Ep Tx one packet
* Input:  ep	: the number of ep,form 0 to 8
		  buf   : transmit data memory address 
		  len   : transmit data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void usbEpnTx(uint8_t ep, uint8_t * buf,uint8_t len)
{
	if(ep == 0)
		usbEp0Tx(buf,len);
	else
		usbEpTx(ep,buf,len);
}

/***************************************************************************
* Function: usbEpTx
* Description: USB Epn except Ep0 Tx one packet
* Input:  ep	: the number of ep,form 1 to 8
		  buf   : transmit data memory address 
		  len   : transmit data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
static void usbEpTx(uint8_t ep, uint8_t * buf,uint8_t len)
{
	uint32_t TimeCounter = 0;				
#if	DYNAMICFIFO
	USBEPBF(ep) = (uint32_t)buf;

	EPBUF0CLR(ep);														//	Clear Buffer 0
	while(USBEPCON(ep) & Bit9_En)
	{	//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}
	EPPKTLENL(ep) = len;
#else
	uint8_t i;
	EPBUF0CLR(ep);														//	Clear Buffer 0
	while(USBEPCON(ep) & Bit9_En)
	{ //TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}	
	for(i=0;i<len;i++)
		USBEPDAT_B(ep) = buf[i];
#endif
	if(len < USBMAXPKTSIZE(ep))
		EPPKTRDY(ep);													//	buf 0 ready			
	TimeCounter = 0;		
	while(USBEPSTS(ep) & Bit19_En)
	{	//TMC(20240103)-zt: Exit execution in case of timeout
		TimeCounter++;
		if(TimeCounter > USBOVERTIME_3MS)
		{
			TimeCounter = 0;
			return;
		}
	}
	EPDPKTCLR(ep);														//	Clear DPKT interrupt	
}

/***************************************************************************
* Function: usbEpTxAll
* Description: USB Epn except Ep0 Tx all data
* Input:  ep	: the number of ep,form 1 to 8
		  buf   : transmit data memory address 
		  len   : transmit data length
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
//#define USB_SEND_WAIT_CNT	   0x00100000
static void usbEpTxAll(uint8_t ep, uint8_t * buf, uint16_t len)
{	
	uint16_t maxpacketsize = USBMAXPKTSIZE(ep);
	uint32_t TimeCounter = 0;	
#if	DYNAMICFIFO
	USBEPBF(ep) = (uint32_t)buf;
	
	if(len == 0)
	{
		EPPKTLENL(ep) = 0;
		EPPKTRDY(ep);													//	buf 0 ready
		while(USBEPSTS(ep) & Bit19_En)
		{	//TMC(20240103)-zt: Exit execution in case of timeout -- i-> TimeCounter
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
		EPDPKTCLR(ep);													//	Clear DPKT interrupt
	}
	else
	{
		while(len)
		{
			EPDPKTCLR(ep);												//	Clear DPKT interrupt 移到这里
			EPBUF0CLR(ep);												//	Clear Buffer 0
			TimeCounter = 0;
			while(USBEPCON(ep) & Bit9_En)
			{	//TMC(20240103)-zt: Exit execution in case of timeout
				TimeCounter++;
				if(TimeCounter > USBOVERTIME_3MS)
				{
					TimeCounter = 0;
					return;
				}
			}
			if(len > maxpacketsize)
			{
				EPPKTLENL(ep) = maxpacketsize;
				TimeCounter = 0;
				while(USBEPSTS(ep) & Bit19_En)		
				{	//TMC(20240103)-zt: Exit execution in case of timeout -- i-> TimeCounter
					TimeCounter++;
					if(TimeCounter > USBOVERTIME_3MS)
					{
						TimeCounter = 0;
						return;
					}
				}
				len -= maxpacketsize;
				USBEPBF(ep) += maxpacketsize;
			}
			else if(len == maxpacketsize)
			{
				EPPKTLENL(ep) = maxpacketsize;
				TimeCounter = 0;
				while(USBEPSTS(ep) & Bit19_En) 
				{	//TMC(20240103)-zt: Exit execution in case of timeout -- i-> TimeCounter
					TimeCounter++;
					if(TimeCounter > USBOVERTIME_3MS)
					{
						TimeCounter = 0;
						return;
					}
				}
				len = 0;
			}
			else
			{
				EPPKTLENL(ep) = len;
				EPPKTRDY(ep);											//	buf 0 ready
				TimeCounter = 0;
				while(USBEPSTS(ep) & Bit19_En) 
				{	//TMC(20240103)-zt: Exit execution in case of timeout -- i-> TimeCounter
					TimeCounter++;
					if(TimeCounter > USBOVERTIME_3MS)
					{
						TimeCounter = 0;
						return;
					}
				}	
				len = 0;	
			}
			//提前到函数入口 EPDPKTCLR(ep);												//	Clear DPKT interrupt
		}
	}
#else
	//uint8_t i;
	if(len == 0)
	{
		EPPKTRDY(ep);													//	buf 0 ready	
		while(USBEPSTS(ep) & Bit19_En)
		{	//TMC(20240103)-zt: Exit execution in case of timeout
			TimeCounter++;
			if(TimeCounter > USBOVERTIME_3MS)
			{
				TimeCounter = 0;
				return;
			}
		}
		EPDPKTCLR(ep);													//	Clear DPKT interrupt
	}
	else
	{
		while(len)
		{
		//	EPBUF0CLR(ep);												//	Clear Buffer 0
			TimeCounter = 0;
			while(USBEPCON(ep) & Bit9_En)
			{	//TMC(20240103)-zt: Exit execution in case of timeout
				TimeCounter++;
				if(TimeCounter > USBOVERTIME_3MS)
				{
					TimeCounter = 0;
					return;
				}
			}
			if(len > maxpacketsize)
			{
				for(i=0;i<(maxpacketsize>>2);i++)
					USBEPDAT(ep) = *(uint32_t *)(buf + (i<<2));
				len -= maxpacketsize;
				buf += maxpacketsize;
			}
			else if(len == maxpacketsize)
			{
				for(i=0;i<(maxpacketsize>>2);i++)
					USBEPDAT(ep) = *(uint32_t *)(buf + (i<<2));
				len = 0;
			}
			else //if(len < transmodelen)
			{
				for(i=0;i<len;i++)
					USBEPDAT_B(ep) = buf[i];
				EPPKTRDY(ep);											//	buf 0 ready	
				len = 0;			
			}
			TimeCounter = 0;
			while(USBEPSTS(ep) & Bit19_En)
			{	//TMC(20240103)-zt: Exit execution in case of timeout
				TimeCounter++;
				if(TimeCounter > USBOVERTIME_3MS)
				{
					TimeCounter = 0;
					return;
				}
			}
			EPDPKTCLR(ep);												//	Clear DPKT interrupt
		}
	}
#endif		
}


//static void usbEpTxAll(uint8_t ep, uint8_t * buf, uint16_t len)
//{	

//	uint16_t maxpacketsize = USBMAXPKTSIZE(ep),j=0;
//				
//#if	DYNAMICFIFO
//	USBEPBF(ep) = (uint32_t)buf;
//	
//	if(len == 0)
//	{
//		EPPKTLENL(ep) = 0;
//		EPPKTRDY(ep);													//	buf 0 ready
//		while(USBEPSTS(ep) & Bit19_En)
//		{
//			j++;	  
//			if(j > USBOVERTIME)
//			return;
//		}	
//		EPDPKTCLR(ep);													//	Clear DPKT interrupt
//	}
//	else
//	{
//		while(len)
//		{
//			j = 0;
//			EPBUF0CLR(ep);												//	Clear Buffer 0
//			while(USBEPCON(ep) & Bit9_En)
//			{;}
//			if(len > maxpacketsize)
//			{
//				EPPKTLENL(ep) = maxpacketsize;
//				while(USBEPSTS(ep) & Bit19_En)
//				{
//					j++;	  
//					if(j > USBOVERTIME)
//					return;
//				}
//				len -= maxpacketsize;
//				USBEPBF(ep) += maxpacketsize;
//			}
//			else if(len == maxpacketsize)
//			{
//				EPPKTLENL(ep) = maxpacketsize;
//				while(USBEPSTS(ep) & Bit19_En)
//				{
//					j++;	  
//					if(j > USBOVERTIME)
//					return;					
//				}
//				len = 0;

//				EPPKTLENL(ep) = 0;   				//	for debug， 整包长时，增加发送空包的情况。
//				EPPKTRDY(ep);
//				while(USBEPSTS(ep) & Bit19_En)
//				{
//					j++;	  
//					if(j > USBOVERTIME)
//					return;					
//				}
//			}
//			else
//			{
//				EPPKTLENL(ep) = len;
//				EPPKTRDY(ep);											//	buf 0 ready
//				while(USBEPSTS(ep) & Bit19_En)
//				{
//					j++;	  
//					if(j > USBOVERTIME)
//					return;
//				}
//				len = 0;	
//			}
//			EPDPKTCLR(ep);												//	Clear DPKT interrupt
//		}
//	}
//#else
//	uint8_t i;
//	if(len == 0)
//	{
//		EPPKTRDY(ep);													//	buf 0 ready	
//		while(USBEPSTS(ep) & Bit19_En)
//		{
//			j++;	  
//			if(j > USBOVERTIME)
//			return;
//		}
//		EPDPKTCLR(ep);													//	Clear DPKT interrupt
//	}
//	else
//	{
//		while(len)
//		{
//		//	EPBUF0CLR(ep);												//	Clear Buffer 0
//			while(USBEPCON(ep) & Bit9_En)
//			{;}	
//			if(len > maxpacketsize)
//			{
//				for(i=0;i<(maxpacketsize>>2);i++)
//					USBEPDAT(ep) = *(uint32_t *)(buf + (i<<2));
//				len -= maxpacketsize;
//				buf += maxpacketsize;

//			}
//			else if(len == maxpacketsize)
//			{
//				for(i=0;i<(maxpacketsize>>2);i++)
//					USBEPDAT(ep) = *(uint32_t *)(buf + (i<<2));
//				len = 0;
//			}
//			else //if(len < transmodelen)
//			{
//				for(i=0;i<len;i++)
//					USBEPDAT_B(ep) = buf[i];
//				EPPKTRDY(ep);											//	buf 0 ready	
//				len = 0;		
//			}
//			while(USBEPSTS(ep) & Bit19_En);
//			EPDPKTCLR(ep);												//	Clear DPKT interrupt
//		}
//	}
//#endif		
//}


#endif



