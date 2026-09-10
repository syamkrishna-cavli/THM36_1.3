/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: thm36_1.3.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/   
#include "thm36_1.3.h"
#include "tmc_spi.h"
#include "tmc_dma.h"
#include "rcc.h"
#include "tmc_adc.h"
#include "tmc_vbat.h"
#include "tmc_exti.h"
#include "tmc_uart.h"
#include "tmc_uart5.h"
#include "USB.h"
#include "tmc_iso7816Mx.h"
#include "tmc_tim.h"
#include "tmc_pwm.h"
#include "thm36_1.3_it.h"
#include "tmc_wdt.h"
#include "wdt_demomain.h"
#include "printfunc.h"

extern void ADC_InterruptHandle(void);
extern WDT_HandleTypeDef hwdt;
extern volatile uint8_t receive_ilde_flag;
extern volatile uint8_t IsRxFlag;



void LD_IRQHandler (void)
{

}

void NMI_Handler(void)
{

}
/**
  * @brief  interrupt service function for HardFault
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    UART_printf("HardFault_Handler!!!\r\n");
	while(1);
}
/**
  * @brief  interrupt service function for MemManage
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{

}
/**
  * @brief  interrupt service function for BusFault
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{

}

/**
  * @brief  interrupt service function for UsageFault
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{

}

/**
  * @brief  interrupt service function for SVC
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{

}

/**
  * @brief  interrupt service function for DebugMon
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void) 
{

}

/**
  * @brief  interrupt service function for PendSV
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{

}

/**
  * @brief  interrupt service function for SysTick
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TMC_IncTick();
}

/**
  * @brief  interrupt service function for RCC
  * @param  None
  * @retval None
  */
void RCC_IRQHandler(void)
{

}

/**
  * @brief  interrupt service function for DMA
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{
    GPIOB->OD |= GPIO_PIN_2;/*TMC(20240409)-gsx:DMA+IDLE: Pull up RTS*/
	DMA_InterruptHandle();
}

/**
  * @brief  interrupt service function for AHBMMU
  * @param  None
  * @retval None
  */
void AHBMMU_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for MPU
  * @param  None
  * @retval None
  */
void MPU_IRQHandler(void) 
{
}

/**
  * @brief  interrupt service function for AES
  * @param  None
  * @retval None
  */
void AES_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for DES
  * @param  None
  * @retval None
  */
void DES_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for PKE
  * @param  None
  * @retval None
  */
void PKE_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for TRNG
  * @param  None
  * @retval None
  */
void TRNG_IRQHandler(void)
{
}

/**
  * @brief  interrupt FD2 function for SENSOR
  * @param  None
  * @retval None
  */
void FD2_IRQHandler(void)
{
}

/**
  * @brief  interrupt FD2 function for SENSOR
  * @param  None
  * @retval None
  */
void FD3_IRQHandler(void)
{
	TMC_VBAT_EFD3_IRQHandler(&hVbat_fd3);
}

/**
  * @brief  interrupt FD2 function for SENSOR
  * @param  None
  * @retval None
  */
void EGS_IRQHandler(void)
{
	while(1);
}
/**
  * @brief  interrupt service function for VB_SENSOR
  * @param  None
  * @retval None
  */
void VB_SENSOR_IRQHandler(void)
{
	TMC_VBAT_Sensor_IRQHandler(&hVbat);
}

/**
  * @brief  interrupt service function for ASH
  * @param  None
  * @retval None
  */
void ASH_IRQHandler(void)
{
	while(1);
}

/**
  * @brief  interrupt service function for WWDT
  * @param  None
  * @retval None
  */
void WWDT_IRQHandler(void)
{
	TMC_WDT_IRQHandler(&hwdt);
}

/**
  * @brief  interrupt service function for TIMER1
  * @param  None
  * @retval None
  */
void TIMER1_IRQHandler(void)
{
	
	TMC_TIM_IRQHandler(&htim1);
}
/**
  * @brief  interrupt service function for TIMER2
  * @param  None
  * @retval None
  */
void TIMER2_IRQHandler(void)
{
	//TMC_TIM_IRQHandler(&htim2);
}

/**
  * @brief  interrupt service function for TIMER3
  * @param  None
  * @retval None
  */
void TIMER3_IRQHandler(void)
{
	//TMC_TIM_IRQHandler(&htim3);
}

/**
  * @brief  interrupt service function for TIMER4
  * @param  None
  * @retval None
  */
void TIMER4_IRQHandler(void)
{
	//TMC_TIM_IRQHandler(&htim4);
}

/**
  * @brief  interrupt service function for TIMER5
  * @param  None
  * @retval None
  */
void TIMER5_IRQHandler(void)
{
	//TMC_TIM_IRQHandler(&htim5);
}

/**
  * @brief  interrupt service function for TIMER6
  * @param  None
  * @retval None
  */
void TIMER6_IRQHandler(void)
{
	//TMC_TIM_IRQHandler(&htim6);
}

/**
  * @brief  interrupt service function for Flash1
  * @param  None
  * @retval None
  */
