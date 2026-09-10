 /******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: 7816Mx_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "tmc_dma.h"
#include "tmc_iso7816Mx.h"
#include "iso7816M_demomain.h"
#include "7816Mx_syncard_demomain.h" 

M7816x_InitTypeDef	  ISO7816M2_Initx;
//TMC(20240408)-zt:TMC_ISO7816M->TMC_ISO7816M2
M7816x_HandleTypeDef  TMC_ISO7816M2;


DMA_HandleTypeDef ISO7816M_DMA_RX_Init;
DMA_HandleTypeDef ISO7816M_DMA_TX_Init;

GPIO_InitTypeDef GPIOD_Init;

uint8_t ATR_Buffer[256];
uint8_t M7816x_Buffer[256];

/**
  * @brief  Config M7816x MspInit
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @retval none
  * @note : config M7816x's clk,GPIO Function,systerm clk....
  */
void TMC_M7816x_MspInit(M7816x_HandleTypeDef* h7816m)
{		
	GPIO_InitTypeDef GPIOx_Init;
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_7816M2, ENABLE);
	
	/*Configure the 7816M1 clock source as PLL_L/5.*/
	RCC_7816MClockConfig(INTERFACE_7816M2,RCC_7816M_CLKSRC_PLL_L,4,ENABLE);//7816M CLK=4.8MHz	
	#ifndef	USE_EMV_PORT	
	TMC_GPIO_StructInit(&GPIOx_Init);
	
	/*Configure the RST.*/
	GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source10,GPIO_AF1_7816M2);
	/*Configure the CLK.*/
	GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source11,GPIO_AF1_7816M2);
	/*Configure the IO.*/
	GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source12,GPIO_AF1_7816M2);
	/*Configure the PRE.*/
	GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source13,GPIO_AF1_7816M2);
	/*Configure the PWR.*/
	GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source14,GPIO_AF1_7816M2);
	GPIOx_Init.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	GPIOx_Init.Mode = GPIO_MODE_AF;
	TMC_GPIO_Init(GPIOA,&GPIOx_Init);
	#else
	EMV7816M2_ConfigVoltage(EMV7816M2_VOLTAGE_3V);
	#endif   
	NVIC_ClearPendingIRQ(ISO7816M1_IRQn);
	NVIC_EnableIRQ(ISO7816M1_IRQn);
	NVIC_ClearPendingIRQ(ISO7816M2_IRQn);
	NVIC_EnableIRQ(ISO7816M2_IRQn);
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA,ENABLE);
	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);

}

/**
  * @brief  Initialize the M7816x MSP.
  * @param  hspi pointer to a M7816x_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @retval None
  */
void TMC_M7816x_MspDeInit(M7816x_HandleTypeDef* h7816m)
{
	GPIO_InitTypeDef GPIOx_Init;
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_7816M2,DISABLE);
	RCC_7816MClockConfig(INTERFACE_7816M2,RCC_7816M_CLKSRC_HSI,2,DISABLE);
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	
	
	TMC_GPIO_StructInit(&GPIOx_Init);

	GPIOx_Init.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOA,&GPIOx_Init);

	NVIC_ClearPendingIRQ(ISO7816M1_IRQn);
	NVIC_DisableIRQ(ISO7816M1_IRQn);
	
	NVIC_ClearPendingIRQ(ISO7816M2_IRQn);
	NVIC_DisableIRQ(ISO7816M2_IRQn);
	
}

void M7816x_StructInit(M7816x_InitTypeDef* M7816x_InitStruct)
{
	M7816x_InitStruct->ATR_Code_Mode = Hardware_Mode;
	M7816x_InitStruct->BaudRate_FD = 0x11;
	M7816x_InitStruct->DoubleBaudRate = DISABLE;
	M7816x_InitStruct->CLK_CPOL = CLK_STOP_LOW;
	M7816x_InitStruct->Eorrer_Retrans_times = 2;
	M7816x_InitStruct->protol = T0;
	M7816x_InitStruct->Trans_GTR = 0;		/*  GTR = 2 guard time is 3 ETU*/
	M7816x_InitStruct->ATR_SoftCode_Mode = Reverse_coding;
}  

