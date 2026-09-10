 /********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: thm36_1.3.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/


/** @addtogroup CMSIS_Device
  * @{
  */

/** @addtogroup thm36 1.3
  * @{
  */
  
#ifndef thm36_13_H_	 
#define thm36_13_H_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */
//#define CHIP_THM3622	1
/** @brief Configuration of the SC300 Processor and Core Peripherals
  *
  */	 
#define __SC300_REV			   0x0000U   /* Core revision r0p0 */
#define __MPU_PRESENT			 1U		/* MPU present */
#define __VTOR_PRESENT			1U		/* VTOR present or not */
#define __NVIC_PRIO_BITS		  3U		/* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig	0U		/* Set to 1 if different SysTick Config is used */
/**
  * @}
  */	 

  
/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */
/**
 * @brief THM36 1.3 Interrupt Number Definition
 */	 
typedef enum IRQn
{
/* ---------------------  SC300 Processor Exceptions Numbers  --------------------- */
  NonMaskableInt_IRQn	= -14,	  /*!< 2 Non Maskable Interrupt									  */
  HardFault_IRQn		 = -13,	  /*!< 4 Hardfault Interrupt										 */
  MemoryManagement_IRQn  = -12,	  /*!< 4 SC300 Memory Management Interrupt						   */
  BusFault_IRQn		  = -11,	  /*!< 5 SC300 Bus Fault Interrupt								   */
  UsageFault_IRQn		= -10,	  /*!< 6 SC300 Usage Fault Interrupt								 */
  SVCall_IRQn			= -5,	   /*!< 11 SC300 SV Call Interrupt									*/
  DebugMonitor_IRQn	  = -4,	   /*!< 12 SC300 Debug Monitor Interrupt							  */
  PendSV_IRQn			= -2,	   /*!< 14 SC300 Pend SV Interrupt									*/
  SysTick_IRQn		   = -1,	   /*!< 15 SC300 System Tick Interrupt								*/
/* ---------------------  ARMSC300 Specific Interrupt Numbers  -------------------- */
  RCC_IRQn			=  0, 
  DMA_IRQn			=  3, 
  AHBMMU_IRQn		 =  4, 
  MPU_IRQn			=  7,
  AES_IRQn			=  11, 
  DES_IRQn			=  12, 
  PKE_IRQn			=  13, 
  EGS_IRQn			=  15,
  LD_IRQn			 =  16,
  TRNG_IRQn		   =  17,
  FD2_IRQn			=  18,
  FD3_IRQn			=  19, 
  VB_SENSOR_IRQn	  =  20, 
  ASH_IRQn			=  21, 
  WWDT_IRQn		   =  22, 
  TIMER1_IRQn		 =  23, 
  TIMER2_IRQn		 =  24, 
  TIMER3_IRQn		 =  25, 
  TIMER4_IRQn		 =  26, 
  TIMER5_IRQn		 =  27, 
  TIMER6_IRQn		 =  28, 
  Flash1_IRQn		 =  33, 
  GPIO_IRQn		   =  36, 
  SPI1_IRQn		   =  37,  
  SPI2_IRQn		   =  38, 
  SPI3_IRQn		   =  39, 
  UART1_IRQn		  =  40,
  UART2_IRQn		  =  41,  
  I2C_IRQn			=  44, 
  ISO7816M1_IRQn	  =  45, 
  ISO7816M2_IRQn	  =  46,  
  ISO7816M3_IRQn	  =  47, 
  PWM1_IRQn		   =  48,
  PWM2_IRQn		   =  49,
  USB_IRQn			=  51,
  UART5_IRQn		  =  52,
  ADC_IRQn			=  53,
  CHARGER_IRQn		=  56,
  PWM3_IRQn		   =  58,
  PWM4_IRQn		   =  59,
  EXTI0_15_IRQn	   =  64,
  EXTI16_31_IRQn	  =  65,
  EXTI32_47_IRQn	  =  66,
  EXTI48_63_IRQn	  =  67,
  EXTI64_79_IRQn	  =  68, 
  EXTI80_IRQn		 =  69,
  EXTI81_IRQn		 =  70,
  EXTI82_IRQn		 =  71,
  EXTI83_IRQn		 =  72,
  EXTI84_IRQn		 =  73,
} IRQn_Type;


/**
  * @}
  */

#include "core_sc300.h"			 /* SC300 processor and core peripherals */
#include <stdint.h>
#include "string.h"
#include "system_thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "thm36_1.3_hal_conf.h"
#include "tmc_systick.h"

	 
 
/** @addtogroup Peripheral_registers_structures
  * @{
  */   


/** @brief Serial Peripheral Interface
  * 
  */
typedef struct
{
    volatile uint32_t RFU[2];
    volatile uint32_t CON2;		
    volatile uint32_t DAT;		
    volatile uint32_t STS;		
    volatile uint32_t MSK;	 
    volatile uint32_t RFU1;
    volatile uint32_t CON3;		
    volatile uint32_t DMACON;	
}SPI_TypeDef;

/** 
  * @brief Power Control
  */


typedef struct
{
	volatile uint32_t MAGCON;
	volatile uint32_t MAGSTS;
	volatile uint32_t MAGMASK;
	volatile uint32_t MAGDAT;
	volatile uint32_t MAGTH;
	volatile uint32_t MAGNOISEPA;
	volatile uint32_t MAGVOPA;
	volatile uint32_t MAGFLTPA;
	volatile uint32_t MAGPEEL_TH;
}MAG_TypeDef;

typedef struct
{
	volatile uint32_t CMDW0;
	volatile uint32_t CMDW1;
	volatile uint32_t CMDW2;
	volatile uint32_t CMDW3;
	volatile uint32_t CON;
	volatile uint32_t CSDLY;
	volatile uint32_t STS;
	volatile uint32_t RFU[1];
	volatile uint32_t INTCON;
	volatile uint32_t INTSTS;
	volatile uint32_t RSTS;
	volatile uint32_t RFU1[1];
	volatile uint32_t XIPCMD;
	volatile uint32_t RFU2[51];
	volatile uint32_t DAT;
	volatile uint32_t RFU3[63];
	volatile uint32_t SCKDLYCON;	
}SPI_Controller_TypeDef;
typedef struct
{
	volatile uint32_t PWRC;				/*!< PWR power control register, Address offset: 0x00 */
	volatile uint32_t RFU1[3];
	volatile uint32_t PWRPSWC;
	volatile uint32_t PWRPSWSTS;		/*!< PWR power status register, Address offset: 0x14 */
	volatile uint32_t PWRPSWSTS1;
	volatile uint32_t PWRPSWWKUPTIME;
}PWR_TypeDef;
typedef struct
{
	volatile uint32_t AMP0CON;										
	volatile uint32_t AMP0PARA;	
	volatile uint32_t AMP1CON;										
	volatile uint32_t AMP1PARA;	
	volatile uint32_t AMP2CON;										
	volatile uint32_t AMP2PARA;		
} ADC_AMP_TypeDef;

typedef struct
{
	volatile uint32_t BWP;
	volatile uint32_t RFU[2];
	volatile uint32_t WKUPMODE;
	volatile uint32_t WKUPTIME;
	volatile uint32_t WKUPSTS;
}PWR_VBAT_TypeDef;

typedef struct
{
	volatile uint32_t DAT;
	volatile uint32_t CON;
	volatile uint32_t CON1;
}CRC_TypeDef;


