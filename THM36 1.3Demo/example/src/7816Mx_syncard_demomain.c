 /******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: 7816Mx_syncard_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "TMC_RCC.h"
#include "TMC_GPIO.h"
#include "TMC_DMA.h"
#include "TMC_ISO7816Mx.h"
#include "7816Mx_Syncard_demomain.h"
#include "tmc_iso7816Mx_Syncard.h"   


M7816_TypeDef *ISO7816M_SYN = ISO7816M2;    

static	uint8_t m_SynardData[256];
static	uint8_t	m_SynardPSCcode[4]; 
static	uint8_t atr[4];
static	uint8_t passcode[4] =  {0xff,0xff,0xff,0xff};
static	uint8_t carddata[0x100];

static	uint8_t Reverse_byte(uint8_t data);

extern uint32_t softpulsewidth; 
extern M7816x_HandleTypeDef  TMC_ISO7816M2;

/**
  * @brief  Synchronous Card Init
  * @param[in]  none
  * @retval none
  * @note: Config Synchronous Card mode, Enable Synchronous card and 7816M module, Active card
  */
void ISLE_InitSynard(void)
{
	uint32_t i=0;

	#ifdef	USE_EMV_PORT
	EMV7816M2_ConfigElectronic(EMV7816M2_IO_ELECTRONIC, EMV7816M2_TIMING_SOFTWARE); 
	#endif

	M7816x_SynCardCmd(ISO7816M_SYN,ENABLE);
	M7816x_ConfigClkOutput(ISO7816M_SYN,ENABLE);
	for(i=0;i<3;i++)
	{
		M7816x_SynSoftClk(ISO7816M_SYN,TRUE);
		M7816x_SynSoftETU(ISO7816M_SYN,softpulsewidth);
		M7816x_SynSoftClk(ISO7816M_SYN,FALSE);
	}
	M7816x_ITConfigHMSK(ISO7816M_SYN,M7816x_HSTS_Mask_PRTL|M7816x_HSTS_Mask_PRL,DISABLE);
	M7816x_ITConfigMMSK(ISO7816M_SYN,M7816x_MSTS_Mask_BGT|M7816x_MSTS_Mask_RBF|M7816x_MSTS_Mask_TBE,DISABLE);
	M7816x_ITConfigUMSK(ISO7816M_SYN,M7816x_USTS_Mask_TO3|M7816x_USTS_Mask_TO2|M7816x_USTS_Mask_TO1|M7816x_USTS_Mask_EA|M7816x_USTS_Mask_PE|
						M7816x_USTS_Mask_OVR|M7816x_USTS_Mask_FER|M7816x_USTS_Mask_TACT,DISABLE);
	M7816x_Cmd(ISO7816M_SYN,ENABLE);
	M7816x_SynSoftMode(ISO7816M_SYN);
	M7816x_ConfigActiveCard(ISO7816M_SYN,M7816x_POWER_UP);
}
/**
  * @brief Compare Verification Data
  * @param pscode: command
  * @param add:    Address
  * @param data:   Data
  * @retval none
*/
void ISLE_CompVerifiData(uint8_t pscode,uint8_t add,uint8_t data)
{	
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftCmd(ISO7816M_SYN,pscode,add,data);
	M7816x_SynSoftRecvStart(ISO7816M_SYN);
	M7816x_SynSoftStart(ISO7816M_SYN);
	M7816x_SynSoftProcEnd(ISO7816M_SYN);
}
/**
  * @brief Synchronous reset and read ATR
  * @param DataBuffer: ATR buffer
  * @retval ATR
*/
uint32_t ISLE_Reset(uint8_t* AtrBuffer)
{
	uint32_t atr = 0;
	
	delay_ms(10);
	M7816x_SynSoftRst2Atr(ISO7816M_SYN,AtrBuffer);
	memcpy((uint8_t *)&atr,AtrBuffer,4);
	
	return atr;
}
/**
  * @brief Read data from IC card Main Memory 
  * @param Offset: Start address of IC card Main Memory 
  * @param len: data length
  * @param DataBuffer: Read data buffer
  * @retval None
*/
void ISLE_Read_MainMem(uint16_t Offset,uint16_t len, uint8_t * DataBuffer)
{
	uint16_t i;
	uint16_t rlen = 0x100 - Offset;	//Wanted to read Length
	uint32_t atr;

	ISLE_Reset((uint8_t* )&atr);
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftCmd(ISO7816M_SYN,CMD_RD_MAIN_MEM,Offset,NO_EFFECT_CHAR);
	M7816x_SynSoftRecvStart(ISO7816M_SYN);
	for(i = 0; i < rlen;i++)
	{
		m_SynardData[Offset+i] = M7816x_SynSoftReadByte(ISO7816M_SYN);
	}
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftEnd(ISO7816M_SYN);
		
	memcpy(DataBuffer,&m_SynardData[Offset],len);
}
/**
  * @brief Write one byte to IC card memory
  * @param cmd: Command of write to card memory
  * @param Offset: Address of IC card memory 
  * @param value: Write data
  * @retval None
*/
void ISLE_Write_Mem(uint8_t cmd,uint8_t type, uint16_t Offset, uint8_t value)
{
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftCmd(ISO7816M_SYN,cmd,Offset,value);
	M7816x_SynSoftRecvStart(ISO7816M_SYN);
	if((cmd == CMD_WR_MAIN_MEM)||(cmd == CMD_WR_PSC_MEM)||(cmd == CMD_WR_PROTECT_MEM)||(cmd == CMD_COMP_PSC_MEM))
		M7816x_SynSoftProcEnd(ISO7816M_SYN);
}
/**
  * @brief Write one byte to IC card Main memory 
  * @param Offset: Start address of IC card Main memory 
  * @param DataByte: Write data
  * @retval None
*/
void ISLE_WriteMMByte(uint16_t Offset,uint8_t DataByte)
{
	if(DataByte == 0xFF)
	{		
		ISLE_Write_Mem(CMD_WR_MAIN_MEM,TYPE_UPDATE_E2NW,Offset,DataByte);//Erase only		
	}
	else if((m_SynardData[Offset] & DataByte) == DataByte)
	{	
		ISLE_Write_Mem(CMD_WR_MAIN_MEM,TYPE_UPDATE_NE2W,Offset,DataByte);//Write only
	}
	else
	{
		ISLE_Write_Mem(CMD_WR_MAIN_MEM,TYPE_UPDATE_E2W,Offset,DataByte);//Write and Erase
	}
	m_SynardData[Offset] = 	DataByte;
}
/**
  * @brief Write data to IC card Main Memory 
  * @param Offset: Start address of IC card Main Memory 
  * @param len: data length
  * @param DataBuffer: Write data buffer
  * @retval None
*/
void ISLE_Write_MainMem(uint16_t Offset,uint16_t len, uint8_t* DataBuffer)
{
	uint16_t i;
	uint32_t atr;
	ISLE_Reset((uint8_t* )&atr);
	for (i = 0;i < len ;i++)
	{  
		if(m_SynardData[Offset+i] != DataBuffer[i])
		{
			ISLE_WriteMMByte(Offset+i,DataBuffer[i]);
		}
	}
}

