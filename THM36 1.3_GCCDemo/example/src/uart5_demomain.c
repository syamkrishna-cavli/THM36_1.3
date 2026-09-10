/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart5_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   
#include "thm36_1.3.h"
#include "tmc_uart.h"
#include "tmc_uart5.h"
#include "uart5_demomain.h"
#include "string.h"
#include "tmc_rcc.h"
#include "tmc_gpio.h"
/*TMC(20240409)-gsx:DMA+IDLE soft flow control DMA Receive length */
#define UART5_TEST_DATALENGHT     14

UART5_HandleTypeDef huart5;
UART5_InitTypeDef uart5_init;
DMA_HandleTypeDef hdma_UART5_rx;
DMA_HandleTypeDef hdma_UART5_tx;

/* Private variables ---------------------------------------------------------*/
__attribute__ ((aligned(4))) uint8_t UART5_TxBuf[2048];
__attribute__ ((aligned(4))) uint8_t UART5_RxBuf[2048];

static __attribute__ ((aligned(4))) LLI_StructTypeDef UART5_receive_lli[5];

volatile uint8_t receive_flag = 0;
volatile uint8_t receive_ilde_flag = 0;
volatile uint8_t IsRxFlag = 0;

/*TMC(20240409)-gsx:DMA+IDLE soft flow control Link-Receive DMA Receive length */
void TMC_UART5_DMA_LLI_Init(void)
{
    for(int i=0; i < UART5_TEST_DATALENGHT*2; i++)
    {
        UART5_RxBuf[i] = 0;
    }

    UART5_receive_lli[0].source_addr = (uint32_t)(&(UART5->RDR));
    UART5_receive_lli[0].dest_addr = (uint32_t)UART5_RxBuf + UART5_TEST_DATALENGHT;
    UART5_receive_lli[0].lli_addr = ((uint32_t)(&UART5_receive_lli[1]));
    /* The length can be configured by user */
    UART5_receive_lli[0].ctrl = Bit31_En | Bit27_En | Bit25_En | UART5_TEST_DATALENGHT;
    
    UART5_receive_lli[1].source_addr = (uint32_t)(&(UART5->RDR));
    UART5_receive_lli[1].dest_addr = (uint32_t) UART5_RxBuf;
    UART5_receive_lli[1].lli_addr = ((uint32_t)(&UART5_receive_lli[0]));
    /* The length can be configured by user */
    UART5_receive_lli[1].ctrl = Bit31_En | Bit27_En | Bit25_En | UART5_TEST_DATALENGHT;
}

/**
  * @brief Set each @ref UART5_InitTypeDef field to expected value.
  * @param UART_InitStruct pointer to a @ref UART5_InitTypeDef structure
  *                         whose fields will be set to expected values.
  * @retval None
  */
void UART5_StructInit(UART5_InitTypeDef* UART_InitStruct)
{
	UART_InitStruct->ClockPrescaler = UART5_PRESCALER_DIV1;
	UART_InitStruct->BaudRate = 3000000;
	UART_InitStruct->WordLength = UART5_WordLength_8b;
	UART_InitStruct->Parity = UART5_Parity_None;
	UART_InitStruct->StopBits = UART5_StopBits_1; 
	UART_InitStruct->Mode = UART5_Mode_Rx | UART5_Mode_Tx;
	UART_InitStruct->HardwareFlowControl = UART5_HardwareFlowControl_RTS_CTS;/*TMC(20240409)-gsx:DMA+IDLE soft flow control need set:UART5_HardwareFlowControl_None*/
	UART_InitStruct->FIFOMode = UART5_FIFOMODE_ENABLE;/*TMC(20240409)-gsx:DMA+IDLE soft flow control need set:UART5_FIFOMODE_DISABLE*/
	UART_InitStruct->TXFIFOThreshold = UART5_TXFIFO_THRESHOLD_1_8;
	UART_InitStruct->RXFIFOThreshold = UART5_RXFIFO_THRESHOLD_1_8;
	UART_InitStruct->TxRx_Swap = UART5_NSWAP_TXRX;  
}
/**
  * @brief Enable clock,Config UART5 GPIO function, Config IRQ, DMA
  * @param huart pointer to a @ref UART5_HandleTypeDef structure
  *                         whose fields will be set to expected values.
  * @retval None
  */
