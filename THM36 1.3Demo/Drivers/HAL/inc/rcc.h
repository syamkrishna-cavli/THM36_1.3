/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: rcc.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RCC_H
#define _RCC_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @addtogroup RCC 
  * @brief RCC driver modules
  * @{
  */ 
/** @defgroup RCC_Exported_Types RCC Exported Types 
  * @{
  */
	 
/**
  * @brief  RCC HSE configuration structure definition
  */	 
typedef struct
{		  	
	FunctionalState FeedbackResEanble;			/*!< Enable or Disable Internal feedback Resistor */

	uint32_t HSE_ClockValue;					/*!< The value of HSE input's clock */

	FunctionalState HSE_ByPass;					/*!< Enable or Disable HSE bypass */

} RCC_HSEConfigTypeDef;

/**
  * @brief  RCC PLL configuration structure definition
  */
typedef struct
{		  
	uint32_t PLL_M;								/*!< The value of PLL_M,the range is 1 to 4 */

	uint32_t PLL_N;								/*!< The value of PLL_N,the range is 10 to 127 */

	uint32_t PLL_Q;								/*!< The value of PLL_Q,the range is 2/4/6/8 */																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						 

	float SourceClockVaule;						/*!< The value of Source Clock */

	uint32_t PLL_Input;							/*!< The clock to be configured.
													 This parameter can be a value of @ref PLL_source_clock_define	  */	
} RCC_PLL_InitTypeDef;

/**
  * @brief  RCC LSE configuration structure definition
  */	 
typedef struct
{		  	
	FunctionalState FeedbackResEanble;			/*!< Enable or Disable Internal feedback Resistor */

} RCC_LSEConfigTypeDef;
/**
  * @}
  */

/** @defgroup RCC_Exported_Constants RCC Exported Constants
  * @{
  */

/** @defgroup AHB_peripheral AHB peripheral 
  * @{
  */

#define RCC_AHBPeriph_DMA				0x00000001UL
#define RCC_AHBPeriph_SPI2				0x00000008UL
#define RCC_AHBPeriph_FLASHCACHE		0x00010000UL

/**
  * @}
  */

/** @defgroup APB2_peripheral APB2 peripheral 
  * @{
  */

#define RCC_APB2Periph_SPI1				0x00000001UL
#define RCC_APB2Periph_SPI3				0x00000002UL
#define RCC_APB2Periph_UART1			0x00000008UL
#define RCC_APB2Periph_TIMER1_2			0x00000010UL
#define RCC_APB2Periph_TIMER5_6			0x00000020UL 
#define RCC_APB2Periph_I2C				0x00000040UL 
#define RCC_APB2Periph_7816M1			0x00000080UL 
#define RCC_APB2Periph_ADC				0x00000100UL 
#define RCC_APB2Periph_CRC				0x00000200UL 
#define RCC_APB2Periph_WDT				0x00000400UL 
#define RCC_APB2Periph_PWM1				0x00000800UL 
#define RCC_APB2Periph_7816M3			0x00001000UL 
#define RCC_APB2Periph_PWM3  			0x00004000UL
/**
  * @}
  */ 

/** @defgroup APB3_peripheral  APB3 peripheral
  * @{
  */
#define RCC_APB3Periph_PWM4				0x00000800UL
#define RCC_APB3Periph_PWM2				0x00000400UL
#define RCC_APB3Periph_UART5			0x00000002UL
#define RCC_APB3Periph_UART2			0x00000004UL
#define RCC_APB3Periph_TIMER3_4			0x00000008UL
#define RCC_APB3Periph_GPIO				0x00000040UL
#define RCC_APB3Periph_7816M2			0x00000200UL


/**
  * @}
  */
/** @defgroup Security1_peripheral Security1 peripheral 
  * @{
  */
#define RCC_SEC1Periph_INT								0x00000002UL 
#define RCC_SEC1Periph_PWRC								0x00000004UL 
#define RCC_SEC1Periph_AES								0x00001000UL 
#define RCC_SEC1Periph_DES								0x00002000UL 
#define RCC_SEC1Periph_PKE								0x00004000UL 
#define RCC_SEC1Periph_TRNG								0x00010000UL 
#define RCC_SEC1Periph_ActiveShield						0x00080000UL 
#define RCC_SEC1Periph_ExternalVoltageGlitchDetection	0x00300000UL
#define RCC_SEC1Periph_LightDerector					0x00400000UL
#define RCC_SEC1Periph_FrequencyDerector				0x00800000UL
#define RCC_SEC1Periph_VBSENSOR							0x10000000UL

/**
  * @}
  */
/** @defgroup Security2_peripheral Security2 peripheral 
  * @{
  */
#define RCC_SEC2Periph_FLASH			0x00000002UL
#define RCC_SEC2Periph_USB				0x00010000UL

/**
  * @}
  */
/** @defgroup VBAT_security_module VBAT security module
  * @{
  */

#define RCC_BATS_SENSOR					0x00000020UL 
#define RCC_BATS_STDBYCON				0x00000004UL 
#define RCC_BATS_IWDT					0x00000001UL 

/**
  * @}
  */
/** @defgroup reset_define reset_define
  * @{
  */

#define RCC_RST_PIN						0x01UL
#define RCC_RST_ALARM_WWDT				0x02UL
#define RCC_RST_ALARM_IWDT				0x04UL
#define RCC_RST_DBG						0x08UL
#define RCC_RST_POR						0x10UL
#define RCC_RST_S7816					0x20UL
#define RCC_RST_ALARM_SENIP				0x40UL
#define RCC_RST_ALARM_SEC				0x80UL

/**
  * @}
  */


/** @defgroup USB_source_clock_define USB source clock
  * @{  
  */
  
#define RCC_USB_CLKSRC_HSE				0x00000002UL 
#define RCC_USB_CLKSRC_USBPHY			0x00000001UL 
#define RCC_USB_CLKSRC_PLL_L			0x00000000UL 

/**
  * @}
  */
/** @defgroup system_source_clock_define system source clock
  * @{  
  */  
#define RCC_SYS_CLKSRC_HSI				0x00000000UL
#define RCC_SYS_CLKSRC_PLL_H			0x00000001UL
#define RCC_SYS_CLKSRC_USBPHY			0x00000002UL
#define RCC_SYS_CLKSRC_HSE				0x00000003UL

/**
  * @}
  */
/** @defgroup PLL_source_clock_define	PLL source clock
  * @{  
  */
#define RCC_PLLC_PLLSRC_HSI				0x00000000UL
#define RCC_PLLC_PLLSRC_HSE				0x00000001UL

/**
  * @}
  */

/** @defgroup SPI2_source_clock_define SPI2 source clock
  * @{  
  */  
#define RCC_SPI2_CLKSRC_PLL_H			0x00000002UL 
#define RCC_SPI2_CLKSRC_HSE				0x00000001UL 
#define RCC_SPI2_CLKSRC_HSI				0x00000000UL

/**
  * @}
  */
  
/** @defgroup ADC_source_clock_define ADC source clock
  * @{  
  */  
#define RCC_ADC_CLKSRC_PLL_L			0x00000002UL 
#define RCC_ADC_CLKSRC_HSE				0x00000001UL 
#define RCC_ADC_CLKSRC_HSI				0x00000000UL

/**
  * @}
  */
/** @defgroup 7816M_source_clock_define 7816master source clock
  * @{  
  */  
#define RCC_7816M_CLKSRC_PLL_L			0x00000002UL
#define RCC_7816M_CLKSRC_HSE			0x00000001UL 
#define RCC_7816M_CLKSRC_HSI			0x00000000UL

/**
  * @}
  */
/** @defgroup UART5_source_clock_define UART5 source clock
  * @{  
  */  
#define RCC_UART5_CLKSRC_PLL_L			0x00000002UL
#define RCC_UART5_CLKSRC_HSE			0x00000001UL 
#define RCC_UART5_CLKSRC_HSI			0x00000000UL

/**
  * @}
  */  
/** @defgroup 7816M_interface_number_define 7816master interface number
  * @{  
  */  
#define INTERFACE_7816M1				0x00000000U
#define INTERFACE_7816M2				0x00000001U
#define INTERFACE_7816M3				0x00000002U

/**
  * @}
  */
/** @defgroup PWM_source_clock_define PWM source clock define
  * @{  
  */  
#define RCC_PWM_CLKSRC_PLL_L			0x00000002UL
#define RCC_PWM_CLKSRC_HSE				0x00000001UL 
#define RCC_PWM_CLKSRC_HSI				0x00000000UL 

/**
  * @}
  */  
/** @defgroup PWM_interface_number_define PWM interface number
  * @{  
  */  
#define INTERFACE_PWM1					0x00U
#define INTERFACE_PWM2					0x01U
#define INTERFACE_PWM3					0x02U
#define INTERFACE_PWM4					0x03U
/**
  * @}
  */

/**
  * @}
  */
/** @defgroup TIMER_source_clock_define TIMER source clock
  * @{  
  */  
#define RCC_TIMER_CLKSRC_PLL_L			0x00000002UL 
#define RCC_TIMER_CLKSRC_HSE			0x00000001UL 
#define RCC_TIMER_CLKSRC_HSI			0x00000000UL

/**
  * @}
  */
/** @defgroup TIMER_peripheral_number_define TIMER peripheral number
  * @{  
  */  
#define PERIPHERAL_TIMER1				0x00000000UL
#define PERIPHERAL_TIMER2				0x00000001UL
#define PERIPHERAL_TIMER3				0x00000002UL
#define PERIPHERAL_TIMER4				0x00000003UL
#define PERIPHERAL_TIMER5				0x00000004UL
#define PERIPHERAL_TIMER6				0x00000005UL
#define PERIPHERAL_WWDT 				0x00000006UL
/**
  * @}
  */ 
/** @defgroup WWDT_source_clock_define WWDT source clock
  * @{  
  */  
#define RCC_WWDT_CLKSRC_PLL_L			0x00000002UL 
#define RCC_WWDT_CLKSRC_HSE				0x00000001UL 
#define RCC_WWDT_CLKSRC_HSI				0x00000000UL 

/**
  * @}
  */
/** @defgroup system_timer_clock_define system timer clock
  * @{  
  */  
#define RCC_SYSTICK_CLKSRC_HSI			0x00000000UL
#define RCC_SYSTICK_CLKSRC_PLL_H		0x00000001UL
#define RCC_SYSTICK_CLKSRC_USBPHY		0x00000002UL
#define RCC_SYSTICK_CLKSRC_HSE			0x00000003UL
													 
#define RCC_SYSTICK_CLKDIV_0			0x00000000UL
#define RCC_SYSTICK_CLKDIV_2			0x00000001UL
#define RCC_SYSTICK_CLKDIV_4			0x00000002UL
#define RCC_SYSTICK_CLKDIV_8			0x00000003UL
#define RCC_SYSTICK_CLKDIV_16			0x00000004UL
#define RCC_SYSTICK_CLKDIV_32			0x00000005UL


/**
  * @}
  */
 
/** @defgroup monitor_clock_define monitor clock
  * @{  
  */

#define RCC_MONITOR_CLKSRC_LSE			0x00000006UL
#define RCC_MONITOR_CLKSRC_LSI			0x00000005UL
#define RCC_MONITOR_CLKSRC_USBPHY		0x00000004UL
#define RCC_MONITOR_CLKSRC_PLL_H		0x00000003UL
#define RCC_MONITOR_CLKSRC_PLL_L		0x00000002UL
#define RCC_MONITOR_CLKSRC_HSE			0x00000001UL
#define RCC_MONITOR_CLKSRC_HSI			0x00000000UL																																																																																																																																																																																																																																																																																																																																																																																																																																																																																							  

#define RCC_MONITOR_CLKDIV_0			0x00000000UL
#define RCC_MONITOR_CLKDIV_2			0x00000001UL
#define RCC_MONITOR_CLKDIV_4			0x00000002UL
#define RCC_MONITOR_CLKDIV_8			0x00000003UL
#define RCC_MONITOR_CLKDIV_16			0x00000004UL
#define RCC_MONITOR_CLKDIV_32			0x00000005UL

/**
  * @}
  */

/** @defgroup BAT_clock_define BAT clock define
  * @{  
  */  
#define RCC_BAT_CLKSRC_LSE				0x00000001UL
#define RCC_BAT_CLKSRC_LSI				0x00000000UL 
		 
													 
#define RCC_BAT_CLKDIV_0				0x00000000UL
#define RCC_BAT_CLKDIV_2				0x00000001UL

#define RCC_RTC_CLKSRC_LSE				0x00000001UL
#define RCC_RTC_CLKSRC_LSI				0x00000000UL													 
/**
  * @}
  */
/** @defgroup alarm_reset_define alarm reset
  * @{  
  */  
#define RCC_ALARM_RSTEN_IWDT			0x00000002UL
#define RCC_ALARM_RSTEN_WWDT			0x00000001UL
/**
  * @}
  */
/** @defgroup BAT_reset_define BAT reset 
  * @{  
  */
#define RCC_BAT_RSTEN_VBSENSOR			0x00000020UL
#define RCC_BAT_RSTEN_VBATGPIO			0x00000010UL
#define RCC_BAT_RSTEN_STDBY				0x00000004UL
#define RCC_BAT_RSTEN_RTC				0x00000002UL
#define RCC_BAT_RSTEN_IWDT				0x00000001UL
/**
  * @}
  */
/** @defgroup CLK_interrupt_define CLK interrupt
  * @{  
  */
#define RCC_IT_PLLRDY					0x00000004UL
#define RCC_IT_HSERDY		   			0x00000002UL
#define RCC_IT_HSIRDY		   			0x00000001UL
/**
  * @}
  */
/** @defgroup RCC_clockflag_define RCC clockflag
  * @{  
  */
#define RCC_FLAG_HSIRDY					0x00000000UL
#define RCC_FLAG_HSERDY					0x00000001UL
#define RCC_FLAG_PLLRDY					0x00000002UL
#define RCC_FLAG_LSIRDY					0x00000003UL
#define RCC_FLAG_LSERDY					0x00000004UL
/**
  * @}
  */
/**
  * @}
  */
 

/** @addtogroup RCC_Exported_Functions
  * @{  
  */

void RCC_DeInit(void);
void RCC_SystemClockConfig(uint32_t source, uint32_t division);
uint32_t RCC_GetSystemClockSource(void);
uint32_t RCC_GetSystemClockDivision(void);
uint32_t RCC_GetUART5ClockSource(void);
uint32_t RCC_GetUART5ClockDivision(void);
void RCC_USBClockConfig(uint32_t source, FunctionalState NewState);
void RCC_SPI2ClockConfig(uint32_t source, FunctionalState NewState);
void RCC_ADCClockConfig(uint32_t source, FunctionalState NewState);
void RCC_7816MClockConfig(uint8_t interface, uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_UART5ClockConfig(uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_PWMClockConfig(uint8_t interface, uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_TimerClockConfig(uint8_t peripheral, uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_WWDTClockConfig(uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_SysTickClockConfig(uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_MonitorClockConfig(uint32_t source, uint32_t division, FunctionalState NewState);
void RCC_BATClockConfig(uint32_t source, uint32_t division);
void RCC_RTCClockConfig(uint32_t source, FunctionalState NewState);
void RCC_HSEConfig(RCC_HSEConfigTypeDef *HSE_Config,FunctionalState NewState);
void RCC_HSICmd(FunctionalState NewState);
void RCC_PLLConfig(RCC_PLL_InitTypeDef *PLL_Config);
void RCC_PLLCmd(FunctionalState NewState);
uint32_t RCC_GetPLLClockSource(void);
void RCC_LSICmd(FunctionalState NewState);
void RCC_LSECmd(FunctionalState NewState, FunctionalState ResState);
FlagStatus RCC_GetFlagStatus(uint32_t RCC_FLAG);
uint32_t RCC_GetSysClocksFreq(void);
void RCC_ITConfig(uint32_t Msk,FunctionalState NewState);
FlagStatus RCC_GetResetStatus(uint32_t status);
void RCC_ClearResetStatus(uint32_t Status);
void RCC_AHBClockCmd(uint32_t RCC_AHBPerfor, FunctionalState NewState);
void RCC_AHBPerforClockCmdInSleepMode(uint32_t RCC_AHBPerfor, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB2PeriphClockCmdInSleepMode(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB3PeriphClockCmd(uint32_t RCC_APB3Periph, FunctionalState NewState);
void RCC_APB3PeriphClockCmdInSleepMode(uint32_t RCC_APB3Periph, FunctionalState NewState);
void RCC_Security1PeriphClockCmd(uint32_t RCC_SEC1Periph, FunctionalState NewState);
void RCC_Security1PeriphClockSleepCmd(uint32_t RCC_SEC1Periph, FunctionalState NewState);
void RCC_Security2PeriphClockCmd(uint32_t RCC_SEC2Periph, FunctionalState NewState);
void RCC_Security2PeriphClockSleepCmd(uint32_t RCC_SEC2Periph, FunctionalState NewState);
void RCC_BATSecurityClockCmd(uint32_t BAT_Security, FunctionalState NewState);
void RCC_AHBPeriphSoftReset(uint32_t RCC_AHBPeriph);
void RCC_APB2PeriphSoftReset(uint32_t RCC_APB2Periph);
void RCC_APB3PeriphSoftReset(uint32_t RCC_APB3Periph);
void RCC_Security1PeriphSoftReset(uint32_t RCC_SEC1Periph);
void RCC_Security2PeriphSoftReset(uint32_t RCC_SEC2Periph);
void RCC_WDTResetSystemCmd(uint32_t RCC_WDT, FunctionalState NewState);
void RCC_BATSoftResetCmd(uint32_t RCC_BAT);
FlagStatus RCC_AHBPeriphGetResetFlag(uint32_t RCC_AHBPeriph);
FlagStatus RCC_APB2PeriphGetResetFlag(uint32_t RCC_APB2Periph);
FlagStatus RCC_APB3PeriphGetResetFlag(uint32_t RCC_APB3Periph);
FlagStatus RCC_Security1PeriphGetResetFlag(uint32_t RCC_SEC1Periph);
FlagStatus RCC_Security2PeriphGetResetFlag(uint32_t RCC_SEC2Periph);
uint32_t RCC_GetPLL_L_Clock(void);
uint32_t RCC_GetSystermClkSource(void);
uint32_t RCC_GetSystermClkDiv(void);
uint32_t RCC_GetPLL_H_Clock(void);
  /**
  * @}
  */

/** @defgroup RCC_Private_Macros RCC Private Macros
  * @{
  */
								
#define IS_SYSTEM_DIVSION_PARAMETER_OUT(b) ((b>=0)||(b<32))
#define IS_PLLQ_PARAMETR_OUT(b) ((b==2)||(b==4)||(b==6)||(b==8))
#define IS_PLLM_PARAMETR_OUT(b) ((b==1)||(b==2)||(b==3)||(b==4))
#define IS_PLLN_PARAMETR_OUT(b) ((b>=10)||(b<=127))

#define IS_SYSTEM_DIV(b) (b<=31)
#define IS_7816M_DIV(b)  (b<=31)
#define IS_UART5_DIV(b)  (b<=31)
#define IS_MONITOR_DIV(b)(b<=5)

#define IS_RCC_AHBPeriph(PERIPH) 					(((PERIPH) == RCC_AHBPeriph_DMA)   		|| \
													 ((PERIPH) == RCC_AHBPeriph_SPI2)   	|| \
													 ((PERIPH) == RCC_AHBPeriph_FLASHCACHE))
#define IS_RCC_APB2Periph(PERIPH) 					(((PERIPH) == RCC_APB2Periph_SPI1)		|| \
													 ((PERIPH) == RCC_APB2Periph_SPI3)   	|| \
													 ((PERIPH) == RCC_APB2Periph_UART1)  	|| \
													 ((PERIPH) == RCC_APB2Periph_TIMER1_2) 	|| \
													 ((PERIPH) == RCC_APB2Periph_TIMER5_6) 	|| \
													 ((PERIPH) == RCC_APB2Periph_I2C)   	|| \
													 ((PERIPH) == RCC_APB2Periph_7816M1) 	|| \
													 ((PERIPH) == RCC_APB2Periph_ADC) 	 	|| \
													 ((PERIPH) == RCC_APB2Periph_CRC) 	 	|| \
													 ((PERIPH) == RCC_APB2Periph_WDT)	 	|| \
													 ((PERIPH) == RCC_APB2Periph_PWM1) 	 	|| \
													 ((PERIPH) == RCC_APB2Periph_7816M3))
#define IS_RCC_APB3Periph(PERIPH) 					(((PERIPH) == RCC_APB3Periph_UART5)		|| \
													 ((PERIPH) == RCC_APB3Periph_UART2)		|| \
													 ((PERIPH) == RCC_APB3Periph_TIMER3_4)	|| \
													 ((PERIPH) == RCC_APB3Periph_GPIO)		|| \
													 ((PERIPH) == RCC_APB3Periph_7816M2)	|| \
													 ((PERIPH) == RCC_APB3Periph_PWM2)  	|| \
													 ((PERIPH) == RCC_APB3Periph_PWM4))


#define IS_RCC_SEC1Periph(PERIPH) 					(((PERIPH) == RCC_SEC1Periph_INT)									|| \
													 ((PERIPH) == RCC_SEC1Periph_PWRC)									|| \
													 ((PERIPH) == RCC_SEC1Periph_AES) 									|| \
													 ((PERIPH) == RCC_SEC1Periph_DES) 									|| \
													 ((PERIPH) == RCC_SEC1Periph_PKE) 									|| \
													 ((PERIPH) == RCC_SEC1Periph_TRNG)									|| \
													 ((PERIPH) == RCC_SEC1Periph_ActiveShield) 							|| \
													 ((PERIPH) == RCC_SEC1Periph_ExternalVoltageGlitchDetection) 		|| \
													 ((PERIPH) == RCC_SEC1Periph_LightDerector) 						|| \
													 ((PERIPH) == RCC_SEC1Periph_FrequencyDerector) 					|| \
													 ((PERIPH) == RCC_SEC1Periph_VBSENSOR))
													 
#define IS_RCC_SEC2Periph(PERIPH) 					(((PERIPH) == RCC_SEC2Periph_FLASH)   	|| \
													 ((PERIPH) == RCC_SEC2Periph_USB))
#define IS_RCC_BATFUNC(FUNCTION) 					(((FUNCTION) == RCC_BATS_SENSOR)   		|| \
													 ((FUNCTION) == RCC_BATS_STDBYCON)   	|| \
													 ((FUNCTION) == RCC_BATS_IWDT))
													 
#define IS_RCC_RESET(RESET) 						(((RESET) == RCC_RST_PIN)   || \
													 ((RESET) == RCC_RST_ALARM_WWDT)|| \
													 ((RESET) == RCC_RST_ALARM_IWDT)|| \
													 ((RESET) == RCC_RST_DBG)|| \
													 ((RESET) == RCC_RST_POR)|| \
													 ((RESET) == RCC_RST_S7816)|| \
													 ((RESET) == RCC_RST_ALARM_SENIP)|| \
													 ((RESET) == RCC_RST_ALARM_SEC))
#define IS_RCC_USB_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_USB_CLKSRC_USBPHY) 	|| \
													 ((SOURCE) == RCC_USB_CLKSRC_PLL_L)  	|| \
													 ((SOURCE) == RCC_USB_CLKSRC_HSE)) 
#define IS_RCC_SYS_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_SYS_CLKSRC_HSI)		|| \
													 ((SOURCE) == RCC_SYS_CLKSRC_PLL_H)   	|| \
													 ((SOURCE) == RCC_SYS_CLKSRC_USBPHY) 	|| \
													 ((SOURCE) == RCC_SYS_CLKSRC_HSE))
													 
