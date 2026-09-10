;/**************************************************************************//**
; * @file	 startup_thm36.s
; * @brief	Core Device Startup File for
; *			thm36_1.3 Device Series
; * @version  V5.00
; * @date	 02. March 2018
; ******************************************************************************/


;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;	<o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size	  EQU	 0x00002000

				AREA	STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem		SPACE	Stack_Size
__initial_sp


; <h> Heap Configuration
;	<o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size		EQU	 0x00000100

				AREA	HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem		SPACE	Heap_Size
__heap_limit


				PRESERVE8
				THUMB


; Vector Table Mapped to Address 0 at Reset

				AREA	RESET, DATA, READONLY
				EXPORT  __Vectors
				EXPORT  __Vectors_End
				EXPORT  __Vectors_Size

__Vectors		DCD	 __initial_sp			  ; Top of Stack		
				DCD	 Reset_Handler			 ; Reset Handler		
				DCD	 NMI_Handler				; NMI Handler			
				DCD	 HardFault_Handler		 ; Hard Fault Handler	
				DCD	 MemManage_Handler		 ; MPU Fault Handler	
				DCD	 BusFault_Handler		  ; Bus Fault Handler	
				DCD	 UsageFault_Handler		; Usage Fault Handler	
				DCD	 0						 ; Reserved			
				DCD	 0						 ; Reserved			
				DCD	 0						 ; Reserved			
				DCD	 0						 ; Reserved			
				DCD	 SVC_Handler				; SVCall Handler		
				DCD	 DebugMon_Handler		  ; Debug Monitor Handler 
				DCD	 0						 ; Reserved			  
				DCD	 PendSV_Handler			; PendSV Handler		  
				DCD	 SysTick_Handler			; SysTick Handler		  

				; External Interrupts
; ToDo:  Add here the vectors for the device specific external interrupts handler
				DCD		RCC_IRQHandler			;  0: Default
				DCD		0						 ;  1: 
				DCD		0						 ;  2: 
				DCD		DMA_IRQHandler			;  3: 
				DCD		AHBMMU_IRQHandler		 ;  4: 
				DCD		0						 ;  5: 
				DCD		0						 ;  6: 
				DCD		MPU_IRQHandler			;  7: 
				DCD		0						 ;  8: 
				DCD		0						 ;  9: 
				DCD		0						 ;  10: 
				DCD		AES_IRQHandler			;  11: 
				DCD		DES_IRQHandler			;  12: 
				DCD		PKE_IRQHandler			;  13: 
				DCD		0						 ;  14: 
				DCD		EGS_IRQHandler			;  15: 
				DCD		LD_IRQHandler			 ;  16: 
				DCD		TRNG_IRQHandler			;  17: 
				DCD		FD2_IRQHandler			;  18: 
				DCD		FD3_IRQHandler			;  19: 
				DCD		VB_SENSOR_IRQHandler	  ;  20: 
				DCD		ASH_IRQHandler			;  21: 
				DCD		WWDT_IRQHandler			;  22: 
				DCD		TIMER1_IRQHandler		 ;  23: 
				DCD		TIMER2_IRQHandler		 ;  24: 
				DCD		TIMER3_IRQHandler		 ;  25: 
				DCD		TIMER4_IRQHandler		 ;  26: 
				DCD		TIMER5_IRQHandler		 ;  27: 
				DCD		TIMER6_IRQHandler		 ;  28: 
				DCD		0						 ;  29: 
				DCD		0						 ;  30: 
				DCD		0						 ;  31: 
				DCD		0						 ;  32: 0
				DCD		Flash1_IRQHandler		 ;  33: 1
				DCD		0						 ;  34: 2
				DCD		0						 ;  35: 3
				DCD		GPIO_IRQHandler			;  36: 4
				DCD		SPI1_IRQHandler			;  37: 5
				DCD		SPI2_IRQHandler			;  38: 6
				DCD		SPI3_IRQHandler			;  39: 7
				DCD		UART1_IRQHandler		  ;  40: 8
				DCD		UART2_IRQHandler		  ;  41: 9
				DCD		0						 ;  42: 10  
				DCD		0						 ;  43: 11
				DCD		I2C_IRQHandler			;  44: 12
				DCD		ISO7816M1_IRQHandler	  ;  45: 13
				DCD		ISO7816M2_IRQHandler	  ;  46: 14
				DCD		ISO7816M3_IRQHandler	  ;  47: 15
				DCD		PWM1_IRQHandler			;  48: 16
				DCD		PWM2_IRQHandler			;  49: 17
				DCD		0						 ;  50: 18
				DCD		USB_IRQHandler			;  51: 19
				DCD		UART5_IRQHandler		  ;  52: 20
				DCD		ADC_IRQHandler			;  53: 21
				DCD		0						 ;  54: 22
				DCD		0						 ;  55: 23
				DCD		PWRC_IRQHandler			;  56: 23
				DCD		0						 ;  57: 23
				DCD		PWM3_IRQHandler			;  58: 23
				DCD		PWM4_IRQHandler			;  59: 23
				DCD		0						 ;  60: 23
				DCD		0						 ;  61: 23
				DCD		0						 ;  62: 23
				DCD		0						 ;  63: 23
				DCD		EXTI0_15_IRQHandler		;  64: 23
				DCD		EXTI16_31_IRQHandler	  ;  65: 23
				DCD		EXTI32_47_IRQHandler	  ;  66: 23
				DCD		EXTI48_63_IRQHandler	  ;  67: 23
				DCD		EXTI64_79_IRQHandler	  ;  68: 23
				DCD		EXTI80_IRQHandler		 ;  69: 23
				DCD		EXTI81_IRQHandler		 ;  70: 23
				DCD		EXTI82_IRQHandler		 ;  71: 23
				DCD		EXTI83_IRQHandler		 ;  72: 23
				DCD		EXTI84_IRQHandler		 ;  73: 23