void TMC_UART5_MspInit(UART5_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIOx_Init;
	
	/*Enable gated clock*/
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_UART5, ENABLE);	
	/*Config UART5 clock source,div,enable*/
	RCC_UART5ClockConfig(RCC_UART5_CLKSRC_PLL_L,0,ENABLE);
    
    GPIO_PinAFConfig(GPIOA,GPIO_PIN_Source15,GPIO_AF4_UART5);
    GPIO_PinAFConfig(GPIOB,GPIO_PIN_Source0,GPIO_AF4_UART5);
    GPIO_PinAFConfig(GPIOB,GPIO_PIN_Source1,GPIO_AF4_UART5);
    GPIO_PinAFConfig(GPIOB,GPIO_PIN_Source2,GPIO_AF4_UART5);/*TMC(20240409)-gsx:DMA+IDLE soft flow control GPIOB2 need set to output mode*/
    
	/**************************Config UART5 GPIO function*******************************************/	
	/***********Init UART5_TX(GA_IO15),UART5_RX(GB_IO0),UART5_CTS(GB_IO1),UART5_RTS(GB_IO2)***************/
	TMC_GPIO_StructInit(&GPIOx_Init);							
	GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
    //GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1;  /*TMC(20240409)-gsx:DMA+IDLE soft flow control initialization GPIOB0 and GPIOB1 */
	GPIOx_Init.Mode = GPIO_MODE_AF;							
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;						
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);							
	
	GPIOx_Init.Pin = GPIO_PIN_15;								
	TMC_GPIO_Init(GPIOA, &GPIOx_Init);
    
    /*TMC(20240409)-gsx:DMA+IDLE soft flow control initialization GPIOB2 */
    //GPIOx_Init.Pin = GPIO_PIN_2;		
	//GPIOx_Init.Mode = GPIO_MODE_OUT;	 
	//GPIOx_Init.Speed = GPIO_HIGH_SPEED;						
	//TMC_GPIO_Init(GPIOB, &GPIOx_Init);	
    //TMC_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
    
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA, ENABLE);	/*Enable DMA clock*/
	/*Enable IRQ*/
	NVIC_ClearPendingIRQ(UART5_IRQn);
	NVIC_EnableIRQ(UART5_IRQn);
	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);

}
/**
  * @brief Enable clock,Config UART5 GPIO function, Config IRQ, DMA
  * @param huart pointer to a @ref UART5_HandleTypeDef structure
  *                         whose fields will be set to expected values.
  * @retval None
  */
void TMC_UART5_MspDeInit(UART5_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIOx_Init;
	TMC_GPIO_StructInit(&GPIOx_Init);
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Otype = GPIO_MODE_OUTPUT_PP;
	GPIOx_Init.Pull = GPIO_PULLUP;
	GPIOx_Init.SMIT = GPIO_INPUTSCHMIT_DISABLE;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;	

	GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);
	
	GPIOx_Init.Pin = GPIO_PIN_15;
	TMC_GPIO_Init(GPIOA, &GPIOx_Init);
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_UART5, DISABLE);
	
	NVIC_ClearPendingIRQ(UART5_IRQn);
	NVIC_DisableIRQ(UART5_IRQn);
}
/*There is a '__weak' type TMC_UART5_TxCpltCallback in TMC_UART5.c,redefine a new TMC_UART5_TxCpltCallback will replace it.*/
void TMC_UART5_TxCpltCallback(UART5_HandleTypeDef *huart)
{
	//add user callback function 
}
/*There is a '__weak' type TMC_UART5_RxCpltCallback in TMC_UART5.c,redefine a new TMC_UART5_RxCpltCallback will replace it.*/
void TMC_UART5_RxCpltCallback(UART5_HandleTypeDef *huart)
{
	//add user callback function 
}
/*There is a '__weak' type TMC_UART5_ErrorCallback in TMC_UART5.c,redefine a new TMC_UART5_ErrorCallback will replace it.*/
void TMC_UART5_ErrorCallback(UART5_HandleTypeDef *huart)
{
	//add user callback function 
}
/**
    * @brief  UART5 send/receive by polling:
    *             Receive 16 bytes data Then send the 16 bytes data in query mode.
    *         UART5 send/receive by interruption:
    *             Receive 16 bytes data Then send the 16 bytes data in interrupt mode.
    *         UART5 send/receive by DMA:
    *             Receive 16 bytes data Then send the 16 bytes data in DMA mode.
    * @param  None
    * @retval None
 */
