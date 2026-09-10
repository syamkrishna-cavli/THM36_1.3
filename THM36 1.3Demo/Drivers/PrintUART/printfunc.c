/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: printf function
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/

#include "printfunc.h"
#include "TMC_GPIO.h"

#define ZEROPAD	 1
#define SIGN	 2
#define PLUS	 4
#define SPACE	 8
#define LEFT	 16
#define SPECIAL	 32
#define LARGE	 64


#define DEBUG_UART1		0
#define DEBUG_UART5		1

#define DEBUG_UART		DEBUG_UART5

#if (DEBUG_UART == DEBUG_UART5)
UART5_HandleTypeDef huart5_printf;
UART5_InitTypeDef uart5_init_printf;
#else
UART_HandleTypeDef huartx_printf;
UART_InitTypeDef uart_init_printf;
#endif

/**
  * @brief Set each @ref UART5_InitTypeDef field to expected value.
  * @param UART_InitStruct pointer to a @ref UART5_InitTypeDef structure
  *                         whose fields will be set to expected values.
  * @retval None
  */
#if (DEBUG_UART == DEBUG_UART5)
static void printf_UART5_StructInit(UART5_InitTypeDef* UART_InitStruct)
{
	UART_InitStruct->ClockPrescaler = UART5_PRESCALER_DIV1;
	UART_InitStruct->BaudRate = 115200;
	UART_InitStruct->WordLength = UART5_WordLength_8b;
	UART_InitStruct->Parity = UART5_Parity_None;
	UART_InitStruct->StopBits = UART5_StopBits_1; 
	UART_InitStruct->Mode = UART5_Mode_Rx | UART5_Mode_Tx;
	UART_InitStruct->HardwareFlowControl = UART5_HardwareFlowControl_None;
	UART_InitStruct->FIFOMode = UART5_FIFOMODE_DISABLE;
	UART_InitStruct->TXFIFOThreshold = UART5_TXFIFO_THRESHOLD_1_8;
	UART_InitStruct->RXFIFOThreshold = UART5_RXFIFO_THRESHOLD_1_8;
	UART_InitStruct->TxRx_Swap = UART5_NSWAP_TXRX;
}

/**
  * @brief Enable clock,Config UART5 GPIO function, Config IRQ, DMA
  * @param huart pointer to a @ref UART5_HandleTypeDef structure
  *                         whose fields will be set to expected values.
  * @retval None
  */
void printf_UART5_MspInit(UART5_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIOx_Init;
	
	/*Enable gated clock*/
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO, ENABLE);
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_UART5, ENABLE);	
	/*Config UART5 clock source,div,enable*/
	RCC_UART5ClockConfig(RCC_UART5_CLKSRC_PLL_L,0,ENABLE);
    
    GPIO_PinAFConfig(GPIOD,GPIO_PIN_Source8,GPIO_AF4_UART5);
    GPIO_PinAFConfig(GPIOD,GPIO_PIN_Source9,GPIO_AF4_UART5);
    
	/**************************Config UART5 GPIO function*******************************************/	
	/***********Init UART5_TX(GD_IO8),UART5_RX(GD_IO9)***************/
	TMC_GPIO_StructInit(&GPIOx_Init);							
	GPIOx_Init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIOx_Init.Mode = GPIO_MODE_AF;							
	GPIOx_Init.Speed = GPIO_HIGH_SPEED;						
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);							
}

/**
  * @brief Initializes the UART mode according to the specified parameters in
  *		 the UART_InitTypeDef and create the associated handle.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef printf_UART5_Init(UART5_HandleTypeDef *huart)
{
	/* Check the UART handle allocation */
	if(huart == NULL)
	{
		return TMC_ERROR;
	}

	if((huart->TxState == TMC_UART_STATE_RESET)||(huart->RxState == TMC_UART_STATE_RESET))
	{  
		/* Init the low level hardware */
		printf_UART5_MspInit(huart);
	}
	/* Set the UART Communication parameters */
	UART5_Init(huart->Instance, huart->Init);
	
	/* Initialize the UART state */
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState= TMC_UART_STATE_READY;
	huart->RxState= TMC_UART_STATE_READY;
	
	return TMC_OK;
}
#endif
/**
  *@brief	UART printf init
  *@param	None.
  *@retval	None.
  */
