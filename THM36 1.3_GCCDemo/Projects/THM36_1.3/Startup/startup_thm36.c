
#include <string.h>

extern int main(void);
extern void SystemInit(void);
extern void Reset_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);

extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void RCC_IRQHandler(void);
extern void DMA_IRQHandler(void);
extern void AHBMMU_IRQHandler(void);
extern void MPU_IRQHandler(void);
extern void AES_IRQHandler(void);
extern void DES_IRQHandler(void);
extern void PKE_IRQHandler(void);
extern void TRNG_IRQHandler(void);
extern void FD2_IRQHandler(void);
extern void FD3_IRQHandler(void);
extern void EGS_IRQHandler(void);
extern void LD_IRQHandler (void);
extern void VB_SENSOR_IRQHandler(void);
extern void ASH_IRQHandler(void);
extern void WWDT_IRQHandler(void);
extern void TIMER1_IRQHandler(void);
extern void TIMER2_IRQHandler(void);
extern void TIMER3_IRQHandler(void);
extern void TIMER4_IRQHandler(void);
extern void TIMER5_IRQHandler(void);
extern void TIMER6_IRQHandler(void);
extern void Flash1_IRQHandler(void);
extern void GPIO_IRQHandler(void);
extern void SPI1_IRQHandler(void);
extern void SPI2_IRQHandler(void);
extern void SPI3_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void UART5_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void ISO7816M1_IRQHandler(void);
extern void ISO7816M2_IRQHandler(void);
extern void ISO7816M3_IRQHandler(void);
extern void PWM1_IRQHandler(void);
extern void PWM2_IRQHandler(void);
extern void PWM3_IRQHandler(void);
extern void PWM4_IRQHandler(void);
extern void USB_IRQHandler(void);
extern void ADC_IRQHandler(void);
extern void EXTI0_15_IRQHandler(void);
extern void EXTI16_31_IRQHandler(void);
extern void EXTI32_47_IRQHandler(void);
extern void EXTI48_63_IRQHandler(void);
extern void EXTI64_79_IRQHandler(void);
extern void EXTI80_IRQHandler(void);
extern void EXTI81_IRQHandler(void);
extern void EXTI82_IRQHandler(void);
extern void EXTI83_IRQHandler(void);
extern void EXTI84_IRQHandler(void);

extern unsigned long _estack;

typedef void (*pfnISR)(void);// Pointer to exception handle function 

#define Reserved (pfnISR)0

__attribute__ ((section(".isr_vector")))
pfnISR VectorTable[] =
{
	(pfnISR)(0x20020000),	  //The initial stack pointer is the top of SRAM 
	Reset_Handler,        	      // The reset handler 
	NMI_Handler,      		  //NMI Handler			
	HardFault_Handler,         // ; Hard Fault Handler	
	MemManage_Handler ,        //; MPU Fault Handler	
	BusFault_Handler ,         //; Bus Fault Handler	
	UsageFault_Handler,        //; Usage Fault Handler	
	Reserved,                         //; Reserved			
	Reserved ,                        //; Reserved			
	Reserved ,                        //; Reserved			
	Reserved ,                        //; Reserved			
	SVC_Handler,               //; SVCall Handler		
	DebugMon_Handler,          //; Debug Monitor Handler 
	Reserved ,                        //; Reserved			  
	PendSV_Handler ,           //; PendSV Handler		  
	SysTick_Handler ,          //; SysTick Handler		  
  //External Interrupts
  //; ToDo:  Add here the vectors for the device specific external interrupts handler
	RCC_IRQHandler ,     	  //;  0: Default
	Reserved ,        				  //;  1: 
	Reserved ,      				  //;  2: 
	DMA_IRQHandler  ,    	  //;  3: 
	AHBMMU_IRQHandler ,     	  //;  4: 
	Reserved,      				  	  //;  5: 
	Reserved  ,    				  	  //;  6: 
	MPU_IRQHandler  ,    	  //;  7: 
	Reserved   ,   				      //;  8: 
	Reserved   ,   				      //;  9: 
	Reserved ,     	 		 //;  10: 
	AES_IRQHandler ,     	  //;  11: 
	DES_IRQHandler ,     	  //;  12: 
	PKE_IRQHandler ,     	  //;  13: 
	Reserved ,     				      //;  14: 
	EGS_IRQHandler	,	      //;  15: 
	LD_IRQHandler	,	      //;  16: 
	TRNG_IRQHandler ,     	  //;  17: 
	FD2_IRQHandler	,	      //;  18: 
	FD3_IRQHandler  ,    	  //;  19: 
	VB_SENSOR_IRQHandler,	  //;  20: 
	ASH_IRQHandler  ,    	  //;  21: 
	WWDT_IRQHandler  ,    	  //;  22: 
	TIMER1_IRQHandler ,        //;  23: 
	TIMER2_IRQHandler  ,    	  //;  24: 
	TIMER3_IRQHandler ,     	  //;  25: 
	TIMER4_IRQHandler ,     	  //;  26: 
	TIMER5_IRQHandler ,     	  //;  27: 
	TIMER6_IRQHandler ,     	  //;  28: 
	Reserved        	,			  //;  29: 
	Reserved      		,		      //;  30: 
	Reserved		    ,  			  //;  31: 
	Reserved      		,		      //;  32: 0
	Flash1_IRQHandler ,    	  //;  33: 1
	Reserved      				,      //;  34: 2
	Reserved      			,	      //;  35: 3
	GPIO_IRQHandler   ,   	  //;  36: 4
	SPI1_IRQHandler  ,    	  //;  37: 5
	SPI2_IRQHandler      	  	,			  //;  38: 6
	SPI3_IRQHandler ,     	  //;  39: 7
	UART1_IRQHandler  ,    	  //;  40: 8
	UART2_IRQHandler ,     	 //;  41: 9
	Reserved ,     	 //;  42: 10  
	Reserved               ,         //;  43: 11
	I2C1_IRQHandler  ,    	 //;  44: 12
	ISO7816M1_IRQHandler,     //;  45: 13
	ISO7816M2_IRQHandler,     //;  46: 14
	ISO7816M3_IRQHandler      				,	 //;  47: 15
	PWM1_IRQHandler   ,   	 //;  48: 16
	PWM2_IRQHandler   ,   	 //;  49: 17
	Reserved      	  		,		 //;  50: 18
	USB_IRQHandler   ,   	 //;  51: 19
	UART5_IRQHandler      	  		,		 //;  52: 20
	ADC_IRQHandler   ,   	 //;  53: 21
	Reserved       		,		 //;  54: 22
	Reserved            	,		 //;  55: 23
	Reserved      			,	     //;  56: 23
	Reserved      	  		,		 //;  57: 23
	PWM3_IRQHandler      			,	     //;  58: 23
	PWM4_IRQHandler      			,	     //;  59: 23
	Reserved      			,	     //;  60: 23
	Reserved      			,	     //;  61: 23
	Reserved      			,	     //;  62: 23
	Reserved      			,	     //;  63: 23
	EXTI0_15_IRQHandler ,     //;  64: 23
	EXTI16_31_IRQHandler,     //;  65: 23
	EXTI32_47_IRQHandler ,    //;  66: 23
	EXTI48_63_IRQHandler ,    //;  67: 23
	EXTI64_79_IRQHandler ,    //;  68: 23
	EXTI80_IRQHandler   ,   	 //;  69: 23
	EXTI81_IRQHandler   ,   	 //;  70: 23
	EXTI82_IRQHandler  ,    	 //;  71: 23
	EXTI83_IRQHandler   ,   	 //;  72: 23
	EXTI84_IRQHandler  ,    	 //;  73: 23
};

