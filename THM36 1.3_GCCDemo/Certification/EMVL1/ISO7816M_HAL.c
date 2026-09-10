/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ISO7816M_HAL.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
 
#include "ISO7816M_HAL.h"
#include "ISO7816_EMV_L1.h" 
#include "thm36_1.3.h"
#include "rcc.h"
#include "gpio.h"
#include "tmc_gpio.h"
#include "iso7816M.h"

M7816_TypeDef * EMV_M7816;
	 
unsigned char workingBuffer[51];   
unsigned char ATRLength[CARD_SLOTS];   
struct EMVATR lastATR[CARD_SLOTS];   
unsigned char TMode[CARD_SLOTS];							//	T=0 or T=1	
unsigned long WWT[CARD_SLOTS];	   						//	Work wait time	
unsigned long CWT[CARD_SLOTS];	   						//	Character wait time	
unsigned long BWT[CARD_SLOTS];	   						//	Block wait time 
unsigned long WWT_BWT_timeout;
unsigned char M[CARD_SLOTS]={1};	   					//	WTX multiplier	
unsigned char EDCtype[CARD_SLOTS];   
unsigned char NAD[CARD_SLOTS];								//	Node address byte to use when communicating in T=1	
unsigned char IFSC[CARD_SLOTS];	   						//	Maximum segment length when sending to card in T=1	
unsigned char currentSlot = 0;   
unsigned char F[CARD_SLOTS] = {1};		 				//	F 
unsigned char D[CARD_SLOTS] = {1};		 				//	D  
volatile unsigned char RST_COLD = 1;  
unsigned char N = 0x00;
	
unsigned char sequenceNumber = 0;
unsigned char sequenceNumber_ICC = 0;
short T1RcvPe = 0; 
unsigned char CWT_Flag;

extern void delay_ms_static(unsigned int ms);
extern void delay_1us_static(unsigned int us);
extern void Delay_200ms(void);


/***************************************************************************
* Function: m7816Init
* Description: 7816M2 clock config, GPIO config
* Input: NULL
* Output: NULL
* Return: NULL
**************************************************************************/
void  m7816Init(void) 
{
	GPIO_InitTypeDef GPIOx_Init;

	//Enable 7816M2 clock
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_7816M2, ENABLE);
    
    /*Config 7816M2 clock source, example for source frequency is PLL_L 48MHz, DIV is 4, then the 7816M2 frequency=(Fsrc/2)/(DIV+1)==4.8MHz*/
	//TMC(20240329)-zt:change 4 to 2 to make frequency 5MHz
	RCC_7816MClockConfig(INTERFACE_7816M2,RCC_7816M_CLKSRC_PLL_L,2,ENABLE);
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	
	/*Config pre GPIO function*/
	GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source13,GPIO_AF1_7816M2);
	TMC_GPIO_StructInit(&GPIOx_Init);	
	GPIOx_Init.Pin = GPIO_PIN_13;
	GPIOx_Init.Mode = GPIO_MODE_AF;
	TMC_GPIO_Init(GPIOA,&GPIOx_Init);
	
	EMV_M7816 = ISO7816M2;
	M7816x_Cmd(EMV_M7816, ENABLE);
	
//	GPIO_InitTypeDef GPIOx_Init;

//	//Enable 7816M1 clock
//	RCC_APB3PeriphClockCmd(RCC_APB3Periph_7816M2, ENABLE);   
//  /*Config 7816M2 clock source, example for source frequency is PLL_L 48MHz, DIV is 4, then the 7816M2 frequency=(Fsrc/2)/(DIV+1)==4.8MHz*/
//	RCC_7816MClockConfig(INTERFACE_7816M2,RCC_7816M_CLKSRC_PLL_L,2,ENABLE);
//	
//	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
//	
//	
//	EMV_M7816 = ISO7816M2;
//	M7816x_Cmd(EMV_M7816, ENABLE);
}


/***************************************************************************
* Function: void clearATRStruct(struct EMVATR *myatr) 
* Description: Used to clear the ATR struct before card power up.
* Input: 
		  myatr: ATR buffer 
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/	 
void clearATRStruct(struct EMVATR *myatr)   
{	
	memset((unsigned char *)myatr,0xFF,sizeof(struct EMVATR));   
	myatr->HistoricalLength = 0;   
} 

/***************************************************************************
* Function: updateCRC
* Description: update_crc
* Input: NULL
* Output: NULL
* Return: NULL	
* Other:  CCITT CRC 16  
*		  X^16+X^12+X^5+1  
*		 Be sure to use 0xFFFF as initial crc value.
**************************************************************************/  
unsigned short update_crc(unsigned char value, unsigned short crc)   
{   
	int i;   
   
	unsigned short newval = value << 8;   
   
	for (i = 0;i < 8; i++)   
	{   
		if ((crc ^ newval) & 0x8000)   
		{   
			crc <<= 1;   
			crc ^= 0x1021;   
		}   
		else   
		{   
			crc <<= 1;   
		}   
		newval <<= 1;   
	}   
	return crc;   
}
/***************************************************************************
* Function: short readByte(void) 
* Description: readByte
* Input: NULL
* Output: NULL
* Return: 7816 received data
* Other:  NULL
**************************************************************************/
short readByte(M7816_TypeDef* M7816x)   
{   
   volatile unsigned char val;   
	  
	/*	clear TOL bit */
	M7816x_ClearUStatus(M7816x, M7816x_USTS_TO3);
	  
	while(M7816x_GetMSTS(M7816x, M7816x_MSTS_RBF) == RESET)   
	{	   
		if(M7816x_GetUSTS(M7816x, M7816x_USTS_TO3))   
		{				   
		   	return ERR_RECEIVE_TIMEOUT;   
		}	   

		if (M7816x_GetUSTS(M7816x, M7816x_USTS_PE) == SET)	   
		{		   	
			if (TMode[currentSlot] == 0)
			{
				return ERR_RECEIVE_PARITY;
			}
			else
			{  
				T1RcvPe = ERR_RECEIVE_PARITY;					
				M7816x_ClearUStatus(M7816x, M7816x_USTS_PE);	//	clear parity bit
			}
		} 	
	}
	 // Read and store byte 
	val = M7816x_ReciveData(M7816x);
	M7816x_ClearUStatus(M7816x, 0xFF);	
	return val;   
}   
  


