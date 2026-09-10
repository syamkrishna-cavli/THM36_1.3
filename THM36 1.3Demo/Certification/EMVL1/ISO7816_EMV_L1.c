/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ISO7816_EMV_L1.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#include "iso7816M.h"
#include "ISO7816M_HAL.h" 
#include "ISO7816_EMV_L1.h"

T_APDU C_Apdu, R_Apdu; 			//	C_Apdu sent to the Card, R_Apdu received from the Card  

const unsigned char SelectPSE[]={0x00,0xA4,0x04,0x00,0x0E,0x31,0x50,0x41,0x59,0x2E,0x53,0x59,0x53,0x2E,0x44,0x44,0x46,0x30,0x31,0x00} ;

extern unsigned char CWT_Flag;
	
extern unsigned char N;

/***************************************************************************
* Function: void EMV_ISO7816Polling(unsigned char looptime)
* Description: EMV Loop-back
* Input:  looptime 0-5s ; 1-30s
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void EMV_ISO7816Polling(unsigned char looptime)	
{
	m7816Init();
	while(1)
	{
		EMV_SingleTest();
		powerdown();
		if(looptime == 0)
			Delay_5S();
		else
			Delay_30S();
	}
}
//TMC(20240327)-zt:改变延时模式-同步已过检代码
void delay_1us_static(unsigned int us)
{
//	volatile unsigned long  i,j,k;
////	k = 6;
//    k = 12;
//	for(i=0;i<us;i++)
//	{
//		for(j=0;j<k;j++)
//		{}
//	}
    delay_us(us);
}
void delay_ms_static(unsigned int ms)
{
//	volatile unsigned long  i,j,k;
////	k = 8000;
//    k = 22283;
//	for(i=0;i<ms;i++)
//	{
//		for(j=0;j<k;j++)
//		{}
//	}
    delay_ms(ms);
}
/***************************************************************************
* Function: void Delay_200ms(void)
* Description: Delay 200 ms
* Input:  NULL
* Output: NULL
* Return: NULL
* Other:  NULL
**************************************************************************/
void Delay_200ms(void)
{
//	unsigned long i;

//	for(i=0;i<0x440000;i++);  	
    delay_ms_static(200);
}
/***************************************************************************
* Function: void Delay_5S(void)
* Description: Delay 5s
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void Delay_5S(void)
{
	unsigned long i;
	for(i=0;i<25;i++)
	{
		delay_ms_static(200);
	}
}

/***************************************************************************
* Function: void Delay_30S(void)
* Description: Delay 30s
* Input:  NULL
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
void Delay_30S(void)
{
	Delay_5S();
	Delay_5S();
	Delay_5S();
	Delay_5S();
	Delay_5S();
	Delay_5S();
}

/***************************************************************************
* Function: void InitSelectPSE(T_APDU *apdu)
* Description: First C_APDU is SELECT PSE, this function is In order to distinguish T0 or T1
* Input:  NULL
* Output: apdu
* Return: NULL	
* Other:  NULL
**************************************************************************/
static void EMV_InitSelectPSE ( T_APDU *apdu )
{ 
	if(TMode[currentSlot]==0)
	{
		memcpy(apdu->data,(unsigned char*)SelectPSE,19); 
		apdu->length = 19;
	}
	else
	{
		memcpy(apdu->data,(unsigned char*)SelectPSE,20); 
		apdu->length = 20;
	}
}

