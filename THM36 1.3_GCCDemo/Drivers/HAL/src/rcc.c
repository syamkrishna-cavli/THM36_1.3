/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: rcc.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_conf.h"
#include "rcc.h"
#include "pll_enable.h"
/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup RCC RCC
  * @brief RCC HAL modules driver 
  * @{
  */ 


/** @defgroup RCC_Private_Defines RCC Private Defines
  * @{
  */

/* ---------------------- RCC registers bit mask ------------------------ */

/* CLKCON register bit mask */
#define RCC_CLKC_PLLRDY_Pos			(5U)
#define RCC_CLKC_PLLRDY_Msk			(0x1UL << RCC_CLKC_PLLRDY_Pos)		/*!< 0x00000020 */
#define RCC_CLKC_PLLRDY				RCC_CLKC_PLLRDY_Msk
#define RCC_CLKC_PLLON_Pos			(4U)
#define RCC_CLKC_PLLON_Msk			(0x1UL << RCC_CLKC_PLLON_Pos)		/*!< 0x00000010 */
#define RCC_CLKC_PLLON				RCC_CLKC_PLLON_Msk
#define RCC_CLKC_HSERDY_Pos			(3U)
#define RCC_CLKC_HSERDY_Msk			(0x1UL << RCC_CLKC_HSERDY_Pos)		/*!< 0x00000008 */
#define RCC_CLKC_HSERDY				RCC_CLKC_HSERDY_Msk
#define RCC_CLKC_HSEON_Pos			(2U)
#define RCC_CLKC_HSEON_Msk			(0x1UL << RCC_CLKC_HSEON_Pos)		/*!< 0x00000004 */
#define RCC_CLKC_HSEON				RCC_CLKC_HSEON_Msk
#define RCC_CLKC_HSIRDY_Pos			(1U)
#define RCC_CLKC_HSIRDY_Msk			(0x1UL << RCC_CLKC_HSIRDY_Pos)		/*!< 0x00000002 */
#define RCC_CLKC_HSIRDY				RCC_CLKC_HSIRDY_Msk
#define RCC_CLKC_HSION_Pos			(0U)
#define RCC_CLKC_HSION_Msk			(0x1UL << RCC_CLKC_HSION_Pos)		/*!< 0x00000001 */
#define RCC_CLKC_HSION				RCC_CLKC_HSION_Msk

/* CLKHSECFG register bit mask */
#define RCC_HSEC_FBEN_Pos			(3U)
#define RCC_HSEC_FBEN_Msk			(0x1UL << RCC_HSEC_FBEN_Pos)			/*!< 0x00000008 */
#define RCC_HSEC_FBEN				RCC_HSEC_FBEN_Msk
#define RCC_HSEC_FRE_Pos			(1U)
#define RCC_HSEC_FRE_Msk			(0x3UL << RCC_HSEC_FRE_Pos)
#define RCC_HSEC_FRE				RCC_HSEC_FRE_Msk
#define RCC_HSEC_FRE_0				(0x0UL << RCC_HSEC_FRE_Pos)
#define RCC_HSEC_FRE_1				(0x1UL << RCC_HSEC_FRE_Pos)
#define RCC_HSEC_FRE_2				(0x2UL << RCC_HSEC_FRE_Pos)
#define RCC_HSEC_FRE_3				(0x3UL << RCC_HSEC_FRE_Pos)
#define RCC_HSEC_BYP_Pos			(0U)
#define RCC_HSEC_BYP_Msk			(0x1UL << RCC_HSEC_FBEN_Pos)			/*!< 0x00000008 */
#define RCC_HSEC_BYP				RCC_HSEC_BYP_Msk

/* CLKINTMSK register bit mask */
#define RCC_IT_PLLRDY_Pos			(2U)
#define RCC_IT_PLLRDY_Msk			(0x1UL << RCC_INT_PLLRDY_Pos)
#define RCC_IT_HSERDY_Pos			(1U)
#define RCC_IT_HSERDY_Msk			(0x1UL << RCC_INT_HSERDY_Pos)
#define RCC_IT_HSIRDY_Pos			(0U)
#define RCC_IT_HSIRDY_Msk			(0x1UL << RCC_INT_HSIRDY_Pos)

/* CLKPLLCFG register bit mask */
#define RCC_PLLC_PLLQ_Pos			(14U)
#define RCC_PLLC_PLLQ_Msk			(0x3UL << RCC_PLLC_PLLQ_Pos)			/*!< 0x0000C000 */
#define RCC_PLLC_PLLQ				RCC_PLLC_PLLQ_Msk
#define RCC_PLLC_PLLQ_2				(0x0UL << RCC_PLLC_PLLQ_Pos)
#define RCC_PLLC_PLLQ_4				(0x1UL << RCC_PLLC_PLLQ_Pos)
#define RCC_PLLC_PLLQ_6				(0x2UL << RCC_PLLC_PLLQ_Pos)
#define RCC_PLLC_PLLQ_8				(0x3UL << RCC_PLLC_PLLQ_Pos)

#define RCC_PLLC_PLLN_Pos			(4U)
#define RCC_PLLC_PLLN_Msk			(0x7FUL << RCC_PLLC_PLLN_Pos)			/*!< 0x000007F0 */
#define RCC_PLLC_PLLN				RCC_PLLC_PLLN_Msk

#define RCC_PLLC_PLLM_Pos			(2U)
#define RCC_PLLC_PLLM_Msk			(0x3UL << RCC_PLLC_PLLM_Pos)			/*!< 0x0000000C */
#define RCC_PLLC_PLLM				RCC_PLLC_PLLM_Msk
#define RCC_PLLC_PLLM_1				(0x0UL << RCC_PLLC_PLLM_Pos)
#define RCC_PLLC_PLLM_2				(0x1UL << RCC_PLLC_PLLM_Pos)
#define RCC_PLLC_PLLM_3				(0x2UL << RCC_PLLC_PLLM_Pos)
#define RCC_PLLC_PLLM_4				(0x3UL << RCC_PLLC_PLLM_Pos)

#define RCC_PLLC_PLLSRC_Pos			(0U)
#define RCC_PLLC_PLLSRC_Msk			(0x3UL << RCC_PLLC_PLLSRC_Pos)			/*!< 0x00000003 */
#define RCC_PLLC_PLLSRC				RCC_PLLC_PLLSRC_Msk


/* CLKPLLFREQ register bit mask */
#define RCC_PLLC_FREQ_Pos			(0U)
#define RCC_PLLC_FREQ_Msk			(0x1FUL << RCC_PLLC_FREQ_Pos)			/*!< 0x0000001F */
#define RCC_PLLC_FREQ				RCC_PLLC_FREQ_Msk

/* CLKINTMSK register bit mask */
#define RCC_INT_HSIRDYMSK_Pos		(0U)
#define RCC_INT_HSIRDYMSK_Msk		(0x1UL << RCC_INT_HSIRDYMSK_Pos)
#define RCC_INT_HSIRDYMSK			RCC_INT_HSIRDYMSK_Msk
#define RCC_INT_HSERDYMSK_Pos		(1U)
#define RCC_INT_HSERDYMSK_Msk		(0x1UL << RCC_INT_HSERDYMSK_Pos)
#define RCC_INT_HSERDYMSK			RCC_INT_HSERDYMSK_Msk
#define RCC_INT_PLLRDYMSK_Pos		(2U)
#define RCC_INT_PLLRDYMSK_Msk		(0x1UL << RCC_INT_PLLRDYMSK_Pos)
#define RCC_INT_PLLRDYMSK			RCC_INT_PLLRDYMSK_Msk

/* CLKSYSSEL register bit mask */
#define RCC_SYS_CLKSRC_Pos			(0U)
#define RCC_SYS_CLKSRC_Msk			(0x7UL << RCC_SYS_CLKSRC_Pos)
#define RCC_SYS_CLKSRC				RCC_SYS_CLKSRC_Msk
#define RCC_SYS_CLKDIV_Pos			(8U)
#define RCC_SYS_CLKDIV_Msk			(0x1FUL << RCC_SYS_CLKDIV_Pos)
#define RCC_SYS_CLKDIV				RCC_SYS_CLKDIV_Msk

/* CLKUSBSEL register bit mask */
#define RCC_USB_CLKEN_Pos			(4U)
#define RCC_USB_CLKEN_Msk			(0x1UL << RCC_USB_CLKEN_Pos)
#define RCC_USB_CLKEN				RCC_USB_CLKEN_Msk

/* CLKEXSPISEL register bit mask */
#define RCC_SPI2_CLKEN_Pos			(4U)
#define RCC_SPI2_CLKEN_Msk			(0x1UL << RCC_SPI2_CLKEN_Pos)
#define RCC_SPI2_CLKEN				RCC_SPI2_CLKEN_Msk

/* CLKADCSEL register bit mask */
#define RCC_ADC_CLKEN_Pos			(4U)
#define RCC_ADC_CLKEN_Msk			(0x1UL << RCC_ADC_CLKEN_Pos)
#define RCC_ADC_CLKEN				RCC_ADC_CLKEN_Msk

/* CLKGENAL2SEL/CLKGENAL3SEL/CLKGENAL4SEL register bit mask */
#define RCC_7816M_CLKEN_Pos			(4U)
#define RCC_7816M_CLKEN_Msk			(0x1UL << RCC_7816M_CLKEN_Pos)
#define RCC_7816M_CLKEN				RCC_7816M_CLKEN_Msk
#define RCC_7816M_CLKDIV_Pos		(8U)
#define RCC_7816M_CLKDIV_Msk		(0x1FUL << RCC_7816M_CLKDIV_Pos)
#define RCC_7816M_CLKDIV			RCC_7816M_CLKDIV_Msk

/* CLKGENAL5SEL/CLKGENAL6SEL/CLKGENAL8SEL/CLKGENAL9SEL register bit mask */
#define RCC_PWM_CLKEN_Pos			(4U)
#define RCC_PWM_CLKEN_Msk			(0x1UL << RCC_PWM_CLKEN_Pos)
#define RCC_PWM_CLKEN				RCC_PWM_CLKEN_Msk
#define RCC_PWM_CLKDIV_Pos			(8U)
#define RCC_PWM_CLKDIV_Msk			(0x1FUL << RCC_PWM_CLKDIV_Pos)
#define RCC_PWM_CLKDIV				RCC_PWM_CLKDIV_Msk

/* CLKGENAL10SEL/ register bit mask */
#define RCC_UART5_CLKSRC_Pos		 (0U)
#define RCC_UART5_CLKSRC_Msk		 (0x7UL << RCC_UART5_CLKSRC_Pos)
#define RCC_UART5_CLKSRC			 RCC_UART5_CLKSRC_Msk
#define RCC_UART5_CLKEN_Pos			(4U)
#define RCC_UART5_CLKEN_Msk			(0x1UL << RCC_UART5_CLKEN_Pos)
#define RCC_UART5_CLKEN				RCC_UART5_CLKEN_Msk
#define RCC_UART5_CLKDIV_Pos		(8U)
#define RCC_UART5_CLKDIV_Msk		(0x1FUL << RCC_UART5_CLKDIV_Pos)
#define RCC_UART5_CLKDIV			RCC_UART5_CLKDIV_Msk

/* CLKTIM1SEL/CLKTIM2SEL/CLKTIM3SEL/CLKTIM4SEL/CLKTIM5SEL/CLKTIM6SEL register bit mask */
#define RCC_TIMER_CLKEN_Pos			(4U)
#define RCC_TIMER_CLKEN_Msk			(0x1UL << RCC_TIMER_CLKEN_Pos)
#define RCC_TIMER_CLKEN				RCC_TIMER_CLKEN_Msk
#define RCC_TIMER_CLKDIV_Pos		(8U)
#define RCC_TIMER_CLKDIV_Msk		(0x1FUL << RCC_TIMER_CLKDIV_Pos)
#define RCC_TIMER_CLKDIV			RCC_TIMER_CLKDIV_Msk

/* CLKTIM7SEL register bit mask */
#define RCC_WWDT_CLKEN_Pos			(4U)
#define RCC_WWDT_CLKEN_Msk			(0x1UL << RCC_WWDT_CLKEN_Pos)
#define RCC_WWDT_CLKEN				RCC_WWDT_CLKEN_Msk
#define RCC_WWDT_CLKDIV_Pos			(8U)
#define RCC_WWDT_CLKDIV_Msk			(0x1FUL << RCC_WWDT_CLKDIV_Pos)
#define RCC_WWDT_CLKDIV				RCC_WWDT_CLKDIV_Msk

/* CLKSYSTIMSEL register bit mask */
#define RCC_SYSTIM_CLKEN_Pos		(3U)
#define RCC_SYSTIM_CLKEN_Msk		(0x1UL << RCC_SYSTIM_CLKEN_Pos)
#define RCC_SYSTIM_CLKEN			RCC_SYSTIM_CLKEN_Msk
#define RCC_SYSTIM_CLKDIV_Pos		(4U)
#define RCC_SYSTIM_CLKDIV_Msk		(0x7UL << RCC_SYSTIM_CLKDIV_Pos)
#define RCC_SYSTIM_CLKDIV			RCC_SYSTIM_CLKDIV_Msk

/* CLKMONITORSEL register bit mask */
#define RCC_MONITOR_CLKEN_Pos		(3U)
#define RCC_MONITOR_CLKEN_Msk		(0x1UL << RCC_MONITOR_CLKEN_Pos)
#define RCC_MONITOR_CLKEN			RCC_MONITOR_CLKEN_Msk
#define RCC_MONITOR_CLKDIV_Pos		(4U)
#define RCC_MONITOR_CLKDIV_Msk		(0x7UL << RCC_MONITOR_CLKDIV_Pos)
#define RCC_MONITOR_CLKDIV			RCC_MONITOR_CLKDIV_Msk

/* CLKLSICON register bit mask */
#define RCC_BAT_LSION_Pos			(0U)
#define RCC_BAT_LSION_Msk			(0x1UL << RCC_BAT_LSION_Pos)
#define RCC_BAT_LSION				RCC_BAT_LSION_Msk
#define RCC_BAT_LSIRDY_Pos			(4U)
#define RCC_BAT_LSIRDY_Msk			(0x1UL << RCC_BAT_LSIRDY_Pos)
#define RCC_BAT_LSIRDY				RCC_BAT_LSIRDY_Msk

/* CLKLSECON register bit mask */
#define RCC_BAT_LSEON_Pos			(0U)
#define RCC_BAT_LSEON_Msk			(0x1UL << RCC_BAT_LSEON_Pos)
#define RCC_BAT_LSEON				RCC_BAT_LSEON_Msk
#define RCC_BAT_LSE_RESON_Pos		(1U)
#define RCC_BAT_LSE_RESON_Msk		(0x1UL << RCC_BAT_LSE_RESON_Pos)
#define RCC_BAT_LSE_RESON			RCC_BAT_LSE_RESON_Msk
#define RCC_BAT_LSERDY_Pos			(2U)
#define RCC_BAT_LSERDY_Msk			(0x1UL << RCC_BAT_LSERDY_Pos)
#define RCC_BAT_LSERDY				RCC_BAT_LSERDY_Msk

/* CLKVBATSEL register bit mask */
#define RCC_BAT_CLKDIV_Pos			(1U)
#define RCC_BAT_CLKDIV_Msk			(0x1UL << RCC_BAT_CLKDIV_Pos)
#define RCC_BAT_CLKDIV				RCC_BAT_CLKDIV_Msk

/* CLKRTCSEL register bit mask */
#define RCC_RTC_CLKEN_Pos			(1U)
#define RCC_RTC_CLKEN_Msk			(0x1UL << RCC_RTC_CLKEN_Pos)
#define RCC_RTC_CLKEN				RCC_RTC_CLKEN_Msk

/**
  * @}
  */ 


/** @defgroup RCC_Exported_Functions RCC Exported Functions
  * @{
  */  


/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @param  None
  * @retval None
  */
void RCC_DeInit(void)
{
	/* Set HSION bit */
	RCC_CLK->CLKCON |= RCC_CLKC_HSION;

	/* Set system clock is HSI */
	RCC_CLK->CLKSYSSEL = RCC_SYS_CLKSRC_HSI;
  
	/* Reset HSEON,  and PLLON bits */
	RCC_CLK->CLKCON &= ~(RCC_CLKC_HSEON | RCC_CLKC_PLLON);

	/* Reset HSECFG */
	RCC_CLK->CLKHSE = (uint32_t)0x00000000;
	
	/* Reset PLLCFG, PLLFREQ */
	RCC_CLK->CLKPLL = (uint32_t)0x0000442C;
	RCC_CLK->CLKPLLFREQ = (uint32_t)0x00000014;
	
	/* Mask the CLKINT  */
	RCC_CLK->CLKINTMSK = 0x07;
	
	RCC_CLK->CLKSLV1SCON = 0x00010000;
	RCC_CLK->CLKSLV2SCON = 0x00000000;
	RCC_CLK->CLKSLV3SCON = 0x00000040;
	RCC_CLK->CLKSLV4SCON = 0x00688006;
	RCC_CLK->CLKSLV5SCON = 0x00009802;
	
	RCC_CLK->CLKSLV1LPSCON = 0xFFFFFFFF;
	RCC_CLK->CLKSLV2LPSCON = 0xFFFFFFFF;
	RCC_CLK->CLKSLV3LPSCON = 0xFFFFFFFF;
	RCC_CLK->CLKSLV4LPSCON = 0xFFFFFFFF;
	RCC_CLK->CLKSLV5LPSCON = 0xFFFFFFFF;
	
	RCC_CLK->CLKUSBSEL = 0x00000000;
	RCC_CLK->CLKEXSPISEL = 0x00000002;
	RCC_CLK->CLKADCSEL = 0x00000000;
	
	RCC_CLK->CLKGENAL2SEL = 0x00000502;
	RCC_CLK->CLKGENAL3SEL = 0x00000502;
	RCC_CLK->CLKGENAL4SEL = 0x00000502;
	RCC_CLK->CLKGENAL5SEL = 0x00000000;
	RCC_CLK->CLKGENAL6SEL = 0x00000000;
	RCC_CLK->CLKGENAL8SEL = 0x00000000;
    RCC_CLK->CLKGENAL9SEL = 0x00000000;
    RCC_CLK->CLKGENAL10SEL = 0x00000000;
	
	RCC_CLK->CLKTIM1SEL = 0x00000000;
	RCC_CLK->CLKTIM2SEL = 0x00000000;
	RCC_CLK->CLKTIM3SEL = 0x00000000;
	RCC_CLK->CLKTIM4SEL = 0x00000000;
	RCC_CLK->CLKTIM5SEL = 0x00000000;
	RCC_CLK->CLKTIM6SEL = 0x00000000;
	RCC_CLK->CLKTIM7SEL = 0x00000000;
	
	RCC_CLK->CLKSYSTIMSEL = 0x00000000;
	//RCC_CLK->CLKMONITORSEL = 0x00000000;

}

/**
  * @brief Config System Clock
  * @param[in] Source
  * 	This parameter can be one of the following values: 
  * 	@arg RCC_SYS_CLKSRC_USBPHY 
  * 	@arg RCC_SYS_CLKSRC_PLL_H  
  * 	@arg RCC_SYS_CLKSRC_HSE	
  * 	@arg RCC_SYS_CLKSRC_HSI	
  * @param[in] division: the division of system clock 
  * @retval none
  */
void RCC_SystemClockConfig(uint32_t source, uint32_t division)
{
	assert_param(IS_SYSTEM_DIV(division));
	assert_param(IS_RCC_SYS_CLKSOURCE(source));
	
	RCC_CLK->CLKSYSSEL = source | (division << RCC_SYS_CLKDIV_Pos);
}

/**
  * @brief Get System Clock Source
  * @param none
  * @retval source: the source of system clock 
  * 	@arg RCC_SYS_CLKSRC_HSE
  * 	@arg RCC_SYS_CLKSRC_USBPHY
  * 	@arg RCC_SYS_CLKSRC_PLL_H
  * 	@arg RCC_SYS_CLKSRC_HSI
  */
uint32_t RCC_GetSystemClockSource(void)
{
	uint32_t source;
	source = RCC_CLK->CLKSYSSEL & RCC_SYS_CLKSRC;
	return source;
}
/**
  * @brief Get UART5 Clock Source
  * @param none
  * @retval source: the source of UART5 clock 
  * 	@arg RCC_UART5_CLKSRC_PLL_L
  * 	@arg RCC_UART5_CLKSRC_HSE
  * 	@arg RCC_UART5_CLKSRC_HSI
  */
uint32_t RCC_GetUART5ClockSource(void)
{
	uint32_t source;
	source = RCC_CLK->CLKGENAL10SEL & RCC_UART5_CLKSRC;
	return source;
}
/**
  * @brief Get System Clock Division
  * @param none
  * @retval division: the division of system clock 
  */
uint32_t RCC_GetUART5ClockDivision(void)
{
	uint32_t division,temp;
	division = (RCC_CLK->CLKGENAL10SEL & RCC_UART5_CLKDIV) >> RCC_UART5_CLKDIV_Pos;
	temp = division + 1;
	return temp;
}
/**
  * @brief Get System Clock Division
  * @param none
  * @retval division: the division of system clock 
  */
uint32_t RCC_GetSystemClockDivision(void)
{
	uint32_t division,temp;
	division = (RCC_CLK->CLKSYSSEL & RCC_SYS_CLKDIV) >> RCC_SYS_CLKDIV_Pos;
	switch(division)
	{
		case 0:temp = 1;break;
		case 1:temp = 2;break;
		case 2:temp = 4;break;
		case 3:temp = 8;break;
		case 4:temp = 16;break;
		default:temp = 32;break;
		
	}
	return temp;
}

/**
  * @brief Get PLL_L clock,before call the function you must enable PLL_L Clock
  * @param none
  * @retval division: the division of system clock 
  */
uint32_t RCC_GetPLL_L_Clock(void)
{
	volatile uint32_t SourceClock,PLL,M,N,Q;
	/*PLL_L's source clock is HSI*/
	if((RCC_CLK->CLKPLL&0x03)==RCC_PLLC_PLLSRC_HSI)
	{
		SourceClock = HSI_VALUE;
	}
	/*PLL_L's source clock is HSE*/
	if((RCC_CLK->CLKPLL&0x03)==RCC_PLLC_PLLSRC_HSE)
	{
		SourceClock = HSE_VALUE;
	}
	M = ((RCC_CLK->CLKPLL&0x0CU)>>2)+1;
	N = (RCC_CLK->CLKPLL>>4)&0x7F;
	switch((RCC_CLK->CLKPLL>>14)&0x03)
	{
		case 0:Q = 2;break;
		case 1:Q = 4;break;
		case 2:Q = 6;break;
		case 3:Q = 8;break;
	}
	PLL = (SourceClock*N)/(M*Q);
	return PLL;
}

uint32_t RCC_GetPLL_H_Clock(void)
{
	volatile uint32_t SourceClock,PLL,M,N,Q;
	/*PLL_L's source clock is HSI*/
	if((RCC_CLK->CLKPLL&0x03)==RCC_PLLC_PLLSRC_HSI)
	{
		SourceClock = HSI_VALUE;
	}
	/*PLL_L's source clock is HSE*/
	if((RCC_CLK->CLKPLL&0x03)==RCC_PLLC_PLLSRC_HSE)
	{
		SourceClock = HSE_VALUE;
	}
	M = ((RCC_CLK->CLKPLL&0x0CU)>>2)+1;
	N = (RCC_CLK->CLKPLL>>4)&0x7F;
	PLL = (SourceClock*N)/(M);
	return PLL;
}


/**
  * @brief  Configures the USB clock source and switch.   
  * @param[in] source: specifies the clock source of USB.
  * This parameter can be one of the following values:
  *	@arg RCC_USB_CLKSRC_USBPHY
  *	@arg RCC_USB_CLKSRC_PLL_L
  *	@arg RCC_USB_CLKSRC_HSE
  * @param[in] NewState: new state of the USB clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable USB clock
  *	 @arg ENABLE  :enable USB clock
  * @retval None
  */
void RCC_USBClockConfig(uint32_t source, FunctionalState NewState)
{ 
	assert_param(IS_RCC_USB_CLKSOURCE(source));
	if (NewState != DISABLE)
	{
		RCC_CLK->CLKUSBSEL = source | RCC_USB_CLKEN;
	}
	else
	{
		RCC_CLK->CLKUSBSEL &= ~RCC_USB_CLKEN;
	}
}

/**
  * @brief  Configures the SPI2 communication clock source and switch.   
  * @param[in] source: specifies the clock source of SPI2.
  * This parameter can be one of the following values:
  *	@arg RCC_SPI2_CLKSRC_PLL_H
  *	@arg RCC_SPI2_CLKSRC_HSE
  *	@arg RCC_SPI2_CLKSRC_HSI
  * @param[in] NewState: new state of the SPI2 clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable SPI2 clock
  *	 @arg ENABLE  :enable SPI2 clock
  * @retval None
  */
void RCC_SPI2ClockConfig(uint32_t source, FunctionalState NewState)
{ 
	assert_param(IS_RCC_SPI2_CLKSOURCE(source));
	RCC_CLK->CLKEXSPISEL = 0;
	if (NewState != DISABLE)
	{
		RCC_CLK->CLKEXSPISEL = source | RCC_SPI2_CLKEN;
	}
	else
	{
		RCC_CLK->CLKEXSPISEL &= ~RCC_SPI2_CLKEN;
	}
}

/**
  * @brief  Configures the ADC clock source.   
  * @param[in] source: specifies the clock source of ADC.
  * This parameter can be one of the following values:
  *	@arg RCC_ADC_CLKSRC_PLL_L
  *	@arg RCC_ADC_CLKSRC_HSE
  *	@arg RCC_ADC_CLKSRC_HSI
  * @param[in] NewState: new state of the ADC clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable ADC clock
  *	 @arg ENABLE  :enable ADC clock
  * @retval None
  */
void RCC_ADCClockConfig(uint32_t source, FunctionalState NewState)
{ 
	assert_param(IS_RCC_ADC_CLKSOURCE(source));
	if (NewState != DISABLE)
	{
		RCC_CLK->CLKADCSEL = source | RCC_ADC_CLKEN;
	}
	else
	{
		RCC_CLK->CLKADCSEL &= ~RCC_ADC_CLKEN;

	}
}

/**
  * @brief  Configures the 7816 Master clock source.   
  * @param[in] interface: specifies the 7816 master interface number.
  * This parameter can be one of the following values:
  *	@arg INTERFACE_7816M1
  *	@arg INTERFACE_7816M2
  *	@arg INTERFACE_7816M3
  * @param[in] source: specifies the clock source of 7816 Master.
  * This parameter can be one of the following values:
  *	@arg RCC_7816M_CLKSRC_PLL_L
  *	@arg RCC_7816M_CLKSRC_HSE
  *	@arg RCC_7816M_CLKSRC_HSI
  * @param[in] division: the division of 7816 master clock 
  * @param[in] NewState: new state of the 7816 Master clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable 7816 Master clock
  *	 @arg ENABLE  :enable 7816 Master clock
  * @retval None
  */
void RCC_7816MClockConfig(uint8_t interface, uint32_t source, uint32_t division, FunctionalState NewState)
{ 
	assert_param(IS_RCC_7816M_NUMBER(interface));
	assert_param(IS_7816M_DIV(division));
	assert_param(IS_RCC_7816M_CLKSOURCE(source));

	if (NewState != DISABLE)
	{
		*(&(RCC_CLK->CLKGENAL2SEL) + interface) = source | RCC_7816M_CLKEN | (division << RCC_7816M_CLKDIV_Pos);
	}
	else
	{
		*(&(RCC_CLK->CLKGENAL2SEL) + interface) &= ~RCC_7816M_CLKEN;
	}	
}

/**
  * @brief  Configures the UART5 clock source.   
  * @param[in] source: specifies the clock source of UART5.
  * This parameter can be one of the following values:
  *	@arg RCC_UART5_CLKSRC_PLL_L
  *	@arg RCC_UART5_CLKSRC_HSE
  *	@arg RCC_UART5_CLKSRC_HSI
  * @param[in] division: the division of UART5_clock 
  * @param[in] NewState: new state of the UART5 clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable UART5 clock
  *	 @arg ENABLE  :enable UART5 clock
  * @retval None
  */
void RCC_UART5ClockConfig(uint32_t source, uint32_t division, FunctionalState NewState)
{ 
	assert_param(IS_UART5_DIV(division));
	assert_param(IS_RCC_UART5_CLKSOURCE(source));

	if (NewState != DISABLE)
	{
		*(&(RCC_CLK->CLKGENAL10SEL)) = source | RCC_UART5_CLKEN | (division << RCC_UART5_CLKDIV_Pos);
	}
	else
	{
		*(&(RCC_CLK->CLKGENAL10SEL)) &= ~RCC_UART5_CLKEN;
	}	
}

/**
  * @brief  Configures the PWM clock source.   
  * @param[in] interface: specifies the PWM interface number.
  * This parameter can be one of the following values:
  *	@arg INTERFACE_PWM1
  *	@arg INTERFACE_PWM2
  *	@arg INTERFACE_PWM3
  *	@arg INTERFACE_PWM4
  * @param[in] source: specifies the clock source of PWM.
  * This parameter can be one of the following values:
  *	@arg RCC_PWM_CLKSRC_PLL_L
  *	@arg RCC_PWM_CLKSRC_HSE
  *	@arg RCC_PWM_CLKSRC_HSI
  * @param[in] division: the division of PWM clock 
  * @param[in] NewState: new state of the PWM clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable PWM clock
  *	 @arg ENABLE  :enable PWM clock
  * @retval None
  */
void RCC_PWMClockConfig(uint8_t interface, uint32_t source, uint32_t division, FunctionalState NewState)
{ 
	assert_param(IS_RCC_PWM_NUMBER(interface));
	assert_param(IS_RCC_PWM_CLKSOURCE(source));
	if (NewState != DISABLE)
	{
        if((interface==INTERFACE_PWM1) || (interface==INTERFACE_PWM2))
        {
            *((&RCC_CLK->CLKGENAL5SEL) + interface) = source | RCC_PWM_CLKEN | (division << RCC_PWM_CLKDIV_Pos);
        }
        else
        {
            *((&RCC_CLK->CLKGENAL8SEL) + interface) = source | RCC_PWM_CLKEN | (division << RCC_PWM_CLKDIV_Pos);
        }
	}
	else
	{
        if((interface==INTERFACE_PWM1) || (interface==INTERFACE_PWM2))
        {
            *((&RCC_CLK->CLKGENAL5SEL) + interface) &= ~RCC_PWM_CLKEN;
        }
        else
        {
            *((&RCC_CLK->CLKGENAL8SEL) + interface) &= ~RCC_PWM_CLKEN;
        }
	}	
}


/**
  * @brief  Configures the timer clock source.   
  * @param[in] interface: specifies the timer interface number.
  * This parameter can be one of the following values:
  *	@arg PERIPHERAL_TIMER1
  *	@arg PERIPHERAL_TIMER2
  *	@arg PERIPHERAL_TIMER3
  *	@arg PERIPHERAL_TIMER4
  *	@arg PERIPHERAL_TIMER5
  *	@arg PERIPHERAL_TIMER6
  * @param[in] source: specifies the clock source of timer.
  * This parameter can be one of the following values:
  *	@arg RCC_TIMER_CLKSRC_PLL_L
  *	@arg RCC_TIMER_CLKSRC_HSE
  *	@arg RCC_TIMER_CLKSRC_HSI
  * @param[in] division: the division of timer clock 
  * @param[in] NewState: new state of the timer clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable timer clock
  *	 @arg ENABLE  :enable timer clock
  * @retval None
  */
void RCC_TimerClockConfig(uint8_t peripheral, uint32_t source, uint32_t division, FunctionalState NewState)
{ 
	assert_param(IS_RCC_TIMER_CLKSOURCE(source));
	assert_param(IS_RCC_TIMER_NUMBER(peripheral));
	
	if (NewState != DISABLE)
	{
		*(&(RCC_CLK->CLKTIM1SEL) + peripheral)  = source | RCC_TIMER_CLKEN | (division << RCC_TIMER_CLKDIV_Pos);
	}
	else
	{
		*(&(RCC_CLK->CLKTIM1SEL) + peripheral) &= ~RCC_TIMER_CLKEN;
	}
}
/**
  * @brief  Configures the WWDT clock source.   
  * @param[in] source: specifies the clock source of WWDT.
  * This parameter can be one of the following values:
  *	@arg RCC_WWDT_CLKSRC_PLL_L
  *	@arg RCC_WWDT_CLKSRC_HSE
  *	@arg RCC_WWDT_CLKSRC_HSI
  * @param[in] division: the division of WWDT clock 
  * @param[in] NewState: new state of the WWDT clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable WWDT clock
  *	 @arg ENABLE  :enable WWDT clock
  * @retval None
  */
void RCC_WWDTClockConfig(uint32_t source, uint32_t division, FunctionalState NewState)
{ 
	assert_param(IS_RCC_WWDT_CLKSOURCE(source));
	if (NewState != DISABLE)
	{
		RCC_CLK->CLKTIM7SEL = source | RCC_WWDT_CLKEN | (division << RCC_WWDT_CLKDIV_Pos);
	}
	else
	{
		RCC_CLK->CLKTIM7SEL &= ~RCC_TIMER_CLKEN;
	}

}
/**
  * @brief  Configures the systick clock source.
  * @note The configuration of this register is valid only when the core register 
  *	   systick->CTRL.CLKSOURCE is selected to use an external reference clock.
  * @param[in] source: specifies the clock source of system timer.
  * This parameter can be one of the following values:
  *	@arg RCC_SYSTICK_CLKSRC_HSE
  *	@arg RCC_SYSTICK_CLKSRC_USBPHY
  *	@arg RCC_SYSTICK_CLKSRC_PLL_H
  *	@arg RCC_SYSTICK_CLKSRC_HSI
  * @param[in] division: the division of systick clock 
  *	@arg RCC_SYSTICK_CLKDIV_0
  *	@arg RCC_SYSTICK_CLKDIV_2
  *	@arg RCC_SYSTICK_CLKDIV_4
  *	@arg RCC_SYSTICK_CLKDIV_8
  *	@arg RCC_SYSTICK_CLKDIV_16
  *	@arg RCC_SYSTICK_CLKDIV_32
  * @param[in] NewState: new state of the system timer clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable system timer clock
  *	 @arg ENABLE  :enable system timer clock
  * @retval None
  */
void RCC_SysTickClockConfig(uint32_t source, uint32_t division, FunctionalState NewState)
{ 
	assert_param(IS_RCC_SYSTICK_CLKSOURCE(source));
	assert_param(IS_RCC_SYSTICK_CLKDIV(division));
	
	if (NewState != DISABLE)
	{
		RCC_CLK->CLKSYSTIMSEL = source | RCC_SYSTIM_CLKEN | (division << RCC_SYSTIM_CLKDIV_Pos);
	}
	else
	{
		RCC_CLK->CLKSYSTIMSEL &= ~RCC_SYSTIM_CLKEN;
	}
}


/**
  * @brief  Configures the BAT clock source.   
  * @param[in] source: specifies the clock source of BAT.
  * This parameter can be one of the following values:
  *	@arg RCC_BAT_CLKSRC_LSE
  *	@arg RCC_BAT_CLKSRC_LSI
  * @param[in] division: the division of BAT clock 
  *	@arg RCC_BAT_CLKDIV_0
  *	@arg RCC_BAT_CLKDIV_2
  * @retval None
  */
void RCC_BATClockConfig(uint32_t source, uint32_t division)
{ 
	assert_param(IS_RCC_BAT_CLKSOURCE(source));
	assert_param(IS_RCC_BAT_CLKDIV(division));
	
	RCC_VBAT->VBATSEL = source | (division << RCC_BAT_CLKDIV_Pos);
}

/**
  * @brief  Configures the RTC clock source.   
  * @param[in] source: specifies the clock source of RTC.
  * This parameter can be one of the following values:
  *	@arg RCC_RTC_CLKSRC_LSE
  *	@arg RCC_RTC_CLKSRC_LSI
  * @param[in] NewState: new state of the RTC clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable RTC clock
  *	 @arg ENABLE  :enable RTC clock
  * @retval None
  */
void RCC_RTCClockConfig(uint32_t source, FunctionalState NewState)
{ 
	assert_param(IS_RCC_RTC_CLKSOURCE(source));
	if (NewState != DISABLE)
	{
		RCC_VBAT->RTCSEL = source | RCC_RTC_CLKEN;
	}
	else
	{
		RCC_VBAT->RTCSEL &= ~RCC_RTC_CLKEN;
	}

}

//==============================================
/**
  * @brief  Configure the External High Speed oscillator (HSE).
  * @param[in] HSE_Config pointer to an RCC_HSEConfigTypeDef structure that
  *		 contains the configuration information for the External High Speed oscillator. 
  * @param[in] NewState: new state of the specified HSE clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable HSE
  *	 @arg ENABLE  :enable HSE
  * @retval none
  * @note	none
  */  
void RCC_HSEConfig(RCC_HSEConfigTypeDef *HSE_Config,FunctionalState NewState)
{
	/* Reset HSEON before configuring the HSE ------------------*/
	RCC_CLK->CLKCON &= ~RCC_CLKC_HSEON;
	
	if(NewState == DISABLE)
	{
		return;
	}
	/* Configure HSE bypass ------------------*/
	if(HSE_Config->HSE_ByPass != DISABLE)
	{
		RCC_CLK->CLKHSE	|= RCC_HSEC_BYP;	
	}
	else
	{
		RCC_CLK->CLKHSE	&= ~RCC_HSEC_BYP;	
	}
	/* Configure HSE feedback resistance ------------------*/
	if(HSE_Config->FeedbackResEanble != DISABLE)
	{
		RCC_CLK->CLKHSE |= RCC_HSEC_FBEN;
	}
	else
	{
		RCC_CLK->CLKHSE &= ~RCC_HSEC_FBEN;
	}
	/* Configure HSE output frequency ------------------*/
	/* Reset output frequency bits*/
	RCC_CLK->CLKHSE &= ~RCC_HSEC_FRE;
	/* The output frequency is in range of 1MHz to 12MHz*/
	if((HSE_Config->HSE_ClockValue >=1000000)&&(HSE_Config->HSE_ClockValue <=12000000))
	{
		RCC_CLK->CLKHSE |= RCC_HSEC_FRE_0;
	}
	/* The output frequency is in range of 12MHz to 24MHz*/
	else if((HSE_Config->HSE_ClockValue >12000000)&&(HSE_Config->HSE_ClockValue <=24000000))
	{
		RCC_CLK->CLKHSE |= RCC_HSEC_FRE_1;
	}
	/* The output frequency is in range of 24MHz to 42MHz*/
	else if((HSE_Config->HSE_ClockValue >24000000)&&(HSE_Config->HSE_ClockValue <=42000000))
	{
		RCC_CLK->CLKHSE |= RCC_HSEC_FRE_2;
	}
	/* The output frequency is in range of 42MHz to 66MHz*/
	else if((HSE_Config->HSE_ClockValue >42000000)&&(HSE_Config->HSE_ClockValue <=66000000))
	{
		RCC_CLK->CLKHSE |= RCC_HSEC_FRE_3;
	}

	/* Set HSEON ------------------*/
	RCC_CLK->CLKCON |= RCC_CLKC_HSEON;
}

/**
  * @brief  Turn on or turn off the HSI clock.
  * @param[in] NewState: new state of the specified HSI clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable HSI
  *	 @arg ENABLE  :enable HSI
  * @retval none
  * @note	none
  */  
void RCC_HSICmd(FunctionalState NewState)
{
	/* Reset HSEON before configuring the HSE ------------------*/
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKCON |= RCC_CLKC_HSION;
	}
	else
	{
		RCC_CLK->CLKCON &= ~RCC_CLKC_HSION;
	}
}