typedef struct
{
	volatile uint32_t MODE;
	volatile uint32_t ODEN;
	volatile uint32_t OSPEED;
	volatile uint32_t PUPD;
	volatile uint32_t ID;	
	volatile uint32_t OD;
	volatile uint32_t BSRR;
	volatile uint32_t AF;
	volatile uint32_t SMIT;
	volatile uint32_t INTC;
	volatile uint32_t INTMSK;
	volatile uint32_t INTSTS;
	volatile uint32_t RESUMSK;
}GPIO_TypeDef;

typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t ODEN;
	volatile uint32_t OSPEED;
	volatile uint32_t PUPD;
	volatile uint32_t ODR;
	volatile uint32_t ODSR;
	volatile uint32_t ODRR;
	volatile uint32_t SMIT;
	volatile uint32_t TEST;
	volatile uint32_t RFU[7];
	volatile uint32_t INTC;
	volatile uint32_t INTMSK;
	volatile uint32_t INTSTS;
	volatile uint32_t IDR;
}GPIO_VBAT_TypeDef;


typedef struct
{
	volatile uint32_t CON;
	volatile uint32_t STS;
	volatile uint32_t ALMCON;
	volatile uint32_t ALM;
	volatile uint32_t SEC;
	volatile uint32_t MIN;
	volatile uint32_t HOUR;
	volatile uint32_t DAY;
	volatile uint32_t MON;
	volatile uint32_t YEAR;
	volatile uint32_t WEEK;
	volatile uint32_t ALMSEC;
	volatile uint32_t ALMMIN;
	volatile uint32_t ALMHOUR;
	volatile uint32_t ALMDAY;
	volatile uint32_t ALMMON;
	volatile uint32_t ALMYEAR;
	volatile uint32_t ALMWEEK;
	volatile uint32_t ADJ;
	volatile uint32_t REFCLKADJ;
}VBAT_RTC_TypeDef;


typedef struct
{
	volatile uint32_t SLV1SCON;
	volatile uint32_t SLV2SCON;
	volatile uint32_t SLV3SCON;
	volatile uint32_t SLV4SCON;
	volatile uint32_t SLV5SCON;
	volatile uint32_t ALMCON;
	volatile uint32_t STS;
}SCU_RST_TypeDef;

typedef struct
{
	volatile uint32_t INTSTS;	   
	volatile uint32_t INTTCSTS; 
	volatile uint32_t INTTCCLR;
	volatile uint32_t INTERRSTS;
	volatile uint32_t INTERRCLR;
	volatile uint32_t RAWINTTCSTS;
	volatile uint32_t RAWINTERRSTS;
	volatile uint32_t ENBLDCHNS;
	volatile uint32_t RFU0[4];
	volatile uint32_t CONFIG;
	volatile uint32_t RFU[3];
}DMA_TypeDef;


typedef struct
{
	volatile uint32_t SRCADDR;
	volatile uint32_t DESTADDR;
	volatile uint32_t LLI;
	volatile uint32_t CONTROL;
	volatile uint32_t CONFIGURATION;	
}DMA_Channel_TypeDef;


typedef struct
{
	volatile uint32_t CLKCON;
	volatile uint32_t CLKPLL;
	volatile uint32_t CLKPLLFREQ;
	volatile uint32_t CLKHSE;
	volatile uint32_t CLKINTMSK;
	volatile uint32_t RFU[3];
	volatile uint32_t CLKSYSSEL;
	volatile uint32_t RFU1[3];
	volatile uint32_t CLKSLV1SCON;
	volatile uint32_t CLKSLV2SCON;
	volatile uint32_t CLKSLV3SCON;
	volatile uint32_t CLKSLV4SCON;
	volatile uint32_t CLKSLV5SCON;
	volatile uint32_t RFU2[3];
	volatile uint32_t CLKSLV1LPSCON;
	volatile uint32_t CLKSLV2LPSCON;
	volatile uint32_t CLKSLV3LPSCON;
	volatile uint32_t CLKSLV4LPSCON;
	volatile uint32_t CLKSLV5LPSCON;
	volatile uint32_t RFU3[3];
	volatile uint32_t CLKUSBSEL;
	volatile uint32_t CLKEXSPISEL;
	volatile uint32_t CLKADCSEL;
	volatile uint32_t RFU4[2];
	volatile uint32_t CLKGENAL2SEL;
	volatile uint32_t CLKGENAL3SEL;
	volatile uint32_t CLKGENAL4SEL;
	volatile uint32_t CLKGENAL5SEL;
	volatile uint32_t CLKGENAL6SEL;
	volatile uint32_t RFU5[2];
	volatile uint32_t CLKTIM1SEL;
	volatile uint32_t CLKTIM2SEL;
	volatile uint32_t CLKTIM3SEL;
	volatile uint32_t CLKTIM4SEL;
	volatile uint32_t CLKTIM5SEL;
	volatile uint32_t CLKTIM6SEL;
	volatile uint32_t CLKTIM7SEL;
	volatile uint32_t RFU6[4];
	volatile uint32_t CLKSYSTIMSEL;
	volatile uint32_t CLKMONITORSEL;
	volatile uint32_t CLKGENAL8SEL;
	volatile uint32_t CLKGENAL9SEL;
	volatile uint32_t CLKGENAL10SEL;
}SCU_CLK_TypeDef;



typedef struct
{
	volatile uint32_t RFU1;
	volatile uint32_t VBATSEL;
	volatile uint32_t VBATSCON;
	volatile uint32_t LSICON;
	volatile uint32_t LSECON;
	volatile uint32_t RTCSEL;
	volatile uint32_t RFU2[26];
	volatile uint32_t RSTVBATSCON;
	
}SCU_VBAT_TypeDef;

typedef struct
{
	volatile uint32_t ICON;
	volatile uint32_t ICFG;
	volatile uint32_t DCON;
	volatile uint32_t DCFG;
	volatile uint32_t ISADDR;
	volatile uint32_t IEADDR;
	volatile uint32_t DSADDR;
	volatile uint32_t DEADDR;
	volatile uint32_t DVSCON;
	volatile uint32_t DVS;
	volatile uint32_t RFU;
	volatile uint32_t RFU1;
	volatile uint32_t STS;
}Flash_Cache_TypeDef;

typedef struct
{
	volatile uint32_t FLSTS;
	volatile uint32_t FLSTS1;
	volatile uint32_t FLMSK;
	volatile uint32_t RFU;
	volatile uint32_t FLMSK1;
	volatile uint32_t RFU1[3];
	volatile uint32_t FLSDP1;
	volatile uint32_t FLSDP2;
	volatile uint32_t RFU2[2];
	volatile uint32_t FLCON;
	volatile uint32_t RFU3[3];
	volatile uint32_t FLPCON;
	volatile uint32_t FLPCON1;
	volatile uint32_t FLEPMODE;
	
	
}Flash_TypeDef;

typedef struct
{
	volatile uint32_t SENTIMERCON;
	volatile uint32_t SENTIMESTS;
	volatile uint32_t SENTIMEREVAL;
	volatile uint32_t RFU1;
	volatile uint32_t VBATVDCON;
	volatile uint32_t VBATVDSTS;
	volatile uint32_t RFU2;
	volatile uint32_t RFU3;
	volatile uint32_t VBATTDCON;
	volatile uint32_t VBATTDSTS;
	volatile uint32_t RFU4;
	volatile uint32_t RFU5;
	volatile uint32_t VBATSDCON;
	volatile uint32_t VBATSDLV;
	volatile uint32_t VBATSDSTS;
	volatile uint32_t VBATSDTYPE;
	volatile uint32_t VBATSDROUND;
	volatile uint32_t VBATSDSEED;
	volatile uint32_t VBATSDSTATOL;
	volatile uint32_t VBATSDDYNTOL;
	volatile uint32_t RFU6[44];
	volatile uint32_t VBATVDMSTS;
	volatile uint32_t VBATVDINTMSK;
	volatile uint32_t RFU7[2];
	volatile uint32_t VBATTDMSTS;
	volatile uint32_t VBATTDINTMSK;
	volatile uint32_t RFU8[2];
	volatile uint32_t VBATSDMSTS;
	volatile uint32_t VBATSDINTMSK;
}VBAT_SENSOR_TypeDef;

