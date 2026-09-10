
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)



MEMORY
{
	RAM(xrw) : ORIGIN = 0x20000000,LENGTH = 96K
	FLASH(rx): ORIGIN = 0x10000000,LENGTH = 512K
    RAM4_KEY(xrw): ORIGIN = 0x21003000,LENGTH = 4K
	RAM4_USB(xrw) : ORIGIN = 0x21004000,LENGTH = 4K
}

SECTIONS
{
	. = ALIGN(4);
	.isr_vector :
	{
    	KEEP(*(.isr_vector))       /* interrupt vector table  */
	    . = ALIGN(4);
	} > FLASH

	.text : 
    {   
        *(.text)                   /* remaining code */
        *(.text.*)                 /* remaining code */
        . = ALIGN(4);
    } > FLASH
	
	.rodata :
    {
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

	. = ALIGN(4);
    _exit = .;
    _keyram_load_addr = LOADADDR(.keyram);
	.keyram :
	{
    	. = ALIGN(4);
	    __keyram_start = .;       /* create a global symbol at keyram start */
    	*(.keyram)                /* secret key */
	    *(.keyram*)
    	. = ALIGN(4);
	    __keyram_end = .;         /* create a global symbol at keyram end */
	} > RAM4_KEY AT > FLASH

    _usbram_load_addr = LOADADDR(.usbram);
	.usbram :
	{
    	. = ALIGN(4);
	    __usbram_start = .;       /* create a global symbol at usbram start */
    	*(.usbram)                /* USB communication buffer */
	    *(.usbram*)
    	. = ALIGN(4);
	    __usbram_end = .;         /* create a global symbol at usbram end */
	} > RAM4_USB AT > FLASH

	. = ALIGN(4);
    _sidata_load_addr = LOADADDR(.data);
	.data :
	{
	    . = ALIGN(4);
		__data_start__ = .;
		*(vtable)
		*(.data*)

		. = ALIGN(4);
		PROVIDE (__preinit_array_start = .);
		*(.preinit_array)
		PROVIDE (__preinit_array_end = .);

		. = ALIGN(4);
		PROVIDE (__init_array_start = .);
		*(SORT(.init_array.*))
		*(.init_array)
		PROVIDE (__init_array_end = .);

		. = ALIGN(4);
		PROVIDE (__fini_array_start = .);
		*(SORT(.fini_array.*))
		*(.fini_array)
		PROVIDE (__fini_array_end = .);

		. = ALIGN(4);
		__data_end__ = .;
	} > RAM AT > FLASH

	. = ALIGN(4);
	.bss :
	{
		__bss_start__ = .;
		*(.bss)
		*(.bss.*)
		. = ALIGN(4);
		__bss_end__ = .;
	} > RAM
	
	. = ALIGN(4);
	.heap :
	{		
		__end__ = .;
		end = __end__;
		*(.heap*)
		__HeapLimit = .;
	} > RAM
	
	.stack_dummy :
	{
		*(.stack)
	} > RAM
	
	__StackTop = ORIGIN(RAM) + LENGTH(RAM);
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);
	
	/*检查数据+堆+栈是否超出RAM*/
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}