/**
  * @brief Read IC card Protection Memory, 32bits
  * @param ProtBuffer: read data buffer
  * @retval None
*/
void ISLE_ReadProtection (uint8_t* ProtBuffer)
{
	uint16_t i;
	uint32_t atr;
	
	ISLE_Reset((uint8_t* )&atr);
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftCmd(ISO7816M_SYN,CMD_RD_PROTECT_MEM,NO_EFFECT_CHAR,NO_EFFECT_CHAR);
	M7816x_SynSoftRecvStart(ISO7816M_SYN);
	for(i = 0; i < 4;i++)
	{
		ProtBuffer[i] = M7816x_SynSoftReadByte(ISO7816M_SYN);
		ProtBuffer[i] = Reverse_byte(ProtBuffer[i]);
		//m_SynardProtectData[i] = ProtBuffer[i] ;
	}
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftEnd(ISO7816M_SYN);
}
/**
  * @brief Write one byte of IC card Protection Memory 
  * @param Offset: Start address of IC card Protection Memory 
  * @param Protdata: Write data
  * @retval None
*/
void ISLE_WriteProtectionByte(uint16_t Offset,uint8_t Protdata)
{
	if(m_SynardData[Offset] == Protdata)
	{
		ISLE_Write_Mem(CMD_WR_PROTECT_MEM,TYPE_UPDATE_E2W,Offset,Protdata);
	}
}
/**
  * @brief Write Protection Memory of IC card, max 32 Bytes
  * @param Offset: Start address of IC card Protection Memory 
  * @param len: protected bytes
  * @param ProtBuffer: pointer to protected data
  * @retval None
*/
void ISLE_WriteProtection (uint16_t Offset, uint16_t len,uint8_t *ProtBuffer)
{
	uint16_t i;
	uint32_t atr;
    
	ISLE_Reset((uint8_t* )&atr);
	for (i = 0;i < len ;i++)
	{   
		ISLE_WriteMMByte(Offset+i,ProtBuffer[i]);
		ISLE_WriteProtectionByte(Offset+i,ProtBuffer[i]);
	}
}