//***************************************************************************** 
// 
// The following are constructs created by the linker, indicating where the 
// the "data" and "bss" segments reside in memory.  The initializers for the 
// for the "data" segment resides immediately following the "text" segment. 
// 
//***************************************************************************** 

extern unsigned long _keyram_load_addr;
extern unsigned long __keyram_start;
extern unsigned long __keyram_end;
extern unsigned long _usbram_load_addr;
extern unsigned long __usbram_start;
extern unsigned long __usbram_end;
extern unsigned long _sidata_load_addr;
extern unsigned long __data_start__;
extern unsigned long __data_end__;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;

const unsigned long PWR_PSWCR_ADDR = 0x40008010, PWR_RAM_PD_VALUE = 0x803f0000, PWR_RAM_PU_VALUE = 0x80000000;
void Reset_Handler(void)
{

__asm__ volatile
(
    "LDR R1, =PWR_PSWCR_ADDR;"
    "LDR R0, =PWR_RAM_PD_VALUE;"
    "STR R0, [R1];"
    "NOP;"
    "MOV R0, #0x0400;"

"Loop:;"

    "NOP;"
    "SUBS R0, #1;"
    "BNE Loop;"

    "LDR R1, =PWR_PSWCR_ADDR;"
    "LDR R0, =PWR_RAM_PU_VALUE;"
    "STR R0, [R1];"
    "NOP;"
    "MOV R0, #0x0400;"

"Loop1:;"

    "NOP;"
    "SUBS R0, #1;"
    "BNE Loop1;"
);

	SystemInit();

	unsigned long *pulSrc, *pulDest;

// 
// Copy the data segment initializers from flash to SRAM. 
// 
	pulSrc = &_sidata_load_addr;
	for(pulDest = &__data_start__; pulDest < &__data_end__; )
	{
		*pulDest++ = *pulSrc++;
	}

	pulSrc = &_keyram_load_addr;
	for(pulDest = &__keyram_start; pulDest < &__keyram_end; )
	{
		*pulDest++ = *pulSrc++;
    }

	pulSrc = &_usbram_load_addr;
	for(pulDest = &__usbram_start; pulDest < &__usbram_end; )
	{
		*pulDest++ = *pulSrc++;
    }

// 
// Zero fill the bss segment. 
// 
	for(pulDest = &__bss_start__; pulDest < &__bss_end__; )
	{
		*pulDest++ = 0;
	}

// 
// Call the application's entry point. 
// 
	main();
}
