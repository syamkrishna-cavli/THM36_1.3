/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_uart.h"
#include "uart_demomain.h"
#include "string.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
#include "vbat_demomain.h"

//TMC(20240408)-zt:huartx->huart1,uart_init->uart_init1
 UART_HandleTypeDef huart1;
 UART_InitTypeDef uart_init1;
 DMA_HandleTypeDef hdma_UART1_rx;
 DMA_HandleTypeDef hdma_UART1_tx;



/* Private variables ---------------------------------------------------------*/
__attribute__ ((aligned (4))) uint8_t UART_TxBuf[256];
__attribute__ ((aligned (4))) uint8_t UART_RxBuf[256];



/*There is a '__weak' type UART_StructInit in UART.c,redefine a new PWM_StructInit will replace it.*/
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
	UART_InitStruct->BaudRate = 115200;
	UART_InitStruct->StopBits = UART_STOP_BIT_1;
	UART_InitStruct->DataWidth = UART_DATA_WIDTH_8;
	UART_InitStruct->ParityMode = UART_PARITY_MODE_NONE;
}

/*There is a '__weak' type TMC_UART_MspInit in TMC_UART.c,redefine a new TMC_UART_MspInit will replace it.*/
void TMC_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIOx_Init;
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);

	if(huart->Instance == UART1)
	{
        GPIO_PinAFConfig(GPIOB,GPIO_PIN_Source13,GPIO_AF4_UART1);
        GPIO_PinAFConfig(GPIOB,GPIO_PIN_Source14,GPIO_AF4_UART1);
        TMC_GPIO_StructInit(&GPIOx_Init);							/*Clear the GPIOx_Init structure.*/
        GPIOx_Init.Pin = GPIO_PIN_13|GPIO_PIN_14;					/*Select GPIO.*/
        GPIOx_Init.Mode = GPIO_MODE_AF;								/*Select AF function.*/
        GPIOx_Init.Speed = GPIO_HIGH_SPEED;							/*Set High speed.*/
        TMC_GPIO_Init(GPIOB, &GPIOx_Init);							/*GPIOB Initialize.*/
	}
	
    /*User can decide open UART1 clock or UART2 clock in condition.*/
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_UART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

	RCC_AHBClockCmd(RCC_AHBPeriph_DMA, ENABLE);					/*Enable DMA clock.*/
    /*User can decide open UART1 interrupt or UART2 interrupt in condition.*/
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);
	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);
}

/*There is a '__weak' type TMC_UART_MspDeInit in TMC_UART.c,redefine a new TMC_UART_MspDeInit will replace it.*/
void TMC_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIOx_Init;
	TMC_GPIO_StructInit(&GPIOx_Init);
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Otype = GPIO_MODE_OUTPUT_PP;
	GPIOx_Init.Pull = GPIO_PULLUP;
	GPIOx_Init.SMIT = GPIO_INPUTSCHMIT_DISABLE;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;	

	GPIOx_Init.Pin = GPIO_PIN_13|GPIO_PIN_14;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);

	RCC_APB3PeriphClockCmd(RCC_APB3Periph_UART2, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, DISABLE);
	
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_DisableIRQ(UART2_IRQn);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_DisableIRQ(UART1_IRQn);	
}

/*There is a '__weak' type TMC_UART_TxCpltCallback in TMC_UART.c,redefine a new TMC_UART_TxCpltCallback will replace it.*/
void TMC_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	//User can add code here.
}

/*There is a '__weak' type TMC_UART_RxCpltCallback in TMC_UART.c,redefine a new TMC_UART_RxCpltCallback will replace it.*/
void TMC_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//User can add code here.
}
/*There is a '__weak' type TMC_UART_ErrorCallback in TMC_UART.c,redefine a new TMC_UART_ErrorCallback will replace it.*/
void TMC_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	//User can add code here.
}
/**
    * @brief  UART send/receive by polling:
    *             Receive 16 bytes data Then send the 16 bytes data in query mode.
    *         UART send/receive by interruption:
    *             Receive 16 bytes data Then send the 16 bytes data in interrupt mode.
    *         UART send/receive by DMA:
    *             Receive 16 bytes data Then send the 16 bytes data in DMA mode.
    * @param  None
    * @retval None
 */