#define IS_RCC_PLL_CLKSOURCE(SOURCE)				(((SOURCE) == RCC_PLLC_PLLSRC_HSI)		|| \
													 ((SOURCE) == RCC_PLLC_PLLSRC_HSE))

#define IS_RCC_SPI2_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_SPI2_CLKSRC_PLL_H)   	|| \
													 ((SOURCE) == RCC_SPI2_CLKSRC_HSE)		|| \
													 ((SOURCE) == RCC_SPI2_CLKSRC_HSI))
#define IS_RCC_ADC_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_ADC_CLKSRC_PLL_L)   	|| \
													 ((SOURCE) == RCC_ADC_CLKSRC_HSE)		|| \
													 ((SOURCE) == RCC_ADC_CLKSRC_HSI))
#define IS_RCC_UART5_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_UART5_CLKSRC_PLL_L)   || \
													 ((SOURCE) == RCC_UART5_CLKSRC_HSE)		|| \
													 ((SOURCE) == RCC_UART5_CLKSRC_HSI))
#define IS_RCC_7816M_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_7816M_CLKSRC_PLL_L)   || \
													 ((SOURCE) == RCC_7816M_CLKSRC_HSE)		|| \
													 ((SOURCE) == RCC_7816M_CLKSRC_HSI))
#define IS_RCC_7816M_NUMBER(NUMBER) 				(((NUMBER) == INTERFACE_7816M1)   		|| \
													 ((NUMBER) == INTERFACE_7816M2)   		|| \
													 ((NUMBER) == INTERFACE_7816M3))