typedef struct
{
	volatile uint32_t CON;
	volatile uint32_t PRE;
	volatile uint32_t VALUL;
	volatile uint32_t VALUH;
	volatile uint32_t STS;
}IWDT_CFG_TypeDef;

typedef struct
{
	volatile uint32_t LOAD;
	volatile uint32_t VALUE;
	volatile uint32_t CONTROL;
	volatile uint32_t INTCLR;
	volatile uint32_t RIS;
	volatile uint32_t MIS;
	volatile uint32_t RFU[0x2FA];
	volatile uint32_t LOCK;
}WWDT_TypeDef;

typedef struct
{
	volatile uint32_t REEN0;
	volatile uint32_t REEN1;
	volatile uint32_t REEN2;
	volatile uint32_t REEN3;
	volatile uint32_t FEEN0;
	volatile uint32_t FEEN1;
	volatile uint32_t FEEN2;
	volatile uint32_t FEEN3;
	volatile uint32_t HLEN0;
	volatile uint32_t HLEN1;
	volatile uint32_t HLEN2;
	volatile uint32_t HLEN3;
	volatile uint32_t LLEN0;
	volatile uint32_t LLEN1;
	volatile uint32_t LLEN2;
	volatile uint32_t LLEN3;
	volatile uint32_t SWIER0;
	volatile uint32_t SWIER1;
	volatile uint32_t SWIER2;
	volatile uint32_t SWIER3;
	volatile uint32_t PR0;
	volatile uint32_t PR1;
	volatile uint32_t PR2;
	volatile uint32_t PR3;
	volatile uint32_t MCUINTCON0;
	volatile uint32_t MCUINTCON1;
	volatile uint32_t MCUINTCON2;

	volatile uint32_t MCUEVCON0;
	volatile uint32_t MCUEVCON1;
	volatile uint32_t MCUEVCON2;

}EXTI_TypeDef;

typedef struct
{
	volatile uint32_t TxDATA;
	volatile uint32_t RxDATA;
	volatile uint32_t CON1;
	volatile uint32_t CON2;
	volatile uint32_t STS;
	volatile uint32_t CON3;
	volatile uint32_t RFU[2];
	volatile uint32_t MSK;
	volatile uint32_t DMACON;
}UART_TypeDef;

typedef struct
{
  volatile uint32_t CR1;	/*!< UART Control register 1,				 Address offset: 0x00 */
  volatile uint32_t CR2;	/*!< UART Control register 2,				 Address offset: 0x04 */
  volatile uint32_t CR3;	/*!< UART Control register 3,				 Address offset: 0x08 */
  volatile uint32_t BRR;	/*!< UART Baud rate register,				 Address offset: 0x0C */
  volatile uint32_t RFU[2];
  volatile uint32_t RQR;	/*!< UART Request register,				   Address offset: 0x18 */											
  volatile uint32_t ISR;	/*!< UART Interrupt and status register,	  Address offset: 0x1C */
  volatile uint32_t ICR;	/*!< UART Interrupt flag Clear register,	  Address offset: 0x20 */
  volatile uint32_t RDR;	/*!< UART Receive Data register,			  Address offset: 0x24 */										   
  volatile uint32_t TDR;	/*!< UART Transmit Data register,			 Address offset: 0x28 */
  volatile uint32_t PRESC;  /*!< UART clock Prescaler register,		   Address offset: 0x2C */
} UART5_TypeDef;

typedef struct
{
	volatile uint32_t CSEL;
	volatile uint32_t HSTS;
	volatile uint32_t HMSK;
	volatile uint32_t TOR;
	volatile uint32_t TOC;
	volatile uint32_t TBUF;
	volatile uint32_t RBUF;
	volatile uint32_t FFCON;
	volatile uint32_t MSTS;
	volatile uint32_t USTS;
	volatile uint32_t MMSK;
	volatile uint32_t UMSK;
	volatile uint32_t PDR;
	volatile uint32_t GTR;
	volatile uint32_t UCON1;
	volatile uint32_t UCON2;
	volatile uint32_t CKCON;
	volatile uint32_t PWCON;
	volatile uint32_t RFU1;
	volatile uint32_t RFU2;
	volatile uint32_t DMACON;
	volatile uint32_t RFU3[14];
	volatile uint32_t SXCON;
	volatile uint32_t RFU4;
	volatile uint32_t SYNCBUF;
}M7816_TypeDef;

typedef struct
{   
	volatile uint32_t EMVCON;   
}EMV7816M2_TypeDef;

#define EMV7816VCCTRIM			 *(volatile unsigned long *)(EMV7816M2_BASE_ADDR + 0x64)
 
typedef struct
{
	volatile uint32_t CON;			//*((volatile unsigned long *)(I2C_BASE_ADDR+0x00))
	volatile uint32_t STS;		//*((volatile unsigned long *)(I2C_BASE_ADDR+0x04))
	volatile uint32_t DATA;		//*((volatile unsigned long *)(I2C_BASE_ADDR+0x08))
	volatile uint32_t ADDR0;		//*((volatile unsigned long *)(I2C_BASE_ADDR+0x0C))
	volatile uint32_t RFU[4];
	volatile uint32_t CK;	//*((volatile unsigned long *)(I2C_BASE_ADDR+0x10))
	volatile uint32_t MSK;	//*((volatile unsigned long *)(I2C_BASE_ADDR+0x14))
 }I2C_TypeDef;

typedef struct
{
	volatile uint32_t CON;
	volatile uint32_t CFG;
	volatile uint32_t STS;
	volatile uint32_t MSK;
	volatile uint32_t CLK;
	volatile uint32_t PSC;
	volatile uint32_t DTY;
	volatile uint32_t PER;
}PWM_TypeDef; 

typedef struct
{
	volatile uint32_t MODECON1;
	volatile uint32_t MODECON2;
	volatile uint32_t STS;
	volatile uint32_t MSK;
	volatile uint32_t AP;
	volatile uint32_t R0STRAT;
	volatile uint32_t R0End;
	volatile uint32_t R1STRAT;
	volatile uint32_t R1End;
	volatile uint32_t R2STRAT;
	volatile uint32_t R2End;
	volatile uint32_t R3STRAT;
	volatile uint32_t R3End;
}MPU_TypeDef; 


 /* ADC STRUCT */