/**
  * @brief  Configure the PLL clock source and clock frequency .
  * @param[in] PLL_Config pointer to an RCC_PLL_InitTypeDef structure that
  *		 contains the configuration information for the External High Speed oscillator. 
  * @param[in] NewState: new state of the specified HSE clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable HSE
  *	 @arg ENABLE  :enable HSE
  * @retval none
  * @note	none
  */  
void RCC_PLLConfig(RCC_PLL_InitTypeDef *PLL_Config)
{
	float Temp;
	RCC_HSEConfigTypeDef HSE_Config;
	assert_param(IS_RCC_PLL_CLKSOURCE(PLL_Config->PLL_Input));
	assert_param(IS_PLLQ_PARAMETR_OUT(PLL_Config->PLL_Q));
	assert_param(IS_PLLN_PARAMETR_OUT(PLL_Config->PLL_N));
	assert_param(IS_PLLM_PARAMETR_OUT(PLL_Config->PLL_M));
	
	/* Choose PLL clock source ------------------*/
	RCC_CLK->CLKPLL &= ~RCC_PLLC_PLLSRC;  
	if (PLL_Config->PLL_Input == RCC_PLLC_PLLSRC_HSI)
	{
		RCC_CLK->CLKPLL |= RCC_PLLC_PLLSRC_HSI;
	}
	else if (PLL_Config->PLL_Input == RCC_PLLC_PLLSRC_HSE)
	{
		if(RCC_GetSystemClockSource() != RCC_SYS_CLKSRC_HSE)
		{
			HSE_Config.FeedbackResEanble =ENABLE;
			HSE_Config.HSE_ByPass = DISABLE;
			HSE_Config.HSE_ClockValue = HSE_VALUE;
			RCC_HSEConfig(&HSE_Config,ENABLE);
			while(!(RCC_CLK->CLKCON&(1UL<<3)));
		}		
		RCC_CLK->CLKPLL |= RCC_PLLC_PLLSRC_HSE;
	}
	else
	{
		return;
	}
	
	/* Configure PLL_L ------------------*/
	RCC_CLK->CLKPLL &= ~RCC_PLLC_PLLQ;
	switch (PLL_Config->PLL_Q)
	{
		case 2:	RCC_CLK->CLKPLL	|= RCC_PLLC_PLLQ_2;
				break;
		case 4:	RCC_CLK->CLKPLL	|= RCC_PLLC_PLLQ_4;
				break;
		case 6: RCC_CLK->CLKPLL	|= RCC_PLLC_PLLQ_6;
				break;
		case 8: RCC_CLK->CLKPLL	|= RCC_PLLC_PLLQ_8;
				break;
		default: 
				break;
	}
	
	/* Configure PLL_H ------------------*/
	RCC_CLK->CLKPLL &= ~RCC_PLLC_PLLM;
	switch (PLL_Config->PLL_M)
	{
		case 1:	RCC_CLK->CLKPLL	|= RCC_PLLC_PLLM_1;
				break;
		case 2:	RCC_CLK->CLKPLL	|= RCC_PLLC_PLLM_2;
				break;
		case 3: RCC_CLK->CLKPLL	|= RCC_PLLC_PLLM_3;
				break;
		case 4: RCC_CLK->CLKPLL	|= RCC_PLLC_PLLM_4;
				break;
		default: 
				break;
	}
	
	RCC_CLK->CLKPLL &= ~RCC_PLLC_PLLN;
	RCC_CLK->CLKPLL	|= PLL_Config->PLL_N << RCC_PLLC_PLLN_Pos;
	
	/* Set PLL_H frequency ---------------*/
	Temp = (((PLL_Config->SourceClockVaule)*(PLL_Config->PLL_N))/PLL_Config->PLL_M) / 1000000;
	if(Temp<= 50)
	{
		RCC_CLK->CLKPLLFREQ = 0;
	}
	else if((50<Temp)&&(Temp<= 55))
	{
		RCC_CLK->CLKPLLFREQ = 0x01;
	}
	else if((55<Temp)&&(Temp<= 60))
	{
		RCC_CLK->CLKPLLFREQ = 0x02;
	}
	else if((60<Temp)&&(Temp<= 75))
	{
		RCC_CLK->CLKPLLFREQ = 0x03;
	}
	else if((75<Temp)&&(Temp<= 80))
	{
		RCC_CLK->CLKPLLFREQ = 0x04;
	}
	else if((80<Temp)&&(Temp<= 85))
	{
		RCC_CLK->CLKPLLFREQ = 0x05;
	}
	else if((85<Temp)&&(Temp<= 90))
	{
		RCC_CLK->CLKPLLFREQ = 0x06;
	}
	else if((90<Temp)&&(Temp<= 95))
	{
		RCC_CLK->CLKPLLFREQ = 0x07;
	}
	else if((95<Temp)&&(Temp<= 100))
	{
		RCC_CLK->CLKPLLFREQ = 0x08;
	}
	else if((100<Temp)&&(Temp<= 105))
	{
		RCC_CLK->CLKPLLFREQ = 0x09;
	}
	else if((105<Temp)&&(Temp<= 110))
	{
		RCC_CLK->CLKPLLFREQ = 0x0A;
	}
	else if((110<Temp)&&(Temp<= 115))
	{
		RCC_CLK->CLKPLLFREQ = 0x0B;
	}
	else if((115<Temp)&&(Temp<= 120))
	{
		RCC_CLK->CLKPLLFREQ = 0x0C;
	}
	else if((120<Temp)&&(Temp<= 130))
	{
		RCC_CLK->CLKPLLFREQ = 0x0D;
	}
	else if((130<Temp)&&(Temp<= 140))
	{
		RCC_CLK->CLKPLLFREQ = 0x0E;
	}
	else if((140<Temp)&&(Temp<= 150))
	{
		RCC_CLK->CLKPLLFREQ = 0x0F;
	}
	else if((150<Temp)&&(Temp<= 160))
	{
		RCC_CLK->CLKPLLFREQ = 0x10;
	}
	else if((160<Temp)&&(Temp<= 170))
	{
		RCC_CLK->CLKPLLFREQ = 0x11;
	}
	else if((170<Temp)&&(Temp<= 180))
	{
		RCC_CLK->CLKPLLFREQ = 0x12;
	}
	else if((180<Temp)&&(Temp<= 190))
	{
		RCC_CLK->CLKPLLFREQ = 0x13;
	}
	else if((190<Temp)&&(Temp<= 200))
	{
		RCC_CLK->CLKPLLFREQ = 0x14;
	}
}

