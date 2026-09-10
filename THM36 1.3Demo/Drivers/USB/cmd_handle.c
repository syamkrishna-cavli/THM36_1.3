/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: cmd_handle.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

#include "thm36_1.3.h"
#include "TMC_Flash.h"
#include "cmd_handle.h"
#include "romTable.h"
#include "RCC.h"
#include "TMC_Flash.h"
#include "flash_demomain.h"
uint8_t	SWptr = 0;
uint32_t	Foffset = 0;
uint8_t g_abComBuf[5];
uint16_t Nbpage;
uint32_t FlashAddress;
TMC_StatusTypeDef state;
__align(4) uint8_t IOBuf[256];
//BigNum_t inputBuf,outputBuf;
#define KEYLEN 0x200	
const uint8_t SW[]={0x90,0x00,0x6D,0x00,0x6C,0x00,0x65,0x01,0x65,0x04,0x6A,0x00,0x6A,0x88,0x6E,0x00,0x65,0x81,0x92,0x40};


/**
  *@brief	Execute CRC by hardware.
  *@param	inputdata:Input data.
  *@param	inputdata:Output data.
  *@param	len:The length of data.
  *@retval	Result.  				 
  */
uint16_t CRC_Calculate(uint8_t * inputData,uint16_t len)
{
	uint16_t result;

	result = CRC->DAT;	   					
	result = CRC->DAT;
	while(len)
	{
		CRC->DAT = *inputData;
		inputData++;
		len--;
	}
	result = CRC->DAT << 8;
	result += CRC->DAT;
	return result;		
}



/**
  *@brief	Execute CRC by hardware.
  *@param	address:Address.
  *@param	length:Data length.
  *@param	buf:Data buffer.
  *@retval	None.  				 
  */
void	CheckCRC(uint32_t address,uint8_t length,uint8_t * buf)
{
	uint16_t	blockSize = 0,blockNumber,i,crcCalculate,crcInput;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_CRC,ENABLE);
	
	/* Can not operate with address beyond OTP and flash */
	if(((address < FLASH1_ADDR_LIMIT))|| ((address < FLASH2_ADDR_LIMIT)&&(address >= FLASH2_ADDR_START)) || ((address >= OTP_ADDR_START)&&(address < OTP_ADDR_LIMIT)))
	{
		if(length < 0x04)
		{
			SWptr = P3ERROR;
		}
		else
		{
			blockSize += (buf[1] + ((uint16_t)buf[0]<<8));  					/* CRC calculate data length */	
			blockNumber = (length - 2) >> 1;								/* Cycle time,right shift because CRC-16 */	
			for(i = 0;i < blockNumber;i++)
			{
				crcInput = buf[(i<<1) + 2]; 								/* Read received CRC result */
				crcInput <<= 8;
				crcInput += buf[(i<<1) + 3];
			
				crcCalculate = CRC_Calculate((uint8_t *)address,blockSize);	/* Caltuleta CRC */	
				if(crcCalculate != crcInput)
				{
					SWptr = RDERROR;										/* 6504 */	
					break; 
				}
				address += blockSize;
			}
		}
	}
	else
	{
		SWptr = P1P2ERR;													/* 6A00	*/
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_CRC,DISABLE);
}


/***************************************************************************
* Function: CMD_Handle
* Description: commands handling
* Input:  NULL¡£
* Output: NULL
* Return: NULL
* Other:  Format: CLA INS P1 P2 P3 
**************************************************************************/
uint8_t g_FlashBuff[512];
void CMD_Handle(void)
{
	uint32_t destAddress;
	SWptr = SUCCESS;
	switch(INS)
	{ 				
		case 0xcd:		break;
		case 0x84:	  pkeGetRandomNum(IOBuf,P3);break;
		case SETBASE:
						SetBase();
						break;
		case ERFLASH:		
		case FERFLASH:
						EraseMode.EraseVread = FLASH_ERASE_NOT_VREAD;
						EraseMode.HardVerify = FLASH_ERASE_VERIFY;
						destAddress = FlashAddress + Foffset;
						Nbpage = (IOBuf[0]<<8)+IOBuf[1];
						if((destAddress&0xFFFFF)>0x00080000)
						{
							SWptr = WRERROR;
							return;
						}
						state = TMC_FLASH_ErasePage(destAddress,Nbpage,&EraseMode,0xFFFFFF);
						if(state != TMC_OK)
						{
							SWptr = WRERROR;
						}
						break;
		case WRFLASH:
						Program.HardVerify = FLASH_PROGRAM_VERIFY;
						Program.ProgramVread = FLASH_PROGRAM_VREAD;
						destAddress = FlashAddress + Foffset;
						if((destAddress&0xFFFFF)>0x00080000)
						{
							SWptr = WRERROR;
						}
						else if ((destAddress & 0x0FFFFFFF) < 0x200)//first page
						{
							memcpy((uint8_t*) g_FlashBuff + Foffset,IOBuf,P3);
							if ((destAddress & 0x0FFFFFFF) == 0x0)//last 128 bytes.
							{
								__disable_irq();
								state = TMC_FLASH_ErasePage(destAddress,0x01,&EraseMode,0xFFFFFF);
								if(state != TMC_OK)
								{
									SWptr = WRERROR;
									return ;
								}
								state = TMC_FLASH_ProgramDoubleWord(FLASH2_ADDR_START,(uint32_t)g_FlashBuff,64,&Program,0xFFFFFF);
								if(state != TMC_OK)
								{
									SWptr = WRERROR;
								}
							}
						}
						else
						{
							state = TMC_FLASH_ProgramDoubleWord(destAddress,(uint32_t)IOBuf,P3/8,&Program,0xFFFFFF);
							if(state != TMC_OK)
							{
								SWptr = WRERROR;
							}
						}	
						
						break;
						
		case 	CHECKCRC:		CheckCRC(destAddress,P3,IOBuf);					  	
						break;	  
		case	RESERVE :
						break;					
		default:		SWptr = IVDINS;								
						break;
	}																																					 
}



void SetBase(void)
{
	if(P3 != 0x00)
	{
		SWptr = P3ERROR;								/* 6C00	*/
	}
	else
	{
		FlashAddress = (P1<<24)+(P2<<16);
	}
}

