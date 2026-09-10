/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: main.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   
#include "thm36_1.3.h"
#include "gpio.h"
#include "rcc.h"
#include "tmc_gpio.h"
#include "tmc_uart.h"
#include "USB.h"
#include "tmc_flash.h"
#include "romTable.h"
#include "pwr.h"
#include "tmc_exti.h"
#include "tmc_vbat.h"
#include "tmc_dma.h"
#include "adc_demomain.h"
#include "flash_demomain.h"
#include "spi_demomain.h"
#include "tim_demomain.h"
#include "vbat_demomain.h"
#include "wdt_demomain.h"
#include "pwr_demomain.h"
#include "tmc_iso7816Mx.h"
#include "iso7816M_demomain.h"
#include "7816Mx_syncard_demomain.h"
#include "ISO7816_EMV_L1.h"
#include "tmc_dma.h"
#include "dma_demomain.h"
#include "uart_demomain.h"
#include "uart5_demomain.h"
#include "dma_demomain.h"
#include "pwm_demomain.h"
#include "tmc_spi_controller.h"
#include "spi2_W25Q256.h"
#include "i2c_demomain.h"
#include "mag_demomain.h"
#include "usb_demomain.h"
#include "crypto_demomain.h"
#include "sn_demomain.h"
#include "printfunc.h"


void InitHardware(void);

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

/* Test function pointer. */
typedef	void (* pTest)(void);
typedef void (* pDmaIRQHandle)(void);

void GetBLVersion(unsigned char *Version)
{
    Version[0] = BootLoderVersion;
}


void GetChipID(unsigned char *id)
{
    id[0] = CHIPID;
}

unsigned char g_RAM_init[4] = {0xAA, 0x55, 0x33, 0xCC};

/**
  * @name   main
  * @brief  The main process.
  * @param  None
  * @retval None
  */
int main(void)
{
	InitHardware();

    for (int i = 0; i < ARRAY_LENGTH(g_RAM_init); i++)
    {
	    UART_printf("g_RAM_init[%d] = ", i);
	    UART_printf("%#x\r\n", g_RAM_init[i]);
    }

	unsigned char BLVersion = 0x00, ChipID = 0x00;
	/* Get BLVersion */
	GetBLVersion(&BLVersion);
	//UART print Example
	UART_printf("THM36 1.3 BLVersion = %#x\r\n", BLVersion);
	
	GetChipID(&ChipID);
	/* If Chip ID is equal to 0x45, the chip is THM36 1.3 */
	if(CHIP_IS_THM36V13 == ChipID)
	{
		UART_printf("Chip ID = %#x\r\n", ChipID); 
		UART_printf("THM36 1.3 Demo!!!\r\n");
	}
		
	//void (* pTest)(void) = SN_DemoMain;					//This demo show how to get the Serial Number,and the structure of the Serial Number.
	//void (* pTest)(void) = USB_DemoMain;					//This demo show the usb function,the protocol can be CCID/HID/Virtual COM(VSP).
	//void (* pTest)(void) = Flash_CacheDemomain;			//This demo show the flash cache function.
	//void (* pTest)(void) = FLASH_DemoMain;				//This demo show the flash function.
	//void (* pTest)(void) = SPI_DemoMain;					//This demo show the SPI function by polling/interruption/DMA mode.
	//void (* pTest)(void) = TIMER_DemoMain;				//This demo show the TIMER function by 
	//void (* pTest)(void) = UART_DemoMain;					//This demo show UARTx(except UART5) receive and send data by polling/interruption/DMA mode.
	void (* pTest)(void) = UART5_DemoMain;				//This demo show UART5 receive and send data by polling/interruption/DMA mode.
	//void (* pTest)(void) = VBAT_DemoMain;					//This demo show the RTC,IWDT,SD,VD,TD function.
	//void (* pTest)(void) = WDT_Demo_Main;					//This demo show the WDT function.This Test doesn't enable the Reset function of WDT alarm.If open the WDT reset funciton,should consider to feed the watchdag,otherwise will cause the DEBUG port lose efficacy.
	//void (* pTest)(void) = ISO7816M2_AsynDemoMain;		//This demo show the 7816M2 Asyn Card function.
	//void (* pTest)(void) = ISO7816M2_SynDemoMain;			//This demo show the 7816M2 Syn card function.
	//void (* pTest)(void) = PWR_Stop_DemoMain;				//This demo show the Stop mode when GPIOB_14 is low level,wake up after 60 seconds by RTC alarm.
	//void (* pTest)(void) = PWR_Standby_DemoMain;			//This demo show the Standy mode when GPIOB_14 is low level,wake up after 60 seconds by RTC alarm.
	//void (* pTest)(void) = ADC_DeomMain;					//This demo show ADC function by polling/interruption/DMA mode
	//void (* pTest)(void) = DMA_DemoMain;					//This demo show the memcpy function by DMA mode.
	//void (* pTest)(void) = SPI2_DemoMain;					//This demo show SPI receive and send data by polling/interruption/DMA mode.
	//void (* pTest)(void) = PWM_DemoMain;					//This demo show PWM function by polling/Interruption mode.
	//void (* pTest)(void) = I2C_DemoMain;					//This demo show the master test with EEPROM(M24256-BRMN6TP) by I2C port.
	//void (* pTest)(void) = CRYPTO_DemoMain;				//This demo show the Crypto function.
	//uint8_t (* pTest)(void) = MAG_DemoMain;				//This demo show the Magnetic Card function.
	//EMV_ISO7816Polling(0);								//EMV Electronic Test polling.This demo need open #define EMV7816_10A in "ISO7816_EMV_L1.h"
	
	while(1)
	{
		(*pTest)();
	}
}