/***************************************************************************
* Function: short writeByte(unsigned char onebyte)  
* Description: writeByte
* Input: 
		  onebyte: data will be send
* Output: NULL
* Return: status value
* Other:  NULL
**************************************************************************/
short writeByte(unsigned char onebyte, M7816_TypeDef* M7816x)   
{	 
//	uint32_t timeout_i=0;	//ly
	M7816x_ConfigIODirection(M7816x, M7816x_SEND_MODE); //	set Transmit mode
	
	while (!M7816x_GetMSTS(M7816x, M7816x_MSTS_TBE)) ;
		
	M7816x_SendData(M7816x, onebyte);	   //	send data

	/*
	*   after write data, must be wait 680ns
	*/
	//delay_1us_static(2);
	//while(M7816x_GetUSTS(M7816x, M7816x_USTS_TACT)){;}		//	wait for byte to go out	
	//while (!M7816x_GetMSTS(M7816x, M7816x_MSTS_TBE) || (M7816x_GetMSTS(M7816x, M7816x_MSTS_BGT) != SET))	;
	//while (!M7816x_GetMSTS(M7816x, M7816x_MSTS_TBE))   ;
//	{	
//		timeout_i++;
//		if(timeout_i>65530)
//		{		
//			M7816x_DeInit(EMV_M7816);//ly 20191028
//			return ERR_RECEIVE_TIMEOUT;
//		}
//	}  
	while (!M7816x_GetMSTS(M7816x, M7816x_MSTS_TBE));
	while(M7816x_GetUSTS(M7816x, M7816x_USTS_RETRY));  
	//while(!M7816x_GetUSTS(M7816x, M7816x_USTS_TO3));  

	if (M7816x_GetUSTS(M7816x, M7816x_USTS_PE | M7816x_USTS_FER))	
	{
		M7816x_ClearUStatus(M7816x, 0xFF);
		return ERR_TRANSMIT_PARITY;
	}

	return 0;
}   

   
/***************************************************************************
* Function: writeLastByte 
* Description: writeByte
* Input: NULL
* Output: NULL
* Return: status value
* Other: Write a byte , turn to receive mode  
**************************************************************************/ 
short writeLastByte(unsigned char onebyte, M7816_TypeDef* M7816x)   
{	 
	unsigned long timeout;  
	volatile unsigned char temp;
	unsigned char d[6] = {1,1,2,4,8,16}; 
	
	if (TMode[currentSlot] == 0)   	
		timeout = WWT[currentSlot]+480*d[D[currentSlot]]+20;
	else 
		timeout = (BWT[currentSlot]+960*d[D[currentSlot]])*M[currentSlot]+32;  
	
	M7816x_ClearUStatus(M7816x, M7816x_USTS_TO1|M7816x_USTS_TO2|M7816x_USTS_TO3);
	M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);				// Set up timer for 24 bit timer, edge trigger start
	M7816x_ConfigLCT(M7816x);
	M7816x_ConfigTOR(M7816x,timeout);
	M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE8);
	
	M7816x_ConfigIODirection(M7816x, M7816x_SEND_MODE); 				//	set Transmit mode   
	
	while(!M7816x_GetMSTS(M7816x, M7816x_MSTS_TBE)){;}   
	M7816x_SendData(M7816x, onebyte);		//	send data
	
	/*
	*   after write data, must be wait 680ns
	*/
	delay_1us_static(1);
	while(M7816x_GetUSTS(M7816x, M7816x_USTS_TACT)){;}		//	wait for byte to go out	
	//..while(!M7816x_GetMSTS(M7816x, M7816x_MSTS_TBE)){;}   
	while(!M7816x_GetMSTS(M7816x, M7816x_MSTS_BGT)){;} 
	while(M7816x_GetUSTS(M7816x, M7816x_USTS_RETRY));   

	if (M7816x_GetUSTS(M7816x, M7816x_USTS_PE| M7816x_USTS_FER))	
	{
		M7816x_ClearUStatus(M7816x, M7816x_USTS_PE);
		return ERR_TRANSMIT_PARITY;
	}
	
	M7816x_ConfigIODirection(M7816x, M7816x_RECEIVE_MODE);
	//set CWT when T=1,Test Case - 1790
	if (TMode[currentSlot] == 1)
	{
		M7816x_ConfigTOR(M7816x,CWT[currentSlot] + 4);
	}
	
	return 0;
}   

/***************************************************************************
* Function: unsigned short generateEDC(unsigned char type,unsigned char onebyte,unsigned short value)   
* Description: generate EDC or CRC
* Input: 
* Output: NULL
* Return: status value
* Other:  
**************************************************************************/
unsigned short generateEDC(unsigned char type,unsigned char onebyte,unsigned short value)   
{   
	if (type == EDC_TYPE_LRC)   
	{   
		return (value ^ onebyte);   
	}   
	else   
	{   
		return update_crc(onebyte,value);   
	}   
}

/***************************************************************************
* Function: sendBlock(unsigned char NAD,unsigned char PCB,unsigned short length,unsigned char *tbuffer,unsigned char type)  
* Description: sendBlock
* Input: 
* Output: NULL
* Return: status value
* Other:  
**************************************************************************/
short sendBlock(unsigned char NAD,unsigned char PCB,unsigned short length,unsigned char *buffer,unsigned char type, M7816_TypeDef* M7816x)
{   
	int i, index;   
	unsigned short EDC; 
	short txval;  
   	
	M7816x_ClearUStatus(M7816x, M7816x_USTS_TO1 | M7816x_USTS_TO2 | M7816x_USTS_TO3);
	M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ConfigTOR(M7816x, 0x00 | (0x30 << 8) | (0x00 << 16));	//	add 32 etu before sending anything 
	M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE3);

	while(M7816x_GetUSTS(M7816x, M7816x_USTS_TO3));
	
	M7816x_ClearUStatus(M7816x, M7816x_USTS_TO3);
	
	if (type == EDC_TYPE_LRC)   
	{   
		EDC = 0; 
	}   
	else		//	type = CRC	
	{   
		EDC = 0xFFFF;   
	} 

	EDC = generateEDC(type, NAD, EDC);
	EDC = generateEDC(type, PCB, EDC);
	EDC = generateEDC(type, length, EDC);
	for (index=0; index<length; index++)
	{
		EDC = generateEDC(type,buffer[index], EDC);
	}
	
	txval=writeByte(NAD, M7816x);	 //NAD
	
	if(txval==ERR_TRANSMIT_PARITY)
	return txval;
	//EDC = generateEDC(type, NAD, EDC);
	
	
	txval=writeByte(PCB, M7816x);   //PCB
	if(txval==ERR_TRANSMIT_PARITY)
	return txval;   
	//EDC = generateEDC(type,PCB,EDC);  
	   
	txval=writeByte(length, M7816x); //Length
	
	if(txval==ERR_TRANSMIT_PARITY)
	return txval;  
	//EDC = generateEDC(type,length,EDC);   
	for (i=0;i<length;i++)   
	{   
		txval=writeByte(buffer[i], M7816x);		
		if(txval==ERR_TRANSMIT_PARITY)
		return txval; 
		//EDC = generateEDC(type,buffer[i],EDC);   
	}   
	if (type == EDC_TYPE_LRC)   
	{   
		txval=writeLastByte(EDC, M7816x);
		if(txval==ERR_TRANSMIT_PARITY)
		return txval;   
	}   
	else
	{   
		txval=writeByte(EDC, M7816x);
		if(txval==ERR_TRANSMIT_PARITY)
		return txval;   
		txval=writeLastByte(EDC>>8, M7816x);
		if(txval==ERR_TRANSMIT_PARITY)
		return txval;   
	}
	return 0;   
} 
/***************************************************************************
* Function: sendBlock(unsigned char NAD,unsigned char PCB,unsigned short length,unsigned char *tbuffer,unsigned char type)  
* Description: sendBlock
* Input: NULL
* Output: NULL
* Return: status value
* Other:  
**************************************************************************/
short receiveBlock(unsigned char *rNAD,unsigned char *rPCB,unsigned char *rLEN,unsigned char *buffer,unsigned char type, M7816_TypeDef* M7816x)
{   
	short retval;   
	short index = 0;   
	unsigned short EDC;   
	short i;   
	unsigned short expectedLength;

	T1RcvPe = 0;  
	   
	retval = readByte(M7816x);   	  									//	Get NAD, PCB, and Length   
	if (retval == ERR_RECEIVE_TIMEOUT)
	return retval; 
	else
	*rNAD = retval;

	retval = readByte(M7816x); 
	if(retval==ERR_RECEIVE_TIMEOUT)
	return retval; 
	else
	*rPCB = retval;  
		
	retval = readByte(M7816x); 
	if(retval==ERR_RECEIVE_TIMEOUT)
	return retval; 
	else
	*rLEN = retval;
	if(retval==0xFF)
	{
		delay_ms_static(35);	
		return ERR_RECEIVE_TIMEOUT; 
	}			
	expectedLength = *rLEN + 1;   								//	Add one to length for LRC	 
	if (type == EDC_TYPE_CRC)  									//	Add additional byte if using CRC  
	expectedLength++;		
	for (i = 0;i < expectedLength;i++)							//	Get all data bytes plus EDC (1 or 2 bytes at end)   
	{   
		retval = readByte(M7816x);   
		if(retval == ERR_RECEIVE_TIMEOUT)
		return retval; 
		else	   
		buffer[index++] = retval;   
	}
	   
	if (type == EDC_TYPE_LRC) 									//	Check the LRC or CRC   
	{   
		EDC = 0;   
		EDC = generateEDC(EDC_TYPE_LRC,*rNAD,EDC);   
		EDC = generateEDC(EDC_TYPE_LRC,*rPCB,EDC);   
		EDC = generateEDC(EDC_TYPE_LRC,*rLEN,EDC);   
		for(i = 0;i < index;i++)   
		{   
			EDC = generateEDC(EDC_TYPE_LRC,buffer[i],EDC);   
		}   
		if(EDC != 0)   
		{   
			return ERR_RECEIVE_LRC;   
		}   
	}   
	else 														//	EDC is CRC	
	{   
		EDC = 0xFFFF;   
		EDC = generateEDC(EDC_TYPE_LRC,*rNAD,EDC);   
		EDC = generateEDC(EDC_TYPE_LRC,*rPCB,EDC);   
		EDC = generateEDC(EDC_TYPE_LRC,*rLEN,EDC);   
		for (i = 0;i < (index - 2);i++)   
		{   
			EDC = generateEDC(EDC_TYPE_CRC,buffer[i],EDC);   
		}   
	   
		if (((EDC >> 8) != buffer[index-2]) ||   
			((EDC & 0xFF) != buffer[index-1]))   
		{   
			return ERR_RECEIVE_CRC;   
		}   
	}
	if (T1RcvPe==0)   
   		return *rLEN;
   	else   
   		return T1RcvPe;
} 