#define IS_RCC_PWM_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_PWM_CLKSRC_PLL_L)   	|| \
													 ((SOURCE) == RCC_PWM_CLKSRC_HSE)		|| \
													 ((SOURCE) == RCC_PWM_CLKSRC_HSI))
#define IS_RCC_PWM_NUMBER(NUMBER) 					(((NUMBER) == INTERFACE_PWM1)   		|| \
													 ((NUMBER) == INTERFACE_PWM2)		   || \
													 ((NUMBER) == INTERFACE_PWM3)   		|| \
													 ((NUMBER) == INTERFACE_PWM4))
#define IS_RCC_TIMER_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_TIMER_CLKSRC_PLL_L)  	|| \
													 ((SOURCE) == RCC_TIMER_CLKSRC_HSE)   	|| \
													 ((SOURCE) == RCC_TIMER_CLKSRC_HSI))
#define IS_RCC_TIMER_NUMBER(NUMBER) 				(((NUMBER) == PERIPHERAL_TIMER1)  		|| \
													 ((NUMBER) == PERIPHERAL_TIMER2)  		|| \
													 ((NUMBER) == PERIPHERAL_TIMER3)  		|| \
													 ((NUMBER) == PERIPHERAL_TIMER4)  		||\
													 ((NUMBER) == PERIPHERAL_TIMER5)  		||\
													 ((NUMBER) == PERIPHERAL_TIMER6))