void UART5_DemoMain(void)
{
	Uart5_Demo_TypefDef Mode;
	UART5_StructInit(&uart5_init);
	huart5.Init = &uart5_init;
	huart5.Instance = UART5;
	TMC_UART5_StructInit(&huart5);
	TMC_UART5_Init(&huart5);
	Mode = Uart5_Demo_IRQ;
	
	if(Mode == Uart5_Demo_Polling)
	{
        while(1)
        {
            TMC_UART5_Receive(&huart5,UART5_RxBuf,16,10000000);
            TMC_UART5_Transmit(&huart5, UART5_RxBuf,16, 10000);
        }			
	}
	if(Mode == Uart5_Demo_IRQ)
	{
        while(1)
        {
            /*TMC(20240409)-gsx:FIFO+RTS: IsRxFlag-in the interrupt mode receiving function, the data is read only after a delay of 1ms */
            IsRxFlag = 1;
            TMC_UART5_Receive_IT(&huart5,UART5_RxBuf,16);
            while(huart5.RxState == TMC_UART_STATE_BUSY);
            CLEAR_BIT(huart5.Instance->CR1, UART5_CR1_IDLEIE);//TMC-gsx:20240430:disable IDLE INT
            IsRxFlag = 0;
            TMC_UART5_Transmit_IT(&huart5,UART5_RxBuf,16);
            while(huart5.TxState == TMC_UART_STATE_BUSY);
            memset(UART5_RxBuf, 0, 16);
        }		
	}
	if(Mode == Uart5_Demo_DMA)
	{
		hdma_UART5_rx.Instance = DMA_Channel1;						
		hdma_UART5_rx.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;	
		hdma_UART5_rx.Init.DestInc = DMA_DEST_INC_ENABLE;			
		hdma_UART5_rx.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;	
		hdma_UART5_rx.Init.RequestSelect = DMA_REQUEST_UART5_RX;	
		hdma_UART5_rx.Init.SrcInc = DMA_SRC_INC_DISABLE;			
		hdma_UART5_rx.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;	
		hdma_UART5_rx.Init.IT_Enable = DMA_IT_ENABLE;
		hdma_UART5_rx.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		hdma_UART5_rx.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		huart5.hdmarx = &hdma_UART5_rx;
		
		hdma_UART5_tx.Instance = DMA_Channel0;
		hdma_UART5_tx.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;	
		hdma_UART5_tx.Init.DestInc = DMA_DEST_INC_DISABLE;				
		hdma_UART5_tx.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;		
		hdma_UART5_tx.Init.RequestSelect = DMA_REQUEST_UART5_TX;		
		hdma_UART5_tx.Init.SrcInc = DMA_SRC_INC_ENABLE;				
		hdma_UART5_tx.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;		
		hdma_UART5_tx.Init.IT_Enable = DMA_IT_ENABLE;
		hdma_UART5_tx.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
		hdma_UART5_tx.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
		huart5.hdmatx = &hdma_UART5_tx;
		TMC_DMA_DeInit(&hdma_UART5_rx);
		TMC_DMA_DeInit(&hdma_UART5_tx);
		TMC_DMA_Init(huart5.hdmarx);
		TMC_DMA_Init(huart5.hdmatx);
        /*TMC(20240409)-gsx:ADD DMA+IDLE soft flow control*/
        TMC_UART5_DMA_LLI_Init();
        TMC_UART5_Receive_DMA_IT(&huart5, UART5_RxBuf, UART5_TEST_DATALENGHT, (uint32_t)(&UART5_receive_lli[0]));        
        while(1)
        {
            if (receive_flag)
            {
                receive_flag = 0;
                CLEAR_BIT(huart5.Instance->CR1, UART5_CR1_IDLEIE);/*TMC(20240409)-gsx:disable IDLE INT*/
                if(hdma_UART5_rx.Instance->LLI == UART5_receive_lli[0].lli_addr)
                {
                    TMC_UART5_Transmit_DMA_IT(&huart5, UART5_RxBuf, UART5_TEST_DATALENGHT, 0);
                }
                else if(hdma_UART5_rx.Instance->LLI == UART5_receive_lli[1].lli_addr)
                {
                    TMC_UART5_Transmit_DMA_IT(&huart5, UART5_RxBuf+UART5_TEST_DATALENGHT, UART5_TEST_DATALENGHT, 0);
                }
                while(huart5.TxState == TMC_UART_STATE_BUSY);
                GPIOB->OD &= ~GPIO_PIN_2;/*TMC(20240409)-gsx:Pull down RTS*/
                SET_BIT(huart5.Instance->ICR, UART5_FLAG_IDLE);/*TMC(20240409)-gsx:clear IDLE status */
                SET_BIT(huart5.Instance->CR1, UART5_CR1_IDLEIE);/*TMC(20240409)-gsx:enable IDLE INT */               
            }
            else if(receive_ilde_flag)/*TMC(20240409)-gsx:DMA has not receive configure length but ilde*/
            {
                uint16_t recv_len = (UART5_TEST_DATALENGHT - hdma_UART5_rx.Instance->CONTROL & 0xfff);
                receive_ilde_flag = 0;
                DMA_Cmd(hdma_UART5_rx.Instance, DISABLE);                
                if(hdma_UART5_rx.Instance->LLI == UART5_receive_lli[1].lli_addr)
                {
                    TMC_UART5_Transmit_DMA_IT(&huart5, UART5_RxBuf, recv_len, 0);
                    hdma_UART5_rx.Instance->DESTADDR = (uint32_t)UART5_RxBuf + UART5_TEST_DATALENGHT;
                    hdma_UART5_rx.Instance->LLI = UART5_receive_lli[0].lli_addr; 
                }
                else
                {
                    TMC_UART5_Transmit_DMA_IT(&huart5, (UART5_RxBuf+UART5_TEST_DATALENGHT), recv_len, 0);
                    hdma_UART5_rx.Instance->DESTADDR = (uint32_t) UART5_RxBuf;
                    hdma_UART5_rx.Instance->LLI = UART5_receive_lli[1].lli_addr;                        
                }
                while(huart5.TxState == TMC_UART_STATE_BUSY);
                DMA_SetLength(hdma_UART5_rx.Instance, UART5_TEST_DATALENGHT);/*TMC(20240409)-gsx:Set the DMA Transfer length*/
                DMA_Cmd(hdma_UART5_rx.Instance, ENABLE);
                GPIOB->OD &= ~GPIO_PIN_2; /*TMC(20240409)-gsx:Pull down RTS*/
            }

        }
    }
    return;
}

void UART5_DMA_Handle(void)
{
	if(DMA_GetITStatus(DMA_IT_GL1) == SET)
	{
        CLEAR_BIT(huart5.Instance->CR1, UART5_CR1_IDLEIE);/*TMC(20240409)-gsx:disable IDLE INT*/
        huart5.Instance->ICR = 0x1F;/*TMC(20240409)-gsx:clear IDLE and error status*/
        receive_flag = 1;
        DMA_ClearITPendingBit(DMA_IT_TC1);
        
	}
	if(DMA_GetITStatus(DMA_IT_GL0) == SET)
	{
		TMC_DMA_IRQHandler(huart5.hdmatx);
	}
}

void UART5_Demo_Main(void)
{
	UART5_DemoMain();
}

/**
  * @}
  */ 

/**
  * @}
  */