void UART_DemoMain(void)
{
	Uartx_Demo_TypefDef Mode;
	UART_StructInit(&uart_init1);
	huart1.Init = &uart_init1;
	huart1.Instance = UART1;
	TMC_UART_StructInit(&huart1);
	TMC_UART_Init(&huart1);
	Mode = Uartx_Demo_DMA;
	
	if(Mode == Uartx_Demo_Polling)
	{
		TMC_UART_Receive(&huart1,UART_RxBuf,16,10000000);
		TMC_UART_Transmit(&huart1, UART_RxBuf,16, 10000);
		
	}
	if(Mode == Uartx_Demo_IRQ)
	{
		TMC_UART_Receive_IT(&huart1,UART_RxBuf,16);
		while(huart1.RxState == TMC_UART_STATE_BUSY);
		TMC_UART_Transmit_IT(&huart1,UART_RxBuf,16);
		while(huart1.TxState == TMC_UART_STATE_BUSY);
		
	}
	if(Mode == Uartx_Demo_DMA)
	{
		hdma_UART1_rx.Instance = DMA_Channel1;							/*Enable DMA channel 1.*/
		hdma_UART1_rx.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;		/*DMA copy in byte.*/
		hdma_UART1_rx.Init.DestInc = DMA_DEST_INC_ENABLE;				/*DMA Destination Address increase.*/
		hdma_UART1_rx.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;		/*DMA memory to peripheral.*/
		hdma_UART1_rx.Init.RequestSelect = DMA_REQUEST_UART1_RX;		/*DMA request: UART1_RX.*/
		hdma_UART1_rx.Init.SrcInc = DMA_SRC_INC_DISABLE;				/*DMA Source Address decrease.*/
		hdma_UART1_rx.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;		/*DMA copy for Source address in BYTE.*/
		hdma_UART1_rx.Init.IT_Enable = DMA_IT_ENABLE;
		hdma_UART1_rx.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		hdma_UART1_rx.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		huart1.hdmarx = &hdma_UART1_rx;
		
		hdma_UART1_tx.Instance = DMA_Channel0;							/*Enable DMA channel 0.*/
		hdma_UART1_tx.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;		/*DMA copy in byte.*/
		hdma_UART1_tx.Init.DestInc = DMA_DEST_INC_DISABLE;				/*DMA Destination Address increase.*/
		hdma_UART1_tx.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;		/*DMA memory to peripheral.*/
		hdma_UART1_tx.Init.RequestSelect = DMA_REQUEST_UART1_TX;		/*DMA request: UART1_TX.*/
		hdma_UART1_tx.Init.SrcInc = DMA_SRC_INC_ENABLE;					/*DMA Source Address decrease.*/
		hdma_UART1_tx.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;		/*DMA copy for Source address in BYTE.*/
		hdma_UART1_tx.Init.IT_Enable = DMA_IT_ENABLE;
		hdma_UART1_tx.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		hdma_UART1_tx.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		huart1.hdmatx = &hdma_UART1_tx;
		TMC_DMA_DeInit(&hdma_UART1_rx);
		TMC_DMA_DeInit(&hdma_UART1_tx);
		TMC_DMA_Init(huart1.hdmarx);
		TMC_DMA_Init(huart1.hdmatx);
		TMC_UART_Receive_DMA_IT(&huart1,UART_RxBuf,16,0);
		while(huart1.RxState == TMC_UART_STATE_BUSY);
		TMC_UART_Transmit_DMA_IT(&huart1,UART_RxBuf,16,0);
		while(huart1.TxState == TMC_UART_STATE_BUSY);
	}	
	
	return;
}

void UART_DMA_Handle(void)
{
	if(DMA_GetITStatus(DMA_IT_GL1) == SET)
	{
		TMC_DMA_IRQHandler(huart1.hdmarx);
	}
	if(DMA_GetITStatus(DMA_IT_GL0) == SET)
	{
		TMC_DMA_IRQHandler(huart1.hdmatx);
	}
}

void UART_Demo_Main(void)
{
	UART_DemoMain();
}

/**
  * @}
  */ 

/**
  * @}
  */

