/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ISO7816_EMV_L1.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

#ifndef __ISO7816EMVL1_H_
#define __ISO7816EMVL1_H_




typedef struct 
{ 
	unsigned char data[262];	// ¡®CLA INS P1 P2 [Lc + Lc data] [Le]¡¯ for command or 
	unsigned short length ; 	// ¡®[Le data] SW1 SW2¡¯ for response ( max = 256+2 ) 
} T_APDU ; 

/***************************************************************************
* Function: void EMV_ISO7816Polling(unsigned char looptime)
* Description: EMV Loop-back
* Input:  looptime 0-5s ; 1-30s
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
extern void EMV_ISO7816Polling(unsigned char looptime);

/***************************************************************************
* Function: unsigned char EMV_PowerOn (unsigned char *HistoricalChar)
* Description: EMV_PowerOn
* Input:  PowerUpClass:	0-5V; 1-3V; 2-1.8V
* Output: HistoricalChar: historical bytes
* Return: 0-success; 1-fail	
* Other:  
**************************************************************************/
extern unsigned char EMV_PowerOn (unsigned int PowerUpClass, unsigned char *HistoricalChar);

/***************************************************************************
* Function: unsigned char EMV_apdu (T_APDU *C_Apdu , T_APDU *R_Apdu ) 
* Description: EMV_apdu
* Input:  C_Apdu: CLA INS P1 P2 P3 DATA
* Output: R_Apdu: response data
* Return: 0-success; 1-fail	
* Other:  
**************************************************************************/
extern unsigned char EMV_Apdu (T_APDU *C_Apdu , T_APDU *R_Apdu );

/***************************************************************************
* Function: unsigned char EMV_SingleTest(void)
* Description: single EMV L1 Loop-back Application
* Input:  NULL
* Output: NULL
* Return: 0-normal stop; other-error condition	
* Other:  NULL
**************************************************************************/
extern unsigned char EMV_SingleTest(void) ;

void delay_1us_static(unsigned int us);

void Delay_200ms(void);
	
void Delay_5S(void);

void Delay_30S(void);


/***************************************************************************
* Function: mem_cpy
* Description: memory copy
* Input:  *dst :point destination address
*		 *src :point source address
*		  len :length to be copyed, unit byte
* Output: NULL
* Return: NULL
* Other:  NULL
**************************************************************************/
extern void mem_cpy(unsigned char * dst,unsigned char * src,unsigned short len);

/***************************************************************************
* Function: SleepMode
* Description: sleep
* Input:  *dstaddr :point destination address 
*		  datain  :need to be set data value 
*		  len	 :length to be set, unit byte
* Output: NULL
* Return: NULL
* Other:  NULL
**************************************************************************/
extern void mem_set(unsigned long dstaddr,unsigned char datain,unsigned short len);

/***************************************************************************
* Function: gpioSetDirInBit
* Description: set the specified only one port pin to output
* Input:  gpio_pin:  0 = gpio0
*					  ....
*					55 = gpio55
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
extern unsigned char gpioSetDirOut(unsigned char gpio_pin);

/***************************************************************************
* Function: gpioResetBit
* Description: clear the specified port pin
* Input:  gpio_pin:  0 = gpio0
*					  ....
*					61 = gpio55
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
extern unsigned char gpioResetBit(unsigned char gpio_pin);

/***************************************************************************
* Function: gpioSetBit
* Description: set the specified port pin
* Input:  gpio_pin:  0 = gpio0
*					  ....
*					55 = gpio55
* Output: NULL
* Return: NULL	
* Other:  NULL
**************************************************************************/
extern unsigned char gpioSetBit(unsigned char gpio_pin);


//TMC(20240401)-zt:EMVL1 test symbol
//#define EMV7816_10A



#endif