/***************************************************************************
* Function: sendsblockIFSD(unsigned char IFSD)   
* Description: sendsblockIFSD
* Input: IFSD
* Output: NULL
* Return: status value
* Other:  
**************************************************************************/
short sendsblockIFSD(unsigned char IFSD)   
{	  
	unsigned char buffer[1]; 
	unsigned char rNAD,rPCB,rLEN;   
	unsigned char rbuffer[10];   	
   	short retval;
   	short txval;
   	 
	buffer[0] = IFSD;   
	txval=sendBlock(0,0xC1,1,buffer,EDCtype[currentSlot],EMV_M7816);
	if(txval==ERR_TRANSMIT_PARITY)
	return txval; 
		 
	retval=receiveBlock(&rNAD,&rPCB,&rLEN,rbuffer,EDCtype[currentSlot],EMV_M7816);   
	if (retval == ERR_RECEIVE_TIMEOUT)
	{
		return retval;
	}			 	 
	// If we do not get a confirmation response, resent the request once.	
	else if((rPCB == 0xE1) && (rLEN == 1) && (rbuffer[0] == IFSD)&&(rNAD==NAD[currentSlot])&&((retval != ERR_RECEIVE_PARITY)&&(retval != ERR_RECEIVE_LRC)))   
	{
		return 0;   
	}
	else   														//	resent IFSD request once
	{ 
		txval=sendBlock(0,0xC1,1,buffer,EDCtype[currentSlot],EMV_M7816);
		if(txval==ERR_TRANSMIT_PARITY)
		return txval;

		retval=receiveBlock(&rNAD,&rPCB,&rLEN,rbuffer,EDCtype[currentSlot],EMV_M7816);
		if (retval == ERR_RECEIVE_TIMEOUT)
		{
			return retval;
		}
		else if((retval == ERR_RECEIVE_PARITY)|| (retval == ERR_RECEIVE_LRC))	
		{
			txval=sendBlock(0,0xC1,1,buffer,EDCtype[currentSlot],EMV_M7816);
			if(txval==ERR_TRANSMIT_PARITY)
			return txval;

			retval=receiveBlock(&rNAD,&rPCB,&rLEN,rbuffer,EDCtype[currentSlot],EMV_M7816);
			if (retval < 0)
			return retval;
		}
		if ((rPCB == 0xE1) && (rLEN == 1) && (rbuffer[0] == IFSD))   
		return 0; 
		else   
		return ERR_SETIFSD_FAILURE;  
	} 
}   