void TMC_M7816x_TxCpltCallback(M7816x_HandleTypeDef *h7816m)
{
	
	
}


void TMC_M7816x_RxCpltCallback(M7816x_HandleTypeDef *h7816m)
{
	//IRQ_M7816x_CompleteFlag = 2;
	M7816x_StopTOC(h7816m->Instance,M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TO3|M7816x_USTS_TO2|M7816x_USTS_TO1);
}

/**
  * @brief TMC 7816Mx Error callback.
  * @param  h7816m pointer to a M7816x_HandleTypeDef structure that contains
  *			   the configuration information for ISO7816Mx module.
  * @retval None
  */
void TMC_M7816x_ErrorCallback(M7816x_HandleTypeDef *h7816m)
{
	//IRQ_M7816x_CompleteFlag = 3;
	M7816x_StopTOC(h7816m->Instance,M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TO3|M7816x_USTS_TO2|M7816x_USTS_TO1);
}
/**
  * @brief  Get M7816x's ATR
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @param[in] Buffer: pointer to ATR buffer 
  * @param[in] Size: Size of ATR 
  * @retval none
  * @note none
  */
TMC_StatusTypeDef ISO7816Mx_GetATR(M7816x_HandleTypeDef* h7816m,uint8_t *Buffer,uint8_t Size)
{
	TMC_StatusTypeDef status;
	
	/* Deactive Card */	
	M7816x_ConfigClkOutput(h7816m->Instance,DISABLE);	
	M7816x_ConfigActiveCard(h7816m->Instance,M7816x_POWER_DOWN);
	M7816x_ConfigReset(h7816m->Instance,M7816x_RST_LOW_LEVEL);
	
	/*Clear the PRL and M7816x_HSTS PRES flags.*/
	M7816x_ClearHStatus(h7816m->Instance,M7816x_HSTS_PRL|M7816x_HSTS_PRES);
	
	/*Card detection.*/
	//while(M7816x_GetHSTS(h7816m->Instance,M7816x_HSTS_PRL)==RESET){}
	/*Card is insert,VCC on.*/
	M7816x_ConfigActiveCard(h7816m->Instance,M7816x_POWER_UP);
	delay_ms(1);
	/*CLK outpuit.*/
	M7816x_ConfigClkOutput(h7816m->Instance,ENABLE);
	M7816x_ConfigIODirection(h7816m->Instance,M7816x_RECEIVE_MODE);
	/*Clear the 7816Mx flag.*/
	M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_EA|M7816x_USTS_PE|M7816x_USTS_OVR|M7816x_USTS_FER);
	M7816x_ReciveData(h7816m->Instance);//clear RBF
//	/*Sotp the TOC before configure the TOR.*/
//	GPIOA_Init.Pin = GPIO_PIN_3;
//	GPIOA_Init.Mode = GPIO_MODE_OUT;
//	TMC_GPIO_Init(GPIOA,&GPIOA_Init);
//	GPIO_ResetBits(GPIOA, GPIO_PIN_3);
	
	M7816x_StopTOC(h7816m->Instance,M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ConfigTOR(h7816m->Instance,0x100<<8);	
	M7816x_ConfigTOC(h7816m->Instance,M7816x_TIM_MODE3);
	while(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TO3)==RESET);
	M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TO3);
	M7816x_StopTOC(h7816m->Instance,M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ConfigTOR(h7816m->Instance,0);
	M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TO3|M7816x_USTS_TO2|M7816x_USTS_TO1);
	
	/* Wait 40000 to 45000 cycles to release reset */
	delay_ms(9);
	M7816x_ConfigReset(h7816m->Instance,M7816x_RST_HIGH_LEVEL);

	/*Receive the ATR,length is 'Size'.*/
	status = TMC_M7816x_Receive(h7816m,Buffer,Size);
	if(status != TMC_OK)
	{
		return TMC_ERROR;
	}
	return status;
}
 /**
* @brief ISO7816M2_AsynDemoMain:
*************1.TMC_M7816x_StructInit: Fills each M7816x_HandleTypeDef member with its default value
*************2.M7816x_StructInit: Init M7816x_InitTypeDef member
*************3.TMC_M7816x_DeInit: DeInitializes the M7816x peripheral
*************4.TMC_M7816x_Init: Initializes the M7816x Unit according to the specified parameters in the M7816x_HandleTypeDef and create the associated handle.
*************5.M7816x_ITConfigUMSK: Mask interrupt
*************6.EMV7816M2_ConfigElectronic: Config EMVCON register when using 7816M2
*************7.ISO7816Mx_GetATR: Get ATR
*************8.If Get ATR sucess, then config PPS, send random command, receive the subsequent rand numbers by polling or interruption or DMA 
    * @param  None
    * @retval None
 */