void UART_PrintInit(void)
{
#if (DEBUG_UART == DEBUG_UART5)
	printf_UART5_StructInit(&uart5_init_printf);
	huart5_printf.Init = &uart5_init_printf;
	huart5_printf.Instance = UART5;
	TMC_UART5_StructInit(&huart5_printf);
	printf_UART5_Init(&huart5_printf);
#else
	UART_StructInit(&uart_init_printf);
	huartx_printf.Init = &uart_init_printf;
	huartx_printf.Instance = UART1;
	TMC_UART_StructInit(&huartx_printf);
	TMC_UART_Init(&huartx_printf);
#endif
}

const unsigned char _ctype[256] = 
{
	_C,_C,_C,_C,_C,_C,_C,_C,								/* 0-7 */	
	_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,				 /* 8-15 */   
	_C,_C,_C,_C,_C,_C,_C,_C,								/* 16-23 */  
	_C,_C,_C,_C,_C,_C,_C,_C,								/* 24-31 */  
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,							/* 32-39 */  
	_P,_P,_P,_P,_P,_P,_P,_P,								/* 40-47 */  
	_D,_D,_D,_D,_D,_D,_D,_D,								/* 48-55 */  
	_D,_D,_P,_P,_P,_P,_P,_P,								/* 56-63 */  
	_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,			  /* 64-71 */  
	_U,_U,_U,_U,_U,_U,_U,_U,								/* 72-79 */  
	_U,_U,_U,_U,_U,_U,_U,_U,								/* 80-87 */  
	_U,_U,_U,_P,_P,_P,_P,_P,								/* 88-95 */  
	_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,			  /* 96-103 */ 
	_L,_L,_L,_L,_L,_L,_L,_L,								/* 104-111 */
	_L,_L,_L,_L,_L,_L,_L,_L,								/* 112-119 */
	_L,_L,_L,_P,_P,_P,_P,_C,								/* 120-127 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,						/* 128-143 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,						/* 144-159 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,	/* 160-175 */
	_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,		/* 176-191 */
	_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,		/* 192-207 */
	_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,		/* 208-223 */
	_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,		/* 224-239 */
	_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L		 /* 240-255 */
};

unsigned char T_ctype[3] = {1};

/**
  *@brief	printfStr
  *@param	string
  *@retval   None
  **/
void printfStr(char* string)
{
	while(*string)
	{
		#if (DEBUG_UART == DEBUG_UART5)
		/* Check the send buffer is empty */
		while((UART5->ISR & Bit7_En) == 0){};
		if(*string=='\n')
		{
			UART5_SendData(UART5, 0x0d);
		}
		UART5_SendData(UART5, *string++);
		#else
		/* Check the send buffer is empty */
		while((UART1->STS & Bit0_En) == 0){};
		if(*string=='\n')
		{
			UART_SendData(UART1, 0x0d);
		}
		UART_SendData(UART1, *string++);
		#endif
	}
}

/**
  *@brief	do_div
  *@param	num
  *@param	base
  *@retval   the remainder of the (*num)/base
  **/
unsigned do_div(unsigned long *num, unsigned int base)
{
	unsigned ret = 0;
	ret = (*num) % base;
	(*num) /= base;
	return  ret;
}

/**
  *@brief	skip_atoi
  *@param	s
  *@retval   
  **/
static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

/**
  *@brief	strnlen
  *@param	s - the char pointer
  *@param	count
  *@retval   buf
  **/
int strnlen(const char * s, int count)
   {
	 const char *sc;

	 for (sc = s; count-- && *sc != '\0'; ++sc)
	 {
	 }
	 return sc - s;
   }


/**
  *@brief	number
  *@param	buf
  *@param	end
  *@param	num
  *@param	base
  *@param	size
  *@param	precision
  *@param	type
  *@retval   buf
  **/
