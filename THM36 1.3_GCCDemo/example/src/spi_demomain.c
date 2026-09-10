/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: spi_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

#include "thm36_1.3.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "tmc_dma.h"
#include "tmc_spi.h"
#include "dma.h"
#include "spi_demomain.h"

//TMC(20240408)-zt:TMC_SPIx->TMC_SPI3
SPI_HandleTypeDef TMC_SPI3;
SPI_InitTypeDef   SPI1_Init;
SPI_InitTypeDef   SPI3_Init;
DMA_HandleTypeDef SPI_DMA_tx_Init;
DMA_HandleTypeDef SPI_DMA_rx_Init;


__attribute__ ((aligned (4)))  uint8_t SPITestBuffer1[256] ={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F
									};
__attribute__ ((aligned (4))) uint8_t SPITestBuffer_tx[256]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
							  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
uint8_t SPITestBuffer_rx[256];
uint8_t SPITestBuffer2[256];
uint8_t IRQ_CompleteFlag = 0;



TMC_StatusTypeDef SPIx_DemoReceiveTest(SPIx_Demo_TypefDef Demo_Test);
TMC_StatusTypeDef SPIx_DemoTransmitTest(SPIx_Demo_TypefDef Demo_Test);
TMC_StatusTypeDef SPIx_DemoReciveTransmitTest(SPIx_Demo_TypefDef Demo_Test);
SPIx_Demo_TypefDef DemoTest;

void SPI_DemoMain(void)
{
	/* The 'DemoTest' can be the value below:
		SPIx_Demo_Polling
		SPIx_Demo_IRQ
		SPIx_Demo_DMA
	*/
	DemoTest = SPIx_Demo_IRQ;
	while(1)
	{
		SPIx_DemoReciveTransmitTest(DemoTest);
		Delay();
		SPIx_DemoTransmitTest(DemoTest);
	}
	
}

/*There is a '__weak' type SPIx_StructInit in SPI.c,redefine a new SPIx_StructInit will replace it.*/
void SPIx_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
	SPI_InitStruct->Master_EGT = 0;
	SPI_InitStruct->BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;	/*SPI clk is Fsys/(2*256)*/
	SPI_InitStruct->CLKPhase = SPI_PHASE_1EDGE;						/*SPI sample at first edge.*/
	SPI_InitStruct->CLKPolarity = SPI_POLARITY_LOW;					/*SPI ploarity is low.*/
	SPI_InitStruct->Mode = SPI_MASTER_MODE;							/*SPI master mode.*/
	SPI_InitStruct->SQI_Select = SPI_MODE;							/*SPI mode.*/
}

/*There is a '__weak' type TMC_SPI_StructInit in TMC_SPI.c,redefine a new TMC_SPI_StructInit will replace it.*/
void TMC_SPI_StructInit(SPI_HandleTypeDef* hspi)
{
	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
	hspi->Lock = HAL_UNLOCKED;
	hspi->State = TMC_SPI_STATE_RESET;
	hspi->hdmarx = NULL;
	hspi->hdmatx = NULL;
	hspi->Instance = NULL;
	hspi->pRxBuffPtr = NULL;
	hspi->pTxBuffPtr = NULL;
	hspi->RxISR = NULL;
	hspi->RxXferCount = 0;
	hspi->RxXferSize = 0;
	hspi->TxISR = 0;
	hspi->TxXferCount = 0;
	hspi->TxXferSize = 0;
}

/*There is a '__weak' type ¸öTMC_SPI_MspInit in TMC_SPI.c,redefine a new ¸öTMC_SPI_MspInit will replace it.*/
void TMC_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIOx_Init;
	/*This demo using GPIOA0/GPIOA1/GPIOA2 as SPI1 MOSI/MISO/CLK.*/
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI3,ENABLE);
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA,ENABLE);
	
	/*This demo using GPIOD5/GPIOD6/GPIOD7 as SPI3 MOSI/MISO/CLK.*/
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source5, GPIO_AF1_SPI3);
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source6, GPIO_AF1_SPI3);
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source7, GPIO_AF1_SPI3);
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIOx_Init.Mode = GPIO_MODE_AF;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);

	/*SSN*/
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_13;
	GPIOx_Init.Mode = GPIO_MODE_OUT;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	TMC_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
	/*Open SPI1 interrupt.*/
	NVIC_ClearPendingIRQ(SPI1_IRQn);
	NVIC_EnableIRQ(SPI1_IRQn);

	NVIC_ClearPendingIRQ(SPI3_IRQn);
	NVIC_EnableIRQ(SPI3_IRQn);
	
	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);
}

