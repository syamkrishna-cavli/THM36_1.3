/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwr_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_pwr.h"
#include "tmc_rcc.h"
#include "tmc_exti.h"
#include "vbat.h"
#include "vbat_sensor.h"
#include "tmc_vbat.h"
#include "pwr.h"
#include "tmc_exti.h"
#include "tmc_gpio.h"
#include "pwr_demomain.h"
#include "tim_demomain.h"

/* EXTI Wake-Up Source 
	bit0  GB13
	bit1  GD4
	bit2  USB        
	bit3  RTC
	bit4  IWDT      
*/
uint32_t EXTI_Result;
//TMC(20240408)-zt:split EXTI_StateTypeDef EXTI_State
//EXTI_StateTypeDef EXTI_State;
uint32_t EXTI_State_GPIO_A;
uint32_t EXTI_State_GPIO_B;
uint32_t EXTI_State_GPIO_C;
uint32_t EXTI_State_GPIO_D;
uint32_t EXTI_State_GPIO_E;
uint32_t EXTI_State_USB_Resume_Rsing;
uint32_t EXTI_State_RTC_Alarm;				
uint32_t EXTI_State_IWDT_Alarm;	

extern void VBAT_SENSOR_ExampleOfInitial(void);
extern void TMC_VBAT_RTC_Alarm_Test(void);
extern VBAT_HandleTypeDef hVbat;

/**
  * @brief After waking up from stop mode, increase system clock to PLL_H_192M step by step
	* @param[in]  RCC_SysclkInitStruct:  pointer to an RCC_OscInitTypeDef structure that
  *		 contains the configuration information for the RCC peripheral.
  * @retval None
 */
void TMC_SystemClock_Step_Up(RCC_SysclkInitTypeDef *RCC_SysclkInitStruct)
{
		/*Users can change the delay method to an empty loop, as long as the time interval between frequency changes is ensured to be 10us*/
		TMC_Timer_Delay_Init();

		TMC_Timer_Delay_us(10);
		RCC_SysclkInitStruct->SYSCLKSource  = RCC_SYS_CLKSRC_PLL_H;
		RCC_SysclkInitStruct->SYSCLKDIV = 0x03;
		TMC_RCC_SystemClockConfig(RCC_SysclkInitStruct);
	
		TMC_Timer_Delay_us(10);
		RCC_SysclkInitStruct->SYSCLKSource = RCC_SYS_CLKSRC_PLL_H;
		RCC_SysclkInitStruct->SYSCLKDIV = 0x01;
		TMC_RCC_SystemClockConfig(RCC_SysclkInitStruct);
	
		TMC_Timer_Delay_us(10);
		RCC_SysclkInitStruct->SYSCLKSource = RCC_SYS_CLKSRC_PLL_H;
		RCC_SysclkInitStruct->SYSCLKDIV = 0x00;
		TMC_RCC_SystemClockConfig(RCC_SysclkInitStruct);

		TMC_Timer_Delay_DeInit();
}
/**
    * @brief PWR_Stop_Demo_IRQ_WakeUp:
                1.Enter STOP mode when GPIOB_14 is low level
				2.Configuring the wake source(GPIO/RTC/IWDT/WakeUpPin/USB Resume)
				3.Switch system clock switches to the low-frequency clock
                4.Select which modules are powered off as required
				5.Run the WFI/WFE command to make the chip enter StopMode
				6.Waiting to be awakened
				7.Restore the system clock to PLL_H
    * @param[in]  Cpu_Mode: WFI or WFE enter stop mode
					@arg PWR_STOPEntry_WFI 
					@arg PWR_STOPEntry_WFE
	* @param[in]  wakeup_source: RTC or IWDT wakeup chip
    				@arg RTCwakeup 
					@arg IWDTwakeup
    * @retval None
 */
