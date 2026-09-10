/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwm_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#include "thm36_1.3.h"
#include "TMC_RCC.h"
#include "TMC_PWM.h"
#include "PWM_DemoMain.h"
#include "TMC_GPIO.h"
//TMC(20240408)-zt:hpwm->hpwm1,PWM_Init->PWM_Init1
PWM_HandleTypeDef hpwm1;
PWM_InitTypeDef	PWM_Init1;
typedef enum 
{
	PWM_Poll = 0,
	PWM_IRQ  = 1
	
} PWM_TypefDef;
/*There is a '__weak' type PWM_StructInit in PWM.c,redefine a PWM_StructInit will replace it.*/
void PWM_StructInit(PWM_InitTypeDef *PWM_InitStruct)
{
	PWM_InitStruct->PWM_Start_Level = START_LEVEL_LOW;
	PWM_InitStruct->PWM_Stop_Level = STOP_LEVEL_IS_START;
	PWM_InitStruct->PWM_Div = PWM_CLK_DIV1;
	PWM_InitStruct->PWM_PSC = 11;	 /*The register will add 1 at this number.*/
	PWM_InitStruct->PWM_PER = 999;	 /*The register will add 1 at this number.*/
	PWM_InitStruct->PWM_DTY = 999>>1;/*The register will add 1 at this number.*/
}

void TMC_PWM_MspInit(PWM_HandleTypeDef* hpwm)
{
	GPIO_InitTypeDef GPIOx_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_PWM1, ENABLE);
		TMC_GPIO_StructInit(&GPIOx_Init);
	/*Set GPIOE PIN6 as PWM TO.(User can config other pin of PWM function as PWM TO,please refer the User Mannual document.)*/
	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source6, GPIO_AF1_PWM1);
	GPIOx_Init.Pin = GPIO_PIN_6;
	GPIOx_Init.Mode = GPIO_MODE_AF;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	TMC_GPIO_Init(GPIOE, &GPIOx_Init);
	/*Configure the PWM clock source.*/
	RCC_PWMClockConfig(INTERFACE_PWM1, RCC_PWM_CLKSRC_HSI,0,ENABLE);
	/*Set the NVIC interrupt of PWM1.*/
	NVIC_ClearPendingIRQ(PWM1_IRQn);
	NVIC_EnableIRQ(PWM1_IRQn);
}

void TMC_PWM_MspDeInit(PWM_HandleTypeDef* hpwm)
{
	GPIO_InitTypeDef GPIOx_Init;
	/*Enable the PWM1 clock.*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_PWM1, DISABLE);
	TMC_GPIO_StructInit(&GPIOx_Init);
	
	GPIOx_Init.Pin = GPIO_PIN_6;
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOE, &GPIOx_Init);
	/*Configure the PWM clock source.*/
	RCC_PWMClockConfig(INTERFACE_PWM1, RCC_PWM_CLKSRC_HSI,0,DISABLE);
	NVIC_ClearPendingIRQ(PWM1_IRQn);
	NVIC_DisableIRQ(PWM1_IRQn);
}

void PWM_DemoMain(void)
{
	PWM_TypefDef PWM_Test;
	TMC_PWM_StructInit(&hpwm1);
	PWM_StructInit(&PWM_Init1);
	hpwm1.Init = &PWM_Init1;
	hpwm1.Instance = PWM1;
	TMC_PWM_Init(&hpwm1);
	
	PWM_Test = PWM_IRQ;
	
	/*PWM polling mode.*/
	if(PWM_Test == PWM_Poll)
	{
		TMC_PWM_OutputWaveforms(&hpwm1,10,1000);
	}
	/*PWM Interrupt mode.*/
	else
	{
		TMC_PWM_OutputWaveforms_IT(&hpwm1,10); 
		while(hpwm1.State == TMC_PWM_STATE_BUSY);
	}
	TMC_PWM_DeInit(&hpwm1);
}

void TMC_PWM_CpltCallback(PWM_HandleTypeDef* hpwm)
{

}

void PWM_Demo_Main(void)
{
	PWM_DemoMain();
}
