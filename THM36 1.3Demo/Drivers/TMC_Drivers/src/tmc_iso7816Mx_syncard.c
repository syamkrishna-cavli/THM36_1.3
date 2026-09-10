 /******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_iso7816Mx_syncard.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_iso7816Mx_Syncard.h"
#include "ISO7816M.h"

extern uint8_t ATR_Buffer[256];
extern uint8_t M7816x_Buffer[256];
uint32_t softpulsewidth = ST_BP_WITH;

void M7816x_SynSoftETU(M7816_TypeDef* M7816x,uint32_t timeout)
{
	delay_us(timeout);
}

void M7816x_SynSoftStart(M7816_TypeDef* M7816x)
{	
	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftIO(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	//start
	M7816x_SynSoftClk(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftIO(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	//20220804				
	//while(M7816x_SynSoftReadIO(M7816x)!=0); 
	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
}

void M7816x_SynSoftEnd(M7816_TypeDef* M7816x)
{
	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth>>1);

	//STOP
	M7816x_SynSoftIO(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftClk(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftIO(M7816x,TRUE);				//IO is High level in idle state.
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	
	M7816x_SynSoftClk(M7816x,FALSE);			//CLK is Low level in idle state.
	M7816x_SynSoftETU(M7816x,softpulsewidth>>1);
}

void M7816x_SynSoftStop(M7816_TypeDef* M7816x)
{
	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth>>1); 

	//STOP
	M7816x_SynSoftIO(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftClk(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftIO(M7816x,TRUE);				//IO is High level in idle state.

	M7816x_SynSoftETU(M7816x,softpulsewidth);
	
	M7816x_SynSoftClk(M7816x,FALSE);			//CLK is low level in idle state.
	M7816x_SynSoftETU(M7816x,softpulsewidth>>1);
}

void M7816x_SynSoftClock(M7816_TypeDef* M7816x)
{	
	M7816x_SynSoftClk(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
}

void M7816x_SynSoftOutByte(M7816_TypeDef* M7816x,uint8_t data)
{
	uint8_t i,datatemp;

	datatemp = data;
	for(i= 0 ; i < 8;i++)
	{
		M7816x_SynSoftIO(M7816x, (bool)(0x01&datatemp));
		datatemp >>= 1;
		M7816x_SynSoftClock(M7816x);
	}
}

void M7816x_SynSoftSendStart(M7816_TypeDef* M7816x)
{
	M7816x_SynStartSend(M7816x);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
}

void M7816x_SynSoftRecvStart(M7816_TypeDef* M7816x)
{
	M7816x_SynStartRecv(M7816x);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
}

uint8_t	M7816x_SynSoftReadByte(M7816_TypeDef* M7816x)
{	
	uint8_t i,rddata;

	for(i= 0 ; i < 8;i++)
	{
		M7816x_SynSoftClk(M7816x,TRUE);
		M7816x_SynSoftETU(M7816x,softpulsewidth>>1);
		rddata = rddata>>1;
		if(M7816x_SynSoftReadIO(M7816x) == 1)
			rddata |= 0x80;
		else
			rddata &= 0x7f;
		M7816x_SynSoftETU(M7816x,softpulsewidth>>1);
		M7816x_SynSoftClk(M7816x,FALSE);
		M7816x_SynSoftETU(M7816x,softpulsewidth);
	}
	return rddata;
}

void M7816x_SynSoftBreak(M7816_TypeDef* M7816x)
{
	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftRst(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftRst(M7816x,TRUE);
}

void M7816x_SynSoftRst2Atr(M7816_TypeDef* M7816x,uint8_t *Atr)
{
	uint8_t i;

	M7816x_SynSoftClk(M7816x,FALSE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftRst(M7816x,TRUE);
	M7816x_SynSoftETU(M7816x,softpulsewidth);
	M7816x_SynSoftClock(M7816x);
	M7816x_SynSoftRst(M7816x,FALSE);
	M7816x_SynSoftRecvStart(M7816x);

	for(i = 0; i < 4;i++)
	{
		Atr[i] = M7816x_SynSoftReadByte(M7816x);
	}
}

void M7816x_SynSoftProcEnd(M7816_TypeDef* M7816x)
{
	uint16_t i;
	for(i = 0; i < 512 ;i++)
	{
		if(M7816x_SynSoftReadIO(M7816x)!=0)
			i = 511;
		else
			M7816x_SynSoftClock(M7816x);
	}
	M7816x_SynSoftStop(M7816x);
}
void M7816x_SynSoftCmd(M7816_TypeDef* M7816x,uint8_t ctl,uint8_t  add,uint8_t data)
{
	M7816x_SynSoftStart(M7816x);
	M7816x_SynSoftOutByte(M7816x,ctl);
	M7816x_SynSoftOutByte(M7816x,add);
	M7816x_SynSoftOutByte(M7816x,data);
	M7816x_SynSoftStop(M7816x);	
}