typedef struct
{
	volatile uint32_t ADCPCON;										
	volatile uint32_t ADCCON;										
	volatile uint32_t ADCSTS;															
	volatile uint32_t ADCMSK;										
	volatile uint32_t ADCDAT;										
	volatile uint32_t ADCPSC;										
	volatile uint32_t ADCBAUD;									
	volatile uint32_t RFU0;  										/* BASE + 0x1c */
	volatile uint32_t ADCTST;										/* BASE + 0x20 */
	volatile uint32_t ADCDCMP;								
	volatile uint32_t ADCIOSEL;												
	volatile uint32_t ADCDMACFG;						
	volatile uint32_t RFU1[1];
	volatile uint32_t ADCCVTIO;						
	volatile uint32_t RFU2[46];
	volatile uint32_t ADC7938;									  /* BASE + 0xF0 */
 	volatile uint32_t RFU3[67];
	volatile uint32_t AMP0CON;										/* BASE + 0x200 */
	volatile uint32_t AMP0PARA;										/* BASE + 0x204 */
	volatile uint32_t AMP1CON;										/* BASE + 0x208 */
	volatile uint32_t AMP1PARA;										/* BASE + 0x20C */
	volatile uint32_t AMP2CON;										/* BASE + 0x210 */
	volatile uint32_t AMP2PARA;										/* BASE + 0x214 */
	volatile uint32_t RFU4[58];											 
 	volatile uint32_t MAGCON_0;										/* BASE + 0x300 */
	volatile uint32_t MAGSTS_0;										/* BASE + 0x304 */
	volatile uint32_t MAGMSK_0;										/* BASE + 0x308 */
	volatile uint32_t MAGDAT_0;										/* BASE + 0x30C */
	volatile uint32_t MAGTH_0;										/* BASE + 0x310 */
	volatile uint32_t MAGNOISEPA_0;									/* BASE + 0x314 */
	volatile uint32_t MAGOVPA_0;									/* BASE + 0x318 */
	volatile uint32_t MAGFLTPA_0;									/* BASE + 0x31C */
	volatile uint32_t MAGPEELTH_0;									/* BASE + 0x320 */
	volatile uint32_t RFU5[7];										 
 	volatile uint32_t MAGCON_1;										/* BASE + 0x340 */
	volatile uint32_t MAGSTS_1;										/* BASE + 0x344 */
	volatile uint32_t MAGMSK_1;										/* BASE + 0x348 */
	volatile uint32_t MAGDAT_1;										/* BASE + 0x34C */
	volatile uint32_t MAGTH_1;										/* BASE + 0x350 */
	volatile uint32_t MAGNOISEPA_1;									/* BASE + 0x354 */
	volatile uint32_t MAGOVPA_1;										/* BASE + 0x358 */
	volatile uint32_t MAGFLTPA_1;									/* BASE + 0x35C */
	volatile uint32_t MAGPEELTH_1;									/* BASE + 0x360 */
	volatile uint32_t RFU6[7];										 
	volatile uint32_t MAGCON_2;										/* BASE + 0x380 */
	volatile uint32_t MAGSTS_2;										/* BASE + 0x384 */
	volatile uint32_t MAGMSK_2;										/* BASE + 0x388 */
	volatile uint32_t MAGDAT_2;										/* BASE + 0x38C */
	volatile uint32_t MAGTH_2;										/* BASE + 0x390 */
	volatile uint32_t MAGNOISEPA_2;									/* BASE + 0x394 */
	volatile uint32_t MAGOVPA_2;									/* BASE + 0x398 */
	volatile uint32_t MAGFLTPA_2;									/* BASE + 0x39C */
	volatile uint32_t MAGPEELTH_2;									/* BASE + 0x3A0 */	
} ADC_TypeDef;

typedef struct
{
	volatile uint32_t LOAD;
	volatile uint32_t VALUE;
	volatile uint32_t CONTROL;
	volatile uint32_t CLR;
	volatile uint32_t RIX;
	volatile uint32_t MIX;
	volatile uint32_t BGLOAD;
}TIM_TypeDef;

typedef struct
{
	volatile uint32_t CON;
	volatile uint32_t STS;
	volatile uint32_t INTMSK;
	volatile uint32_t LFN;
	volatile uint32_t HFN;
} EFD2_TypeDef;

typedef struct
{
	volatile uint32_t CON;
	volatile uint32_t STS;
	volatile uint32_t INTMSK;
	volatile uint32_t LFN;
	volatile uint32_t HFN;
	volatile uint32_t PER;
} EFD3_TypeDef;

typedef struct
{
	volatile uint32_t BKPMEM1;
	volatile uint32_t BKPMEM2;
	volatile uint32_t BKPMEM3;
	volatile uint32_t BKPMEM4;
	volatile uint32_t BKPMEM5;
	volatile uint32_t BKPMEM6;
	volatile uint32_t BKPMEM7;
	volatile uint32_t BKPMEM8;
	volatile uint32_t BKPMEM9;
	volatile uint32_t BKPMEM10;
	volatile uint32_t BKPMEM11;
	volatile uint32_t BKPMEM12;
	volatile uint32_t BKPMEM13;
	volatile uint32_t BKPMEM14;
	volatile uint32_t BKPMEM15;
	volatile uint32_t BKPMEM16;
	volatile uint32_t BKPMEM17;
	volatile uint32_t BKPMEM18;
	volatile uint32_t BKPMEM19;
	volatile uint32_t BKPMEM20;
	volatile uint32_t BKPMEM21;
	volatile uint32_t BKPMEM22;
	volatile uint32_t BKPMEM23;
	volatile uint32_t BKPMEM24;
	volatile uint32_t BKPMEM25;
	volatile uint32_t BKPMEM26;
	volatile uint32_t BKPMEM27;
	volatile uint32_t BKPMEM28;
	volatile uint32_t BKPMEM29;
	volatile uint32_t BKPMEM30;
	volatile uint32_t BKPMEM31;
	volatile uint32_t BKPMEM32;
}VBAT_RAM_TypeDef;

