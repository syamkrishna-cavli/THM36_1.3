/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ISO7816M_HAL.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

#ifndef __ISO7816M_HAL_H_
#define __ISO7816M_HAL_H_

#include "iso7816M.h"

/* Register definitions */
//#define ISO7816M0_BASE_ADDR	0x40120000L
//#define  ISO7816M_ADDR 	 ISO7816M0_BASE_ADDR

////ISO7816M0CSEL
//#define CSR	 (ISO7816M_ADDR+0x00)
////ISO7816M0CKCON
//#define CCR	 (ISO7816M_ADDR+0x40) 
////ISO7816M0PDR
//#define PDR	 (ISO7816M_ADDR+0x30) 
////ISO7816M0UCON2
//#define UCR2	(ISO7816M_ADDR+0x3c) 
////ISO7816M0GTR
//#define GTR	 (ISO7816M_ADDR+0x34) 
////ISO7816M0UCON1
//#define UCR1	(ISO7816M_ADDR+0x38) 
////ISO7816M0PWCON
//#define PCR	 (ISO7816M_ADDR+0x44) 
////ISO7816M0TOC
//#define TOC	 (ISO7816M_ADDR+0x10) 
////ISO7816M0TOR
//#define TOR1	(ISO7816M_ADDR+0x0c) 
////ISO7816M0TOR
//#define TOR2	(ISO7816M_ADDR+0x0d) 
//// ISO7816M0TOR
//#define TOR3	(ISO7816M_ADDR+0x0e) 
////ISO7816M0MSTS
//#define MSTSR	 (ISO7816M_ADDR+0x20) 
////ISO7816M0FFCON
//#define FCR	 (ISO7816M_ADDR+0x1c) 
////ISO7816M0TBUF
//#define UTR	 (ISO7816M_ADDR+0x14) 
////ISO7816M0RBUF
//#define URR	 (ISO7816M_ADDR+0x18) 
////ISO7816M0USTS
//#define USR	 (ISO7816M_ADDR+0x24) 
////ISO7816M0HSTS
//#define HSR	 (ISO7816M_ADDR+0x04) 

/* GPIO define */
#define VCC_SEL1		3
#define VCC_SEL2 		56

#define CLK_DIV1		2
#define CLK_DIV2 		1
 
/* Structure for holding ATR information. */ 
struct EMVATR 
{ 
  unsigned char TS; 
  unsigned char T0; 
  unsigned short TA[8]; 
  unsigned short TB[8]; 
  unsigned short TC[8]; 
  unsigned short TD[8]; 
  unsigned char HistoricalLength; 
  unsigned char Historical[15]; 
  unsigned short TCK; 
};


/* Number of cards supported (used for sizing data structures) */
/* this parameter must be set to 1 */
#define CARD_SLOTS  1 
 
/* Register bit definitions */
#define CSR_ID_MASK		 0xF0 
#define CSR_nRIU_MASK	   0x08 
#define CSR_SC3_MASK		0x04 
#define CSR_SC2_MASK		0x02 
#define CSR_SC1_MASK		0x01
#define CSR_PUP_MASK		0x02
#define CSR_UARTEN_MASK	 0x01 
 
#define HSR_PRTL2_MASK	  0x40 
#define HSR_PRTL1_MASK	  0x20 
#define HSR_SUPL_MASK	   0x10 
#define HSR_PRL2_MASK	   0x08 
#define HSR_PRL1_MASK	   0x04 
#define HSR_INTAUXL_MASK	0x02 
#define HSR_PTL_MASK		0x01 
 
#define MSR_CLKSW_MASK	  0x80 
#define MSR_FE_MASK		 0x40 
#define MSR_BGT_MASK		0x20 
#define MSR_CRED_MASK	   0x10 
#define MSR_PR2_MASK		0x08 
#define MSR_PR1_MASK		0x04 
#define MSR_RBF_MASK	 	0x02 
#define MSR_TBE_MASK		0x01 
 
#define FCR_PEC_MASK		0x70 
#define FCR_FL_MASK		 0x07 
 
#define USR_TOL3_MASK	   0x80 
#define USR_TOL2_MASK	   0x40 
#define USR_TOL1_MASK	   0x20 
#define USR_EA_MASK		 0x10 
#define USR_PE_MASK		 0x08 
#define USR_OVR_MASK		0x04 
#define USR_FER_MASK		0x02 
#define USR_TACT_MASK	   0x01 
 
#define UCR1_FIP_MASK	   0x40 
#define UCR1_PROT_MASK	  0x10 
#define UCR1_T_R_MASK	   0x08 
#define UCR1_LCT_MASK	   0x04 
#define UCR1_SS_MASK		0x02 
#define UCR1_CONV_MASK	  0x01 
 
#define UCR2_DISTBE_RBF_MASK 0x40 
#define UCR2_DISAUX_MASK	0x20 
#define UCR2_PDWN_MASK	  0x10 
#define UCR2_SAN_MASK	   0x08 
#define UCR2_nAUTOCONV_MASK 0x04 
#define UCR2_CKU_MASK	   0x02 
#define UCR2_PSC_MASK	   0x01 
 
#define CCR_SHL_MASK		0x20 
#define CCR_CST_MASK		0x10 
#define CCR_SC_MASK		 0x08 
#define CCR_AC_MASK		 0x07 
 