/***************************************************************************
* Function: void setNAD(unsigned char value)   
* Description: Set Node address info for this card 
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/   
void setNAD(unsigned char value)   
{   
	NAD[currentSlot] = value;   
}   

/***************************************************************************
* Function: short sendAPDUT1(unsigned char *tbuffer,unsigned short length,unsigned char *rbuffer)   
* Description: sendAPDUT1
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/
short sendAPDUT1(unsigned char *buffer,unsigned short length,unsigned char *rbuffer, M7816_TypeDef* M7816x)   
{		  			   
	short retval;
	short txval;   
	unsigned char rNAD,rPCB,rLEN;	
	unsigned char tempbuffer[264];							//	to receive data 
	unsigned char sPCB = sequenceNumber; 
	unsigned char sendRBlockFlag = 0; 						//	used for receive error data for retransmit R Block
	unsigned char sendRBlockData[2];
	volatile  short Count_Err = 0; 
	short chaining = 0;
	short ICCchaining = 0;									//	resent times   		
	unsigned short SegLength = length;  					//	Set by ATR (TA3), or default to 0x20
	unsigned short rapdu_len = 0;
   	 	
	if (length > IFSC[currentSlot])  						//	if terminal chaining
	{
		sPCB = sequenceNumber | 0x20;
		SegLength=IFSC[currentSlot];
		length -= SegLength; 
		chaining = 1;   		
	}

	/* Send block.  Then, we can wait for the receive side. */
	txval=sendBlock(NAD[currentSlot],sPCB,SegLength,buffer,EDCtype[currentSlot], M7816x); 
	if(txval == ERR_TRANSMIT_PARITY)
	return txval;  
	sequenceNumber ^= 0x40;   
	while(1)  												 //	receiving logic
	{ 	 		   		
		retval = receiveBlock(&rNAD,&rPCB,&rLEN,tempbuffer+rapdu_len,EDCtype[currentSlot], M7816x);
		M[currentSlot]=1;	
		if (retval < 0)
		{
			if((retval == ERR_RECEIVE_TIMEOUT)||(retval == ERR_CWT_ERR))	 	//	check timeout and parity error 
			return retval; 	
			if ((retval == ERR_RECEIVE_PARITY)||(retval == ERR_RECEIVE_LRC))
			{			 		
				if(Count_Err==2)
				{
			 		return retval;
				}
				else										//	send R block with errortype=1
				{
		 			if(sendRBlockFlag == 0)
					{
						delay_ms_static(5);
						txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x81),0,buffer,EDCtype[currentSlot], M7816x);
			 			if(txval==ERR_TRANSMIT_PARITY)
						return txval;
						sendRBlockData[0] = NAD[currentSlot];
					   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x81;
						sendRBlockFlag = 1;
					}
					else
					{
						delay_ms_static(5);
						txval=sendBlock(sendRBlockData[0],sendRBlockData[1],0,buffer,EDCtype[currentSlot], M7816x);
			 			if(txval==ERR_TRANSMIT_PARITY)
						return txval;	
					}
				}
				Count_Err++;
			}
		}
		else if(rNAD!=NAD[currentSlot])
		{
			if(++Count_Err==3)
			{
				return ERR_RECEIVE_INVALID;
			}
			else											//	Send R block with error type =2.
			{					   				
				if(sendRBlockFlag == 0)
				{
					delay_ms_static(5);
					txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x82),0,buffer,EDCtype[currentSlot], M7816x);
					if(txval==ERR_TRANSMIT_PARITY)
					return txval;
					sendRBlockData[0] = NAD[currentSlot];
				   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x82;
					sendRBlockFlag = 1;
				}
				else
				{
					delay_ms_static(5);
					txval=sendBlock(sendRBlockData[0],sendRBlockData[1],0,buffer,EDCtype[currentSlot], M7816x);
		 			if(txval==ERR_TRANSMIT_PARITY)
					return txval;
				}
			}		   	 
		} 
		/* receive WTX request, send WTX response accordingly  */
		else if((rPCB==0xC3)&&(rLEN==0x01))
		{	
 			delay_ms_static(5);
 			M[currentSlot]=tempbuffer[rapdu_len];
			/* Send a WTX response */
			txval=sendBlock(0,0xE3,1,tempbuffer+rapdu_len,EDCtype[currentSlot], M7816x);
			if(txval==ERR_TRANSMIT_PARITY)
			return txval;   			   				
		}
		/* receive IFSC request, send IFSC response accordingly */
		else if((rPCB==0xC1)&&(rLEN==0x01)&&(tempbuffer[rapdu_len]!=0xFF)&&(tempbuffer[rapdu_len]>=0x10))
		{
			delay_ms_static(5);
 			IFSC[currentSlot]=tempbuffer[rapdu_len];
			/* Send a IFSC response */
			txval=sendBlock(0,0xE1,1,tempbuffer+rapdu_len,EDCtype[currentSlot], M7816x);
			if(txval==ERR_TRANSMIT_PARITY)
			return txval;   					
		}
		/* receive abort request during terminal chaining, deactivate ICC */
		else if((rPCB==0xC2)&&(rLEN==0x01)&&chaining)
		{
		 	return ERR_OTHER;   			   				
		}
		/* received data is a R block */
		else if(((rPCB & 0xE0)==0x80)&&(rLEN==0x00))
		{  
			/* Check sequence number , receive NAK case */
			if ((((rPCB<<2)&0x40)== (sequenceNumber^0x40))&&(!ICCchaining))   
			{   
				if(++Count_Err==3)
				{
		 			return ERR_RECEIVE_INVALID;
				}
				else
				{
					delay_ms_static(5);
					/* resent the current sequence */
					txval=sendBlock(NAD[currentSlot],sPCB,SegLength,buffer,EDCtype[currentSlot], M7816x);
					if(txval==ERR_TRANSMIT_PARITY)
					return txval;
				} 
   		 	}
			/* receive ACK */
   		 	else if ((((rPCB<<2)&0x40)== sequenceNumber)&& chaining)
			{
				delay_ms_static(5);	
   		 		buffer += SegLength;
   		 		/* if terminal chaining */
   		 		if(length > IFSC[currentSlot])
				{
					sPCB=sequenceNumber|0x20;
					SegLength=IFSC[currentSlot];						  
					length -= SegLength;
					chaining=1;
				}
				else
				{
					sPCB=sequenceNumber;
					SegLength=length;
					chaining=0;
				}
   		 		/* Send block.  Then, we can wait for the receive side */	
				txval=sendBlock(NAD[currentSlot],sPCB,SegLength,buffer,EDCtype[currentSlot], M7816x);
				if(txval==ERR_TRANSMIT_PARITY)
				return txval;
				sequenceNumber ^= 0x40;   
   		 	}
			 /* R block with wrong sequence number */ 
			else 
			{	  
				if(ICCchaining)
				{
					delay_ms_static(5);
					txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x80),0,buffer,EDCtype[currentSlot], M7816x);
					if(txval==ERR_TRANSMIT_PARITY)
					return txval;
				   	sendRBlockData[0] = NAD[currentSlot];
				   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x80;
					sendRBlockFlag = 1;	 									//	chaining must be send correctly
				}
   		 		else
				{				
					if(sendRBlockFlag == 0)
					{
						delay_ms_static(5);
						txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x82),0,buffer,EDCtype[currentSlot], M7816x);
						if(txval==ERR_TRANSMIT_PARITY)
						return txval;
						sendRBlockData[0] = NAD[currentSlot];
					   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x82;
						sendRBlockFlag = 1;	 	
					}
					else
					{
						delay_ms_static(5);
						txval=sendBlock(sendRBlockData[0],sendRBlockData[1],0,buffer,EDCtype[currentSlot], M7816x);
			 			if(txval==ERR_TRANSMIT_PARITY)
						return txval;
					}
				}					 
   		 	}   	 			   				
		}
		/* see if the received data is an I block  */
		else if(((rPCB & 0x80)==0x00)&&(rLEN!=0xFF))
		{
			if (((rPCB & 0x40)== sequenceNumber_ICC)&&(!chaining))
			{	
				/* icc response with chaining */
				if((rPCB & 0x20)==0x20)
				{
					Count_Err=0;					
					sequenceNumber_ICC ^=0x40;
					rapdu_len+=rLEN;
					ICCchaining=1;
					delay_ms_static(5);
					/* send an ACK to signal next I block  */
					txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x80),0,buffer,EDCtype[currentSlot], M7816x);
					if(txval==ERR_TRANSMIT_PARITY)
					return txval;
					sendRBlockData[0] = NAD[currentSlot];
				   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x80;
					sendRBlockFlag = 1;	 										//	chaining must be send correctly
				
				}
				/* valid I block with no chaining */
				else
				{
					Count_Err=0;
					sequenceNumber_ICC ^=0x40;
					rapdu_len+=rLEN;
					ICCchaining=0;
					sendRBlockFlag = 0;
					memcpy(rbuffer,tempbuffer,rapdu_len);  					//	copy the received message to the receive buffer 
					return rapdu_len;  
				}
			}
			else
			{
				if(++Count_Err==3)
				{
		 			return ERR_RECEIVE_SEQUENCENUM;
				}
				/* Send R block with error type =2.*/
				else
				{						
					if(sendRBlockFlag == 0)
					{
						delay_ms_static(5);
						txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x82),0,buffer,EDCtype[currentSlot], M7816x);
						if(txval==ERR_TRANSMIT_PARITY)
						return txval;
						sendRBlockData[0] = NAD[currentSlot];
					   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x82;
						sendRBlockFlag = 1;
					}			 			
					else
					{
						delay_ms_static(5);
						txval=sendBlock(sendRBlockData[0],sendRBlockData[1],0,buffer,EDCtype[currentSlot], M7816x);
			 			if(txval==ERR_TRANSMIT_PARITY)
						return txval;	
					}
				}
			}

		}
		/* see if the received data is an S block (ABORT request)  */
		else if(rPCB ==0xc2)
		{  
			return ERR_OTHER;
		}
		else
		{
			if(++Count_Err==3)
			{
		 		return ERR_RECEIVE_INVALID;
			}
			else
			{
				/* Send R block with error type =2. */
				if(sendRBlockFlag == 0)
				{
					delay_ms_static(5);
					txval=sendBlock(NAD[currentSlot],((sequenceNumber_ICC>>2)|0x82),0,buffer,EDCtype[currentSlot], M7816x);   //2013.3.25
					if(txval==ERR_TRANSMIT_PARITY)
					return txval;
					sendRBlockData[0] = NAD[currentSlot];
				   	sendRBlockData[1] = (sequenceNumber_ICC>>2)|0x82;
					sendRBlockFlag = 1; 
				}
				else
				{
					delay_ms_static(5);
					txval=sendBlock(sendRBlockData[0],sendRBlockData[1],0,buffer,EDCtype[currentSlot], M7816x);
		 			if(txval==ERR_TRANSMIT_PARITY)
					return txval;	
				}							
			}  
		}	  		  	
 	}  
}	 