void ISO7816M2_AsynDemoMain(void)
{
	uint8_t temp;
	M7816x_Demo_TypefDef M7816x_Mode;
	
	M7816x_Mode = M7816x_Demo_DMA;
	
	TMC_M7816x_StructInit(&TMC_ISO7816M2);
	TMC_ISO7816M2.Instance = ISO7816M2;
	M7816x_StructInit(&ISO7816M2_Initx);
	TMC_ISO7816M2.Init = &ISO7816M2_Initx;
	TMC_M7816x_DeInit(&TMC_ISO7816M2);
	TMC_M7816x_Init(&TMC_ISO7816M2);
	M7816x_ITConfigUMSK(ISO7816M2,M7816x_USTS_Mask_TACT,DISABLE);
	
#ifdef	USE_EMV_PORT
	EMV7816M2_ConfigElectronic(EMV7816M2_IO_ELECTRONIC, EMV7816M2_TIMING_SOFTWARE); 
#else
	if(TMC_ISO7816M2.Instance==ISO7816M2)
	{
		EMV7816M2_ConfigElectronic(EMV7816M2_IO_NORMAL,EMV7816M2_TIMING_SOFTWARE);
	}
#endif
	
	if(ISO7816Mx_GetATR(&TMC_ISO7816M2,ATR_Buffer,11) == TMC_OK)
	{		
		TMC_M7816x_PPS_Config(&TMC_ISO7816M2,0x94);
		M7816x_Buffer[0] = 0x00;
		M7816x_Buffer[1] = 0x84;
		M7816x_Buffer[2] = 0x00;
		M7816x_Buffer[3] = 0x00;
		M7816x_Buffer[4] = 0x08;
		if(M7816x_Mode == M7816x_Demo_Polling)
		{
			/* Set up timer for 10080 etu between characters in ATR, and */
			/* maximum of 20160 etu for whole ATR */
			TMC_M7816x_Transmit(&TMC_ISO7816M2,M7816x_Buffer,5,M7816x_TIM_STOP_TO2_TO3,M7816x_TIM_MODE4,0x276A00);
			TMC_M7816x_Receive(&TMC_ISO7816M2,&M7816x_Buffer[5],1);
			if(M7816x_Buffer[5]!=M7816x_Buffer[1])
			{
				/*Receive the wrong SW.*/
				TMC_M7816x_Receive(&TMC_ISO7816M2,&M7816x_Buffer[6],1);
				return ;
			}
			/*Receive the subsequent rand numbers.*/
			TMC_M7816x_Receive(&TMC_ISO7816M2,&M7816x_Buffer[6],M7816x_Buffer[4]+2);
			TMC_M7816x_DeInit(&TMC_ISO7816M2); 
			temp = M7816x_Buffer[4]+5;
			if((M7816x_Buffer[temp+1] == 0x90)&&(M7816x_Buffer[temp+2] == 0x00))
			{
				return ;
			}
			while(1);
		}
		if(M7816x_Mode == M7816x_Demo_IRQ)
		{
			Delay();
			TMC_M7816x_Receive_IT(&TMC_ISO7816M2,&M7816x_Buffer[5],1); 
			TMC_M7816x_Transmit_IT(&TMC_ISO7816M2,M7816x_Buffer,5,M7816x_TIM_STOP_TO1_TO2_TO3,M7816x_TIM_MODE4,0x576A00); 
			while(TMC_ISO7816M2.State != TMC_M7816x_STATE_READY){}
			if(M7816x_Buffer[5]!=M7816x_Buffer[1])
			{
				/*Receive the wrong SW.*/
				TMC_M7816x_Receive(&TMC_ISO7816M2,&M7816x_Buffer[6],1); 
				return ;
			}
			TMC_M7816x_Receive(&TMC_ISO7816M2,&M7816x_Buffer[6],M7816x_Buffer[4]+2); 
			temp = M7816x_Buffer[4]+5;
			if((M7816x_Buffer[temp+1] == 0x90)&&(M7816x_Buffer[temp+2] == 0x00))
			{
				return ;
			}
			while(1);
		}
		if(M7816x_Mode == M7816x_Demo_DMA)
		{
			/*Initialize 7816M_rx's DMA,config DMA_Channel0 for 7816M_rx*/
			ISO7816M_DMA_TX_Init.Instance = DMA_Channel0;
			ISO7816M_DMA_TX_Init.Init.DestInc = DMA_DEST_INC_DISABLE;
			ISO7816M_DMA_TX_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
			ISO7816M_DMA_TX_Init.Init.IT_Enable  = DMA_IT_ENABLE;
			ISO7816M_DMA_TX_Init.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;
			ISO7816M_DMA_TX_Init.Init.RequestSelect = DMA_REQUEST_7816M2;
			ISO7816M_DMA_TX_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
			ISO7816M_DMA_TX_Init.Init.SrcInc = DMA_SRC_INC_ENABLE;
			ISO7816M_DMA_TX_Init.Init.DecAHBMx_Select = DMA_AHBM1_SELECT;
			ISO7816M_DMA_TX_Init.Init.SrcAHBMx_Select = DMA_AHBM2_SELECT;
			
			
			ISO7816M_DMA_RX_Init.Instance = DMA_Channel1;
			ISO7816M_DMA_RX_Init.Init.DestInc = DMA_DEST_INC_ENABLE;
			ISO7816M_DMA_RX_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
            ISO7816M_DMA_RX_Init.Init.IT_Enable  = DMA_IT_ENABLE;
			ISO7816M_DMA_RX_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
			ISO7816M_DMA_RX_Init.Init.RequestSelect = DMA_REQUEST_7816M2;
			ISO7816M_DMA_RX_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
			ISO7816M_DMA_RX_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
			ISO7816M_DMA_RX_Init.Init.DecAHBMx_Select = DMA_AHBM1_SELECT;
			ISO7816M_DMA_RX_Init.Init.SrcAHBMx_Select = DMA_AHBM2_SELECT;	

			
			ISO7816M_DMA_TX_Init.Parent = &TMC_ISO7816M2;
			ISO7816M_DMA_RX_Init.Parent = &TMC_ISO7816M2;
			TMC_ISO7816M2.hdmarx = &ISO7816M_DMA_RX_Init;
			TMC_ISO7816M2.hdmatx = &ISO7816M_DMA_TX_Init;
			TMC_DMA_Init(&ISO7816M_DMA_TX_Init);
			TMC_DMA_Init(&ISO7816M_DMA_RX_Init);
			
			TMC_M7816x_Transmit_DMA(&TMC_ISO7816M2, M7816x_Buffer, 5);
			while(TMC_ISO7816M2.State != TMC_M7816x_STATE_READY){}

			TMC_M7816x_Receive_DMA(&TMC_ISO7816M2, &M7816x_Buffer[5], M7816x_Buffer[4]+3);
			while(TMC_ISO7816M2.State != TMC_M7816x_STATE_READY){}
                
            temp = M7816x_Buffer[4]+5;   
            if((M7816x_Buffer[temp+1] == 0x90)&&(M7816x_Buffer[temp+2] == 0x00))
			{
				return ;
			}
			while(1);
		}
	}
	return ;
}

void M7816x_DMA_Handle(void)
{
	if(DMA_GetITStatus(DMA_IT_GL0) == SET)
	{
		TMC_DMA_IRQHandler(TMC_ISO7816M2.hdmatx);
	}
	if(DMA_GetITStatus(DMA_IT_GL1) == SET)
	{
		TMC_DMA_IRQHandler(TMC_ISO7816M2.hdmarx);
	}
}

void ISO7816M_Demo_Main(void)
{
	ISO7816M2_AsynDemoMain();
}