/***************************************************************************
* Function: unsigned char EMV_SingleTest(void)
* Description: single EMV L1 Loop-back Application
* Input:  NULL
* Output: NULL
* Return: 0-normal stop; other-error condition	
* Other:  NULL
**************************************************************************/
unsigned char EMV_SingleTest(void)	 
{
	unsigned char RetCode;  
	unsigned char HistoricalChar[15]; 
	//unsigned char ReceiveData[30];
	CWT_Flag = 0;
	RetCode = EMV_PowerOn(EMV7816M2_VOLTAGE_3V, HistoricalChar);			//	power on
	
	 
 	if(RetCode != 0) 
	{
		return RetCode; 
	}
	EMV_InitSelectPSE(&C_Apdu);
	Delay_200ms();
	while (1) 
	{
		RetCode = EMV_Apdu(&C_Apdu, &R_Apdu);					//	Proprietary Command/Response function 
	
		if(RetCode != 0) 
		{
			return RetCode; 
		}
		
		if((R_Apdu.length == 7 ) && (R_Apdu.data[1] == 0x70))	//	Test case over	
		{
			delay_etus(60); 
			return 0;
		}

		else if(R_Apdu.length == 2 )   							//	Next C_APDU is predefined default C_APDU( SELECT PSE ) 
		{
			EMV_InitSelectPSE(&C_Apdu);
			Delay_200ms();
			continue;
		}

		memcpy(C_Apdu.data,R_Apdu.data,R_Apdu.length-2); 		//	APDU Exchange
		C_Apdu.length = R_Apdu.length-2;
		Delay_200ms();
	}
}

/***************************************************************************
* Function: unsigned char EMV_PowerOn (unsigned char *HistoricalChar)
* Description: EMV_PowerOn
* Input:  PowerUpClass:	EMV7816M2_VOLTAGE_5V-5V; EMV7816M2_VOLTAGE_3V-3V; EMV7816M2_VOLTAGE_1V8-1.8V
* Output: HistoricalChar: historical bytes, the length is 15 bytes
* Return: 0-success; 1-fail	
* Other:  
**************************************************************************/
unsigned char EMV_PowerOn (unsigned int PowerUpClass, unsigned char *HistoricalChar)
{
	short ATRrecFlag;
	unsigned char *k1;
	ATRrecFlag = powerup(POWERUP_EMV, PowerUpClass);
	if(ATRrecFlag == 0)
	{
	
		k1=(unsigned char *)lastATR[currentSlot].Historical;
		memcpy(HistoricalChar,k1,15);
		
		if(TMode[currentSlot]==1)								//	if T=1,send IFSD request
		{
			delay_ms_static(5);
			ATRrecFlag =  sendsblockIFSD(0xfe);
			if(ATRrecFlag < 0)
			{
				//repeat powerdown, delete it Modified 2019.10.30
//				powerdown();
				return 1;
			}
		}
		
	}
	else if(ATRrecFlag==ERR_POWERUP_ATR_INVALID)
	{
		delay_ms_static(2);
		ATRrecFlag = warmreset(POWERUP_EMV);			
		if(ATRrecFlag == 0)
		{
			k1=(unsigned char *)lastATR[currentSlot].Historical;
			memcpy(HistoricalChar,k1,15);
			
			if(TMode[currentSlot]==1)							//	if T=1,send IFSD request
			{
				delay_ms_static(5);
				if(sendsblockIFSD(0xfe)<0)
				{
//					powerdown();
					return 1;
				}
			}
//			return 0;
		}
		else
		{
//			delay_ms_static(5);
//			powerdown();
			return 1;
		}	
		
	}
	else
	{
//		delay_ms_static(1);
//		powerdown();
		
		return 1;
	}	
	return 0;
}

/***************************************************************************
* Function: unsigned char EMV_apdu (T_APDU *C_Apdu , T_APDU *R_Apdu ) 
* Description: EMV_apdu
* Input:  C_Apdu: CLA INS P1 P2 P3 DATA
* Output: R_Apdu: response data	and the data length
* Return: 0-success; 1-fail	
* Other:  
**************************************************************************/
unsigned char EMV_Apdu (T_APDU *C_Apdu , T_APDU *R_Apdu )
{
	short errflag;
	CWT_Flag = 0;		
	errflag = sendAPDU((unsigned char *)C_Apdu->data,C_Apdu->length,(unsigned char*)R_Apdu);
	if(errflag <0 )
	{
		if(errflag == ERR_CWT_ERR)
		{
				Delay_200ms();
		}
		return 1;
	}
	else
	{
		R_Apdu->length = errflag;
		return 0;
	}
}

