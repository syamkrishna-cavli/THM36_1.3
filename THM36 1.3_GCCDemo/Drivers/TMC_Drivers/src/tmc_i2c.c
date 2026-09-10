/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_i2c.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/

#include "thm36_1.3.h"
#include "tmc_dma.h"
#include "tmc_i2c.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_I2C_CONTROLLER
  * @brief I2C CONTROLLER TMC modules driver 
  * @{
  */

/** @defgroup TMC_I2C_CONTROLLER_private_defines TMC I2C CONTROLLER private defines
  * @{
  */

static uint8_t I2C_WaitFlagStateUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart);
static TMC_StatusTypeDef TMC_I2C_Start_Signal(I2C_HandleTypeDef *hi2c,uint32_t Timeout);
static TMC_StatusTypeDef TMC_I2C_Stop_Signal(I2C_HandleTypeDef *hi2c,uint32_t Timeout);
static TMC_StatusTypeDef TMC_I2C_SendSalveAddr(I2C_HandleTypeDef *hi2c,uint8_t salveAddr,uint8_t mode,uint32_t Timeout);
static TMC_StatusTypeDef TMC_I2C_SendDestAddress(I2C_HandleTypeDef *hi2c, uint16_t desAddr,uint32_t Timeout);
static TMC_StatusTypeDef TMC_I2C_SendData(I2C_HandleTypeDef *hi2c,uint32_t Timeout);
static TMC_StatusTypeDef TMC_I2C_ReceiveData(I2C_HandleTypeDef *hi2c,uint32_t Timeout);
#define DATA_WRITE_DIR 0	
#define DATA_READ_DIR  1
/**
  * @}
  */ 

/** @defgroup TMC_I2C_CONTROLLER_Exported_Function TMC I2C CONTROLLER Exported Function
  * @brief I2C TMC modules driver
  * @{
  */

/**
  * @brief  Initializes the I2C Unit according to the specified
  *		 parameters in the SPI_HandleTypeDef and create the associated handle.
  * @param[in]  hi2c: pointer to a I2C_HandleTypeDef structure that contains
  *		 the configuration information for I2C module.
  * @retval None
  */