__Vectors_End

__Vectors_Size  EQU	 __Vectors_End - __Vectors

				AREA	|.text|, CODE, READONLY


; Reset Handler

Reset_Handler	PROC
				EXPORT  Reset_Handler			 [WEAK]
				IMPORT  SystemInit
				IMPORT  __main
PWR_PSWCR_ADDR	EQU		0x40008010
PWR_RAM_PD_VALUE EQU		0x803f0000
PWR_RAM_PU_VALUE EQU		0x80000000

				LDR		R1,=PWR_PSWCR_ADDR 
				LDR		R0,=PWR_RAM_PD_VALUE
				STR		R0,[R1]
				NOP
				MOV		R0, #0x0400
Loop				
				NOP
				SUBS	  R0, #1
				BNE		Loop

				LDR		R1,=PWR_PSWCR_ADDR  
				LDR		R0,=PWR_RAM_PU_VALUE
				STR		R0,[R1]
				MOV		  R0, #0x0400
Loop1
				NOP
				SUBS	  R0, #1
				BNE		Loop1
				
				LDR	 R0, = SystemInit
				BLX	 R0
				LDR	 R0, =__main
				BX	  R0
				ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler	 PROC
				EXPORT  NMI_Handler				[WEAK]
				B		.
				ENDP
HardFault_Handler\
				PROC
				EXPORT  HardFault_Handler		 [WEAK]
				B		.
				ENDP
MemManage_Handler\
				PROC
				EXPORT  MemManage_Handler		 [WEAK]
				B		.
				ENDP
BusFault_Handler\
				PROC
				EXPORT  BusFault_Handler		  [WEAK]
				B		.
				ENDP
UsageFault_Handler\
				PROC
				EXPORT  UsageFault_Handler		[WEAK]
				B		.
				ENDP
SVC_Handler	 PROC
				EXPORT  SVC_Handler				[WEAK]
				B		.
				ENDP
DebugMon_Handler\
				PROC
				EXPORT  DebugMon_Handler		  [WEAK]
				B		.
				ENDP
PendSV_Handler  PROC
				EXPORT  PendSV_Handler			[WEAK]
				B		.
				ENDP
SysTick_Handler PROC
				EXPORT  SysTick_Handler			[WEAK]
				B		.
				ENDP