typedef struct
{
	volatile uint32_t RFU[3];
	volatile uint32_t STS; 
	volatile uint32_t RFU1[2];
	volatile uint32_t DIV; 
	volatile uint32_t TDAT; 
	volatile uint32_t RDAT; 
	volatile uint32_t INTMSK; 
}ASH_TypeDef;
//------------------------------------------------------------------------
//	USB
#define USBSTS0			*(volatile unsigned long *)(USB_BASE_ADDR + 0x000)	//	USB Device status register
#define USBMISTS		*(volatile unsigned long *)(USB_BASE_ADDR + 0x004)	//	USB major Interrupt status register
#define USBDCON			*(volatile unsigned long *)(USB_BASE_ADDR + 0x008)	//	USB Device control register
#define USBDCC1			*(volatile unsigned long *)(USB_BASE_ADDR + 0x00C)	//	USB device control command register1
#define USBDCC2			*(volatile unsigned long *)(USB_BASE_ADDR + 0x010)	//	USB device control command register2
#define USBBFCON		*(volatile unsigned long *)(USB_BASE_ADDR + 0x014)	//	USB buffer control register
#define USBSTS1			*(volatile unsigned long *)(USB_BASE_ADDR + 0x018)	//	USB resume interrupt status register
#define USBPCON			*(volatile unsigned long *)(USB_BASE_ADDR + 0x01C)	//	USB power supply
#define USBCESTS		*(volatile unsigned long *)(USB_BASE_ADDR + 0x020)	//	USB Control endpoint status register 
#define	USBCESTS_INT 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x020)	//	USB Control endpoint status register 
#define USBCESTS_B1	 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x021)
#define USBCESTS_B2	 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x022)
#define USBCESTS_B3	 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x023)
#define USBCECON		*(volatile unsigned long *)(USB_BASE_ADDR + 0x024)	//	USB Control endpoint control register
#define USBCECON_B0		*(volatile unsigned char *)(USB_BASE_ADDR + 0x024)
#define USBCECON_B1		*(volatile unsigned char *)(USB_BASE_ADDR + 0x025)
#define USBCECON_B2		*(volatile unsigned char *)(USB_BASE_ADDR + 0x026)
#define USBCECON_B3		*(volatile unsigned char *)(USB_BASE_ADDR + 0x027)
#define USBCEDAT		*(volatile unsigned long *)(USB_BASE_ADDR + 0x028)	//	USB Control endpoint data register   
#define USBCEDAT_B		*(volatile unsigned char *)(USB_BASE_ADDR + 0x028)
#define USBCEBF  		*(volatile unsigned long *)(USB_BASE_ADDR + 0x02C)	//	USB control endpoint buffer address  
#define USBEP1STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x030)	//	USB endpoint 1 status register	   
#define USBEP1STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x030)	//	USB endpoint 1 status register	   
#define USBEP1STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x031)	//	USB endpoint 1 status register	   
#define USBEP1STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x032)	//	USB endpoint 1 status register	   
#define USBEP1STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x033)	//	USB endpoint 1 status register	   
#define USBEP1CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x034)	//	USB endpoint 1 control register	  
#define USBEP1CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x034)
#define USBEP1CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x035)
#define USBEP1CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x036)
#define USBEP1CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x037)
#define USBEP1DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x038)	//	USB endpoint 1 data register		 
#define USBEP1DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x038)
#define USBEP1BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x03C)	//	USB endpoint 1 buffer address		
#define USBEP2STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x040)	//	USB endpoint 2 status register	   
#define USBEP2STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x040)	//	USB endpoint 1 status register	   
#define USBEP2STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x041)	//	USB endpoint 1 status register	   
#define USBEP2STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x042)	//	USB endpoint 1 status register	   
#define USBEP2STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x043)	//	USB endpoint 1 status register 
#define USBEP2CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x044)	//	USB endpoint 2 control register	  
#define USBEP2CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x044)
#define USBEP2CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x045)
#define USBEP2CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x046)
#define USBEP2CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x047)
#define USBEP2DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x048)	//	USB endpoint 2 data register		 
#define USBEP2DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x048)
#define USBEP2BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x04C)	//	USB endpoint 2 buffer address		
#define USBEP3STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x050)	//	USB endpoint 3 status register	   
#define USBEP3STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x050)	//	USB endpoint 1 status register	   
#define USBEP3STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x051)	//	USB endpoint 1 status register	   
#define USBEP3STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x052)	//	USB endpoint 1 status register	   
#define USBEP3STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x053)	//	USB endpoint 1 status register 
#define USBEP3CON  		*(volatile unsigned long *)(USB_BASE_ADDR + 0x054)	//	USB endpoint 3 control register	  
#define USBEP3CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x054)
#define USBEP3CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x055)
#define USBEP3CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x056)
#define USBEP3CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x057)
#define USBEP3DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x058)	//	USB endpoint 3 data register		 
#define USBEP3DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x058)
#define USBEP3BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x05C)	//	USB endpoint 3 buffer address		
#define USBEP4STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x060)	//	USB endpoint 4 status register	   
#define USBEP4STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x060)	//	USB endpoint 1 status register	   
#define USBEP4STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x061)	//	USB endpoint 1 status register	   
#define USBEP4STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x062)	//	USB endpoint 1 status register	   
#define USBEP4STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x063)	//	USB endpoint 1 status register 
#define USBEP4CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x064)	//	USB endpoint 4 control register	  
#define USBEP4CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x064)
#define USBEP4CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x065)
#define USBEP4CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x066)
#define USBEP4CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x067)
#define USBEP4DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x068)	//	USB endpoint 4 data register		 
#define USBEP4DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x068)
#define USBEP4BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x06C)	//	USB endpoint 4 buffer address		
#define USBEP5STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x070)	//	USB endpoint 4 status register	   
#define USBEP5STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x070)	//	USB endpoint 1 status register	   
#define USBEP5STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x071)	//	USB endpoint 1 status register	   
#define USBEP5STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x072)	//	USB endpoint 1 status register	   
#define USBEP5STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x073)	//	USB endpoint 1 status register 
#define USBEP5CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x074)	//	USB endpoint 4 control register	  
#define USBEP5CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x074)
#define USBEP5CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x075)
#define USBEP5CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x076)
#define USBEP5CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x077)
#define USBEP5DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x078)	//	USB endpoint 4 data register		 
#define USBEP5DAT_B		*(volatile unsigned char *)(USB_BASE_ADDR + 0x078)
#define USBEP5BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x07C)	//	USB endpoint 4 buffer address
#define USBEP6STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x080)	//	USB endpoint 4 status register	   
#define USBEP6STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x080)	//	USB endpoint 1 status register	   
#define USBEP6STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x081)	//	USB endpoint 1 status register	   
#define USBEP6STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x082)	//	USB endpoint 1 status register	   
#define USBEP6STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x083)	//	USB endpoint 1 status register 
#define USBEP6CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x084)	//	USB endpoint 4 control register	  
#define USBEP6CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x084)
#define USBEP6CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x085)
#define USBEP6CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x086)
#define USBEP6CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x087)
#define USBEP6DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x088)	//	USB endpoint 4 data register		 
#define USBEP6DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x088)
#define USBEP6BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x08C)	//	USB endpoint 4 buffer address
#define USBEP7STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x090)	//	USB endpoint 4 status register	   
#define USBEP7STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x090)	//	USB endpoint 1 status register	   
#define USBEP7STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x091)	//	USB endpoint 1 status register	   
#define USBEP7STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x092)	//	USB endpoint 1 status register	   
#define USBEP7STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x093)	//	USB endpoint 1 status register 
#define USBEP7CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x094)	//	USB endpoint 4 control register	  
#define USBEP7CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x094)
#define USBEP7CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x095)
#define USBEP7CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x096)
#define USBEP7CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x097)
#define USBEP7DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x098)	//	USB endpoint 4 data register		 
#define USBEP7DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x098)
#define USBEP7BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x09C)	//	USB endpoint 4 buffer address
#define USBEP8STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0A0)	//	USB endpoint 4 status register	   
#define USBEP8STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A0)	//	USB endpoint 1 status register	   
#define USBEP8STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A1)	//	USB endpoint 1 status register	   
#define USBEP8STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A2)	//	USB endpoint 1 status register	   
#define USBEP8STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A3)	//	USB endpoint 1 status register 
#define USBEP8CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0A4)	//	USB endpoint 4 control register	  
#define USBEP8CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A4)
#define USBEP8CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A5)
#define USBEP8CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A6)
#define USBEP8CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A7)
#define USBEP8DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0A8)	//	USB endpoint 4 data register		 
#define USBEP8DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0A8)	   
#define USBEP8BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x0AC)	//	USB endpoint 4 buffer address
#define USBEP9STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0B0)	//	USB endpoint 4 status register	   
#define USBEP9STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B0)	//	USB endpoint 1 status register	   
#define USBEP9STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B1)	//	USB endpoint 1 status register	   
#define USBEP9STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B2)	//	USB endpoint 1 status register	   
#define USBEP9STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B3)	//	USB endpoint 1 status register 
#define USBEP9CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0B4)	//	USB endpoint 4 control register	  
#define USBEP9CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B4)
#define USBEP9CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B5)
#define USBEP9CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B6)
#define USBEP9CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B7)
#define USBEP9DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0B8)	//	USB endpoint 4 data register		 
#define USBEP9DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0B8)	   
#define USBEP9BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x0BC)	//	USB endpoint 4 buffer address
#define USBEP10STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0C0)	//	USB endpoint 4 status register	   
#define USBEP10STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C0)	//	USB endpoint 1 status register	   
#define USBEP10STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C1)	//	USB endpoint 1 status register	   
#define USBEP10STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C2)	//	USB endpoint 1 status register	   
#define USBEP10STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C3)	//	USB endpoint 1 status register	   
#define USBEP10CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0C4)	//	USB endpoint 4 control register	  
#define USBEP10CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C4)										   
#define USBEP10CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C5)										   
#define USBEP10CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C6)										   
#define USBEP10CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C7)										   
#define USBEP10DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0C8)	//	USB endpoint 4 data register		 
#define USBEP10DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0C8)										   
#define USBEP10BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x0CC)	//	USB endpoint 4 buffer address		
#define USBEP11STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0D0)	//	USB endpoint 4 status register	   
#define USBEP11STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D0)	//	USB endpoint 1 status register	   
#define USBEP11STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D1)	//	USB endpoint 1 status register	   
#define USBEP11STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D2)	//	USB endpoint 1 status register	   
#define USBEP11STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D3)	//	USB endpoint 1 status register	   
#define USBEP11CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0D4)	//	USB endpoint 4 control register	  
#define USBEP11CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D4)										   
#define USBEP11CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D5)										   
#define USBEP11CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D6)										   
#define USBEP11CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D7)										   
#define USBEP11DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0D8)	//	USB endpoint 4 data register		 
#define USBEP11DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0D8)										   
#define USBEP11BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x0DC)	//	USB endpoint 4 buffer address		
#define USBEP12STS   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0E0)	//	USB endpoint 4 status register	   
#define USBEP12STS_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E0)	//	USB endpoint 1 status register	   
#define USBEP12STS_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E1)	//	USB endpoint 1 status register	   
#define USBEP12STS_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E2)	//	USB endpoint 1 status register	   
#define USBEP12STS_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E3)	//	USB endpoint 1 status register	   
#define USBEP12CON   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0E4)	//	USB endpoint 4 control register	  
#define USBEP12CON_B0	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E4)										   
#define USBEP12CON_B1	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E5)										   
#define USBEP12CON_B2	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E6)										   
#define USBEP12CON_B3	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E7)										   
#define USBEP12DAT   	*(volatile unsigned long *)(USB_BASE_ADDR + 0x0E8)	//	USB endpoint 4 data register		 
#define USBEP12DAT_B 	*(volatile unsigned char *)(USB_BASE_ADDR + 0x0E8)										   
#define USBEP12BF 		*(volatile unsigned long *)(USB_BASE_ADDR + 0x0EC)	//	USB endpoint 4 buffer address		
#define USBDATCON		*(volatile unsigned long *)(USB_BASE_ADDR + 0x0F0)
#define USBPHYCON		*(volatile unsigned long *)(USB_BASE_ADDR + 0x100)	//	USB PHY select control register 
#define USBPHYDAT		*(volatile unsigned long *)(USB_BASE_ADDR + 0x104)
#define USBPHYCKCON1		*(volatile unsigned long *)(USB_BASE_ADDR + 0x108)
#define USBPHYCKCON2		*(volatile unsigned long *)(USB_BASE_ADDR + 0x10C)
#define USBPHYSTS		*(volatile unsigned long *)(USB_BASE_ADDR + 0x110)