/**
  * @name   InitHardware
  * @brief  Initialize the hardware module.
  * @param  None
  * @retval None
  */
void InitHardware(void)
{
	//GPIO_InitTypeDef GPIOx_Init;
	RCC_Security2PeriphClockCmd(RCC_SEC2Periph_FLASH, ENABLE);
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_TRNG, ENABLE);	//The trng function is in the ROM,should enable the trng clock before used.
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_PKE, ENABLE); 	//Assmmetric algorithm is in the ROM,,should enable the trng clock before used.
	pkeConfig(0xFF,0x0F,0x01);									//PKE Init.
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA, ENABLE);
	TMC_FLASH_Msp_Init();
	//TMC_GPIO_StructInit(&GPIOx_Init);
	
	/*Initialization clock*/
	TMC_InitTick(TICK_INT_PRIORITY);
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
	
	/* Serial print initialization, using UART1, Baud rate: 115200 8-bit data bit 1 Stop bit None Parity bit */
	UART_PrintInit();	
	
	/*Enable the Sensors, rst operation in debug mode should not reset all the ASH registers, may cause a false alarm.*/
//	RCC_Security1PeriphClockSleepCmd(RCC_SEC1Periph_ActiveShield,ENABLE);	//Eable the ASH clock.
//	RCC_Security1PeriphClockSleepCmd(RCC_SEC1Periph_ExternalVoltageGlitchDetection,ENABLE);	//Enable the EGS clock.
//	NVIC_ClearPendingIRQ(ASH_IRQn);
//	NVIC_EnableIRQ(ASH_IRQn);
//	NVIC_ClearPendingIRQ(EGS_IRQn);
//	NVIC_EnableIRQ(EGS_IRQn);
//	ASH_ConfigIT(ENABLE);
//	EGS_ConfigIT(ENABLE);
}

/**
  * @name   DMA_InterruptHandle
  * @brief  Indicate the operation of each DMA channel.
  * @param  None
  * @retval None
  */
void DMA_InterruptHandle(void)
{
	/* Because the demo may reuse the DMA channel,the user should open the function when each function is uesd. */
	//void (*pDmaIRQHandle)(void) = UART_DMA_Handle;
	//void (*pDmaIRQHandle)(void) = UART5_DMA_Handle;
	void (*pDmaIRQHandle)(void) = SPI_DMA_Handle;
	//void (*pDmaIRQHandle)(void) = ADC_DMA_Handle;
	//void (*pDmaIRQHandle)(void) = M7816x_DMA_Handle;
	//void (*pDmaIRQHandle)(void) =  SPI2_DMA_Handle;
	(*pDmaIRQHandle)();
}

/**
  * @name   assert_failed
  * @brief  Parameter check.
  * @param  file. 
  * @param  line.
  * @retval None.
  */
void assert_failed(uint8_t* file, uint32_t line)
{
	while(1);
}