static char * number(char * buf, char * end, long num, int base, int size, int precision, int type)
{
	char c,sign,tmp[66];
	const char *digits;
	const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div((unsigned long *)&num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT))) {
		while(size-->0) {
			if (buf <= end)
				*buf = ' ';
			++buf;
		}
	}
	if (sign) {
		if (buf <= end)
			*buf = sign;
		++buf;
	}
	if (type & SPECIAL) {
		if (base==8) {
			if (buf <= end)
				*buf = '0';
			++buf;
		} else if (base==16) {
			if (buf <= end)
				*buf = '0';
			++buf;
			if (buf <= end)
				*buf = digits[33];
			++buf;
		}
	}
	if (!(type & LEFT)) {
		while (size-- > 0) {
			if (buf <= end)
				*buf = c;
			++buf;
		}
	}
	while (i < precision--) {
		if (buf <= end)
			*buf = '0';
		++buf;
	}
	while (i-- > 0) {
		if (buf <= end)
			*buf = tmp[i];
		++buf;
	}
	while (size-- > 0) {
		if (buf <= end)
			*buf = ' ';
		++buf;
	}
	return buf;
}

/**
  *@brief	vsnprintf
  *@param	size
  *@param	fmt
  *@param	args
  *@retval
  */
int vsnprintf(char *buf, unsigned size, const char *fmt, va_list args)
{
	int len;
	unsigned long num;
	int i, base;
	char *str, *end, c;
	const char *s;

	int flags;
	int field_width;
	int precision;
	int qualifier;

	str = buf;
	end = buf + size - 1;

	if (end < buf - 1) {
		end = ((void *) -1);
		size = end - buf + 1;
	}

	for (; *fmt ; ++fmt) {
		if (*fmt != '%') {
			if (str <= end)
				*str = *fmt;
			++str;
			continue;
		}

		flags = 0;
		repeat:
			++fmt;
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
			}

		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt =='Z') {
			qualifier = *fmt;
			++fmt;
		}

		base = 10;

		switch (*fmt) {
			case 'c':
				if (!(flags & LEFT)) {
					while (--field_width > 0) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				c = (unsigned char) va_arg(args, int);
				if (str <= end)
					*str = c;
				++str;
				while (--field_width > 0) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 's':
				s = va_arg(args, char *);
				if (!s)
					s = "<NULL>";

				len = strnlen(s, precision);

				if (!(flags & LEFT)) {
					while (len < field_width--) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				for (i = 0; i < len; ++i) {
					if (str <= end)
						*str = *s;
					++str; ++s;
				}
				while (len < field_width--) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 'p':
				if (field_width == -1) {
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = number(str, end,(unsigned long) va_arg(args, void *),16, field_width, precision, flags);
				continue;


			case 'n':
				if (qualifier == 'l') {
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} else if (qualifier == 'Z') {
					unsigned * ip = va_arg(args, unsigned *);
					*ip = (str - buf);
				} else {
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;

			case '%':
				if (str <= end)
					*str = '%';
				++str;
				continue;

			case 'o':
				base = 8;
				break;

			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;

			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;

			default:
				if (str <= end)
					*str = '%';
				++str;
				if (*fmt) {
					if (str <= end)
						*str = *fmt;
					++str;
				} else {
					--fmt;
				}
				continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, long);
		else if (qualifier == 'l') {
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z') {
			num = va_arg(args, unsigned);
		} else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, end, num, base,
				field_width, precision, flags);
	}
	if (str <= end)
		*str = '\0';
	else if (size > 0)
		*end = '\0';
	return str-buf;
}

/**
  *@brief	vsprintf
  *@param	fmt:the format you want to print
  *@param	args
  *@retval	
  */
int vsprintf(char *buf, const char *fmt, va_list args)
{
	return vsnprintf(buf, 0xFFFFFFFFUL, fmt, args);
}


/**
  *@brief	printf
  *@param	fmt
  *@param	...
  *@retval	None.
  */
void UART_printf(const char *fmt,...)
{
	char __printf_buf[128];
	va_list args;
	va_start(args, fmt);
	vsprintf(__printf_buf,fmt,args);
	va_end(args);
	printfStr(__printf_buf);
}