void Flash1_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for GPIO
  * @param  None
  * @retval None
  */
void GPIO_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for SPI1
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{	
	//TMC_SPI_IRQHandler(&TMC_SPI1);

}

/**
  * @brief  interrupt service function for SPI2
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for SPI3
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void)
{
	
	TMC_SPI_IRQHandler(&TMC_SPI3);
}

/**
  * @brief  interrupt service function for UART1
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
	TMC_UART_IRQHandler(&huart1);

}
/**
  * @brief  interrupt service function for UART2
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
	//TMC_UART_IRQHandler(&huart2);
}

/**
  * @brief  interrupt service function for UART5
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
     /*TMC(20240409)-gsx:FIFO+RTS:In order to test the blocking function of the RTS signal, 
    in the interrupt mode receiving function, the data is read only after a delay of 1ms*/
    if(IsRxFlag)
        delay_ms(1);
    /*TMC(20240409)-gsx:DMA+IDLE*/
    if(UART5->ISR & UART5_FLAG_IDLE)
    {        
        GPIOB->OD |= GPIO_PIN_2; /*TMC(20240409)-gsx:DMA+IDLE: Pull up RTS*/
        UART5->ICR |= UART5_FLAG_IDLE;/*TMC(20240409)-gsx:DMA+IDLE: Clear IDLE status*/
        receive_ilde_flag = 1;
    }
	TMC_UART5_IRQHandler(&huart5);
}

/**
  * @brief  interrupt service function for I2C1
  * @param  None
  * @retval None
  */
void I2C1_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for ISO7816M1
  * @param  None
  * @retval None
  */
void ISO7816M1_IRQHandler(void)
{
	//TMC_M7816x_IRQHandler(&TMC_ISO7816M1);
}

/**
  * @brief  interrupt service function for ISO7816M2
  * @param  None
  * @retval None
  */
void ISO7816M2_IRQHandler(void)
{
	TMC_M7816x_IRQHandler(&TMC_ISO7816M2);
}

/**
  * @brief  interrupt service function for ISO7816M3
  * @param  None
  * @retval None
  */
void ISO7816M3_IRQHandler(void)
{
	//TMC_M7816x_IRQHandler(&TMC_ISO7816M3);
}

/**
  * @brief  interrupt service function for PWM1
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{
	TMC_PWM_IRQHandler(&hpwm1);
}

/**
  * @brief  interrupt service function for PWM2
  * @param  None
  * @retval None
  */
void PWM2_IRQHandler(void)
{
	//TMC_PWM_IRQHandler(&hpwm2);
}

/**
  * @brief  interrupt service function for PWM3
  * @param  None
  * @retval None
  */
void PWM3_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for PWM4
  * @param  None
  * @retval None
  */
void PWM4_IRQHandler(void)
{
}

/**
  * @brief  interrupt service function for USB
  * @param  None
  * @retval None
  */
void USB_IRQHandler(void)
{
	if(USBMISTS & Bit0_En)
	{
		usbEp0RxTx();
		usbReqHandle();
		USB_Time = 1;
	}
}

/**
  * @brief  interrupt service function for ADC
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
	ADC_InterruptHandle();
}

/**
  * @brief  interrupt service function for EXTI0_15
  * @param  None
  * @retval None
  */
void EXTI0_15_IRQHandler(void)
{
	TMC_EXTI_GPIOA_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI16_31
  * @param  None
  * @retval None
  */
void EXTI16_31_IRQHandler(void)
{
	
	TMC_EXTI_GPIOB_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI32_47
  * @param  None
  * @retval None
  */
void EXTI32_47_IRQHandler(void)
{
	
	TMC_EXTI_GPIOC_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI48_63
  * @param  None
  * @retval None
  */
void EXTI48_63_IRQHandler(void)
{
	
	TMC_EXTI_GPIOD_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI64_65
  * @param  None
  * @retval None
  */
void EXTI64_79_IRQHandler(void)
{
	TMC_EXTI_GPIOE_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI80
  * @param  None
  * @retval None
  */
void EXTI80_IRQHandler(void)
{
	
	TMC_EXTI_USB_Resume_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI81
  * @param  None
  * @retval None
  */
void EXTI81_IRQHandler(void)
{
	
	TMC_EXTI_RTC_Alarm_IRQHandler();
}

/**
  * @brief  interrupt service function for EXTI82
  * @param  None
  * @retval None
  */
void EXTI82_IRQHandler(void)
{
	  
	TMC_EXTI_IWDT_Alarm_IRQHandler();
}


/**
  * @brief  interrupt service function for EXTI83
  * @param  None
  * @retval None
  */
void EXTI83_IRQHandler(void)
{
	EXTI_ClearOtherStatus(EXTI_BAT_PIN0_WAKE_UP);
}

/**
  * @brief  interrupt service function for EXTI84
  * @param  None
  * @retval None
  */
void EXTI84_IRQHandler(void)
{
	EXTI_ClearOtherStatus(EXTI_BAT_PIN1_WAKE_UP);	
}