#define IS_RCC_WWDT_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_WWDT_CLKSRC_PLL_L)  	|| \
													 ((SOURCE) == RCC_WWDT_CLKSRC_HSE)   	|| \
													 ((SOURCE) == RCC_WWDT_CLKSRC_HSI))
#define IS_RCC_SYSTICK_CLKSOURCE(SOURCE) 			(((SOURCE) == RCC_SYSTICK_CLKSRC_HSE)   || \
													 ((SOURCE) == RCC_SYSTICK_CLKSRC_USBPHY)|| \
													 ((SOURCE) == RCC_SYSTICK_CLKSRC_PLL_H) || \
													 ((SOURCE) == RCC_SYSTICK_CLKSRC_HSI))
#define IS_RCC_SYSTICK_CLKDIV(DIVISION) 			(((DIVISION) == RCC_SYSTICK_CLKDIV_0)  	|| \
													 ((DIVISION) == RCC_SYSTICK_CLKDIV_2)  	|| \
													 ((DIVISION) == RCC_SYSTICK_CLKDIV_4)  	|| \
													 ((DIVISION) == RCC_SYSTICK_CLKDIV_8)  	|| \
													 ((DIVISION) == RCC_SYSTICK_CLKDIV_16) 	|| \
													 ((DIVISION) == RCC_SYSTICK_CLKDIV_32))
													 