/**
  * @brief  Turn on or turn off the PLL clock.
  * @param[in] NewState: new state of the specified PLL clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable PLL
  *	 @arg ENABLE  :enable PLL
  * @retval none
  * @note	none
  */  
void RCC_PLLCmd(FunctionalState NewState)
{
	/* Reset HSEON before configuring the HSE ------------------*/
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKCON |= RCC_CLKC_PLLON;
	}
	else
	{
		RCC_CLK->CLKCON &= ~RCC_CLKC_PLLON;
	}
}

/**
  * @brief Get PLL Clock Source
  * @retval source: the source of PLL clock 
  * 	@arg RCC_PLLC_PLLSRC_HSE	
  * 	@arg RCC_PLLC_PLLSRC_HSI
  * @retval none
  */
uint32_t RCC_GetPLLClockSource(void)
{
	uint32_t source;
	source = RCC_CLK->CLKPLL & RCC_PLLC_PLLSRC;
	return source;
}
/**
  * @brief  Turn on or turn off the LSI clock.
  * @param[in] NewState: new state of the specified LSI clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable LSI
  *	 @arg ENABLE  :enable LSI
  * @retval none
  * @note	none
  */  
void RCC_LSICmd(FunctionalState NewState)
{
	if(NewState != DISABLE)
	{
		RCC_VBAT->LSICON |= RCC_BAT_LSION;
	}
	else
	{
		RCC_VBAT->LSICON &= ~RCC_BAT_LSION;
	}
}