void TMC_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIOx_Init;
	/*This demo using GPIOA0/GPIOA1/GPIOA2 as SPI1 MOSI/MISO/CLK.*/
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI3,DISABLE);
	
	/*This demo using GPIOD5/GPIOD6/GPIOD7 as SPI3 MOSI/MISO/CLK.*/
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	/*SSN*/
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_13;
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	/*Open the interrupts.*/
	NVIC_ClearPendingIRQ(SPI1_IRQn);
	NVIC_EnableIRQ(SPI1_IRQn);

	NVIC_ClearPendingIRQ(SPI3_IRQn);
	NVIC_EnableIRQ(SPI3_IRQn);

}

void TMC_SPI_SSN_Enable(SPI_HandleTypeDef *hspi)
{
//	if(hspi->Instance == SPI1)
//	{
		/*GPIOA4 as SSN of SPI1.*/
//		TMC_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);
//	}
	if(hspi->Instance == SPI3)
	{
		/*GPIOD4 as SSN of SPI3.*/
		TMC_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
	}
}

void TMC_SPI_SSN_Disable(SPI_HandleTypeDef *hspi)
{
//	if(hspi->Instance == SPI1)
//	{
//		TMC_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
//	}
	if(hspi->Instance == SPI3)
	{
		TMC_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
	}
}

/**
  * @brief SPI receive over callback function. 
  * @param  hspi 
  * @retval None.
  */
/* The callback function after SPI receive over.*/
void TMC_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
}

/**
  * @brief SPI tx over callback function. 
  * @param  hspi 
  * @retval None.
  */
/* The callback function after SPI tx interrupt.*/
void TMC_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
}

/**
  * @brief SPI rx over callback function. 
  * @param  hspi 
  * @retval None.
  */
/* The callback function after SPI rx over.*/
void TMC_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
}

/**
  * @brief SPI error call back function. 
  * @param  SPIx_Demo_TypefDef 
  * @retval TMC_StatusTypeDef status
  */
/* The callback function after SPI Error.*/
void TMC_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
}

/**
  * @brief Demo Test of SPI 
  * @param  SPIx_Demo_TypefDef 
  * @retval TMC_StatusTypeDef status
  */