Default_Handler PROC

				EXPORT  RCC_IRQHandler			[WEAK]
				EXPORT  DMA_IRQHandler			[WEAK]
				EXPORT  AHBMMU_IRQHandler		[WEAK]
				EXPORT  MPU_IRQHandler			[WEAK]
				EXPORT  AES_IRQHandler			[WEAK]
				EXPORT  DES_IRQHandler			[WEAK]
				EXPORT  PKE_IRQHandler			[WEAK]
				EXPORT  EGS_IRQHandler			[WEAK]
				EXPORT  LD_IRQHandler			[WEAK]
				EXPORT  TRNG_IRQHandler			[WEAK]
				EXPORT  FD2_IRQHandler			[WEAK]
				EXPORT  FD3_IRQHandler			[WEAK]
				EXPORT  VB_SENSOR_IRQHandler	[WEAK]
				EXPORT  ASH_IRQHandler			[WEAK]
				EXPORT  WWDT_IRQHandler			[WEAK]
				EXPORT  TIMER1_IRQHandler		[WEAK]
				EXPORT  TIMER2_IRQHandler		[WEAK]
				EXPORT  TIMER3_IRQHandler		[WEAK]
				EXPORT  TIMER4_IRQHandler		[WEAK]
				EXPORT  TIMER5_IRQHandler		[WEAK]
				EXPORT  TIMER6_IRQHandler		[WEAK]
				EXPORT  Flash1_IRQHandler		[WEAK]
				EXPORT  GPIO_IRQHandler			[WEAK]
				EXPORT  SPI1_IRQHandler			[WEAK]
				EXPORT  SPI2_IRQHandler			[WEAK]
				EXPORT  SPI3_IRQHandler			[WEAK]
				EXPORT  UART1_IRQHandler		[WEAK]
				EXPORT  UART2_IRQHandler		[WEAK]
				EXPORT  I2C_IRQHandler			[WEAK]
				EXPORT  ISO7816M1_IRQHandler	[WEAK]
				EXPORT  ISO7816M2_IRQHandler	[WEAK]
				EXPORT  ISO7816M3_IRQHandler	[WEAK]
				EXPORT  PWM1_IRQHandler			[WEAK]
				EXPORT  PWM2_IRQHandler			[WEAK]
				EXPORT  USB_IRQHandler			[WEAK]
				EXPORT  UART5_IRQHandler		[WEAK]
				EXPORT  ADC_IRQHandler			[WEAK]
				EXPORT  PWRC_IRQHandler			[WEAK]
				EXPORT  PWM3_IRQHandler			[WEAK]
				EXPORT  PWM4_IRQHandler			[WEAK]
				EXPORT  EXTI0_15_IRQHandler		[WEAK]
				EXPORT  EXTI16_31_IRQHandler	[WEAK]
				EXPORT  EXTI32_47_IRQHandler	[WEAK]
				EXPORT  EXTI48_63_IRQHandler	[WEAK]
				EXPORT  EXTI64_79_IRQHandler	[WEAK]
				EXPORT  EXTI80_IRQHandler		[WEAK]
				EXPORT  EXTI81_IRQHandler		[WEAK]
				EXPORT  EXTI82_IRQHandler		[WEAK]
				EXPORT  EXTI83_IRQHandler		[WEAK]
				EXPORT  EXTI84_IRQHandler		[WEAK]
RCC_IRQHandler		
DMA_IRQHandler		
AHBMMU_IRQHandler
MPU_IRQHandler
AES_IRQHandler		
DES_IRQHandler		
PKE_IRQHandler
EGS_IRQHandler
LD_IRQHandler
TRNG_IRQHandler		  
FD2_IRQHandler			
FD3_IRQHandler		
VB_SENSOR_IRQHandler  
ASH_IRQHandler		
WWDT_IRQHandler		
TIMER1_IRQHandler	 
TIMER2_IRQHandler	 
TIMER3_IRQHandler	 
TIMER4_IRQHandler	 
TIMER5_IRQHandler	 
TIMER6_IRQHandler	 
Flash1_IRQHandler	 
GPIO_IRQHandler		
SPI1_IRQHandler			
SPI2_IRQHandler		
SPI3_IRQHandler		
UART1_IRQHandler	  
UART2_IRQHandler	  
I2C_IRQHandler		
ISO7816M1_IRQHandler	
ISO7816M2_IRQHandler  
ISO7816M3_IRQHandler  
PWM1_IRQHandler		
PWM2_IRQHandler		
USB_IRQHandler		  
UART5_IRQHandler
ADC_IRQHandler		
PWRC_IRQHandler
PWM3_IRQHandler		
PWM4_IRQHandler		
EXTI0_15_IRQHandler	
EXTI16_31_IRQHandler  
EXTI32_47_IRQHandler  
EXTI48_63_IRQHandler  
EXTI64_79_IRQHandler  
EXTI80_IRQHandler	 
EXTI81_IRQHandler	 
EXTI82_IRQHandler	 
EXTI83_IRQHandler	 
EXTI84_IRQHandler	 




				B		.

				ENDP


				ALIGN


; User Initial Stack & Heap

				IF	  :DEF:__MICROLIB

				EXPORT  __initial_sp
				EXPORT  __heap_base
				EXPORT  __heap_limit

				ELSE

				IMPORT  __use_two_region_memory
				EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
				LDR	 R0, =  Heap_Mem
				LDR	 R1, =(Stack_Mem + Stack_Size)
				LDR	 R2, = (Heap_Mem +  Heap_Size)
				LDR	 R3, = Stack_Mem
				BX	  LR
				ENDP

				ALIGN

				ENDIF


				END