#define USBEPBF(n)					*(volatile unsigned long *)(USB_BASE_ADDR + 0x02C + n * 16)
#define USBEPSTSB1(n)				*(volatile unsigned char *)(USB_BASE_ADDR + 0x021 + n * 16)
#define USBEPSTSB2(n)				*(volatile unsigned char *)(USB_BASE_ADDR + 0x022 + n * 16) 
#define USBEPSTS(n)					*(volatile unsigned long *)(USB_BASE_ADDR + 0x020 + n * 16)	//	USB endpoint n status register	   
#define USBEPCON(n)					*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16)	//	USB endpoint n control register	  
#define USBEPDAT(n)					*(volatile unsigned long *)(USB_BASE_ADDR + 0x028 + n * 16)	//	USB endpoint n data register		 
#define USBEPDAT_B(n) 				*(volatile unsigned char *)(USB_BASE_ADDR + 0x028 + n * 16)

#define	USBMAXPKTSIZE(n)			(8 << ((USBEPCON(n) >> 16) & 0x07))

#define	ISOTRFRMAXPKTSIZE(n)		(n = (n == 1024 ? 1023 : n))
#define	SetEndpointIN(n)			*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) |= Bit20_En;
#define	SetEndpointOUT(n)			*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) &= Bit20_Dis;

#define	SetTransferBULK(n)			*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) &= Bit22_Dis;	\
									*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) |= Bit21_En;	\

#define	SetTransferINTERRUPT(n)		*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) |= Bit22_En;	\
									*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) &= Bit21_Dis;	\

#define	SetTransferISOCHRONOUS(n)	*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) |= Bit22_En;	\
									*(volatile unsigned long *)(USB_BASE_ADDR + 0x024 + n * 16) |= Bit21_En;	\

#define	EPDPKTCLR(n)				USBEPSTS(n) |= Bit0_En 
#define	EPBUF0CLR(n)				USBEPCON(n) |= Bit9_En
#define EPPKTRDY(n)					USBEPSTS(n) |= Bit19_En;
#define EPPKTLENH(n)				USBEPSTSB2(n)
#define EPPKTLENL(n)				USBEPSTSB1(n)
#define EPBUFINC(n,mps)				USBEPBF(n) += mps;


#define	EP0STSCXS 					(USBCESTS & Bit0_En)
#define	EP0STSCXDPKT 				(USBCESTS & Bit1_En)	
#define	EP0STSCXSTS	 				(USBCESTS & Bit2_En)

#define	EP0CXSCLR	 				USBCESTS_INT = Bit0_En		
#define	EP0CXDPKTCLR 				USBCESTS_INT = Bit1_En		
#define	EP0CXSTSCLR	 				USBCESTS_INT = Bit2_En

#define EP0SENDNAK		USBCECON &= Bit9_Dis;\
						USBCECON &= Bit8_Dis;

#define EP0SENDACK		USBCECON &= Bit9_Dis;\
						USBCECON |= Bit8_En;

#define EP0SENDSTALL	USBCECON |= Bit9_En;\
						USBCECON &= Bit8_Dis;

#define	CPCXSCLR	USBCESTS_INT = Bit0_En;		//	Write 1b to clear bit 0,bit 2 and 1 are reserved
#define	CPCXDPKTCLR	USBCESTS_INT = Bit1_En;		//	Write 1b to clear bit 1,bit 2 and 0 are reserved
#define	CPCXSTSCLR	USBCESTS_INT = Bit2_En;		//	Write 1b to clear bit 2,bit 1 and 0 are reserved



/**
  * @}
  */
  
  
/** @addtogroup Peripheral_memory_map
  * @{
  */
//------------------------------------------------------------------------
#define PWR_MANAGE_BASE_ADDR			0x40008000
#define RCC_RSTS_BASE_ADDR	  			0x40000000
#define RCC_CLK_BASE_ADDR				0x40000C00
#define	RCC_CLK_SPECIAL_ADDR			0x40001e04
#define	RCC_CLK_SPECIAL_ADDR			0x40001e04
#define RCC_VBAT_BASE_ADDR	  			0x40001800
#define SCU_CLK_SEC_ADDR				0x40003000
#define EXTI_BASE_ADDR				  0x40004000
#define ASH_BASE_ADDR					0x4004C000
#define VBAT_SENSOR_BASE_ADDR 			0x40070000 
#define IWDT_BASE_ADDR				  0x40074000
#define BAT_RAM_BASE_ADDR 				0x4007C000
#define EFD2_BASE_ADDR					0x4005C000
#define EFD3_BASE_ADDR					0x4005C400
#define MPU_BASE_ADDR					0x40088000
#define WWDT_BASE_ADDR			   	0X40828000
#define Flash_ACC_BASE_ADDR	  			(0x40080000+0x00001C00)
#define Flash_BASE_ADDR					0x40084000
#define ADC_AMP_BASE_ADDR				0x40820200
//------------------------------APB2-------------------------------------
#define SPI1_BASE_ADDR					0x40800000
#define SPI3_BASE_ADDR					0x40804000
#define TIM1_BASE_ADDR					0x40810000
#define TIM2_BASE_ADDR					0x40810020
#define TIM5_BASE_ADDR					0x40814000 
#define TIM6_BASE_ADDR					0x40814020 
#define I2C_BASE_ADDR					0x40818000