void PWR_Stop_Demo_IRQ_WakeUp(uint8_t Cpu_Mode, wakeup_source_t wakeup_source)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
	GPIO_InitTypeDef GPIOx_Init;	
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Otype = GPIO_MODE_OUTPUT_PP;
	GPIOx_Init.Pull = GPIO_PULLUP;
	GPIOx_Init.SMIT = GPIO_INPUTSCHMIT_DISABLE;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;
	GPIOx_Init.Pin = GPIO_PIN_14;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);
	/*Whether the chip enter the STOP mode depend on the GB14 pin.*/
	if(TMC_GPIO_ReadPin(GPIOB,GPIO_PIN_14)== GPIO_PIN_RESET)
	{
		RCC_Security1PeriphClockCmd(RCC_SEC1Periph_PWRC,ENABLE);
		RCC_Security1PeriphClockCmd(RCC_SEC1Periph_INT,ENABLE);
		VBAT_SENSOR_ExampleOfInitial();
		EXTI_Result = 0;
		
	/*******************************Configrue the wakeup exti or event source************************************************************/	
		NVIC_ClearPendingIRQ(EXTI0_15_IRQn); 
		NVIC_ClearPendingIRQ(EXTI16_31_IRQn); 
		NVIC_ClearPendingIRQ(EXTI32_47_IRQn); 
		NVIC_ClearPendingIRQ(EXTI48_63_IRQn);
		NVIC_ClearPendingIRQ(EXTI64_79_IRQn);
		NVIC_ClearPendingIRQ(EXTI80_IRQn);
		NVIC_ClearPendingIRQ(EXTI83_IRQn);
		NVIC_ClearPendingIRQ(EXTI84_IRQn);
		
		NVIC_EnableIRQ(EXTI0_15_IRQn); 
		NVIC_EnableIRQ(EXTI16_31_IRQn);
		NVIC_EnableIRQ(EXTI32_47_IRQn); 
		NVIC_EnableIRQ(EXTI48_63_IRQn);
		NVIC_EnableIRQ(EXTI64_79_IRQn);
		NVIC_EnableIRQ(EXTI80_IRQn);
		NVIC_EnableIRQ(EXTI83_IRQn);	
		NVIC_EnableIRQ(EXTI84_IRQn);

		EXTI_State_GPIO_A = 0;
		EXTI_State_GPIO_B = 0;
		EXTI_State_GPIO_C = 0;
		EXTI_State_GPIO_D = 0;
		EXTI_State_GPIO_E =0;
		EXTI_State_IWDT_Alarm = 0;
		EXTI_State_RTC_Alarm = 0;
		EXTI_State_USB_Resume_Rsing = 0;

/*******************************EXTI configuration.***********************************************************/
		EXTI_Reset();													/*Clear EXTI register.*/
		EXTI_ConfigFalling(EXTI_GPIOB,EXTI_PIN_13,ENABLE);
		EXTI_ConfigFalling(EXTI_GPIOD,EXTI_PIN_4,ENABLE);				/*In slave mode,the falling edge of SSN can wakeup chip.*/
		EXTI_ConfigOtherMode(EXTI_USB_WAKE_UP,ENABLE);					/*Wakeup by USB.*/
		EXTI_ConfigOtherMode(EXTI_BAT_PIN0_FALLING_WAKE_UP, ENABLE);	/*WakeupPin1 Falling edge wakeup chip.*/
		EXTI_ConfigOtherMode(EXTI_BAT_PIN1_FALLING_WAKE_UP, ENABLE);	/*WakeupPin2 Falling edge wakeup chip.*/


	/*******************************System clock reduce to 12MHz.************************************************************/
		RCC_OscInitStruct.ClockSourceType = RCC_ClockSource_HSI;
		RCC_OscInitStruct.HSIState = ENABLE;
		RCC_OscInitStruct.PLLState = DISABLE;
		if (TMC_RCC_OscConfig(&RCC_OscInitStruct) != TMC_OK)
		{
			return ;
		}
		RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
		RCC_SysclkInitStruct.SYSCLKDIV = 0x00;
		if (TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct) != TMC_OK)
		{
			return ;
		}

		
	/***********************************STOP configuration.*****************************************************/
		if(wakeup_source == RTCwakeup)
			TMC_VBAT_RTC_Start_IT(&hVbat);
		else if(wakeup_source == IWDTwakeup)
			TMC_VBAT_IWDT_Start_IT(&hVbat);		
		/*User can configure RAM range in condition.This demo only standby RAM1 aera.*/
		TMC_PWR_EnterStopMode(PWR_STOP_MODE_HSI_PWR_DOWN|PWR_STOP_MODE_FLASH_PWR_DOWN|PWR_STOP_MODE_LDO_PWR_DOWN,
							  Cpu_Mode,
								SRAM_RANGE0PD|SRAM_RANGE2PD|SRAM_RANGE3PD|SRAM_RANGE4PD|SRAM_RANGE5PD);	
	/***********************************Recover system clock.********************************************************/
		RCC_OscInitStruct.ClockSourceType = RCC_ClockSource_PLL;
		RCC_OscInitStruct.PLLConfig.PLL_Input = RCC_PLLC_PLLSRC_HSI;
		RCC_OscInitStruct.PLLConfig.PLL_M = 1;
		RCC_OscInitStruct.PLLConfig.PLL_N = 16;
		RCC_OscInitStruct.PLLConfig.PLL_Q = 4;
		RCC_OscInitStruct.PLLConfig.SourceClockVaule = HSI_VALUE;
		RCC_OscInitStruct.PLLState = ENABLE;
		TMC_RCC_OscConfig(&RCC_OscInitStruct);
		TMC_SystemClock_Step_Up(&RCC_SysclkInitStruct);

		return ;
	}
	
}