/**
  * @brief  Turn on or turn off the LSE clock.
  * @param[in] NewState: new state of the specified LSE clock
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE 
  *	 @arg ENABLE  
  * @param[in] ResState: enable or disable the internal feedback resistor
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable LSE internal feedback resistor
  *	 @arg ENABLE  :enable LSE internal feedback resistor
  * @retval none
  * @note	none
  */  
void RCC_LSECmd(FunctionalState NewState, FunctionalState ResState)
{
	if(ResState != DISABLE)
	{
		RCC_VBAT->LSECON |= RCC_BAT_LSE_RESON;
	}
	else
	{
		RCC_VBAT->LSECON &= ~RCC_BAT_LSE_RESON;
	}
	
	if(NewState != DISABLE)
	{
		RCC_VBAT->LSECON |= RCC_BAT_LSEON;
	}
	else
	{
		RCC_VBAT->LSECON &= ~RCC_BAT_LSEON;
	}
}

/**
  * @brief  Checks whether the specified RCC clock is set or not.
  * @param[in] RCC_FLAG: specifies the clock source to check.
  *	 @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *	 @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *	 @arg RCC_FLAG_PLLRDY: PLL clock ready
  *	 @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *	 @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  * @retval none
  *		@arg SET : Clock ready
  *	 @arg RESET : Clock not ready
  * @note	none
  */  