#define MAG_BASE_ADDR				   0x40820000  
#define MAG1_BASE_ADDR				  0x40820300
#define MAG2_BASE_ADDR				  0x40820340
#define MAG3_BASE_ADDR				  0x40820380
#define CRC_BASE_ADDR					0x40824000
#define PWM1_BASE_ADDR					0x4082C000
#define ISO7816M1_BASE_ADDR			 0x4081C000
#define ISO7816M3_BASE_ADDR			 0x40830000
#define PWM3_BASE_ADDR				  0x40838000
#define UART1_BASE_ADDR				 0x4080C000
//------------------------------APB3-------------------------------------
#define TIM3_BASE_ADDR					0x4100C000 
#define TIM4_BASE_ADDR					0x4100C020 
#define GPIO_GROUP1_BASE_ADDR			0x41018000
#define GPIO_GROUP2_BASE_ADDR			0x41018400
#define GPIO_GROUP3_BASE_ADDR			0x41018800
#define GPIO_GROUP4_BASE_ADDR			0x41018c00
#define GPIO_GROUP5_BASE_ADDR			0x41019000
#define GPIO_VBAT_BASE_ADDR				0x41019800
#define ISO7816M2_BASE_ADDR			 0x41024000
#define EMV7816M2_BASE_ADDR			 0x41024064
#define PWM2_BASE_ADDR					0x41028000
#define PWM4_BASE_ADDR					0x4102C000

#define VBAT_RTC_BASE_ADDR				0x40078000
#define UART2_BASE_ADDR				 0x41008000
#define UART5_BASE_ADDR 				0x41004000
//------------------------------AHB1-------------------------------------
#define USB_BASE_ADDR 					0x41804000

#define SPI_CONTROLLER_ADDR				0x4180C000
#define DMA_BASE_ADDR				   0x41800000
#define DMA_CHANNEL0_BASE_ADDR			0x41800100
#define DMA_CHANNEL1_BASE_ADDR			0x41800120
#define DMA_CHANNEL2_BASE_ADDR			0x41800140
#define DMA_CHANNEL3_BASE_ADDR			0x41800160
#define DMA_CHANNEL4_BASE_ADDR			0x41800180
#define DMA_CHANNEL5_BASE_ADDR			0x418001A0
#define DMA_CHANNEL6_BASE_ADDR			0x418001C0
#define DMA_CHANNEL7_BASE_ADDR			0x418001E0


/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */  
#define SPI1			((SPI_TypeDef *)  SPI1_BASE_ADDR)
#define SPI2			((SPI_Controller_TypeDef *)  SPI_CONTROLLER_ADDR)
#define SPI3			((SPI_TypeDef *)  SPI3_BASE_ADDR)
#define GPIOA			((GPIO_TypeDef *) GPIO_GROUP1_BASE_ADDR)
#define GPIOB			((GPIO_TypeDef *) GPIO_GROUP2_BASE_ADDR)
#define GPIOC			((GPIO_TypeDef *) GPIO_GROUP3_BASE_ADDR)
#define GPIOD			((GPIO_TypeDef *) GPIO_GROUP4_BASE_ADDR)
#define GPIOE			((GPIO_TypeDef *) GPIO_GROUP5_BASE_ADDR)
#define VBAT_GPIO	   ((GPIO_VBAT_TypeDef*) GPIO_VBAT_BASE_ADDR)
#define VBAT_SENSOR 	((VBAT_SENSOR_TypeDef*)VBAT_SENSOR_BASE_ADDR)
#define RCC_REST	   	((SCU_RST_TypeDef *) RCC_RSTS_BASE_ADDR)
#define RCC_CLK		   	((SCU_CLK_TypeDef *) RCC_CLK_BASE_ADDR)
#define RCC_VBAT		((SCU_VBAT_TypeDef*)RCC_VBAT_BASE_ADDR)
#define VBAT_RTC	  	((VBAT_RTC_TypeDef*) VBAT_RTC_BASE_ADDR)
#define Flash_ACC	   	((Flash_Cache_TypeDef *) Flash_ACC_BASE_ADDR)
#define Flash 			((Flash_TypeDef *) Flash_BASE_ADDR)

#define PWR			 	((PWR_TypeDef *)PWR_MANAGE_BASE_ADDR)
#define PWR_VBAT		((PWR_VBAT_TypeDef *)(PWR_MANAGE_BASE_ADDR+0x1000))
#define WWDT			((WWDT_TypeDef *)WWDT_BASE_ADDR)
#define BAT_RAM		 ((VBAT_RAM_TypeDef*)BAT_RAM_BASE_ADDR)
#define EXTI		 	((EXTI_TypeDef*)EXTI_BASE_ADDR)

#define CRC 			((CRC_TypeDef*)CRC_BASE_ADDR)
#define IWDT			((IWDT_CFG_TypeDef*)IWDT_BASE_ADDR)
#define DMA			 ((DMA_TypeDef*)DMA_BASE_ADDR)
#define DMA_Channel0	((DMA_Channel_TypeDef*)DMA_CHANNEL0_BASE_ADDR)
#define DMA_Channel1	((DMA_Channel_TypeDef*)DMA_CHANNEL1_BASE_ADDR)
#define DMA_Channel2	((DMA_Channel_TypeDef*)DMA_CHANNEL2_BASE_ADDR)
#define DMA_Channel3	((DMA_Channel_TypeDef*)DMA_CHANNEL3_BASE_ADDR)
#define DMA_Channel4	((DMA_Channel_TypeDef*)DMA_CHANNEL4_BASE_ADDR)
#define DMA_Channel5	((DMA_Channel_TypeDef*)DMA_CHANNEL5_BASE_ADDR)
#define DMA_Channel6	((DMA_Channel_TypeDef*)DMA_CHANNEL6_BASE_ADDR)
#define DMA_Channel7	((DMA_Channel_TypeDef*)DMA_CHANNEL7_BASE_ADDR)

#define I2C				((I2C_TypeDef *) I2C_BASE_ADDR)

#define ADC				((ADC_TypeDef *) MAG_BASE_ADDR)
#define ADC_AMP			((ADC_AMP_TypeDef*)ADC_AMP_BASE_ADDR)

#define MAG1			((MAG_TypeDef*)(MAG1_BASE_ADDR))
#define MAG2			((MAG_TypeDef*)(MAG2_BASE_ADDR))
#define MAG3			((MAG_TypeDef*)(MAG3_BASE_ADDR))