/**
  * @name   TMC_EXTI_CpltCallback
  * @brief  Call back function of EXTI.
  * @param[in]  State - EXTI state.
  * @retval None
  */
//TMC(20240408)-zt:split void TMC_EXTI_CpltCallback(EXTI_StateTypeDef *State) to 8 functions
void TMC_EXTI_CpltCallback_GPIOA(uint32_t uiState)
{
}

void TMC_EXTI_CpltCallback_GPIOB(uint32_t uiState)
{
	if(uiState & EXTI_PIN_13)
	{
		EXTI_Result |= EXTI_WAKEUP_SOURCE_GPIO_GB13;
	}
}

void TMC_EXTI_CpltCallback_GPIOC(uint32_t uiState)
{
}

void TMC_EXTI_CpltCallback_GPIOD(uint32_t uiState)
{
	if(uiState & EXTI_PIN_4)
	{
		EXTI_Result |= EXTI_WAKEUP_SOURCE_GPIO_GD4;
	}
}

void TMC_EXTI_CpltCallback_GPIOE(uint32_t uiState)
{
}

void TMC_EXTI_CpltCallback_USB(uint32_t uiState)
{
	if(uiState & 1)
	{
		EXTI_Result |= EXTI_WAKEUP_SOURCE_USB;
	}
}

void TMC_EXTI_CpltCallback_RTC_Alarm(uint32_t uiState)
{
	if(uiState & 1)
	{
		EXTI_Result |= EXTI_WAKEUP_SOURCE_RTC;
		hVbat.State = TMC_VBAT_STATE_ALARM;
		hVbat.ALARMCode |= TMC_VBAT_ALARM_RTC;
	}
}

void TMC_EXTI_CpltCallback_IWDT_Alarm(uint32_t uiState)
{
	if(uiState&1)
	{
		EXTI_Result |= EXTI_WAKEUP_SOURCE_IWDT;
		hVbat.State = TMC_VBAT_STATE_ALARM;
		hVbat.ALARMCode |= TMC_VBAT_ALARM_IWDT;
	}
}

/**
    * @brief PWR_StandBy_Test:
                1.Enter Standby mode when GPIOB_14 is low level
				2.Init RTC
				3.Configuring the wake source is RTC
				4.Enable RTC interrupt
				5.Run the WFI command to make the chip enter StandbyMode
				6.Waiting to be awakened
				7.Chip reset
    * @param  None
    * @retval None
 */
void PWR_StandBy_Test(void)
{
	
	GPIO_InitTypeDef GPIOx_Init;	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Otype = GPIO_MODE_OUTPUT_PP;
	GPIOx_Init.Pull = GPIO_PULLUP;
	GPIOx_Init.SMIT = GPIO_INPUTSCHMIT_DISABLE;
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;
	GPIOx_Init.Pin = GPIO_PIN_14;
	TMC_GPIO_Init(GPIOB, &GPIOx_Init);
	if(TMC_GPIO_ReadPin(GPIOB,GPIO_PIN_14)== GPIO_PIN_RESET)/*If GBIO14 is low then enter standby mode.*/
	{
		RCC_OscInitTypeDef RCC_OscInitStruct;
		RCC_SysclkInitTypeDef RCC_SysclkInitStruct;

		VBAT_SENSOR_ExampleOfInitial();
		PWR_ConfigStandbyWakeUpSource(RTC_WAKE_UP);/*Wakeup by RTC_WAKE_UP*/
		

	/*******************************The system clock reduce to 12MHz.************************************************************/	
		RCC_OscInitStruct.ClockSourceType = RCC_ClockSource_HSI;
		RCC_OscInitStruct.HSIState = ENABLE;
		RCC_OscInitStruct.PLLState = DISABLE;
		if (TMC_RCC_OscConfig(&RCC_OscInitStruct) != TMC_OK)
		{
			return ;
		}
		RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
		RCC_SysclkInitStruct.SYSCLKDIV = 0x00;

		if (TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct) != TMC_OK)
		{
			return ;
		}
		TMC_VBAT_RTC_Start_IT(&hVbat);
		EXTI_ConfigOtherMode(EXTI_RTC_ALM_WAKE_UP,DISABLE);/*Clock RTC exti enterrupt.*/
	/***********************************Standby Mode*****************************************************/	
		TMC_PWR_EnterStandByMode();
	}
}

void PWR_Stop_DemoMain(void)
{
	/*user can config WFI or WFE enter Stop mode and wakeup by RTC or IWDT, RTC wakeup after 60s, IWDT wakeup after 5s*/
	PWR_Stop_Demo_IRQ_WakeUp(PWR_STOPEntry_WFI, RTCwakeup);	//GPIOB_14 is low then enter Stop mode.
}