FlagStatus RCC_GetFlagStatus(uint32_t RCC_FLAG)
{
	
	switch(RCC_FLAG)
	{
		case RCC_FLAG_HSIRDY:	/* Determine whether the HSI clock is stable ------------------*/
								if (RCC_CLK->CLKCON & RCC_CLKC_HSIRDY)
								{
									return SET;
								}
								break;

		case RCC_FLAG_HSERDY:	/* Determine whether the HSE clock is stable ------------------*/
								if (RCC_CLK->CLKCON & RCC_CLKC_HSERDY)
								{
									return SET;
								}
								break;
								
		case RCC_FLAG_PLLRDY:	/* Determine whether the PLL clock is stable ------------------*/
								if (RCC_CLK->CLKCON & RCC_CLKC_PLLRDY)
								{
									return SET;
								}
								break;
								
		case RCC_FLAG_LSIRDY:	/* Determine whether the LSI clock is stable ------------------*/
								if (RCC_VBAT->LSICON & RCC_BAT_LSIRDY)
								{
									return SET;
								}
								break;
								
		case RCC_FLAG_LSERDY:	/* Determine whether the LSE clock is stable ------------------*/
								if (RCC_VBAT->LSECON & RCC_BAT_LSERDY)
								{
									return SET;
								}
								break;
								
		default:				break;
	}
	return RESET;
}