/***************************************************************************
* Function: short sendAPDUT0(unsigned char *tbuffer,unsigned short length,unsigned char *rbuffer)  
* Description: sendAPDUT0
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/
short sendAPDUT0(unsigned char *buffer,unsigned short length,unsigned char *rbuffer, M7816_TypeDef* M7816x)   
{   
	short index; 
//	short i;  
	short rindex = 0;   
	short val;   
	unsigned char card_INS = buffer[1];
	unsigned char sendFinishFlag = 0;
	short retval;
	unsigned short lc = length; 
	short revLen=0;
	short txval; 
	unsigned long i;

//	delay_ms(20);

	/* set FCR to 50, parity error time =5 */
	//	wrRegister(FCR,0x40);
	M7816x_ConfigRetry(M7816x, 4);
   	   	
   	/*  Write 5 byte command header */
   	for (index = 0;index < 4;)
	{   
		txval=writeByte(buffer[index++], M7816x);
		if(txval==ERR_TRANSMIT_PARITY)
	   	return txval;
	}
	if(length == 4)		//	case 1 apdu
	{
		lc = 0;
		txval=writeLastByte(0x00, M7816x);
		revLen = 0;	
	}
	else
	{	   
		if(length == 5)
		lc = 0;
		else if(length != (buffer[4] +5)) 		//	P3 and length is not equl, make the send length to P3
		lc = buffer[4] +5;
		txval=writeLastByte(buffer[index++], M7816x);
		revLen =  buffer[4];
	}
	if(txval==ERR_TRANSMIT_PARITY)
		return txval;
   
	while (1)   
	{	  	
		/* Get procedure byte */
		retval = readByte(M7816x);   
		if (retval < 0)   
		{   
			return retval;   
		}   
		val = retval;   
		if ((val & 0xFE) == card_INS)   
		{   
			/* ACK, send/receive all remaining bytes  */
			if (index < lc)   
			{   
				for (;index < (lc-1);index++)
				{   
					txval=writeByte(buffer[index], M7816x);
					if(txval==ERR_TRANSMIT_PARITY)
					return txval; 
				}  
				if (index < lc)
				{   
					txval=writeLastByte(buffer[index++], M7816x);
					if(txval==ERR_TRANSMIT_PARITY)
					return txval;
				}  
				sendFinishFlag = 1;				  
			}   
			else   
			{   
				/* Read bytes up to Licc/P3 */
				while(revLen--) 
				{
					if((retval = readByte(M7816x)) >= 0)
			   	 	rbuffer[rindex++] = retval;
			   	 	else
					return retval;
				}
			}   
		}   
		else if (val == ((~card_INS) & 0xFF))   
		{   
			if (index < lc)   
			{   
				/* ACK, send/receive one remaining byte */
				if (index < lc)   
				txval=writeLastByte(buffer[index++], M7816x);
				if(txval==ERR_TRANSMIT_PARITY)
				return txval;   
				/* NOTE: Does not support VPP state change */
			}   
			else   
			{   
				/* Read one byte or timeout */
				if(revLen--) 
				{
					if((retval = readByte(M7816x)) >= 0)
			   	 		rbuffer[rindex++] = retval;
			   	 	else  
				   	 	return retval;
				}	
			}   
		}   
		else if (val == 0x60)  
		{   
		} 
		else if (val == 0x61)   
		{   
			/* SW1, get SW2; */
			val = readByte(M7816x);   
			if (val < 0) 
			return val; 

			card_INS=0xc0;
			lc=0;			
   			/* write get response to buffer */
   			buffer[0]=0x00;
   			buffer[1]=0xc0;
   			buffer[2]=0x00;
   			buffer[3]=0x00;
   			buffer[4]=val;
   			revLen = val; 

			/* Write get response command */
			for (i = 0;i < 4;)
			{   
				txval=writeByte(buffer[i++], M7816x);
				if(txval==ERR_TRANSMIT_PARITY)
				return txval;
			}   
			txval=writeLastByte(buffer[i++], M7816x);
			if(txval==ERR_TRANSMIT_PARITY)
			return txval; 						
		} 
		else if (val == 0x6C)   
		{   
			/* SW1, get SW2; */ 
			val = readByte(M7816x);   
			if (val < 0) 
			return val; 

			/* let P3=Licc */
			buffer[4]=val;
			revLen = val;					 
			lc=0;  
			rindex = 0x00;
			 
			/* Send APDU command with P3=Licc */
			for (i = 0;i < 4;)
			{   
				txval=writeByte(buffer[i++], M7816x);
				if(txval==ERR_TRANSMIT_PARITY)
				return txval;
			}   
			txval=writeLastByte(buffer[i++], M7816x);
			if(txval==ERR_TRANSMIT_PARITY)
			return txval;   							  
		}  
		else if (((val & 0xF0) == 0x60) || ((val & 0xF0) == 0x90))   
		{   
			/* SW1, get SW2	*/
			rbuffer[rindex++]=val;   
			val = readByte(M7816x);   

			if (val < 0) 
			return val; 

			rbuffer[rindex++]=val; 

			// the following code for 6xxx or 9xxx
			if((rbuffer[0] != 0x62) && (rbuffer[0] != 0x63) && (rbuffer[0] != 0x9F) ) 
			break;

			if((rbuffer[0] == 0x62)&&(rbuffer[1] == 0x83 ) && (card_INS== 0xA4))
			{
				val = 0x00;
			}
			else if((rbuffer[0] == 0x63)&&(rbuffer[1] == 0x35 )&& (card_INS== 0xA4))
			{
				val = 0x00;
			}
			else if((rbuffer[0] == 0x9F)&&(rbuffer[1] == 0xFF )&& (card_INS== 0xA4) && (sendFinishFlag ==1))
			{
				val = 0x00;
			}
			else 
			break;
							
			card_INS=0xc0;	 			// send get response command
			lc=0;			
   			
			/* write get response to buffer */
   			buffer[0]=0x00;
   			buffer[1]=0xc0;
   			buffer[2]=0x00;
   			buffer[3]=0x00;
   			buffer[4]=0x00;
   			revLen = 0x00; 
			rindex = 0x00;	   
   			
			/* Write get response command */
			for (i = 0;i < 4;)
			{   
				txval=writeByte(buffer[i++], M7816x);
				if(txval==ERR_TRANSMIT_PARITY)
				return txval;
			}   
			txval=writeLastByte(buffer[i++], M7816x);
			if(txval==ERR_TRANSMIT_PARITY)
			return txval; 
		}
		else if(val == 0xFF)
		{
			val = readByte(M7816x); 
			return ERR_RECEIVE_INVALID;;
		}
		else
		{		
			retval = ERR_RECEIVE_INVALID;
			return retval;
		}   
	}   
	return rindex;   
}	
/***************************************************************************
* Function: short m7816APDU(unsigned char *tbuffer,unsigned short length,unsigned char *rbuffer)   
* Description: m7816APDU
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/
short sendAPDU(unsigned char *buffer,unsigned short length,unsigned char *rbuffer)   
{   
	short retvalue;
	switch (TMode[currentSlot])   
	{   
		case 0:   
			retvalue = sendAPDUT0(buffer,length,rbuffer, EMV_M7816);   
			break;   
		case 1:   
			retvalue = sendAPDUT1(buffer,length,rbuffer, EMV_M7816);   
			break;   
		default:   
			retvalue = ERR_PROTOCOL_UNSUPPORTED;   
			break;   
	}
	return retvalue;   
} 
  
/***************************************************************************
* Function: short warmreset(unsigned char mode)  
* Description: warmreset
* Input: mode 0-ISO mode; 1- EMV mode
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/  
short warmreset(unsigned char mode)   
{
	RST_COLD = 0;
	/* Check for power status */  
	if (M7816x_GetActiveCardStatus(EMV_M7816) == RESET)
	{
		return ERR_POWERUP_VOLTAGE_INVALID;   
	}
	/* Apply reset */
	M7816x_ConfigReset(EMV_M7816, M7816x_RST_LOW_LEVEL);
	/* Call common getATR routine */
	return ATRsequence(mode, EMV_M7816);   
}   

/***************************************************************************
* Function: short powerup(unsigned char mode, unsigned char voltage)   
* Description: powerup or code reset
* Input: mode 0-ISO mode; 
			  1- EMV mode
		 voltage EMV7816M2_VOLTAGE_5V - 5V
				 EMV7816M2_VOLTAGE_3V - 3V
				 EMV7816M2_VOLTAGE_1V8 - 1.8V
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/	
short powerup(unsigned char mode, unsigned int voltage)   
{
	RST_COLD = 1;

	EMV7816VCCTRIM |= 0x07;//trim value is 5.21V
	EMV7816M2_ConfigElectronic(EMV7816M2_IO_ELECTRONIC, EMV7816M2_TIMING_SOFTWARE); 
	
	EMV7816M2_ConfigVoltage(voltage);
	
	EMV7816M2->EMVCON |= Bit3_En;// pull-up(11K)
	M7816x_ConfigPullUpAfterParity(EMV_M7816, ENABLE);
	
	M7816x_ConfigClkOutput(EMV_M7816, DISABLE);//clock oupput
	/* Deactive Card */
	EMV_ConfigActiveCard(EMV_M7816, M7816x_POWER_DOWN);
	M7816x_ConfigReset(EMV_M7816, M7816x_RST_LOW_LEVEL);

	/* read 7816M0_PRE change or not */
	if((M7816x_GetHSTS(EMV_M7816, M7816x_HSTS_PRL)) != RESET)
	{
		M7816x_ClearHStatus(EMV_M7816, M7816x_HSTS_PRL);
	}