/**
  * @brief  Read password and error counter of SLE4442 card. 
  * @param  SMBuffer: pointer to read data buffer 
  * @retval Error counter
*/
uint16_t ISLE_ReadPass_SLE4442 (uint8_t *SMBuffer)
{
	uint16_t i;
	short rxval = 0;
	
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftCmd(ISO7816M_SYN,CMD_RD_PSC_MEM,NO_EFFECT_CHAR,NO_EFFECT_CHAR);
	M7816x_SynSoftRecvStart(ISO7816M_SYN);
	for(i = 0; i < 4;i++)
	{
		rxval = M7816x_SynSoftReadByte(ISO7816M_SYN);
		SMBuffer[i] = rxval;
		m_SynardPSCcode[i] = rxval;
	}
	M7816x_SynSoftSendStart(ISO7816M_SYN);
	M7816x_SynSoftEnd(ISO7816M_SYN);

	return m_SynardPSCcode[0];
}

/**
  * @brief  Check password of SLE4442 card. 
  * @param  Password: pointer to password 
  * @param  SMBuffer: pointer to read data buffer 
  * @retval   =0: Sucess
  *           <0: Faied. Its absolute value is error code
*/
int ISLE_CheckPass_SLE4442(uint8_t* Password,uint8_t *SMBuffer)
{
	//Read security Memory 31H No effect No effect Check Error Counter
	uint16_t i;
	uint8_t	 ecc;
	short rxval;
	uint32_t atr;
	uint8_t passcode[3];

	memcpy(passcode,Password,3);
	ISLE_Reset((uint8_t* )&atr);

	rxval = ISLE_ReadPass_SLE4442(SMBuffer);
	if(rxval < 0)
	{
		return rxval;
	}
	ecc = rxval;
	ecc &= 0x07;
	if(ecc == 0)
	{
		return rxval;
	}
	
	if(ecc &0x01)											
		ecc &= 0x06;
	else if(ecc &0x02)
		ecc &= 0x05;
	else
		ecc &= 0x03;

	
	ISLE_Reset((uint8_t* )&atr);
	//Counter input data:
	//0000 0ddd binary
	//Update Security Memory 39H 00H Input data Write free bit in Error
	ISLE_CompVerifiData(CMD_WR_PSC_MEM,0x00,ecc);
	//For this procedure clock pulses are necessary 
	//during the processing mode.
	//Compare Verification Data 33H 01H Input data Reference Data Byte 1
	//Compare Verification Data 33H 02H Input data Reference Data Byte 2
	//Compare Verification Data 33H 03H Input data Reference Data Byte 3
	for(i =0;i < 3;i++)
	{
		ISLE_CompVerifiData(CMD_COMP_PSC_MEM,i+1,passcode[i]);
	}
	//The execution times and the required clock pulses are the same 
	//	as described under UPDATE MAIN MEMORY.
	//Update Security Memory 39H 00H FFH Erase Error Counter
	ISLE_CompVerifiData(CMD_WR_PSC_MEM,0x00,0xff);

	ISLE_Reset((uint8_t* )&atr);
	//Read Security Memory 31H No effect No effect Check Error Counter	
	rxval = ISLE_ReadPass_SLE4442(SMBuffer);
	if(rxval < 0)
	{
		return rxval;
	}
	memcpy(SMBuffer,m_SynardPSCcode,4);
	return rxval;
}
/**
  * @brief  Change password of SLE4442 card. If the card is powered off after the password is changed, 
            you must check the password again if you want to write the card or change the password again.
  * @param  Password: pointer to password which has 3 bytes
  * @retval   =0: Sucess
  *           <0: Faied. Its absolute value is error code
*/
uint16_t ISLE_ChangePass_SLE4442(uint8_t* Password)
{
	//Read security Memory 31H No effect No effect Check Error Counter
	uint16_t i;
	uint8_t	 ecc,psccode[4];
	short rxval;
	uint32_t atr;

	ISLE_Reset((uint8_t* )&atr);
	rxval = ISLE_CheckPass_SLE4442 (Password,psccode);
	if(rxval < 0)
	{
		return rxval;
	}
	ecc = rxval;
	ecc &= 0x07;
	if(ecc == 0)
	{
		return rxval;
	}
		
	for(i =0;i < 3;i++)
	{
		ISLE_CompVerifiData(CMD_WR_PSC_MEM,i+1,Password[3+i]);
	}
	ISLE_Reset((uint8_t* )&atr);
	rxval = ISLE_ReadPass_SLE4442(psccode);
	if(rxval < 0)
	{
		return rxval;
	}
	memcpy(Password,m_SynardPSCcode,4);
	return rxval;
}

