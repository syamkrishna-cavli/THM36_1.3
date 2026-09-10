/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: i2c_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*   2023-07-04 Revise version---Ajust I2C timing sequence to satisfy EEPROM(M24256-BRMN6TP)
*********************************************************************************/   
#include "thm36_1.3.h"
#include "tmc_dma.h"
#include "rcc.h"
#include "tmc_gpio.h"
#include "tmc_i2c.h"
#include "i2c_demomain.h"

I2C_HandleTypeDef hi2c;
I2C_InitTypeDef   i2c_Init;
uint16_t Addr0;
uint8_t Txbuffer[64] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                        0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
                        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
                        0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
                        0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
                        0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
                        0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
                        0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F};
uint8_t Rxbuffer[64];

 /**
    * @brief I2C_DemoMaster: 
    *           1. I2C Init
    *           2. Page Write to EEPROM(M24256) which page size is 64 bytes.
    *           3. Read one page of EEPROM(M24256) to Rxbuffer
    * @param  None
    * @retval None
 */                    
void I2C_DemoMaster(void)
{
	hi2c.Init = &i2c_Init;
	hi2c.Instance = I2C;
	I2C_StructInit(hi2c.Init);
	TMC_I2C_Init(&hi2c);
	Addr0 = 0;
	TMC_I2C_Master_Transmit(&hi2c, 0x50, Addr0, Txbuffer, 64, 10000);
	delay_ms(5); //EEPROM page write requires 5ms
	TMC_I2C_Master_Receive(&hi2c, 0x50, Addr0, Rxbuffer, 64, 10000);
}
/**
  * @brief I2C clock enable, GPIO function config
  * @param hi2c pointer to a @ref I2C_HandleTypeDef structure
  *                         whose fields will be set to expected values.
  * @retval None
  */
void TMC_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIOx_Init;
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_I2C,ENABLE);
	/*Set the GPIOB Pin5,Pin6 as SDA and SCL pin.*/
	GPIO_PinAFConfig(GPIOB, GPIO_PIN_Source5, GPIO_AF1_I2C);
	GPIO_PinAFConfig(GPIOB, GPIO_PIN_Source6, GPIO_AF1_I2C);
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_5|GPIO_PIN_6;
	GPIOx_Init.Pull = GPIO_NOPULL;
	GPIOx_Init.Mode = GPIO_MODE_AF;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);
	
}

void I2C_DemoMain(void)
{
	I2C_DemoMaster();
}