//	delay_ms_static(3);
	
	/* Power the card, RST low, C4 and C8 high */
	EMV_ConfigActiveCard(EMV_M7816, M7816x_POWER_UP);
	//TMC(20240327)-zt:将CLK操作移植到函数EMV_ConfigActiveCard()中--同步已过检代码
//	delay_ms(1);
//	M7816x_ConfigClkOutput(EMV_M7816, ENABLE);//clock oupput
//	delay_ms(9);//40000~45000 clock cycle; CLK is 4.8MHZ
	M7816x_ReciveData(EMV_M7816);  //clear RBF
	M7816x_ConfigReset(EMV_M7816, M7816x_RST_HIGH_LEVEL);
	
	/* read 7816M0_PRE change or not */
	if((M7816x_GetHSTS(EMV_M7816, M7816x_HSTS_PRL)) != RESET)
	{		
		powerdown();
		M7816x_ClearHStatus(EMV_M7816, M7816x_HSTS_PRL);		
		return ERR_POWERUP_INTERRUPTED;   
	} 
   
//	/* clear status TO3、TO2、TO1 bit */
//	M7816x_ClearUStatus(EMV_M7816, M7816x_USTS_TO1|M7816x_USTS_TO2|M7816x_USTS_TO3);	

	/* Call common getATR routine */
	return ATRsequence(mode, EMV_M7816);   
}   
/***************************************************************************
* Function: short ATRsequence(unsigned char mode)   
* Description: get ATR
* Input: mode 0-ISO mode; 
			  1- EMV mode
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/ 
short ATRsequence(unsigned char mode, M7816_TypeDef* M7816x)   
{   
	unsigned char val;	
	unsigned char index;   
	unsigned char etucount = 0;   
	unsigned short i;   
	unsigned char historicalBytes = 0;   
	unsigned char expectedCharacters = 0;   
	unsigned char interfaceIteration = 1;   
	unsigned char done = 0;   
	unsigned char check = 0;   
	unsigned char T = 0;	//Default to T=0 mode
	unsigned char curByte; 
	unsigned char TD2_exist = 0; 
	unsigned char T_temp;  
	unsigned char isHigherRates = 0;// TA1 != 0x11/0x12/0x13.
//	unsigned short f[10] = {372,372,558,744,1116,1448,2232,372,372,512};
	unsigned char d[7] = {1,1,2,4,8,16,32};
	unsigned char ERR_ATR = 0;
	
	MemorySet(workingBuffer, 0x00, sizeof(workingBuffer));
	clearATRStruct(&lastATR[currentSlot]);   
	NAD[currentSlot] = 0;   
	sequenceNumber = 0;
	sequenceNumber_ICC = 0;
	
	/*set default value for F & D */
	F[currentSlot] = 1;
	D[currentSlot] = 1;
	
	/* Set T1 default values for CWT and BWT, ISO7816 11.2 and 11.4.3 */
	CWT[currentSlot] = 11 + (1 << 13);   
	BWT[currentSlot] = 11 + (1 << 4) * 960;  

	/* Reset the ISO7816 */
	M7816x_SoftReset(M7816x, ENABLE);   
	//TMC(20240327)-zt:增加延时-同步已过检代码
	delay_us(5);
	
	/* Remove ISO7816 reset */
	M7816x_SoftReset(M7816x, DISABLE);  
	 
	/* Set retry number to 0, no retry */
	M7816x_ConfigRetry(M7816x, 0); 	

	/* Set SS bit to hardware check  direct convention or inverse convention */
	M7816x_ConfigEncodingRuleDetectionEnable(M7816x, ENABLE);
	
	/* Set baud divisor */
	M7816x_ConfigBaudRate(M7816x, 0x11);
	
	M7816x_ClearUStatus(M7816x, M7816x_USTS_TO1|M7816x_USTS_TO2|M7816x_USTS_TO3); 
	if(RST_COLD)
	{
		
	}
	else
	{
		M7816x_ClearUStatus(M7816x, M7816x_USTS_TO3);
		M7816x_ConfigReset(EMV_M7816, M7816x_RST_LOW_LEVEL);
		M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);
		M7816x_ConfigTOR(M7816x,(0x6C00));
		M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE3);
		while((M7816x_GetUSTS(M7816x, M7816x_USTS_TO3)) == RESET)
		{};
		M7816x_ClearUStatus(M7816x, M7816x_USTS_TO3);		   
		M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);
		//TMC(20240327)zt:move CLK-Output into Function void EMV_ConfigActiveCard(M7816_TypeDef* M7816x,uint32_t Power_Swith)
		delay_us(5);
		M7816x_ConfigReset(EMV_M7816, M7816x_RST_HIGH_LEVEL);
			
	}
	/* clear TOL3 bit */
	//M7816x_ConfigActiveCard(EMV_M7816, M7816x_POWER_UP);
	/* Release reset */
	M7816x_ClearUStatus(M7816x, M7816x_USTS_PE|M7816x_USTS_FER|M7816x_USTS_EA); 
	M7816x_ClearUStatus(M7816x, M7816x_USTS_TO3); 
	if (mode == POWERUP_ISO)   
	{   
		/* Wait up to 40000 cycles for ATR to start */
		M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);
		M7816x_ConfigTOR(M7816x,0x00|(0x78 << 8)|(0x00 << 16));
		M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE3); 
	}   
	else   
	{   
	 /* Wait up to 42000 cycles for ATR to start and 19200 etu counter after first byte */
		M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);