/**
  * @brief  Returns the SYSCLK frequency
  *
  * @note   The system frequency computed by this function is not the real
  *		 frequency in the chip. It is calculated based on the predefined
  *		 constant and the selected clock source:
  * @note	 If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note	 If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note	 If SYSCLK source is PLL, function returns values based on HSE_VALUE(**)
  *		   or HSI_VALUE(*) multiplied/divided by the PLL factors.
  * @note	 (*) HSI_VALUE is a constant defined in thm36_1.3_hal_def.h file (default value
  *			   12 MHz) but the real value may vary depending on the variations
  *			   in voltage and temperature.
  * @note	 (**) HSE_VALUE is a constant defined in thm36_1.3_hal_def.h file (default value
  *				12 MHz), user has to ensure that HSE_VALUE is same as the real
  *				frequency of the crystal used. Otherwise, this function may
  *				have wrong result.
  *
  * @note   This function can be used by the user application to compute the
  *		 baudrate for the communication peripherals or configure other parameters.
  *
  * @note   Each time SYSCLK changes, this function must be called to update the
  *		 right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  *
  * @retval SYSCLK frequency
  */
uint32_t RCC_GetSysClocksFreq(void)
{
	uint32_t pllm = 0x00, plln = 0x00;
	uint32_t sysclockfreq = 0x00;
	uint32_t sysdiv = 0x00;
	
	/* Get SYSCLK division -----------------------------------------------------*/
	sysdiv = (RCC_CLK->CLKSYSSEL & RCC_SYS_CLKDIV) >> RCC_SYS_CLKDIV_Pos;

	/* Get SYSCLK source -------------------------------------------------------*/
	switch (RCC_CLK->CLKSYSSEL & RCC_SYS_CLKSRC)
	{
		case RCC_SYS_CLKSRC_HSI:  /* HSI used as system clock source */
		{
			sysclockfreq = (HSI_VALUE/(sysdiv+1));
			break;
		}
		case RCC_SYS_CLKSRC_HSE:  /* HSE used as system clock source */
		{
			sysclockfreq = (HSE_VALUE/(sysdiv+1));
			break;
		}
		case RCC_SYS_CLKSRC_PLL_H:  /* PLL used as system clock source */
		{
#ifndef EMULATION
			pllm = ((RCC_CLK->CLKPLL & RCC_PLLC_PLLM) >> 2) + 1;
			plln = (RCC_CLK->CLKPLL & RCC_PLLC_PLLN) >> 4;
			if((RCC_CLK->CLKPLL &RCC_PLLC_PLLSRC) == RCC_PLLC_PLLSRC_HSE)
			{
				/* HSE used as PLL clock source */
				sysclockfreq = (uint32_t) ((((uint64_t) HSE_VALUE * ((uint64_t)plln)) / (uint64_t)pllm) /(sysdiv+1));
			}
			else
			{
				/* HSI used as PLL clock source */
				sysclockfreq = (uint32_t) ((((uint64_t) HSI_VALUE * ((uint64_t)plln)) / (uint64_t)pllm) /(sysdiv+1));
			}
#else
			sysclockfreq = (HSI_VALUE/(sysdiv+1));
#endif
			break;
		}
		default:
		{
			sysclockfreq = (HSI_VALUE/(sysdiv+1));
			break;
		}
	}
	return sysclockfreq;
}



/**
  * @brief  Enable or disable the IT of RCC
  * @param[in] RCC_MSK
  *	This parameter can be one of the following values: 
  *		@arg RCC_IT_PLLRDY: PLL Ready interrupt
  *	 @arg RCC_IT_HSERDY: HSE Ready interrupt
  *	 @arg RCC_IT_HSIRDY: HSI Ready interrupt
  * @param[in] FunctionalState
  *		@arg DISABLE :disable 
  *	 @arg ENABLE  :enable  
  * @retval none
  * @note none
  */ 
void RCC_ITConfig(uint32_t RCC_MSK,FunctionalState NewState)
{
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKINTMSK &= ~RCC_MSK;
	}
	else
	{
		RCC_CLK->CLKINTMSK |= RCC_MSK;
	}
}

/**
  * @brief  Get the Status of RST's STS Register
  * @param[in] Status
  * 	This parameter can be one of the following values: 
  *		@arg RCC_RST_ALARM_SEC
  *		@arg RCC_RST_ALARM_SENIP
  *		@arg RCC_RST_S7816
  *	 @arg RCC_RST_POR
  *		@arg RCC_RST_DBG
  *	 @arg RCC_RST_ALARM_IWDT
  *		@arg RCC_RST_ALARM_WWDT
  *	 @arg RCC_RST_PIN
  * @retval Reset status
  * @note none
  */ 