TMC_StatusTypeDef TMC_I2C_Init(I2C_HandleTypeDef* hi2c) 
{

	/* check point of hi2c is NULL */
	if(hi2c == NULL)
	{
		return TMC_ERROR;
	}
	/* check state of I2C is RESERT */
	if(hi2c->State == TMC_I2C_STATE_RESET)
	{
	 /* Init the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_I2C_MspInit(hi2c);
	}
	
	/* Disable I2C */
	I2C_Cmd(hi2c->Instance,DISABLE);
	/* Initializes I2Cx */
	I2C_Init(hi2c->Instance,hi2c->Init);
	
	hi2c->ErrorCode = I2C_STS_NO_STATUS_INFORMATION;

	/* ENABLE I2Cx */
	I2C_Cmd(hi2c->Instance,ENABLE);
	I2C_ITConfig(hi2c->Instance,ENABLE);
	/* Set state of I2Cx is BUSY */
	hi2c->State = TMC_I2C_STATE_READY;
	I2C_OutputStartSignal(hi2c->Instance,NONE_GENERATE_STARTING_SIGNAL);
	I2C_OutputStopSignal(hi2c->Instance,NONE_GENERATE_STOP_SIGNAL);
	I2C_Cmd(hi2c->Instance,ENABLE);
	return TMC_OK;
}	

/**
  * @brief  Initializes the I2C Unit according to the specified
  *		 parameters in the I2C_HandleTypeDef and create the associated handle.
  * @param[in]  hi2c: pointer to a I2C_HandleTypeDef structure that contains
  *		 the configuration information for I2C module.
  * @retval None
  */
TMC_StatusTypeDef TMC_I2C_DeInit(I2C_HandleTypeDef* hi2c) 
{

	/* check point of hi2c is NULL */
	if(hi2c == NULL)
	{
		return TMC_ERROR;
	}
	/* check state of i2c is RESERT */
	if(hi2c->State == TMC_I2C_STATE_READY)
	{
	 /* Init the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_I2C_MspDeInit(hi2c);
	}
	
	/* Initializes i2c */
	I2C_DeInit(hi2c->Instance);	
	hi2c->ErrorCode = I2C_STS_NO_STATUS_INFORMATION;
	hi2c->State = TMC_I2C_STATE_RESET;
	return TMC_OK;
}

/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *				the configuration information for the specified I2C.
  * @param  DevAddress Target device address The device 7 bits address value
  *		 in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  **/
TMC_StatusTypeDef TMC_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint8_t salveAddr,uint16_t desAddr, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	//volatile uint8_t State;
	TMC_StatusTypeDef temp_State;
	if((pData == NULL)||(Size == NULL)||(Timeout == NULL))
	{
		return TMC_ERROR;
	}
	if(hi2c->State != TMC_I2C_STATE_READY)
	{
		return TMC_ERROR;	
	}
	hi2c->State = TMC_I2C_STATE_BUSY;
	hi2c->pBuffPtr = pData;
	hi2c->XferSize = Size;
	hi2c->XferCount = 0;
	/*Start Signal*/
	temp_State = TMC_I2C_Start_Signal(hi2c,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	/*Send Salve Address*/
	temp_State =TMC_I2C_SendSalveAddr(hi2c,salveAddr,DATA_WRITE_DIR,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	
	/*Send desAddr*/
    temp_State = TMC_I2C_SendDestAddress(hi2c, ((desAddr & 0xFF00) >> 8), Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	
	/*Send desAddr*/
    temp_State = TMC_I2C_SendDestAddress(hi2c, desAddr, Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	
	/*Send Data*/
	temp_State = TMC_I2C_SendData(hi2c,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	/*Stop Signal*/
	return TMC_I2C_Stop_Signal(hi2c,Timeout);
	

	
}


/**
  * @brief  Receives in master mode an amount of data in blocking mode. 
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *				the configuration information for the specified I2C.
  * @param  DevAddress Target device address The device 7 bits address value
  *		 in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval TMC_StatusTypeDef
  */

TMC_StatusTypeDef TMC_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint8_t salveAddr, uint16_t desAddr, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	TMC_StatusTypeDef temp_State;
	if((pData == NULL)||(Size == NULL)||(Timeout == NULL))
	{
		return TMC_ERROR;
	}
	if(hi2c->State != TMC_I2C_STATE_READY)
	{
		return TMC_ERROR;	
	}
	hi2c->State = TMC_I2C_STATE_BUSY;
	hi2c->pBuffPtr = pData;
	hi2c->XferSize = Size;
	hi2c->XferCount = 0;
	
	/*Start Signal*/
	temp_State = TMC_I2C_Start_Signal(hi2c,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	/*Send Salve Address+W*/
	temp_State =TMC_I2C_SendSalveAddr(hi2c,salveAddr,DATA_WRITE_DIR,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	
	/*Send desAddr*/
    temp_State = TMC_I2C_SendDestAddress(hi2c, ((desAddr & 0xFF00) >> 8), Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	
	/*Send desAddr*/
    temp_State = TMC_I2C_SendDestAddress(hi2c, desAddr, Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	
	/*Stop Signal*/
	temp_State = TMC_I2C_Stop_Signal(hi2c,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	/*Start Signal*/
	temp_State = TMC_I2C_Start_Signal(hi2c,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	/*Send Salve Address+R*/
	temp_State =TMC_I2C_SendSalveAddr(hi2c,salveAddr,DATA_READ_DIR,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	temp_State = TMC_I2C_ReceiveData(hi2c,Timeout);
	if(temp_State!= TMC_OK)
	{
		return temp_State;
	}
	return TMC_I2C_Stop_Signal(hi2c,Timeout);
}

static TMC_StatusTypeDef TMC_I2C_Start_Signal(I2C_HandleTypeDef *hi2c,uint32_t Timeout)
{
	volatile uint8_t State;
	/* I2C send Start signal */
	I2C_OutputStopSignal(hi2c->Instance,NONE_GENERATE_STOP_SIGNAL);
	I2C_OutputStartSignal(hi2c->Instance,GENERATE_STARTING_SIGNAL);
	I2C_ClearStatusChange(hi2c->Instance);
	State = I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
	/* Wait the start signal to complete*/
	if((State!= I2C_MASTER_STS_START)&&(State!= I2C_MASTER_STS_REPEAT_START))
	{
		if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		hi2c->State = TMC_I2C_STATE_ERROR;
		hi2c->ErrorCode = State;
		return TMC_ERROR;
	}
	/* Clear the start signal to complete*/
	I2C_OutputStartSignal(hi2c->Instance,NONE_GENERATE_STARTING_SIGNAL);
	return TMC_OK;
}


static TMC_StatusTypeDef TMC_I2C_SendData(I2C_HandleTypeDef *hi2c,uint32_t Timeout)
{
	volatile uint8_t State;
	/* Data SEND*/
	while(hi2c->XferCount<hi2c->XferSize)
	{
		I2C_SendData(hi2c->Instance,(*hi2c->pBuffPtr));
		I2C_ClearStatusChange(hi2c->Instance);
		State = I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
		if(State != I2C_MASTER_SENDED_STS_DATA_AND_GET_ACK)
		{
			if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
			{
				return TMC_TIMEOUT;
			}
			hi2c->State = TMC_I2C_STATE_ERROR;
			hi2c->ErrorCode = State;
			return TMC_ERROR;
		}
		hi2c->pBuffPtr++;
		hi2c->XferCount++;
	}
	return TMC_OK;

}


static TMC_StatusTypeDef TMC_I2C_ReceiveData(I2C_HandleTypeDef *hi2c,uint32_t Timeout)
{
	volatile uint8_t State;
	/* Data SEND*/
	while(hi2c->XferCount<hi2c->XferSize)
	{
		I2C_configResponse(hi2c->Instance,I2C_CLK_ANSWER_ACK);
		I2C_ClearStatusChange(hi2c->Instance);
		State = I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
		if(State != I2C_MASTER_RECEIVED_STS_DATA_AND_SEND_ACK)
		{
			if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
			{
				return TMC_TIMEOUT;
			}
			hi2c->State = TMC_I2C_STATE_ERROR;
			hi2c->ErrorCode = State;
			return TMC_ERROR;
		}
		hi2c->pBuffPtr[hi2c->XferCount] = I2C_GetData(hi2c->Instance);
		hi2c->XferCount++;
	}
	return TMC_OK;

}

static TMC_StatusTypeDef TMC_I2C_Stop_Signal(I2C_HandleTypeDef *hi2c,uint32_t Timeout)
{
	volatile uint8_t State;
	/* STOP SIGNAL SEND*/
	I2C_OutputStopSignal(hi2c->Instance,GENERATE_STOP_SIGNAL);
	I2C_ClearStatusChange(hi2c->Instance);
	State = I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
	if(State != I2C_MASTER_SENDED_STS_STOP)
	{
		if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		hi2c->State = TMC_I2C_STATE_ERROR;
		hi2c->ErrorCode = State;
		return TMC_ERROR;
	}
//	I2C_OutputStartSignal(hi2c->Instance,NONE_GENERATE_STARTING_SIGNAL);
//	I2C_OutputStopSignal(hi2c->Instance,NONE_GENERATE_STOP_SIGNAL);
	hi2c->State = TMC_I2C_STATE_READY;
	hi2c->ErrorCode = State;
	return TMC_OK;
}

static TMC_StatusTypeDef TMC_I2C_SendDestAddress(I2C_HandleTypeDef *hi2c, uint16_t desAddr,uint32_t Timeout)
{
	volatile uint8_t State;
	/* I2C Send desAddr*/
	I2C_SendData(hi2c->Instance,desAddr);
	/* Clear the state of I2C*/
	I2C_ClearStatusChange(hi2c->Instance);
	/* Wait the  desAddr to complete*/
	State = I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
	if(State != I2C_MASTER_SENDED_STS_DATA_AND_GET_ACK)
	{
		if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		hi2c->State = TMC_I2C_STATE_ERROR;
		hi2c->ErrorCode = State;
		return TMC_ERROR;
	}
	return TMC_OK;
}


static TMC_StatusTypeDef TMC_I2C_SendSalveAddr(I2C_HandleTypeDef *hi2c,uint8_t salveAddr,uint8_t mode,uint32_t Timeout)
{
	volatile uint8_t State;
	/* I2C Send Salve Addres + Write */
	I2C_SendData(hi2c->Instance,(salveAddr<<1)|mode);
	/* Clear the state of I2C*/
	I2C_ClearStatusChange(hi2c->Instance);
	/* Wait the  Addres + Write to complete*/
	State = I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
	
	if(mode == DATA_WRITE_DIR)
	{
		if(State != I2C_MASTER_SENDED_STS_SLA_W_AND_GET_ACK)
		{
			if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
			{
				I2C_OutputStopSignal(hi2c->Instance,GENERATE_STOP_SIGNAL);
				I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
				return TMC_TIMEOUT;
			}
			hi2c->State = TMC_I2C_STATE_ERROR;
			hi2c->ErrorCode = State;
			return TMC_ERROR;
		}
	}
	else if(mode == DATA_READ_DIR)
	{
		if(State != I2C_MASTER_RECEIVED_STS_SLA_R_AND_GET_ACK)
		{
			if(hi2c->State == TMC_I2C_STATE_TIMEOUT)
			{
				I2C_OutputStopSignal(hi2c->Instance,GENERATE_STOP_SIGNAL);
				I2C_WaitFlagStateUntilTimeout(hi2c,Timeout,TMC_GetTick());
				return TMC_TIMEOUT;
			}
			hi2c->State = TMC_I2C_STATE_ERROR;
			hi2c->ErrorCode = State;
			return TMC_ERROR;
		}
	}
	return TMC_OK;
}

/**
  * @brief  I2C STS Change Timeout.
  * @param hi2c pointer to a I2C_TypeDef structure that contains
  *			  the configuration information for I2C module.
  * @param Timeout Timeout duration
  * @param State Expect state
  * @param Tickstart tick start value
  * @retval TMC status
  */
static uint8_t I2C_WaitFlagStateUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
	volatile uint8_t Status;
	while(!((hi2c->Instance->CON)&I2C_CLK_INT_Flag))
	{
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
			{
				hi2c->State = TMC_I2C_STATE_TIMEOUT;
				hi2c->ErrorCode = I2C_STS_NO_STATUS_INFORMATION;
				return I2C_STS_NO_STATUS_INFORMATION;
			}
		}
	}
	Status = hi2c->Instance->STS;
	return Status;
}


//static uint8_t TMC_Salve_ReceiveCheckAddress0(I2C_HandleTypeDef *hi2c,uint32_t Timeout, uint32_t Tickstart)
//{
//	uint8_t status;
//	while(!((hi2c->Instance->CON)&I2C_CLK_INT_Flag))
//	{
//		if(Timeout != HAL_MAX_DELAY)
//		{
//			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
//			{
//				hi2c->State = TMC_I2C_STATE_TIMEOUT;
//				hi2c->ErrorCode = TMC_I2C_ERROR_TIMEOUT;
//				return TMC_TIMEOUT;
//			}
//		}
//	}
//	return hi2c->Instance->STS;
//	
//}


//static TMC_StatusTypeDef TMC_Salve_WriteCheckAddress0(I2C_HandleTypeDef *hi2c,uint32_t Timeout, uint32_t Tickstart)
//{
//	uint8_t status;
//	while(!((hi2c->Instance->CON)&I2C_CLK_INT_Flag))
//	{
//		if(Timeout != HAL_MAX_DELAY)
//		{
//			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
//			{
//				hi2c->State = TMC_I2C_STATE_TIMEOUT;
//				hi2c->ErrorCode = TMC_I2C_ERROR_TIMEOUT;
//				return TMC_TIMEOUT;
//			}
//		}
//	}
//	status = hi2c->Instance->STS;
//	hi2c->ErrorCode = status;
//	if((status == I2C_MASTER_RECEIVED_STS_SLA_R_AND_GET_ACK)||(status == I2C_SLAVE_RECEIVED_STS_BROADCAST_AND_SEND_ACK))
//	{
//		return TMC_ERROR;
//	}
//	return TMC_OK;
//}


//static uint8_t TMC_CheckStateCode(I2C_TypeDef* I2Cx,I2C_StateMachine State)
//{
//	volatile uint8_t i,status;
//	status = I2Cx->STS;
//	if(status == State)
//	{
//		for(i=0;i<26;i++)
//		{
//			if(State == I2C_StateMachine_Table[i][0])
//			{
//				return I2C_StateMachine_Table[i][1];
//			}
//		}
//	}
//	else
//	{
//		for(i=0;i<26;i++)
//		{
//			if(status== I2C_StateMachine_Table[i][0])
//			{
//				return I2C_StateMachine_Table[i][1];
//			}
//		}
//	}
//}
/**
  * @brief  Initialize the I2C MSP.
  * @param  hspi pointer to a I2C_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hi2c);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_I2C_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the I2C MSP.
  * @param  hspi pointer to a I2C_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hi2c);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_I2C_MspInit should be implemented in the user file
	*/
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