//		M7816x_ConfigTOR(M7816x,0xFF|(0xC0<<8)|(0x00<< 16));
		//0xC0->0x73(115 etus),if time >43000 clock, thm36 should be power down.
		//0x73->0x7C(124 etus),Some of Electrical Test Case Failed when 0x73.
		M7816x_ConfigTOR(M7816x,0xFF |(0x7C<<8)|(0x00<< 16));
		//TMC(20240327)-zt:增加延时-同步已过检代码
		M7816x_ConfigTOR(M7816x,0xFF|(0x7D<<8)|(0x00<< 16));//LY 20191107
		
		M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE4);
	}
	 
	/* clear status flag */
	
   	/* waiting for receive buffer full */
	while (1)   
	{   
		/* If we see the first character come in, break. */
		if (M7816x_GetMSTS(M7816x, M7816x_MSTS_RBF) == SET)
		{
			M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO2_TO3);
//			//Inter-Character Timing,Max:10080
			M7816x_ConfigTOR(M7816x,0xFF|(0x61<< 8)|(0x27<< 16));
//			M7816x_ConfigTOR(M7816x,0xFF|(0x5C<< 8)|(0x27<< 16));//Inter-Character Timing,Max:10074(0x275C)
			M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE4);	
		 
			break;   
		}
   		/* If we see error flag be set */
		if (M7816x_GetUSTS(M7816x, M7816x_USTS_PE)) 
		{   
			M7816x_ClearUStatus(M7816x, M7816x_USTS_PE); 
			return ERR_POWERUP_ATR_EA;	
		}   
		if (M7816x_GetUSTS(M7816x, M7816x_USTS_FER)) 
		{   
			M7816x_ClearUStatus(M7816x, M7816x_USTS_FER); 
			return ERR_POWERUP_ATR_EA;	
		}	
		if (M7816x_GetUSTS(M7816x, M7816x_USTS_EA)) 
		{   
			M7816x_ClearUStatus(M7816x, M7816x_USTS_EA); 
			return ERR_POWERUP_ATR_EA;	
		}		
		/*if TO2 and TO3 overflow	*/
		if (M7816x_GetUSTS(M7816x, M7816x_USTS_TO3))   
		{	
			
			M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO2_TO3);
			M7816x_ClearUStatus(M7816x, M7816x_USTS_TO3);
			return ERR_POWERUP_ATR_TIMEOUT;   
		}   
	}	 
	index = 0;
	
	while (!done)   
	{	 
		/* If receive buffer full */
		if (M7816x_GetMSTS(M7816x, M7816x_MSTS_RBF) == SET)   
		{   
			/* Read and store ATR byte */
			val = M7816x_ReciveData(M7816x);  
			M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO2_TO3);
			//M7816x_ConfigTOR(M7816x,0xFF|(0x90<< 8)|(0x27<< 16));

			/* 0x2780->0x2790, 1703.DTS22 is OK.*/
			M7816x_ConfigTOR(M7816x,0xFF|(0xA0<< 8)|(0x27<< 16));
			M7816x_ConfigTOC(M7816x, M7816x_TIM_MODE4);
		  
			if(done)  
			{			 	
				return ERR_POWERUP_ATR_INVALID; 
			}


			curByte = val;   
			workingBuffer[index++] = val;  
			/* store The initial character TS */
			if (index == 1)  
			{   
				lastATR[currentSlot].TS = curByte;   
				if ((curByte != 0x3f) && (curByte != 0x3b))   
				{	 
					return ERR_POWERUP_ATR_TS;   
				}   
				check = 0;   
				
			}   
			else   
			{
				check ^= curByte; 							//	compute the last byte TCK for T=1
			}
			/* store The T0 */	  
			if (index == 2) 			 
			{   
				historicalBytes = curByte & 0x0F;   
				expectedCharacters = curByte & 0xF0;
				/* see if TB1 exists */
				if (((curByte & 0x20) !=0x20) && RST_COLD)  //	TB1 is deprecated 
					if ((curByte & 0x20) != 0x20)
					{
						ERR_ATR = 1;
						if(curByte == 0Xc0)
							ERR_ATR = 0;
					}
				lastATR[currentSlot].T0 = curByte;   
			}  
			
			/* store The TAi,TBi,TCi,TDi... */
			if (index > 2)   
			{   
				switch(expectedCharacters)   
				{   
					case 0x00:   
						/* Historical characters */
						historicalBytes--;   
						lastATR[currentSlot].Historical[lastATR[currentSlot].HistoricalLength++] = curByte;   
						if (historicalBytes == 0)   
						{   
							if ((T==0)&&(!TD2_exist))   		//	T=0 but TD2 exist,Go to checksum state
							{   
								done = 1;   
							}   
							else   
							expectedCharacters = 0x01;  	//	Go to checksum state	
						}   
					break;   
					case 0x01:   
					/* TCK case */
					lastATR[currentSlot].TCK = curByte;   
					done = 1;   
					break;   
					case 0x10:   
					case 0x30:   
					case 0x50:   
					case 0x70:   
					case 0x90:   
					case 0xB0:   
					case 0xD0:   
					case 0xF0:   
						/* TA case */
						expectedCharacters &= 0xE0;   
						if(mode == POWERUP_EMV)
						{	
							if(interfaceIteration == 1)
							{								
								//<IFM Protocol Test Case> Page27, TA1 having any value and continues using the default values of D=1 and F=372, during all subsequent exchanges.
								if((curByte!=0x11)&&(curByte!=0x12)&&(curByte!=0x13))
								{
									//ERR_ATR = 1;  
									isHigherRates = 1;								   
								}
														   
							}
							if(interfaceIteration == 2)
							{
								if(curByte&0x10)
									ERR_ATR = 2;
							}
							if(interfaceIteration == 3)
							{
								if((curByte<0x10)||(curByte>0xfe))
								ERR_ATR = 3;
							}
						}
						lastATR[currentSlot].TA[interfaceIteration] = curByte;   
					break;   

					case 0x20:   
					case 0x60:   
					case 0xA0:   
					case 0xE0:   
						/* TB case */
						expectedCharacters &= 0xD0;
						if(interfaceIteration == 2)  
						{
							if((curByte == 0x01) && (lastATR[currentSlot].TB[1] == 0x00) && (lastATR[currentSlot].TD[1] != 0xA1))
								ERR_ATR = 1;
						}
						if(interfaceIteration == 1)
						{
//							if(curByte && RST_COLD)	
//								ERR_ATR=1;	
						}
						if(interfaceIteration == 3)
						//if(((curByte&0xf0)>0x40)||((curByte&0x0f)>0x05)||((lastATR[currentSlot].TC[1]!=0xff) && (lastATR[currentSlot].TC[1]!=0xffff) && ((1<<(curByte&0x0f))<=lastATR[currentSlot].TC[1]+1)))	
						  if(((curByte&0xf0)>0x40)||((curByte&0x0f)>0x05)||((lastATR[currentSlot].TC[1]!=0xff) && (lastATR[currentSlot].TC[1]!=0xffff) && ((1<<(curByte&0x0f))< lastATR[currentSlot].TC[1]+1)))
//                        if(((curByte & 0xf0) > 0x40) || \
//						   ((curByte & 0x0f) > 0x05) ||(curByte==0x00))//2023.4.14		 
						{
							ERR_ATR = 1;

						}
						lastATR[currentSlot].TB[interfaceIteration] = curByte;   
					break;   
					case 0x40:   
					case 0xC0:   
						/* TC case */
						expectedCharacters &= 0xB0;  
						//if((interfaceIteration == 2)&&(curByte!=0x0a))
						/* EMV Options->protocol: TC2 should support  0x09 & bigger than 0x0A, 1710.DTS20 is OK*/
						if(interfaceIteration == 2)   
						{													   
							if ((curByte != 0x01) && (curByte != 0x09) && (curByte != 0x0A) && (curByte != 0x0B)) 
//							if ( (curByte == 0x01) || (curByte == 0x09) || (curByte == 0x0B)|| (curByte == 0x00))	 //For 1707.10 1707.26 1707.30 1707.31 //2023.4.14
							{
								ERR_ATR = 7;
							}
						}													
						if((interfaceIteration == 3)&&((curByte==0xff)||(curByte==0x1)))
							ERR_ATR = 8;		
						lastATR[currentSlot].TC[interfaceIteration] = curByte;   
					break;   
					case 0x80:   
					/* TD case */
					expectedCharacters=(curByte&0xF0);						
					lastATR[currentSlot].TD[interfaceIteration] = curByte; 

					/* If we get TD1, we have the first protocol selection */
					T_temp = curByte & 0x0F;
					if (interfaceIteration == 1)   
					{   
						if(((curByte&0x0f)!=0x00)&&((curByte&0x0f)!=0x01))
							ERR_ATR = 9;
						if((T_temp==1)||(T_temp==0))
							T = T_temp;   
					}   
					else   
					{   
						/* Changing protocols is only valid under ISO (not allowed in EMV) */
						if (mode == POWERUP_ISO)   
						{	  
							T = T_temp;	
						}   
						else 
						{  
							if ((T_temp != 1) && (T_temp != 0x0e))
							{
								ERR_ATR = 10;
							}
							if((T!=0)&&(T_temp == 0x0e))
							ERR_ATR = 11;
							TD2_exist = 1;					//	TD2 exist，TCK will be checked
						}  
					}   
					interfaceIteration++;   
					break;
					default:
					ERR_ATR = 12;
					break;   
				}   
			}   
			
		}
	
		if ((index > 2) && (expectedCharacters == 0x00) && (historicalBytes == 0))   
		{ 
			if ((T==0)&&(!TD2_exist))   					//	T=0 but TD2 exist,Go to checksum state
			{   
				done = 1;   
			}   
			else   
				expectedCharacters = 0x01;  				//	Go to checksum state
		} 
		//TMC(20240327)-zt:增加判断-同步已过检代码
	  if ((T==0) && (lastATR[currentSlot].TC[1]!=0xFFFF))   					//	T=0 but TD2 exist,Go to checksum state
	  {   
	      N = lastATR[currentSlot].TC[1];   
	  }  	  
	   
		/* Read Timer status */		
		if (M7816x_GetUSTS(M7816x, M7816x_USTS_TO3))   
		{	 
		  //delay_ms_static(350);
			M7816x_ClearUStatus(M7816x, 0xFF); 
			return ERR_POWERUP_ATR_TIMEOUT;   
		}   
		if (M7816x_GetUSTS(M7816x, M7816x_USTS_PE|M7816x_USTS_FER))   
		{	
			M7816x_ClearUStatus(M7816x, M7816x_USTS_PE|M7816x_USTS_FER); 
			return ERR_RECEIVE_PARITY;   
		}	 
		if ((mode == POWERUP_EMV) && (M7816x_GetUSTS(M7816x, M7816x_USTS_TO1)))   
		{   
			/* If 192 ETU rollover, increment total count */
			etucount++;   
			/* If we exceed 192 * 100 or 19200 etu, we fail as the whole ATR has not come */
			if (etucount >= 80)   
			{	
				M7816x_ClearUStatus(M7816x, 0xFF); 
				M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO2_TO3);

				return ERR_POWERUP_ATR_TIMEOUT;	   
			} 
			M7816x_ClearUStatus(M7816x, M7816x_USTS_TO1);	
		}   
	}
 	/*	stop timer */
	M7816x_StopTOC(M7816x, M7816x_TIM_STOP_TO1_TO2_TO3);   

	//if TA2 absent and TA1 != 0x11/0x1/0x13, it's error ATR.
	if((ERR_ATR) || (!(lastATR[currentSlot].TA[2] & Bit5_En) && isHigherRates))
	{
		return ERR_POWERUP_ATR_INVALID;
	}
	/* If we are in T=1 protocol, we get a LRC (xor checksum) at the end */
	if (T==1)   
	{   
		if (check != 0)   
		{
			return ERR_POWERUP_ATR_CRC_FAILURE;
		}
		if(lastATR[currentSlot].TB[3] == 0xffff)
		{
			return  ERR_POWERUP_ATR_INVALID;
		}
	}
  
	/* Set ATR length for the ATR received */
	ATRLength[currentSlot] = index;
	   
	/* Set T protocol mode */
	TMode[currentSlot] = T;   
   
	/* Set  mode according to T value */
	if (T == 0)
	{
		M7816x_ConfigProtocol(M7816x, M7816x_PROTOCOL_T0);   
	}
	else
	{
		M7816x_ConfigProtocol(M7816x, M7816x_PROTOCOL_T1);
	}
	/* Set extra guard time if present in TC1, else use guard time of 0 */
	if (lastATR[currentSlot].TC[1] != 0xffff)
	{
#ifdef _SOFT_TEST_	  
		M7816x_ConfigGuardTime(M7816x, 0);   
#else
		M7816x_ConfigGuardTime(M7816x, lastATR[currentSlot].TC[1]); 
#endif
		//..M7816x_ConfigGuardTime(M7816x, 0);
	}
	else
	{
		M7816x_ConfigGuardTime(M7816x, 0x00);  				
	}
	/* Set the EDC type (either LRC or CRC) */
	EDCtype[currentSlot] = EDC_TYPE_LRC;  			// Default to LRC if not told otherwise 
	for (i = 3;i < 8;i++) 					  
	{   
		if (lastATR[currentSlot].TC[3] !=0xffff)   
		{	
			EDCtype[currentSlot] = (lastATR[currentSlot].TC[3] & 0x01) ? EDC_TYPE_CRC : EDC_TYPE_LRC;   
		}   
	}   
   
	/* Set IFSC (max segment size of card) if TA3 present (only used for T=1 protocol)  */
	if (lastATR[currentSlot].TA[3] != 0xffff)   
	{   
		val = lastATR[currentSlot].TA[3];		
		IFSC[currentSlot] = val;   
	}   
	else   
		IFSC[currentSlot] = 0x20;  					// Default for IFSC is 0x20 according to EMV 4.1 8.3.3.9 and ISO7816 9.5.2.1   
   
	/* set F&D */
 	if(!(lastATR[currentSlot].TA[2] & 0x10))
	{
		if(lastATR[currentSlot].TA[1] !=0xffff)					   
		{
			F[currentSlot] = (lastATR[currentSlot].TA[1]&0xf0)>>4;
			D[currentSlot] = (lastATR[currentSlot].TA[1]&0x0f);
			/* set baud rate */
			M7816x_ConfigBaudRate(M7816x, lastATR[currentSlot].TA[1]);
		}	
	} 
	
	/* Set WWT if present in TC2 (only used for T=0), ISO7816 8.2 */
	if (lastATR[currentSlot].TC[2] != 0xffff)   
		WWT[currentSlot] = lastATR[currentSlot].TC[2] * 960*d[D[currentSlot]];   
	else   
		WWT[currentSlot] = 960 * 10*d[D[currentSlot]];   
   
	/* set CWT and BWT */
	for (i = 3;i < 8;i++)   
	{   
		/* Set CWT and BWT if present in TBi */
		if (lastATR[currentSlot].TB[i] != 0xffff)   
		{   
			val = lastATR[currentSlot].TB[i] & 0x0F;   
			CWT[currentSlot] = 12 + (1 << val);  
			val = (lastATR[currentSlot].TB[i] & 0xF0) >> 4;	 
			BWT[currentSlot] = 11 + (1 << val)*960*d[D[currentSlot]];	
			break;   
		}   
	}  

	/* clear status */
	M7816x_ClearUStatus(M7816x, 0xFF);
	return 0;	
}  

