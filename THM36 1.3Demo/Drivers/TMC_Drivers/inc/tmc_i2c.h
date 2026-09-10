/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: TMC_I2C.h 
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef _TMC_I2C_H
#define _TMC_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif  

 
#include "I2C.h"	

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_I2C
  * @brief I2C TMC modules driver 
  * @{
  */

/** @defgroup TMC_I2C_exported_Types I2C exported types
  * @{
  */
typedef enum
{
  I2C_MODE_MASTER			 = 0x00U,   /*!< I2C communication is in Master Mode	   */
  I2C_MODE_SLAVE			  = 0x01U,   /*!< I2C communication is in Slave Mode		*/
}I2C_ModeTypeDef;


typedef enum
{
	I2C_MASTER_STS_START									= 0x08U,   /*!< MASTER of I2C Strart Signal have been Sended*/
	I2C_MASTER_STS_REPEAT_START 							= 0x10U,   /*!< MASTER of I2C Repeat Signal have been Sended*/
	I2C_MASTER_SENDED_STS_STOP								= 0xE0U,   /*!< MASTER of I2C  STOP Signal have been Sended*/
	
	I2C_MASTER_SENDED_STS_SLA_W_AND_GET_ACK 				= 0x18U,   /*!< MASTER of I2C  SALVE ADDRES+WRITE Bit have been Sended,and have Geted ACK */
	I2C_MASTER_SENDED_STS_SLA_W_AND_GET_NACK 				= 0x20U,   /*!< MASTER of I2C  SALVE ADDRES+WRITE Bit have been Sended,and have Geted NACK*/
	I2C_MASTER_SENDED_STS_DATA_AND_GET_ACK 					= 0x28U,   /*!< MASTER of I2C DATA have been Sended,and have Geted ACK*/
	I2C_MASTER_SENDED_STS_DATA_AND_GET_NACK 				= 0x30U,   /*!< MASTER of I2C DATA have been Sended,and have Geted NACK*/
	
	I2C_MASTER_RECEIVED_STS_SLA_R_AND_GET_ACK   			= 0x40U,   /*!< MASTER of I2C SALVE ADDRES+READ Bit have been Sended,and have Geted ACK*/
	I2C_MASTER_RECEIVED_STS_SLA_R_AND_GET_NACK   			= 0x48U,   /*!< MASTER of I2C SALVE ADDRES+READ Bit have been Sended,and have Geted NACK*/
	I2C_MASTER_RECEIVED_STS_DATA_AND_SEND_ACK   			= 0x50U,   /*!< MASTER of I2C DATA have been Received,and have Sended ACK*/
	I2C_MASTER_RECEIVED_STS_DATA_AND_SEND_NACK   			= 0x58U,   /*!< MASTER of I2C DATA have been Received,and have Sended NACK*/
	
	I2C_SLAVE_SENDED_STS_GETED_SLA_R_AND_SEND_ACK  			= 0xA8U,   /*!< SLAVE of I2C  have been Received the same Address0,and have sended ACK*/
	I2C_SLAVE_SENDED_STS_MASTER_ERROR_SEND_ACK  			= 0xB0U,   /*!< SLAVE of I2C MASTER have Error, SLAVE have recived SALVE ADDRES+READ and have sended ACK*/
	I2C_SLAVE_SENDED_STS_DATA_SEND_GET_ACK  				= 0xB8U,   /*!< SLAVE of I2C Data have been Send Data and have sended ACK*/
	I2C_SLAVE_SENDED_STS_DATA_SEND_GET_NACK  				= 0xC0U,   /*!< SLAVE of I2C Data have been Send Data and have sended NACK*/
	I2C_SLAVE_SENDED_STS_DATA_LAST_BYTE_SEND_GET_ACK  		= 0xC8U,   /*!< SLAVE of I2C the Last Data have been Sended and Received ACK*/
	
	
	I2C_SLAVE_RECEIVED_STS_SLA_W_AND_SEND_ACK				= 0x60U,	/*!< SLAVE of I2C have been Received the same SLA+W,and have sended ACK*/
	I2C_SLAVE_RECEIVED_STS_MASTER_ERROR_AND_SEND_ACK 		= 0x68U,	/*!< SLAVE of I2C MASTER have Error, SLAVE have recived SALVE ADDRES+WRITE and have sended ACK*/
	I2C_SLAVE_RECEIVED_STS_BROADCAST_AND_SEND_ACK 			= 0x70U,	/*!< SLAVE of I2C have been Received Broadcast address,and have sended ACK*/
	I2C_SLAVE_RECEIVED_STS_BROADCAST_ERROR_AND_SEND_ACK 	= 0x78U,	/*!< SLAVE of I2C MASTER have Error, SLAVE have received Broadcast address and have sended ACK*/
	I2C_SLAVE_RECEIVED_STS_GET_DATA_AND_SEND_ACK 			= 0x80U,	/*!< SLAVE of I2C SLA+R mode,Data have received and have sended ACK*/
	I2C_SLAVE_RECEIVED_STS_GET_DATA_AND_SEND_NACK 			= 0x88U,	/*!< SLAVE of I2C SLA+R mode,Data have received and have sended NACK */
	I2C_SLAVE_RECEIVED_STS_GET_BROADCAST_DATA_AND_SEND_ACK 	= 0x90U,	/*!< SLAVE of I2C Broadcast mode,Data have received and have sended ACK */
	I2C_SLAVE_RECEIVED_STS_GET_BROADCAST_DATA_AND_SEND_NACK = 0x98U,	/*!< SLAVE of I2C Broadcast mode,Data have received and have sended NACK */
	

	I2C_STS_GET_STOP_OR_REPEATED_START						= 0xA0U,   	/*!< have been Received STOP Signal,REPEATED Signal*/
	I2C_STS_ARBITRATION_LOSE	   							= 0x38U,   	/*!< ARBITRATION LOSE */
	I2C_STS_NO_STATUS_INFORMATION							= 0xF8U,
	I2C_STS_BUS_ERROR										= 0U
	
}I2C_StateMachine;



/** 
  * @brief  SPI State structures definition  
  */ 
typedef enum
{
	TMC_I2C_STATE_RESET	  = 0x00U,	/*!< Peripheral not Initialized						 */
	TMC_I2C_STATE_READY	  = 0x01U,	/*!< Peripheral Initialized and ready for use		   */
	TMC_I2C_STATE_BUSY	   = 0x02U,	/*!< an internal process is ongoing					 */
	TMC_I2C_STATE_ERROR	  = 0x06U,	 /*!< I2C error state									*/
	TMC_I2C_STATE_TIMEOUT	 = 0x07U
}I2C_StateTypeDef;

/**
  * @brief  I2C handle Structure definition
  */
typedef struct
{
	I2C_TypeDef		   *Instance;	  	/*!< I2C registers base address			*/										 
	I2C_InitTypeDef	   *Init;		   	/*!< I2C Inital							*/
	uint8_t			   *pBuffPtr;	  	/*!< Pointer to I2C transfer buffer		*/											 
	uint16_t  			  XferSize;	   	/*!< I2C transfer size	   			*/										
	uint16_t			  XferCount;	  	/*!< I2C transfer counter			   */																				  
	I2C_StateTypeDef  	  State;		  	/*!< I2C communication state			*/
	__IO uint8_t	  	  ErrorCode;	  	/*!< I2C Error code				 	*/
	
}I2C_HandleTypeDef;

//#define TMC_I2C_ERROR_ACK	   						0x00U   
//#define TMC_I2C_ERROR_NONE	  						0x00U   
//#define TMC_I2C_ERROR_TIMEOUT							0x01U	
//#define TMC_I2C_ERROR_NACK		 						0x02U   
//#define TMC_I2C_ERROR_ARBITRATION_LOSE  				0x03U	
//#define TMC_I2C_ERROR_NO_STATUS_INFORMATION				0x08U
//#define TMC_I2C_ERROR_NO_BUS_ERROR						0x09U
//#define TMC_I2C_ERROR_START_SEND_OK	 				0x04U	
//#define TMC_I2C_ERROR_STOP_SEND_OK	 					0x05U   
//#define TMC_I2C_ERROR_REPEAT_START_SEND_OK				0x06U
//#define TMC_I2C_ERROR_REPEAT_START_OR_STOP_RECEIVED_OK	0x07U




/** @defgroup I2C_Error_Code I2C Error Code
  * @brief	I2C Error Code 
  * @{
  */ 
  

/**
  * @}
  */

/**
  * @}
  */
/** @addtogroup TMC_SPI_Exported_Function 
  * @{
  */
void TMC_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void TMC_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);
											  
TMC_StatusTypeDef TMC_I2C_Init(I2C_HandleTypeDef* hi2c);
TMC_StatusTypeDef TMC_I2C_DeInit(I2C_HandleTypeDef* hi2c);
TMC_StatusTypeDef TMC_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint8_t salveAddr, uint16_t desAddr, uint8_t *pData, uint16_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint8_t salveAddr, uint16_t desAddr, uint8_t *pData, uint16_t Size, uint32_t Timeout);
//TMC_StatusTypeDef TMC_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c,uint8_t *pData, uint16_t Size, uint32_t Timeout);
//TMC_StatusTypeDef TMC_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);

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
#ifdef __cplusplus
}
#endif

#endif /* _TMC_I2C_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/  