FlagStatus RCC_GetResetStatus(uint32_t status)
{
	assert_param(IS_RCC_RESET(status));
	if(RCC_REST->STS & status)
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Clear the Status of RST's STS Register
  * @param[in] Status
  * 	This parameter can be one of the following values: 
  *		@arg RCC_RST_ALARM_SEC
  *		@arg RCC_RST_ALARM_SENIP
  *		@arg RCC_RST_S7816
  *	 @arg RCC_RST_POR
  *		@arg RCC_RST_DBG
  *	 @arg RCC_RST_ALARM_IWDT
  *		@arg RCC_RST_ALARM_WWDT
  *	 @arg RCC_RST_PIN
  * @retval none
  * @note none
  */  
void RCC_ClearResetStatus(uint32_t Status)
{	
	assert_param(IS_RCC_RESET(Status));
	RCC_REST->STS |= Status;
}


/**
  * @brief  Enables or disables the AHB clock.   
  * @param[in]  RCC_HighPerfor: specifies the High peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_AHBPeriph_FLASHCACHE:	FLASH_ACC's clock
  *	@arg RCC_AHBPeriph_SPI2:			SPI2's clock 
  *	@arg RCC_AHBPeriph_DMA:			DMA's clock		   
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_AHBClockCmd(uint32_t RCC_AHBPerfor, FunctionalState NewState)
{
	assert_param(IS_RCC_AHBPeriph(RCC_AHBPerfor));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV1SCON |= RCC_AHBPerfor;
	}
	else
	{
		RCC_CLK->CLKSLV1SCON &= ~RCC_AHBPerfor;
	}
}

/**
  * @brief  Enables or disables the AHB peripheral clock,in Sleep Mode.   
  * @param[in]  RCC_HighPerfor: specifies the AHB peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_AHBPeriph_FLASHCACHE
  *	@arg RCC_AHBPeriph_SPI2
  *	@arg RCC_AHBPeriph_DMA		  
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_AHBPerforClockCmdInSleepMode(uint32_t RCC_AHBPerfor, FunctionalState NewState)
{
	assert_param(IS_RCC_AHBPeriph(RCC_AHBPerfor));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV1LPSCON |= RCC_AHBPerfor;
	}
	else
	{
		RCC_CLK->CLKSLV1LPSCON &= ~RCC_AHBPerfor;
	}
}
/**
  * @brief  Enables or disables the APB2 peripheral clock.   
  * @param[in]  APB2_Periph: specifies the APB2 peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB2Periph_7816M3:	7816M3's clock	   
  *	@arg RCC_APB2Periph_PWM1:	PWM1's clock
  *	@arg RCC_APB2Periph_PWM3:	PWM3's clock
  *	@arg RCC_APB2Periph_WDT:		WDT's clock
  *	@arg RCC_APB2Periph_CRC:		CRC's clock
  *	@arg RCC_APB2Periph_ADC:		ADC's clock
  *	@arg RCC_APB2Periph_7816M1:	7816M1's clock
  *	@arg RCC_APB2Periph_I2C:		I2C's clock
  *	@arg RCC_APB2Periph_TIMER5_6:TIMER5/6's clock  
  *	@arg RCC_APB2Periph_TIMER1_2:TIMER1/2's clock
  *	@arg RCC_APB2Periph_UART1:	UART1's clock  
  *	@arg RCC_APB2Periph_SPI3:	SPI3's clock 
  *	@arg RCC_APB2Periph_SPI1:	SPI1's clock 
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_APB2Periph(RCC_APB2Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV2SCON |= RCC_APB2Periph;
	}
	else
	{
		RCC_CLK->CLKSLV2SCON &= ~RCC_APB2Periph;
	}
}

/**
  * @brief  Enables or disables the APB2 peripheral clock,in Sleep Mode.   
  * @param[in]  APB2_Periph: specifies the APB2 peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB2Periph_7816M3	   
  *	@arg RCC_APB2Periph_PWM1	  
  *	@arg RCC_APB2Periph_WDT 
  *	@arg RCC_APB2Periph_CRC  
  *	@arg RCC_APB2Periph_ADC	   
  *	@arg RCC_APB2Periph_7816M1	   
  *	@arg RCC_APB2Periph_I2C
  *	@arg RCC_APB2Periph_TIMER5_6
  *	@arg RCC_APB2Periph_TIMER1_2
  *	@arg RCC_APB2Periph_UART1
  *	@arg RCC_APB2Periph_SPI3 
  *	@arg RCC_APB2Periph_SPI1 
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_APB2PeriphClockCmdInSleepMode(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_APB2Periph(RCC_APB2Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV2LPSCON |= RCC_APB2Periph;
	}
	else
	{
		RCC_CLK->CLKSLV2LPSCON &= ~RCC_APB2Periph;
	}
}

/**
  * @brief  Enables or disables the APB3 peripheral clock.   
  * @param[in]  APB3_Periph: specifies the APB3 peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB3Periph_PWM2: 	PWM2's clock 
  *	@arg RCC_APB3Periph_7816M2:  7816M2's clock	  
  *	@arg RCC_APB3Periph_GPIO:  	GPIO's clock
  *	@arg RCC_APB3Periph_TIMER3_4:Timer3&4's clock	  
  *	@arg RCC_APB3Periph_UART2:  	UART2's clock 
  *	@arg RCC_APB3Periph_UART5:  	UART5's clock
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_APB3PeriphClockCmd(uint32_t RCC_APB3Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_APB3Periph(RCC_APB3Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV3SCON |= RCC_APB3Periph;
	}
	else
	{
		RCC_CLK->CLKSLV3SCON &= ~RCC_APB3Periph;
	}
}

/**
  * @brief  Enables or disables the APB3 peripheral clock,in sleep mode  
  * @param[in]  APB3_Periph: specifies the APB3 peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB3Periph_PWM2
  *	@arg RCC_APB3Periph_7816M2	 
  *	@arg RCC_APB3Periph_GPIO
  *	@arg RCC_APB3Periph_TIMER3_4	 
  *	@arg RCC_APB3Periph_UART2
  *	@arg RCC_APB3Periph_UART5
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	  
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_APB3PeriphClockCmdInSleepMode(uint32_t RCC_APB3Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_APB3Periph(RCC_APB3Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV3LPSCON |= RCC_APB3Periph;
	}
	else
	{
		RCC_CLK->CLKSLV3LPSCON &= ~RCC_APB3Periph;
	}
}

/**
  * @brief  Enables or disables the SEC1 peripheral clock.   
  * @param[in]  RCC_SEC1Periph: specifies the SEC1 peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC1Periph_VBSENSOR: 						The clock used to read the VBSENSOR status in the main power domain 
  *	@arg RCC_SEC1Periph_FrequencyDerector:				FD's clock
  *	@arg RCC_SEC1Periph_LightDerector:					LD's clock
  *	@arg RCC_SEC1Periph_ExternalVoltageGlitchDetection:	EGS's clock
  *	@arg RCC_SEC1Periph_ActiveShield:					ASH's clock
  *	@arg RCC_SEC1Periph_TRNG:							TRNG's clock
  *	@arg RCC_SEC1Periph_PKE:								PKE's clock
  *	@arg RCC_SEC1Periph_DES:								DES's clock
  *	@arg RCC_SEC1Periph_AES:								AES's clock
  *	@arg RCC_SEC1Periph_PWRC:							POWER CONTROL's clock
  *	@arg RCC_SEC1Periph_INT:								Interrupt's clock
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_Security1PeriphClockCmd(uint32_t RCC_SEC1Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_SEC1Periph(RCC_SEC1Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV4SCON |= RCC_SEC1Periph;
	}
	else
	{
		RCC_CLK->CLKSLV4SCON &= ~RCC_SEC1Periph;
	}
}

/**
  * @brief  Enables or disables the SEC1 peripheral clock, in sleep mode   
  * @param[in]  RCC_SEC1Periph: specifies the SEC1 peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC1Periph_VBSENSOR  
  *	@arg RCC_SEC1Periph_FrequencyDerector 
  *	@arg RCC_SEC1Periph_LightDerector
  *	@arg RCC_SEC1Periph_ExternalVoltageGlitchDetection
  *	@arg RCC_SEC1Periph_ActiveShield
  *	@arg RCC_SEC1Periph_TRNG
  *	@arg RCC_SEC1Periph_PKE
  *	@arg RCC_SEC1Periph_DES		 
  *	@arg RCC_SEC1Periph_AES
  *	@arg RCC_SEC1Periph_PWRC
  *	@arg RCC_SEC1Periph_INT
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_Security1PeriphClockSleepCmd(uint32_t RCC_SEC1Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_SEC1Periph(RCC_SEC1Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV4LPSCON |= RCC_SEC1Periph;
	}
	else
	{
		RCC_CLK->CLKSLV4LPSCON &= ~RCC_SEC1Periph;
	}
}

/**
  * @brief  Enables or disables the SEC2 peripheral clock.   
  * @param[in]  SEC2_Periph: specifies the High peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC2Periph_USB:	 USB's clock 
  *	@arg RCC_SEC2Periph_FLASH:  	FLASH's clock
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_Security2PeriphClockCmd(uint32_t RCC_SEC2Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_SEC2Periph(RCC_SEC2Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV5SCON |= RCC_SEC2Periph;
	}
	else
	{
		RCC_CLK->CLKSLV5SCON &= ~RCC_SEC2Periph;
	}
}

/**
  * @brief  Enables or disables the SEC2 peripheral clock.   
  * @param[in]  SEC2_Periph: specifies the High peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC2Periph_USB
  *	@arg RCC_SEC2Periph_FLASH
  * @param[in]  NewState: new state of the specified peripheral clock.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	  
  *	@arg DISABLE		 
  * @retval None
  */
void RCC_Security2PeriphClockSleepCmd(uint32_t RCC_SEC2Periph, FunctionalState NewState)
{
	assert_param(IS_RCC_SEC2Periph(RCC_SEC2Periph));
	if(NewState != DISABLE)
	{
		RCC_CLK->CLKSLV5LPSCON|= RCC_SEC2Periph;
	}
	else
	{
		RCC_CLK->CLKSLV5LPSCON &= ~RCC_SEC2Periph;
	}
}

/**
  * @brief  Enables or disables the BAT security clock.   
  * @param[in]  BAT_Security: specifies the BAT security mudules to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_BATS_SENSOR:  	BAT Sensor's clock
  *	@arg RCC_BATS_STDBYCON:  Standby control's clock 
  *	@arg RCC_BATS_IWDT:	 	IWDT's clock		 
  * @param[in]  NewState: new state of the specified VBAT security mudules.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE		
  * @retval None
  */
void RCC_BATSecurityClockCmd(uint32_t BAT_Security, FunctionalState NewState)
{
	assert_param(IS_RCC_BATFUNC(BAT_Security));
	if(NewState != DISABLE)
	{
		RCC_VBAT->VBATSCON |= BAT_Security;
	}
	else
	{
		RCC_VBAT->VBATSCON &= ~BAT_Security;
	}
}
/**
  * @brief  Reset the AHB peripheral modules.   
  * @param[in]  RCC_HighPerfor: specifies the AHB peripheral to gates its clock.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_AHBPeriph_FLASHCACHE
  *	@arg RCC_AHBPeriph_SPI2
  *	@arg RCC_AHBPeriph_DMA
  * @retval None
  */
void RCC_AHBPeriphSoftReset(uint32_t RCC_AHBPeriph)
{
	assert_param (IS_RCC_AHBPeriph(RCC_AHBPeriph));
	RCC_REST->SLV1SCON &= ~RCC_AHBPeriph;
}

/**
  * @brief  Get the flag of AHB peripheral modules reset.   
  * @param[in]  RCC_HighPerfor: specifies the AHB peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_AHBPeriph_FLASHCACHE
  *	@arg RCC_AHBPeriph_SPI2
  *	@arg RCC_AHBPeriph_DMA			   
  * @retval FlagStatus
  *		@arg SET : Software reset has been completed
  *	 @arg RESET : Software reset has not been completed
  */
FlagStatus RCC_AHBPeriphGetResetFlag(uint32_t RCC_AHBPeriph)
{
	assert_param (IS_RCC_AHBPeriph(RCC_AHBPeriph));
	if (RCC_REST->SLV1SCON & RCC_AHBPeriph)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Reset the APB2 peripheral modules.   
  * @param[in]  RCC_APB2Periph: specifies the APB2 peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB2Periph_7816M3
  *	@arg RCC_APB2Periph_PWM1	  
  *	@arg RCC_APB2Periph_WDT 
  *	@arg RCC_APB2Periph_CRC  
  *	@arg RCC_APB2Periph_ADC	   
  *	@arg RCC_APB2Periph_7816M1
  *	@arg RCC_APB2Periph_I2C
  *	@arg RCC_APB2Periph_TIMER5_6
  *	@arg RCC_APB2Periph_TIMER1_2	   
  *	@arg RCC_APB2Periph_UART1 
  *	@arg RCC_APB2Periph_SPI3 
  *	@arg RCC_APB2Periph_SPI1		
  * @retval None
  */
void RCC_APB2PeriphSoftReset(uint32_t RCC_APB2Periph)
{
	assert_param(IS_RCC_APB2Periph(RCC_APB2Periph));
	RCC_REST->SLV2SCON &= ~RCC_APB2Periph;
}

/**
  * @brief  Get the flag of APB2 peripheral modules reset.   
  * @param[in]  RCC_APB2Periph: specifies the APB2 peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB2Periph_7816M3
  *	@arg RCC_APB2Periph_PWM1	  
  *	@arg RCC_APB2Periph_WDT 
  *	@arg RCC_APB2Periph_CRC  
  *	@arg RCC_APB2Periph_ADC	   
  *	@arg RCC_APB2Periph_7816M1
  *	@arg RCC_APB2Periph_I2C
  *	@arg RCC_APB2Periph_TIMER5_6
  *	@arg RCC_APB2Periph_TIMER1_2	   
  *	@arg RCC_APB2Periph_UART1 
  *	@arg RCC_APB2Periph_SPI3 
  *	@arg RCC_APB2Periph_SPI1		
  * @retval FlagStatus
  *		@arg SET : Software reset has been completed
  *	 @arg RESET : Software reset has not been completed
  */
FlagStatus RCC_APB2PeriphGetResetFlag(uint32_t RCC_APB2Periph)
{
	assert_param(IS_RCC_APB2Periph(RCC_APB2Periph));

	if (RCC_REST->SLV2SCON & RCC_APB2Periph)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Reset the APB3 peripheral.   
  * @param[in]  RCC_APB3Periph: specifies the APB3 peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB3Periph_PWM2 
  *	@arg RCC_APB3Periph_PWM4
  *	@arg RCC_APB3Periph_7816M2	 
  *	@arg RCC_APB3Periph_GPIO
  *	@arg RCC_APB3Periph_TIMER3_4	  
  *	@arg RCC_APB3Periph_UART2
  *	@arg RCC_APB3Periph_UART5
  * @retval None
  */
void RCC_APB3PeriphSoftReset(uint32_t RCC_APB3Periph)
{
	assert_param(IS_RCC_APB3Periph(RCC_APB3Periph));
	RCC_REST->SLV3SCON &= ~RCC_APB3Periph;
}

/**
  * @brief  Get the flag of APB3 peripheral reset.   
  * @param[in]  RCC_APB3Periph: specifies the APB3 peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_APB3Periph_PWM2 
  *	@arg RCC_APB3Periph_PWM4
  *	@arg RCC_APB3Periph_7816M2	 
  *	@arg RCC_APB3Periph_GPIO
  *	@arg RCC_APB3Periph_TIMER3_4	  
  *	@arg RCC_APB3Periph_UART2
  *	@arg RCC_APB3Periph_UART5
  * @retval FlagStatus
  *		@arg SET : Software reset has been completed
  *	 @arg RESET : Software reset has not been completed
  */
FlagStatus RCC_APB3PeriphGetResetFlag(uint32_t RCC_APB3Periph)
{
	assert_param(IS_RCC_APB3Periph(RCC_APB3Periph));

	if (RCC_REST->SLV3SCON & RCC_APB3Periph)
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Reset the SEC1 peripheral.   
  * @param[in]  RCC_SEC1Periph: specifies the SEC1 peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC1Periph_VBSENSOR 
  *	@arg RCC_SEC1Periph_FrequencyDerector 
  *	@arg RCC_SEC1Periph_LightDerector
  *	@arg RCC_SEC1Periph_ExternalVoltageGlitchDetection  
  *	@arg RCC_SEC1Periph_ActiveShield
  *	@arg RCC_SEC1Periph_TRNG
  *	@arg RCC_SEC1Periph_PKE
  *	@arg RCC_SEC1Periph_DES		 
  *	@arg RCC_SEC1Periph_AES
  *	@arg RCC_SEC1Periph_PWRC
  *	@arg RCC_SEC1Periph_INT	   
  * @retval None
  */
void RCC_Security1PeriphSoftReset(uint32_t RCC_SEC1Periph)
{
	assert_param(IS_RCC_SEC1Periph(RCC_SEC1Periph));
	RCC_REST->SLV4SCON &= ~RCC_SEC1Periph;
}

/**
  * @brief  Get the flag of SEC1 peripheral reset.   
  * @param[in]  RCC_SEC1Periph: specifies the SEC1 peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC1Periph_VBSENSOR 
  *	@arg RCC_SEC1Periph_FrequencyDerector 
  *	@arg RCC_SEC1Periph_LightDerector
  *	@arg RCC_SEC1Periph_ExternalVoltageGlitchDetection  
  *	@arg RCC_SEC1Periph_ActiveShield
  *	@arg RCC_SEC1Periph_TRNG
  *	@arg RCC_SEC1Periph_PKE
  *	@arg RCC_SEC1Periph_DES		 
  *	@arg RCC_SEC1Periph_AES
  *	@arg RCC_SEC1Periph_PWRC
  *	@arg RCC_SEC1Periph_INT	   
  * @retval FlagStatus
  *		@arg SET : Software reset has been completed
  *	 @arg RESET : Software reset has not been completed
  */
FlagStatus RCC_Security1PeriphGetResetFlag(uint32_t RCC_SEC1Periph)
{
	assert_param(IS_RCC_SEC1Periph(RCC_SEC1Periph));

	if (RCC_REST->SLV4SCON & RCC_SEC1Periph)
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Reset the SEC2 peripheral.   
  * @param[in]  RCC_SEC2_Periph: specifies the High peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC2Periph_USB 
  *	@arg RCC_SEC2Periph_FLASH	   
  * @retval None
  */
void RCC_Security2PeriphSoftReset(uint32_t RCC_SEC2Periph)
{
	assert_param(IS_RCC_SEC2Periph(RCC_SEC2Periph));
	RCC_REST->SLV5SCON &= ~RCC_SEC2Periph;
}

/**
  * @brief  Get the flag of SEC2 peripheral reset.   
  * @param[in]  RCC_SEC2_Periph: specifies the High peripheral.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_SEC2Periph_USB 
  *	@arg RCC_SEC2Periph_FLASH	   
  * @retval FlagStatus
  *		@arg SET : Software reset has been completed
  *	 @arg RESET : Software reset has not been completed
  */
FlagStatus RCC_Security2PeriphGetResetFlag(uint32_t RCC_SEC2Periph)
{
	assert_param(IS_RCC_SEC2Periph(RCC_SEC2Periph));

	if (RCC_REST->SLV5SCON & RCC_SEC2Periph)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Enables or disables the reset function when alarm occurs.   
  * @param[in] RCC_Alarm: specifies the alarm type.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_ALARM_RSTEN_IWDT 
  *	@arg RCC_ALARM_RSTEN_WWDT
  * @param[in]  NewState: new state of the specified reset function.
  *	This parameter can be any combination of the following values:
  *	@arg ENABLE	   
  *	@arg DISABLE 
  * @retval None
  */
void RCC_WDTResetSystemCmd(uint32_t RCC_WDT, FunctionalState NewState)
{
	if (NewState == ENABLE)
	{
		RCC_REST->ALMCON |= RCC_WDT;
	}
	else
	{
		RCC_REST->ALMCON &= ~RCC_WDT;
	}
}

/**
  * @brief  Reset the BAT.   
  * @param[in] RCC_Alarm: specifies the alarm type.
  *	This parameter can be any combination of the following values:
  *	@arg RCC_BAT_RSTEN_VBSENSOR
  *	@arg RCC_BAT_RSTEN_VBATGPIO
  *	@arg RCC_BAT_RSTEN_STDBY
  *	@arg RCC_BAT_RSTEN_RTC
  *	@arg RCC_BAT_RSTEN_IWDT
  * @retval None
  */
void RCC_BATSoftResetCmd(uint32_t RCC_BAT)
{
	//Write 0 and read back to comfire that reset occurs
	RCC_VBAT->RSTVBATSCON &= ~RCC_BAT;
	while((RCC_VBAT->RSTVBATSCON & RCC_BAT) != 0);
	
	//Write 1 and read back to clear the reset flag
	RCC_VBAT->RSTVBATSCON |= RCC_BAT;
	while((RCC_VBAT->RSTVBATSCON & RCC_BAT) == 0);
}
/**
  * @brief  Get the systerm clock source  
  * @param[in] none
  * @retval uint32_t
  *	@arg RCC_SYS_CLKSRC_PLL_H	   
  *	@arg RCC_SYS_CLKSRC_USBPHY 
  *	@arg RCC_SYS_CLKSRC_HSE 
  *	@arg RCC_SYS_CLKSRC_HSI 
  */
uint32_t RCC_GetSystermClkSource(void)
{
	if(RCC_CLK->CLKSYSSEL&RCC_SYS_CLKSRC_PLL_H)
	{
		return RCC_SYS_CLKSRC_PLL_H;
	}
	else if(RCC_CLK->CLKSYSSEL&RCC_SYS_CLKSRC_USBPHY)
	{
		return RCC_SYS_CLKSRC_USBPHY;
	}
	else if(RCC_CLK->CLKSYSSEL&RCC_SYS_CLKSRC_HSE)
	{
		return RCC_SYS_CLKSRC_HSE;
	}
	return RCC_SYS_CLKSRC_HSI;
}

/**
  * @brief  Get the systerm clock source  
  * @param[in] none
  * @retval uint32_t bit8~bit12 valid
  */
uint32_t RCC_GetSystermClkDiv(void)
{
	volatile uint32_t temp;
	temp = RCC_CLK->CLKSYSSEL;
	return ((temp>>8)&(0x1FU));
	
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
/************************ (C) COPYRIGHT TMC *****END OF FILE****/