/***************************************************************************
* Function: unsigned char powerdown(void)	
* Description: power down
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/	
unsigned char powerdown(void)   
{	
	/* Power down the card */
	unsigned long i;
//	for(i=0;i<0x70000;i++);  
	delay_ms_static(24);
	EMV_ConfigActiveCard(EMV_M7816, M7816x_POWER_DOWN);

	//add reset,Modified 2019.10.29.
	M7816x_DeInit(EMV_M7816);
	for(i=0;i<0x70000;i++); 


	return 0;   
}   
 
/***************************************************************************
* Function: unsigned char getATRbuffer(unsigned char *buff, unsigned char length)	
* Description: get ATR buffer
* Input: buff- buffer used to store ATR
		 length - buffer length
* Output: 
* Return: ATR length 	
* Other:  
**************************************************************************/	 
unsigned char getATRbuffer(unsigned char *buff, unsigned char length)   
{   
	memcpy(buff, workingBuffer, (length<ATRLength[currentSlot])?length:ATRLength[currentSlot]);   
	return ATRLength[currentSlot];   
}   

/***************************************************************************
* Function: unsigned char getATRbuffer(unsigned char *buff, unsigned char length)	
* Description: get ATR info
* Input: userATR- struct used to store ATR
* Output: 
* Return: 	
* Other:  
**************************************************************************/   
void getATR(struct EMVATR *userATR)   
{   
	memcpy((unsigned char*)userATR,(unsigned char*)&lastATR[currentSlot],sizeof(struct EMVATR));   
}   
/***************************************************************************
* Function: void delay_etus(unsigned short etus)	 
* Description: 	delay N etus
* Input: 
* Output: 
* Return: 	
* Other:  
**************************************************************************/   
void delay_etus(unsigned short etus)   
{
	M7816x_StopTOC(EMV_M7816, M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ConfigTOR(EMV_M7816,0x00|((etus&0xFF) << 8)|((etus>>8) << 16));
	M7816x_ConfigTOC(EMV_M7816, M7816x_TIM_MODE3);
	
	while(M7816x_GetUSTS(EMV_M7816, M7816x_USTS_TO3) == RESET);

	/* clear TOL3 bit */
	M7816x_ClearUStatus(EMV_M7816, M7816x_USTS_TO3);
	
	/* stop TOC */
	M7816x_StopTOC(EMV_M7816, M7816x_TIM_STOP_TO1_TO2_TO3);
}   
/***************************************************************************
* Function: void MemorySet(unsigned char* pBuff, unsigned char val, unsigned short len)	
* Description: set pBuff len val
* Input: val len
* Output: pBuff
* Return: 	
* Other:  
**************************************************************************/   
void MemorySet(unsigned char* pBuff, unsigned char val, unsigned short len)
{
	unsigned short i;
	for (i = 0; i < len; i++)
	{
		pBuff[i] = val;
	}	
}