#define IS_RCC_MONITOR_CLKSOURCE(SOURCE) 			(((SOURCE) == RCC_MONITOR_CLKSRC_PLL_L)||((SOURCE) == RCC_MONITOR_CLKSRC_HSE)||((SOURCE) == RCC_MONITOR_CLKSRC_HSI))
													 
#define IS_RCC_MONITOR_CLKDIV(DIVISION) 			(((DIVISION) == RCC_MONITOR_CLKDIV_0)  	|| \
													 ((DIVISION) == RCC_MONITOR_CLKDIV_2)  	|| \
													 ((DIVISION) == RCC_MONITOR_CLKDIV_4)  	|| \
													 ((DIVISION) == RCC_MONITOR_CLKDIV_8)  	|| \
													 ((DIVISION) == RCC_MONITOR_CLKDIV_16) 	|| \
													 ((DIVISION) == RCC_MONITOR_CLKDIV_32))
#define IS_RCC_BAT_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_BAT_CLKSRC_LSE)  		|| \
													 ((SOURCE) == RCC_BAT_CLKSRC_LSI))
#define IS_RCC_BAT_CLKDIV(DIVISION) 				(((DIVISION) == RCC_BAT_CLKDIV_0)  		|| \
													 ((DIVISION) == RCC_BAT_CLKDIV_2))
#define IS_RCC_RTC_CLKSOURCE(SOURCE) 				(((SOURCE) == RCC_RTC_CLKSRC_LSE)  		|| \
													 ((SOURCE) == RCC_RTC_CLKSRC_LSI))													 
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
#endif

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