/*SPI demo Test,support polling,DMA and IRQ mode.*/
TMC_StatusTypeDef SPIx_DemoReceiveTest(SPIx_Demo_TypefDef Demo_Test)
{
	TMC_StatusTypeDef status;
	SPIx_StructInit(&SPI3_Init);
	TMC_SPI_StructInit(&TMC_SPI3);
	TMC_SPI3.Instance = SPI3;
	TMC_SPI3.Init = &SPI3_Init;
	TMC_SPI_DeInit(&TMC_SPI3);
	TMC_SPI_Init(&TMC_SPI3);
	if(Demo_Test == SPIx_Demo_Polling)
	{
		/*SPI polling receive 16 bytes.*/
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_Receive(&TMC_SPI3,SPITestBuffer1, 16,1000000);
		if(TMC_SPI3.Init->Mode  == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
		TMC_SPI_DeInit(&TMC_SPI3);
		return status;
	}
	if(Demo_Test == SPIx_Demo_IRQ)
	{
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_Receive_IT(&TMC_SPI3,SPITestBuffer1, 16);
		if(status!= TMC_OK)
		return status;
		
		while(TMC_SPI3.State == TMC_SPI_STATE_BUSY_RX);
		if(TMC_SPI3.State == TMC_SPI_STATE_ERROR)
		{
			TMC_SPI_DeInit(&TMC_SPI3);
			return TMC_ERROR;
		}
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
	}
	if(Demo_Test == SPIx_Demo_DMA)
	{
		if(Demo_Test == SPIx_Demo_DMA)
		{
			SPI_DMA_tx_Init.Instance = DMA_Channel1;
			SPI_DMA_tx_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
			SPI_DMA_tx_Init.Init.DestInc = DMA_DEST_INC_DISABLE;
			SPI_DMA_tx_Init.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;
			SPI_DMA_tx_Init.Init.RequestSelect = DMA_REQUEST_SPI3_TX;
			SPI_DMA_tx_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
			SPI_DMA_tx_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
			SPI_DMA_tx_Init.Init.IT_Enable = DMA_IT_ENABLE;
			SPI_DMA_tx_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
			SPI_DMA_tx_Init.Init.DecAHBMx_Select = DMA_AHBM1_SELECT;
			
			SPI_DMA_tx_Init.Parent = &TMC_SPI3;
			TMC_SPI3.hdmatx = &SPI_DMA_tx_Init;
			
			SPI_DMA_rx_Init.Instance = DMA_Channel0;
			SPI_DMA_rx_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
			SPI_DMA_rx_Init.Init.DestInc = DMA_DEST_INC_ENABLE;
			SPI_DMA_rx_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
			SPI_DMA_rx_Init.Init.RequestSelect = DMA_REQUEST_SPI3_RX;
			SPI_DMA_rx_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
			SPI_DMA_rx_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
			SPI_DMA_rx_Init.Init.IT_Enable = DMA_IT_ENABLE;
			SPI_DMA_rx_Init.Parent = &TMC_SPI3;
//			
			SPI_DMA_rx_Init.Init.SrcAHBMx_Select = DMA_AHBM2_SELECT;
			SPI_DMA_rx_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
			SPI_DMA_rx_Init.Parent = &TMC_SPI3;
			TMC_SPI3.hdmarx = &SPI_DMA_rx_Init;
			
			TMC_DMA_DeInit(&SPI_DMA_rx_Init);
			TMC_DMA_DeInit(&SPI_DMA_tx_Init);
			
			TMC_DMA_Init(&SPI_DMA_rx_Init);
			TMC_DMA_Init(&SPI_DMA_tx_Init);
			if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
			{
				TMC_SPI_SSN_Enable(&TMC_SPI3);
			}
			status = TMC_SPI_TransmitReceive_DMA(&TMC_SPI3,SPITestBuffer_tx,SPITestBuffer_rx,16,0,0);
			if(status!= TMC_OK)
			{
				TMC_DMA_DeInit(TMC_SPI3.hdmatx);
				TMC_SPI_DeInit(&TMC_SPI3); 
				return status;
			}
			while(TMC_SPI3.State != TMC_SPI_STATE_READY);
			if(TMC_SPI3.State == TMC_SPI_STATE_ERROR)
			{
				TMC_DMA_DeInit(TMC_SPI3.hdmatx);
				TMC_SPI_DeInit(&TMC_SPI3); 
			}
			if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
			{
				TMC_SPI_SSN_Disable(&TMC_SPI3);
			}
		}
		
	}
	return TMC_ERROR;
}
/**
  * @brief Demo Test of SPI TransMit 
  * @param  SPIx_Demo_TypefDef 
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef SPIx_DemoTransmitTest(SPIx_Demo_TypefDef Demo_Test)
{
	TMC_StatusTypeDef status;
	
	SPIx_StructInit(&SPI3_Init);
	TMC_SPI_StructInit(&TMC_SPI3);
	TMC_SPI3.Instance = SPI3;
	TMC_SPI3.Init = &SPI3_Init;
	TMC_SPI_DeInit(&TMC_SPI3);
	TMC_SPI_Init(&TMC_SPI3);
	//TMC_SPI_SSN_Disable(&TMC_SPI1);
	if(Demo_Test == SPIx_Demo_Polling)
	{
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_Transmit(&TMC_SPI3,SPITestBuffer1,16,1000000);
		if(TMC_SPI3.Init->Mode  == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
		return status;
	}
	if(Demo_Test == SPIx_Demo_IRQ)
	{
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_Transmit_IT(&TMC_SPI3,SPITestBuffer1, 16);
		
		while(TMC_SPI3.State!= TMC_SPI_STATE_READY);
		if(TMC_SPI3.State == TMC_SPI_STATE_ERROR)
		{
			TMC_DMA_DeInit(TMC_SPI3.hdmatx);
			TMC_SPI_DeInit(&TMC_SPI3); 
			return TMC_ERROR;
		}
		
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
		
		if(status!= TMC_OK)
		{
			return status;
		}
		return TMC_OK;
	}
	if(Demo_Test == SPIx_Demo_DMA)
	{
		////**********************DMA Tx demo***********************************************************
		SPI_DMA_tx_Init.Instance = DMA_Channel1;					
		SPI_DMA_tx_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
		SPI_DMA_tx_Init.Init.DestInc = DMA_DEST_INC_DISABLE;		
		SPI_DMA_tx_Init.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;	
		SPI_DMA_tx_Init.Init.RequestSelect = DMA_REQUEST_SPI3_TX;	
		SPI_DMA_tx_Init.Init.SrcInc = DMA_SRC_INC_ENABLE;			
		SPI_DMA_tx_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;	
		SPI_DMA_tx_Init.Init.IT_Enable = DMA_IT_ENABLE;
		SPI_DMA_tx_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		SPI_DMA_tx_Init.Init.DecAHBMx_Select = DMA_AHBM1_SELECT;
		
		SPI_DMA_tx_Init.Parent = &TMC_SPI3;
		TMC_SPI3.hdmatx = &SPI_DMA_tx_Init;
		
		SPI_DMA_rx_Init.Instance = DMA_Channel0;
		SPI_DMA_rx_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
		SPI_DMA_rx_Init.Init.DestInc = DMA_DEST_INC_DISABLE;
		SPI_DMA_rx_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
		SPI_DMA_rx_Init.Init.RequestSelect = DMA_REQUEST_SPI3_RX;
		SPI_DMA_rx_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
		SPI_DMA_rx_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
		SPI_DMA_rx_Init.Init.IT_Enable = DMA_IT_ENABLE;
		SPI_DMA_rx_Init.Parent = &TMC_SPI3;
		
		SPI_DMA_rx_Init.Init.SrcAHBMx_Select = DMA_AHBM2_SELECT;
		SPI_DMA_rx_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		SPI_DMA_rx_Init.Parent = &TMC_SPI3;
		TMC_SPI3.hdmarx = &SPI_DMA_rx_Init;
		
		TMC_DMA_DeInit(&SPI_DMA_rx_Init);
		TMC_DMA_DeInit(&SPI_DMA_tx_Init);
		
		TMC_DMA_Init(&SPI_DMA_rx_Init);
		TMC_DMA_Init(&SPI_DMA_tx_Init);
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_TransmitReceive_DMA(&TMC_SPI3,SPITestBuffer_tx,SPITestBuffer_rx,16,0,0);
		if(status!= TMC_OK)
		{
			TMC_DMA_DeInit(TMC_SPI3.hdmatx);
			TMC_SPI_DeInit(&TMC_SPI3); 
			return status;
		}
		while(TMC_SPI3.State != TMC_SPI_STATE_READY);
		if(TMC_SPI3.State == TMC_SPI_STATE_ERROR)
		{
			TMC_DMA_DeInit(TMC_SPI3.hdmatx);
			TMC_SPI_DeInit(&TMC_SPI3); 
		}
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
		return TMC_OK;	
	}
	return TMC_ERROR;	
}


/**
  * @brief Demo Test of SPI TransMit 
  * @param  SPIx_Demo_TypefDef 
  * @retval TMC_StatusTypeDef status
  */

TMC_StatusTypeDef SPIx_DemoReciveTransmitTest(SPIx_Demo_TypefDef Demo_Test)
{
	//uint32_t i;
	TMC_StatusTypeDef status;
	
	SPIx_StructInit(&SPI3_Init);
	TMC_SPI_StructInit(&TMC_SPI3);
	TMC_SPI3.Instance = SPI3;
	TMC_SPI3.Init = &SPI3_Init;
	TMC_SPI_DeInit(&TMC_SPI3);
	TMC_SPI_Init(&TMC_SPI3);
	//TMC_SPI_SSN_Disable(&TMC_SPI1);
	if(Demo_Test == SPIx_Demo_Polling)
	{
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_TransmitReceive(&TMC_SPI3,SPITestBuffer_tx,SPITestBuffer_rx,16,1000000);
		if(TMC_SPI3.Init->Mode  == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
		return status;
	}
	if(Demo_Test == SPIx_Demo_IRQ)
	{
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_TransmitReceive_IT(&TMC_SPI3,SPITestBuffer_tx,SPITestBuffer_rx,16);
		if(status!= TMC_OK)
		return status;
		
		while(TMC_SPI3.State == TMC_SPI_STATE_BUSY_TX_RX);
		if(TMC_SPI3.State == TMC_SPI_STATE_ERROR)
		{
			TMC_SPI_DeInit(&TMC_SPI3);
			return TMC_ERROR;
		}
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
	}
	if(Demo_Test == SPIx_Demo_DMA)
	{
		SPI_DMA_tx_Init.Instance = DMA_Channel1;
		SPI_DMA_tx_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
		SPI_DMA_tx_Init.Init.DestInc = DMA_DEST_INC_DISABLE;
		SPI_DMA_tx_Init.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;
		SPI_DMA_tx_Init.Init.RequestSelect = DMA_REQUEST_SPI3_TX;
		SPI_DMA_tx_Init.Init.SrcInc = DMA_SRC_INC_ENABLE;
		SPI_DMA_tx_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
		SPI_DMA_tx_Init.Init.IT_Enable = DMA_IT_ENABLE;
		SPI_DMA_tx_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		SPI_DMA_tx_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		
		SPI_DMA_tx_Init.Parent = &TMC_SPI3;
		TMC_SPI3.hdmatx = &SPI_DMA_tx_Init;
//		
		SPI_DMA_rx_Init.Instance = DMA_Channel0;
		SPI_DMA_rx_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
		SPI_DMA_rx_Init.Init.DestInc = DMA_DEST_INC_ENABLE;
		SPI_DMA_rx_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;
		SPI_DMA_rx_Init.Init.RequestSelect = DMA_REQUEST_SPI3_RX;
		SPI_DMA_rx_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;
		SPI_DMA_rx_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
		SPI_DMA_rx_Init.Init.IT_Enable = DMA_IT_ENABLE;
		SPI_DMA_rx_Init.Parent = &TMC_SPI3;
//		
		SPI_DMA_rx_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		SPI_DMA_rx_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		SPI_DMA_rx_Init.Parent = &TMC_SPI3;
		TMC_SPI3.hdmarx = &SPI_DMA_rx_Init;
//		
		TMC_DMA_DeInit(&SPI_DMA_rx_Init);
		TMC_DMA_DeInit(&SPI_DMA_tx_Init);
		
		TMC_DMA_Init(&SPI_DMA_rx_Init);
		TMC_DMA_Init(&SPI_DMA_tx_Init);
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Enable(&TMC_SPI3);
		}
		status = TMC_SPI_TransmitReceive_DMA(&TMC_SPI3, SPITestBuffer_tx, SPITestBuffer_rx, 16, 0, 0);
		if(status!= TMC_OK)
		{
			TMC_DMA_DeInit(TMC_SPI3.hdmatx);
			TMC_SPI_DeInit(&TMC_SPI3); 
			return status;
		}
		while(TMC_SPI3.State != TMC_SPI_STATE_READY);
		if(TMC_SPI3.State == TMC_SPI_STATE_ERROR)
		{
			TMC_DMA_DeInit(TMC_SPI3.hdmatx);
			TMC_SPI_DeInit(&TMC_SPI3); 
		}
		if(TMC_SPI3.Init->Mode == SPI_MASTER_MODE)
		{
			TMC_SPI_SSN_Disable(&TMC_SPI3);
		}
	
	}
	return TMC_OK;
}

void SPI_DMA_Handle(void)
{
	if(DMA_GetITStatus(DMA_IT_GL1) == SET)
	{
		TMC_DMA_IRQHandler(TMC_SPI3.hdmatx);
	}
	if(DMA_GetITStatus(DMA_IT_GL0) == SET)
	{
		TMC_DMA_IRQHandler(TMC_SPI3.hdmarx);
	}
}

void Delay(void)
{
	uint32_t i;
	for(i = 0; i<10000000;i++);
}

void SPI1_Demo_Main(void)
{
	SPI_DemoMain();
}