#define PCR_C8_MASK		 0x20 
#define PCR_C4_MASK		 0x10 
#define PCR_1V8_MASK		0x08 
#define PCR_RSTIN_MASK	  0x04 
#define PCR_3V_5V_MASK	  0x02 
#define PCR_START_MASK	  0x01 
 
/* Power up modes */
#define POWERUP_ISO	 0 
#define POWERUP_EMV	 1 
 
/* Power up voltages */ 
#define POWERUP_5V	  0 
#define POWERUP_3V	  1 
#define POWERUP_1p8V	2 
 
/* Types of ERC */
#define EDC_TYPE_LRC	0 
#define EDC_TYPE_CRC	1 



#define ERR_POWERUP_VOLTAGE_INVALID	 -1 		//FF
#define ERR_POWERUP_INTERRUPTED		 -2 		//FE
#define ERR_POWERUP_ATR_TIMEOUT		 -3		//FD
#define ERR_POWERUP_ATR_INVALID		 -4		//FC
#define ERR_POWERUP_ATR_CRC_FAILURE	 -5   	//FB
#define ERR_PROTOCOL_UNSUPPORTED		-6 		//FA
#define ERR_INVALID_SLOT				-7 		//F9
#define ERR_RECEIVE_TIMEOUT			 -8 		//F8
#define ERR_RECEIVE_PARITY			  -9 		//F7
#define ERR_RECEIVE_LRC				 -10		//F6
#define ERR_RECEIVE_CRC				 -11 	//F5
#define ERR_RECEIVE_SEQUENCENUM		 -12 	//F4
#define ERR_SETIFSD_FAILURE			 -13		//F3
#define ERR_RECEIVE_INVALID			 -14 	//F2
#define ERR_POWERUP_ATR_EA				-15		//F1
#define ERR_POWERUP_ATR_TS 			   	-16		//F0
#define ERR_TRANSMIT_PARITY			 -17 	//EF
#define ERR_OTHER					   -18		//EE
#define ERR_CWT_ERR											-19


extern unsigned char ATRLength[];
extern unsigned char workingBuffer[]; 
  

extern struct EMVATR lastATR[]; 
extern unsigned char currentSlot; 
extern unsigned char TMode[]; 

/***************************************************************************
* Function: m7816UartInit
* Description: 7816m0≥ı ºªØ
* Input: NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
extern void  m7816Init(void);

/***************************************************************************
* Function: unsigned char getATRbuffer(unsigned char *buff, unsigned char length)	
* Description: get ATR buffer
* Input: buff- buffer used to store ATR
		 length - buffer length
* Output: 
* Return: ATR length 	
* Other:  
**************************************************************************/	
extern unsigned char getATRbuffer(unsigned char *buff, unsigned char length);

/***************************************************************************
* Function: short ATRsequence(unsigned char mode)   
* Description: get ATR
* Input: mode 0-ISO mode; 
			  1- EMV mode
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/ 
extern short ATRsequence(unsigned char mode, M7816_TypeDef* M7816x);

/***************************************************************************
* Function: short powerup(unsigned char mode, unsigned char voltage)   
* Description: powerup or code reset
* Input: mode 0-ISO mode; 
			  1- EMV mode
		 voltage 0x00 - 5V
				 0x01 - 3V
				 0x02 - 1.8V
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/ 
short powerup(unsigned char mode,unsigned int voltage); 
 
/***************************************************************************
* Function: short warmreset(unsigned char mode)  
* Description: warmreset
* Input: mode 0-ISO mode; 1- EMV mode
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/  
short warmreset(unsigned char mode); 

/***************************************************************************
* Function: unsigned char powerdown(void)	
* Description: power down
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/  
unsigned char powerdown(void); 

/***************************************************************************
* Function: short m7816APDU(unsigned char *tbuffer,unsigned short length,unsigned char *rbuffer)   
* Description: m7816APDU
* Input: NULL
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/ 
short sendAPDU(unsigned char *buffer,unsigned short length,unsigned char *rbuffer); 
 
/***************************************************************************
* Function: sendsblockIFSD(unsigned char IFSD)   
* Description: sendsblockIFSD
* Input: IFSD
* Output: NULL
* Return: 	
* Other:  
**************************************************************************/ 
short sendsblockIFSD(unsigned char IFSD); 


/***************************************************************************
* Function: unsigned long rdRegister(unsigned long address)	
* Description: 	read chip register
* Input: 
* Output: 
* Return: 	
* Other:  
**************************************************************************/ 
extern unsigned long rdRegister(unsigned long address); 
 
/***************************************************************************
* Function: void wrRegister(unsigned long address,unsigned long value)	
* Description: 	write chip register
* Input: 
* Output: 
* Return: 	
* Other:  
**************************************************************************/  
extern void wrRegister(unsigned long address,unsigned long value); 

/***************************************************************************
* Function: void delay_etus(unsigned short etus)	 
* Description: 	delay N etus
* Input: 
* Output: 
* Return: 	
* Other:  
**************************************************************************/   
extern void delay_etus(unsigned short etus) ; 
/***************************************************************************
* Function: void MemorySet(unsigned char* pBuff, unsigned char val, unsigned short len)	
* Description: 	set memory pBuff len val
* Input: 
* Output: 
* Return: 	
* Other:  
**************************************************************************/ 
void MemorySet(unsigned char* pBuff, unsigned char val, unsigned short len);

#endif