#define ISO7816M1	   ((M7816_TypeDef *) ISO7816M1_BASE_ADDR)
#define ISO7816M2	   ((M7816_TypeDef *) ISO7816M2_BASE_ADDR)
#define EMV7816M2	   ((EMV7816M2_TypeDef *) EMV7816M2_BASE_ADDR)
#define ISO7816M3	   ((M7816_TypeDef *) ISO7816M3_BASE_ADDR)
#define UART1		   ((UART_TypeDef *) UART1_BASE_ADDR)
#define UART2		   ((UART_TypeDef *) UART2_BASE_ADDR)
#define UART5		   ((UART5_TypeDef *) UART5_BASE_ADDR)
#define TMC_MPU		 ((MPU_TypeDef*)MPU_BASE_ADDR)
#define PWM1			((PWM_TypeDef*)PWM1_BASE_ADDR)
#define PWM2			((PWM_TypeDef*)PWM2_BASE_ADDR)
#define PWM3			((PWM_TypeDef*)PWM3_BASE_ADDR)
#define PWM4			((PWM_TypeDef*)PWM4_BASE_ADDR)
#define TIM1		  	((TIM_TypeDef*)TIM1_BASE_ADDR)
#define TIM2		  	((TIM_TypeDef*)TIM2_BASE_ADDR)
#define TIM3		  	((TIM_TypeDef*)TIM3_BASE_ADDR)
#define TIM4		  	((TIM_TypeDef*)TIM4_BASE_ADDR)
#define TIM5		  	((TIM_TypeDef*)TIM5_BASE_ADDR)
#define TIM6		  	((TIM_TypeDef*)TIM6_BASE_ADDR)
#define FD2				((EFD2_TypeDef*)EFD2_BASE_ADDR)
#define FD3				((EFD3_TypeDef*)EFD3_BASE_ADDR)
#define ASH				((ASH_TypeDef *)ASH_BASE_ADDR)


#define FLASH2_BASE_ADDR							(uint32_t*)(0x08100000)
#define FLASH1_BASE_ADDR							(uint32_t*)(0x0800FFFF)
#define RegFile_BASE_ADDR							(uint32_t*)(0x08100000)//修改
#define ROM_BASE_ADDR								(uint32_t*)(0x0A00FFFF)

#define	Bit0_En		0x00000001//0x1
#define	Bit1_En		0x00000002//0x1<<1
#define	Bit2_En		0x00000004//0x1<<2
#define	Bit3_En		0x00000008//0x1<<3
#define	Bit4_En		0x00000010//0x1<<4
#define	Bit5_En		0x00000020//0x1<<5
#define	Bit6_En		0x00000040//0x1<<6
#define	Bit7_En		0x00000080//0x1<<7
#define	Bit8_En		0x00000100//0x1<<8
#define	Bit9_En		0x00000200//0x1<<9
#define	Bit10_En	0x00000400//0x1<<10
#define	Bit11_En	0x00000800//0x1<<11
#define	Bit12_En	0x00001000//0x1<<12
#define	Bit13_En	0x00002000//0x1<<13
#define	Bit14_En	0x00004000//0x1<<14
#define	Bit15_En	0x00008000//0x1<<15
#define	Bit16_En	0x00010000//0x1<<16
#define	Bit17_En	0x00020000//0x1<<17
#define	Bit18_En	0x00040000//0x1<<18
#define	Bit19_En	0x00080000//0x1<<19
#define	Bit20_En	0x00100000//0x1<<20
#define	Bit21_En	0x00200000//0x1<<21
#define	Bit22_En	0x00400000//0x1<<22
#define	Bit23_En	0x00800000//0x1<<23
#define	Bit24_En	0x01000000//0x1<<24
#define	Bit25_En	0x02000000//0x1<<25
#define	Bit26_En	0x04000000//0x1<<26
#define	Bit27_En	0x08000000//0x1<<27
#define	Bit28_En	0x10000000//0x1<<28
#define	Bit29_En	0x20000000//0x1<<29
#define	Bit30_En	0x40000000//0x1<<30
#define	Bit31_En	0x80000000//0x1<<31

#define	Bit0_Dis	0xFFFFFFFE//~(0x1)
#define	Bit1_Dis	0xFFFFFFFD//~(0x1<<1)
#define	Bit2_Dis	0xFFFFFFFB//~(0x1<<2)
#define	Bit3_Dis	0xFFFFFFF7//~(0x1<<3)
#define	Bit4_Dis	0xFFFFFFEF//~(0x1<<4)
#define	Bit5_Dis	0xFFFFFFDF//~(0x1<<5)
#define	Bit6_Dis	0xFFFFFFBF//~(0x1<<6)
#define	Bit7_Dis	0xFFFFFF7F//~(0x1<<7)
#define	Bit8_Dis	0xFFFFFEFF//~(0x1<<8)
#define	Bit9_Dis	0xFFFFFDFF//~(0x1<<9)
#define	Bit10_Dis	0xFFFFFBFF//~(0x1<<10)
#define	Bit11_Dis	0xFFFFF7FF//~(0x1<<11)
#define	Bit12_Dis	0xFFFFEFFF//~(0x1<<12)
#define	Bit13_Dis	0xFFFFDFFF//~(0x1<<13)
#define	Bit14_Dis	0xFFFFBFFF//~(0x1<<14)
#define	Bit15_Dis	0xFFFF7FFF//~(0x1<<15)
#define	Bit16_Dis	0xFFFEFFFF//~(0x1<<16)
#define	Bit17_Dis	0xFFFDFFFF//~(0x1<<17)
#define	Bit18_Dis	0xFFFBFFFF//~(0x1<<18)
#define	Bit19_Dis	0xFFF7FFFF//~(0x1<<19)
#define	Bit20_Dis	0xFFEFFFFF//~(0x1<<20)
#define	Bit21_Dis	0xFFDFFFFF//~(0x1<<21)
#define	Bit22_Dis	0xFFBFFFFF//~(0x1<<22)
#define	Bit23_Dis	0xFF7FFFFF//~(0x1<<23)
#define	Bit24_Dis	0xFEFFFFFF//~(0x1<<24)
#define	Bit25_Dis	0xFDFFFFFF//~(0x1<<25)
#define	Bit26_Dis	0xFBFFFFFF//~(0x1<<26)
#define	Bit27_Dis	0xF7FFFFFF//~(0x1<<27)
#define	Bit28_Dis	0xEFFFFFFF//~(0x1<<28)
#define	Bit29_Dis	0xDFFFFFFF//~(0x1<<29)
#define	Bit30_Dis	0xBFFFFFFF//~(0x1<<30)
#define	Bit31_Dis	0x7FFFFFFF//~(0x1<<31)
/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */

#define	USBFIFO_ADDR_START	0x21004000L
#define USBFIFO_ADDR_LIMIT	0x21004FFFL

#define SFR_ADDR_START 		0x40000000
#define SFR_ADDR_LIMIT 		0x42000000

#define SRAM1_ADDR_START 	0x20000000
#define SRAM1_ADDR_LIMIT 	0x20017FFF

#define SRAM2_ADDR_START 	0x20018000
#define SRAM2_ADDR_LIMIT 	0x20040000

#define SRAM3_ADDR_START 	0x2003C000
#define SRAM3_ADDR_LIMIT 	0x2003FFFF


#define FLASH1_ADDR_START 	0x00000000
#define FLASH1_ADDR_LIMIT 	0x00080000

#define FLASH2_ADDR_START 	0x10000000
#define FLASH2_ADDR_LIMIT 	0x10080000

#define OTP_ADDR_START 		0x1f000000
#define OTP_ADDR_LIMIT 		0x1f0009ff
#define OSC_CLOCK			12


/** @addtogroup Exported_macros
  * @{
  */
#define SET_BIT(REG, BIT)	 ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)	((REG) & (BIT))

#define CLEAR_REG(REG)		((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)		 ((REG))

/**
  * @}
  */

//#define SPI3_mode

#ifdef __cplusplus
}
#endif

#endif  

/**
  * @}
  */

/**
  * @}
  */