/**
  * @brief  Reverse byte 
  * @param  data: the data to reverse 
  * @retval reversed data
*/
uint8_t Reverse_byte(uint8_t data)
{
	data=(data<<4)|(data>>4);
	data=((data<<2)&0xcc)|((data>>2)&0x33);
	data=((data<<1)&0xaa)|((data>>1)&0x55);
	return data;
}
/**
  * @brief  SLE4442 Card Test Demo
  **********1.Synchronous Card Init: Config Synchronous Card mode, Enable Synchronous card and 7816M module, Active card
  **********2.ISLE_Reset: Synchronous Reset and read ATR
  **********3.ISLE_Read_MainMem: Read SLE4442 256 x 8 bit EEPROM main memory
  **********4.ISLE_CheckPass_SLE4442: Check password
  **********5.ISLE_ReadPass_SLE4442: Read password
  **********6.ISLE_ReadProtection: Read 32-bit protection memory
  **********7.ISLE_WriteProtection: Write 32-bit protection memory
  **********8.ISLE_ChangePass_SLE4442: Change password
  * @param  none 
  * @retval none
*/
void SLE4442Demo(void)
{
	int i;
	int rxval;
	uint32_t u32atr;
	uint8_t	errcount =0 ; 
	uint8_t wrdata[16];	
	uint8_t ProtBuffer[4],SMBuffer[4];
	uint8_t onpasscode[6] = {0xFF,0xFF,0xFF,0x11,0x11,0x11};//old(3)+new(3) Change password to 0x111111
	uint8_t nopasscode[6] = {0x11,0x11,0x11,0xFF,0xFF,0xFF};//old(3)+new(3) Change password to 0xFFFFFF
			
	ISLE_InitSynard();
	u32atr = ISLE_Reset(atr);
	
	while(1)
	{
		for(i = 0; i < 5 ; i++)
		{
			ISLE_Read_MainMem(0x00,0x100, &carddata[0x0]);
			delay_ms(2);

			if(rxval >=0)
			{
				if(memcmp(atr,(void *)carddata,4) !=0)
				{
					errcount++;						
				}
			}
		}
		if(memcmp(atr,(void *)carddata,4) ==0)
		{
			if(u32atr >0)
			{
				rxval = ISLE_CheckPass_SLE4442(passcode,SMBuffer);
				if(rxval >=0)
				{
					rxval = ISLE_ReadPass_SLE4442(&SMBuffer[0]);
					ISLE_Read_MainMem(0,0x100, carddata);
                    ISLE_ReadProtection(ProtBuffer);
                    wrdata[0] = ~carddata[0x01];
                    ISLE_WriteProtection(0x01,1,wrdata);
                    ISLE_ReadProtection(ProtBuffer);
                    
                    wrdata[0] = ~carddata[0x30];
                    ISLE_Write_MainMem(0x20,1,wrdata);
                    rxval = ISLE_ChangePass_SLE4442(onpasscode);
                    if(rxval >=0)
                    {
                        rxval = ISLE_CheckPass_SLE4442(&onpasscode[3],SMBuffer);
                        if(rxval >=0)
                            rxval = ISLE_ChangePass_SLE4442(nopasscode);
                    }
				}
			}	
		}
	}
	
}

/* Synchronous card demo for SLE4442*/
void ISO7816M2_SynDemoMain(void)
{		
	TMC_ISO7816M2.Instance = ISO7816M2;		/*Select 7816M2 Interface*/
	TMC_M7816x_MspInit(&TMC_ISO7816M2); 
	
	SLE4442Demo();
}

