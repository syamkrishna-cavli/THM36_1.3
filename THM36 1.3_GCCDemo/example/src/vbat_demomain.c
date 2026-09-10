/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: vbat_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   
#include "thm36_1.3.h"
#include "rcc.h"
#include "tmc_exti.h"
#include "tmc_vbat.h"
#include "vbat_demomain.h"
#include "tmc_flash.h"
#include "flash_demomain.h"

VBAT_HandleTypeDef hVbat;
//TMC(20240408)-zt:to split IQHander enter-parameters,split FD3_Init from VBAT_HandleTypeDef 
VBAT_FD3_HandleTypeDef hVbat_fd3;
volatile uint8_t TMC_ALARM_Flag;

typedef enum 
{
	VBAT_SD_Demo_Test =0,
	VBAT_VD_Demo_Test =1,
	VBAT_TD_Demo_Test =2,
	VBAT_RTC_Demo_Test =3,
	VBAT_IWDT_Demo_Test =4
} VBAT_Demo_TypefDef;


/******************************************************************************************************************* 
This function is configuring all the module of VBAT(Not including the main power wakeup by baterry power function).
 *******************************************************************************************************************/
void VBAT_SENSOR_ExampleOfInitial(void)
{
	TMC_VBAT_MspInit(&hVbat);
	TMC_VBAT_DeInit(&hVbat);

	/*Init the RTC date as 00:00:00, 2018.11.1.*/
	BAT_RTC_StructInit(&hVbat.RTC_Init);
	BAT_RTC_AlarmStructInit(&hVbat.RTC_AlarmInit);
	/*Set the FD3 clock detecting clock is 32K clock,the reference source is PLL_L 48MHz.*/
	hVbat_fd3.FD3_Init.FD3_CheckClock = INSIDE_32K_CLOCK;
	hVbat_fd3.FD3_Init.FD3_ReferenceClock = REFERENCE_CLOCK_PLL_L;
	hVbat_fd3.FD3_Init.FD3_Divsion = REFERENCE_CLOCK_DIVSION_NO;
	/*Set the IWDT initial value.*/
	BAT_IWDT_StructInit(&hVbat.IWDT_Init);
	/*The static SD total rounds is 4 rounds.*/
	hVbat.SD_Init.Static_Init.TotalNumber = 4;
	/*The static SD tolerance rounds is 4 rounds.*/
	hVbat.SD_Init.Static_Init.FaultNumber = 1;
	hVbat.SD_Init.Static_Init.ClearBuckUpReg = SD_STATIC_ALARM_CLEAR_BAT_RAM_ENABLE;
	hVbat.SD_Init.Dynamic_Init.ClearBuckUpReg = SD_DYNAMIC_ALARM_CLEAR_BAT_RAM_ENABLE;
	
	/*SD0 type is static*/
	hVbat.SD_Init.SD0_ConfigMode = VBAT_SDX_TYPE_STATIC;
	/*SD1 type is static*/
	hVbat.SD_Init.SD1_ConfigMode = VBAT_SDX_TYPE_STATIC;
	/*SD2 type is dynamic*/
	hVbat.SD_Init.SD2_ConfigMode = VBAT_SDX_TYPE_DYNAMIC;
	/*SD3 type is dynamic*/
	hVbat.SD_Init.SD3_ConfigMode = VBAT_SDX_TYPE_DYNAMIC; 
	
	/*SD0 alarm as low level.*/
	hVbat.SD_Init.SD0_Alarm_Static = VBAT_SDx_ALARM_LEVEL_IS_LOW;
	/*SD1 alarm as low level.*/
	hVbat.SD_Init.SD1_Alarm_Static = VBAT_SDx_ALARM_LEVEL_IS_LOW;
	/*SD4 alarm as high level.*/
	hVbat.SD_Init.SD4_Alarm_Static = VBAT_SDx_ALARM_LEVEL_IS_HIGH;
	/*SD5 alarm as high level.*/
	hVbat.SD_Init.SD5_Alarm_Static = VBAT_SDx_ALARM_LEVEL_IS_HIGH;
	
	hVbat.SD_Init.Dynamic_Init.ReceiveDelay = 0x20; //Shall not be a smaller value than 0x20.

	/*TD*/
	hVbat.TD_Init.CheckMode = VBAT_TIME_OVERFLOW_CHECK;
	/*TD high temperature will clear the backup RAM.*/
	hVbat.TD_Init.HighTemperatureMode = TD_HIGH_TEMPERATURE_ALARM_CLEAR_BAT_RAM_ENABLE;
	/*TD low temperature will clear the backup RAM.*/
	hVbat.TD_Init.LowTemperatureMode = TD_LOW_TEMPERATURE_ALARM_CLEAR_BAT_RAM_ENABLE;

	/*VD alarm set for timer overflow.*/
	hVbat.VD_Init.CheckMode = VBAT_TIME_OVERFLOW_CHECK;
	/*VD higt voltage alarm will clear the backup RAM.*/
	hVbat.VD_Init.HighAlarmMode = VD_HIGH_LEVEL_ALARM_CLEAR_BAT_RAM_ENABLE;
	/*VD low voltage alarm will clear the backup RAM.*/
	hVbat.VD_Init.LowAlarmMode = VD_LOW_LEVEL_ALARM_CLEAR_BAT_RAM_ENABLE;
	
	/*VD low voltage alarm threshold value is 2.2V.*/
	hVbat.VD_Init.LowLevelMode = VD_LOW_LEVEL_ALARM_LESS_2_2_V;
	
	TMC_VBAT_Init(&hVbat,&hVbat_fd3);
}	

/************************************************************************************************************************************
*	The function of VBAT_RTC_UseLSIToClockRTC_Test is when RTC used the LSI as clock,and used FD3 to detect the frequency of LSI is 
	in the alarm range or not,and count the LSE clock cycles by PLL_L or HSE,and compare with the value of 32768Hz.If it faster than 
	32768Hz then increase the second counter,or else reduce the second counter.
************************************************************************************************************************************/
void VBAT_RTC_UseLSIToClockRTC_Test(void)
{
	volatile uint32_t LowFreq,HighFreq,OffsetValue,PLL_L,temp;
	BAT_RTC_Cmd(DISABLE);
	/*Init the RCT date as 00:00:00, 2018.11.1.*/
//	BAT_RTC_StructInit(&hVbat.RTC_Init);
//	BAT_RTC_AlarmStructInit(&hVbat.RTC_AlarmInit);
	
	/*Set the clock source is LSI 32K for FD3 check.*/
	hVbat_fd3.FD3_Init.FD3_CheckClock = INSIDE_32K_CLOCK;
	/*Set the FD3 clock detecting clock is 32K clock,the reference source is PLL_L 48MHz.*/
	hVbat_fd3.FD3_Init.FD3_ReferenceClock = REFERENCE_CLOCK_PLL_L;
	hVbat_fd3.FD3_Init.FD3_Divsion = REFERENCE_CLOCK_DIVSION_NO;
	TMC_VBAT_MspInit(&hVbat);
	EFD3_Init(&hVbat_fd3.FD3_Init);

	hVbat_fd3.State = TMC_VBAT_STATE_READY;
	LowFreq = 32768;
	HighFreq = 32768;
	while(1)
	{
		TMC_VBAT_EFD3_Start_IT(&hVbat_fd3,LowFreq,HighFreq);
		EFD3_ConfigIT(FD3_FLAG_CHECK_OK,ENABLE);
		while(hVbat_fd3.State == TMC_VBAT_STATE_READY);
		/*Clear the alarm flag.*/
		hVbat_fd3.State = TMC_VBAT_STATE_READY;
		if(hVbat_fd3.ALARMCode &TMC_VBAT_ALARM_FD3_NOT_ALARM)
		{
			/*Get the value of reference clock.*/
			OffsetValue = EFD3_GetPER();
			if(hVbat_fd3.FD3_Init.FD3_ReferenceClock == REFERENCE_CLOCK_PLL_L)
			{
				PLL_L = RCC_GetPLL_L_Clock();
				temp = PLL_L/(OffsetValue+1);
				if(temp>32768)
				{
					temp -= 32768;
					/*The counter is faster than 32768.Enable the SEC increase.*/
					//temp = temp<<8;
					BAT_RTC_ConfigAdjustmentPerSec(temp,RTC_SEC_INCREASE);
				}
				else if(temp<32768)
				{
					temp = 32768 -temp;
					/*The counter is slower than 32768.Enable the SEC decrease.*/
					//temp = temp<<8;
					BAT_RTC_ConfigAdjustmentPerSec(temp,RTC_SEC_DECREASE);
				}
			
				BAT_RTC_Cmd(ENABLE);
				return;
				
			}
			
		}
		if(hVbat_fd3.ALARMCode&TMC_VBAT_ALARM_FD3_LOW_FREQ)
		{
			/*LSI frquency is less than 32K,low frequency alarm.*/
			LowFreq-=1;
		}
		if(hVbat_fd3.ALARMCode&TMC_VBAT_ALARM_FD3_HIGH_FREQ)
		{
			/*LSI frquency is more than 32K,high frequency alarm.*/
			HighFreq+=1;
		}
	}
}

/************************************************************************************************************************************
*	VBAT_RTC_UseLSEToClockRTC_Test1 function is detecting the LSI clock lower than the Low frequency alrm value when RTC used the LSE 
as clock.To calculate the LSE clock cycle,and judge the LSI is slower than the LOW frequency alrm value or faster than the HIGH frequency 
alarm value.Through comparing with the 32.768KHz,if faster than reduce the second time counter,or else increase the second time counter.
In VBAT init function only open the FD3 High&Low frequency alarm,didn't open the clock alrm.More information please refer the user mannal document.
*************************************************************************************************************************************/
void VBAT_RTC_UseLSEToClockRTC_Test(void)
{
	volatile uint32_t LowFreq,HighFreq,OffsetValue,PLL_L,temp;
	/*Init the RCT date as 00:00:00, 2018.11.1.*/
	BAT_RTC_StructInit(&hVbat.RTC_Init);
	BAT_RTC_AlarmStructInit(&hVbat.RTC_AlarmInit);
	
	/*Set the FD3 clock detecting clock is 32K clock,the reference source is LSE 12MHz.*/
	hVbat_fd3.FD3_Init.FD3_CheckClock = EXTERNAL_32K_CLOCK;
	/*Set the FD3 judge clock source as PLL_L(48MHz.It must less than 50MHz.)*/
	hVbat_fd3.FD3_Init.FD3_ReferenceClock = REFERENCE_CLOCK_PLL_L;
	hVbat_fd3.FD3_Init.FD3_Divsion = REFERENCE_CLOCK_DIVSION_NO;
	TMC_VBAT_MspInit(&hVbat);
	EFD3_Init(&hVbat_fd3.FD3_Init);

	hVbat_fd3.State = TMC_VBAT_STATE_READY;
	/*Set the High frequency and Low frequency alarm value are all 32.768K.*/
	LowFreq = 32768;
	HighFreq = 32768;
	while(1)
	{
		TMC_VBAT_EFD3_Start_IT(&hVbat_fd3,LowFreq,HighFreq);
		EFD3_ConfigIT(FD3_FLAG_CHECK_OK,ENABLE);
		while(hVbat_fd3.State == TMC_VBAT_STATE_READY);
		/*Clear the flag.*/
		hVbat_fd3.State = TMC_VBAT_STATE_READY;
		if(hVbat_fd3.ALARMCode &TMC_VBAT_ALARM_FD3_NOT_ALARM)
		{
			/*Get the counter of reference clock.*/
			OffsetValue = EFD3_GetPER();
			if(hVbat_fd3.FD3_Init.FD3_ReferenceClock == REFERENCE_CLOCK_PLL_L)
			{
				PLL_L = RCC_GetPLL_L_Clock();
				temp = PLL_L/(OffsetValue+1);
				if(temp>32768)
				{
					temp -= 32768;
					/*The counter is faster than 32768.Enable the SEC increase.*/
					//temp = temp<<8;
					BAT_RTC_ConfigAdjustmentPerSec(temp,RTC_SEC_INCREASE);
				}
				else if(temp<32768)
				{
					temp = 32768 -temp;
					/*The counter is slower than 32768.Enable the SEC decrease.*/
					//temp = temp<<8;
					BAT_RTC_ConfigAdjustmentPerSec(temp,RTC_SEC_DECREASE);
				}
				return;
			}
			
		}
		if(hVbat_fd3.ALARMCode&TMC_VBAT_ALARM_FD3_LOW_FREQ)
		{
			/*LSI frquency is less than 32K,low frequency alarm.*/
			LowFreq-=1;
		}
		if(hVbat_fd3.ALARMCode&TMC_VBAT_ALARM_FD3_HIGH_FREQ)
		{
			/*LSI frquency is more than 32K,high frequency alarm.*/
			HighFreq+=1;
		}
	}
}

uint32_t Temp[32]={ 0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,
					0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,
					0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,
					0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00,0x11223344,0x55667788,0x99AABBCC,0xDDEEFF00};
uint32_t Temp1[32]={0};

 /**
    * @brief TMC_VBAT_SD_Test:
                1.VBAT Sensor Init:Config SD0,SD1,SD4,SD5 to static SD,Config SD2+SD6,SD3+SD7 to dynamic SD
                2.Write array Temp to BackupRAM 
                3.Read BackupRAM to array Temp1
                4.Compare  Temp and Temp1, if they are unequal, then return
                5.Enable SD static and dynamic alarm interrupt, Start SD
                6.Wait for alarm
                7.When alrming, read BackupRAM to array Temp1, if they are equal, then while(1)
    * @param  None
    * @retval None
 */
void TMC_VBAT_SD_Test(void)
{
	VBAT_SENSOR_ExampleOfInitial();
	/*Test wether the VBAT Sensor alarm will clear the Backup RAM.*/
	BAT_WriteRam(Temp);
	BAT_ReadRam(Temp1);
	if(TMC_Compare((uint8_t*)Temp,(uint8_t*)Temp1,128) == FALSE)
	{
		return;
	}
	TMC_VBAT_SD_Start_IT(&hVbat);
	
	/*Wait for alarm.*/
	while(hVbat.State == TMC_VBAT_STATE_READY);
	if(hVbat.State == TMC_VBAT_STATE_ALARM)
	{
		BAT_ReadRam(Temp1);
        /* Watch hVbat.ALARMCode to confirm alarm is OK.
		   Watch the value of TMC_ALARM_Flag is '1' or not, and wether entered the callback function. */
		if(TMC_Compare((uint8_t*)Temp,(uint8_t*)Temp1,128) == TRUE)
		{
			while(1);
		}
	}
}
/*************************
* TD test
*************************/
void TMC_VBAT_TD_Test(void)
{
	VBAT_SENSOR_ExampleOfInitial();
	BAT_WriteRam(Temp);
	BAT_ReadRam(Temp1);
	if(TMC_Compare((uint8_t*)Temp,(uint8_t*)Temp1,128) == FALSE)
	{
		return;
	}
	TMC_VBAT_TD_Start_IT(&hVbat);

	/*Wait for alarm.*/
	while(hVbat.State == TMC_VBAT_STATE_READY);
	if((hVbat.State == TMC_VBAT_STATE_ALARM)&&((hVbat.ALARMCode&TMC_VBAT_ALARM_TD_LOW_TEMPERATURE)||(hVbat.ALARMCode&TMC_VBAT_ALARM_TD_HIGH_TEMPERATURE)))
	{
		BAT_ReadRam(Temp1);
		/* Watch hVbat.ALARMCode to confirm alarm is OK.*/
		if(TMC_Compare((uint8_t*)Temp,(uint8_t*)Temp1,32) == TRUE)
		{
			while(1);
		}
	}
	else
	{
		while(1);
	}
	
}
/*************************
* VD test
*************************/
void TMC_VBAT_VD_Test(void)
{
	VBAT_SENSOR_ExampleOfInitial();
	BAT_WriteRam(Temp);
	BAT_ReadRam(Temp1);
	if(TMC_Compare((uint8_t*)Temp,(uint8_t*)Temp1,128) == FALSE)
	{
		return;
	}
	TMC_VBAT_VD_Start_IT(&hVbat);
	/*Wait for alarm.*/
	while(hVbat.State == TMC_VBAT_STATE_READY);
	if((hVbat.State == TMC_VBAT_STATE_ALARM)&&((hVbat.ALARMCode&TMC_VBAT_ALARM_VD_LOW_LEVEL)||(hVbat.ALARMCode&TMC_VBAT_ALARM_VD_HIGH_LEVEL)))
	{
		BAT_ReadRam(Temp1);
        /* Watch hVbat.ALARMCode to confirm alarm is OK.*/
		if(TMC_Compare((uint8_t*)Temp,(uint8_t*)Temp1,32) == TRUE)
		{
			while(1);
		}
	}
	
}

 /**
    * @brief TMC_VBAT_IWDT_Test:
                1.Set the IWDT clock source is LSI, PREDIV equal 32, initial value equal 5000, counting time is 5s and not reset chip 
                2.Load the initial value to the counter and start IWDT
                3.Enable IWDT interrupt
                4.Gradually increasing feed dog timer until greater than 5s
                5.Confirm IWDT alarm and stop IWDT
    * @param  None
    * @retval None
 */
void TMC_VBAT_IWDT_Test(void)
{
	volatile uint32_t i ,j,inc;
	VBAT_SENSOR_ExampleOfInitial();
	TMC_VBAT_IWDT_Start_IT(&hVbat);
	inc = 1;
	/*This test to increase the interval time of feed watch dog*/
	while(hVbat.State == TMC_VBAT_STATE_READY)  
	{
		i = TMC_GetTick();
		while(j<(i+inc))
		{
			j = TMC_GetTick();
		}
		/*Time out.*/
		if(hVbat.ALARMCode&TMC_VBAT_ALARM_IWDT)
		{
			BAT_IWDT_DeInit();
		}
		BAT_IWDT_FeedDog();/*Feed dog.*/
		inc++;
	}
	/*Time out.*/
	if(hVbat.ALARMCode&TMC_VBAT_ALARM_IWDT)
	{
		BAT_IWDT_DeInit();
	}
	
}

 /**
    * @brief TMC_VBAT_RTC_Alarm_Test:
                1.Init the RTC date as 00:00:00, 2018.11.1 
                2.Set the RTC for 1 minute to generate the alarm and the interruption
                3.Using LSI as the RTC clock source
                4.Using fd3 to calibrate the LSI and adjust the RTC benchmark clock in real time
                5.Enable alarm interrupt, Start RTC
                6.Wait for alarm
    * @param  None
    * @retval None
 */
void TMC_VBAT_RTC_Alarm_Test(void)
{
	//
	//VBAT_RTC_UseLSEToClockRTC_Test();
	
	VBAT_SENSOR_ExampleOfInitial();
	VBAT_RTC_UseLSIToClockRTC_Test();
	/*Enable the interrupt of EXTI.*/
	EXTI_ConfigOtherMode(EXTI_RTC_ALM_WAKE_UP,ENABLE);
	TMC_VBAT_RTC_Start_IT(&hVbat);
	while(hVbat.State == TMC_VBAT_STATE_READY);
	if(hVbat.ALARMCode&TMC_VBAT_ALARM_RTC)
	{
		BAT_RTC_DeInit();
	}
	
}


void TMC_VBAT_IWDT_AlarmCpltCallback(VBAT_HandleTypeDef *hvbat)
{
	/*Set a ararm flag.*/
	TMC_ALARM_Flag = 1;
}


void TMC_VBAT_SensorAlarmCpltCallback(VBAT_HandleTypeDef *hvbat)
{
	/*Set a ararm flag.*/
	TMC_ALARM_Flag = 1;
//	if((hvbat->ALARMCode&TMC_VBAT_ALARM_FD3_LOW_FREQ)||(hvbat->ALARMCode&TMC_VBAT_ALARM_FD3_HIGH_FREQ))
//	{
//		TMC_VBAT_EFD3_Stop_IT(hvbat);
//	}
	if((hvbat->ALARMCode&TMC_VBAT_ALARM_VD_LOW_LEVEL)||(hvbat->ALARMCode&TMC_VBAT_ALARM_VD_HIGH_LEVEL))
	{
		TMC_VBAT_VD_Stop_IT(hvbat);
	}
	if((hvbat->ALARMCode&TMC_VBAT_ALARM_TD_LOW_TEMPERATURE)||(hvbat->ALARMCode&TMC_VBAT_ALARM_TD_HIGH_TEMPERATURE))
	{
		TMC_VBAT_TD_Stop_IT(hvbat);
	}
	if((hvbat->ALARMCode&TMC_VBAT_ALARM_SD_0)||(hvbat->ALARMCode&TMC_VBAT_ALARM_SD_1)||(hvbat->ALARMCode&TMC_VBAT_ALARM_SD_2)|| \
	   (hvbat->ALARMCode&TMC_VBAT_ALARM_SD_3)||(hvbat->ALARMCode&TMC_VBAT_ALARM_SD_4)||(hvbat->ALARMCode&TMC_VBAT_ALARM_SD_5))
	{
		TMC_VBAT_SD_Stop_IT(hvbat);
	}
	if(hvbat->ALARMCode&TMC_VBAT_ALARM_IWDT)
	{
		TMC_VBAT_IWDT_Stop_IT(hvbat);
	}
	if(hvbat->ALARMCode&TMC_VBAT_ALARM_RTC)
	{
		TMC_VBAT_IWDT_Stop_IT(hvbat);
	}
}
//TMC(20240408)-zt:split VBAT_FD3AlarmCpltCallback from VBAT_SensorAlarmCpltCallback
void TMC_VBAT_FD3AlarmCpltCallback(VBAT_FD3_HandleTypeDef *hvbat_fd3)
{
	/*Set a ararm flag.*/
	TMC_ALARM_Flag = 1;
	if((hvbat_fd3->ALARMCode&TMC_VBAT_ALARM_FD3_LOW_FREQ)||(hvbat_fd3->ALARMCode&TMC_VBAT_ALARM_FD3_HIGH_FREQ))
	{
		TMC_VBAT_EFD3_Stop_IT(hvbat_fd3);
	}
}

void TMC_VBAT_MspInit(VBAT_HandleTypeDef *hvbat)
{
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	/*Enable the FD clock.*/
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_FrequencyDerector, ENABLE);
	/*Enable PWR clock.*/
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_PWRC,ENABLE);
	/*Enable the EXTI clock.*/
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_INT,ENABLE);
	/*Open the BAT write enable.*/
	BAT_WriteEnable(ENABLE);
	/*Enable the LSI clock.*/
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
	

//	/*Enable LSE clock.*/
//	RCC_LSECmd(ENABLE, ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
//	delay(30000);
	
	/*Set the battery clock as LSI.*/
	RCC_BATClockConfig(RCC_BAT_CLKSRC_LSI,RCC_BAT_CLKDIV_0);
	/*Set the RTC clock as LSE.*/
	//RCC_RTCClockConfig(RCC_RTC_CLKSRC_LSE,RCC_BAT_CLKDIV_0);

	/*Set RTC clock as LSI.*/
	RCC_RTCClockConfig(RCC_RTC_CLKSRC_LSI,ENABLE);
	
	/*Open sensor clock.*/
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_VBSENSOR,ENABLE);
	
	/*Enable the VB sensor clock.*/
	RCC_BATSecurityClockCmd(RCC_BATS_SENSOR,ENABLE);
	/*Enable the IWDT clock.*/
	RCC_BATSecurityClockCmd(RCC_BATS_IWDT,ENABLE);	
	RCC_Security1PeriphClockCmd(RCC_SEC1Periph_INT,ENABLE);
	
	NVIC_ClearPendingIRQ(FD3_IRQn);
	NVIC_EnableIRQ(FD3_IRQn);
	
	NVIC_ClearPendingIRQ(VB_SENSOR_IRQn);
	NVIC_EnableIRQ(VB_SENSOR_IRQn);
	/*EXTI RTC interrupt.*/
	NVIC_ClearPendingIRQ(EXTI81_IRQn);
	NVIC_EnableIRQ(EXTI81_IRQn);
	NVIC_ClearPendingIRQ(EXTI82_IRQn);
	NVIC_EnableIRQ(EXTI82_IRQn);
	
	//TMC_VBAT_SD_Test();
}

 /**
    * @brief VBAT Sensor Test Including:
                1. VBAT_RTC_Demo_Test
                2. VBAT_SD_Demo_Test
                3. VBAT_IWDT_Demo_Test
                4. VBAT_VD_Demo_Test
                5. VBAT_TD_Demo_Test
    * @param  None
    * @retval None
 */
void VBAT_Test(void)
{
	VBAT_Demo_TypefDef Test;
	
	Test = VBAT_RTC_Demo_Test;
	
	if(Test == VBAT_RTC_Demo_Test)
	{
		/* If RTC function use the LSI clock, please use the 
		function 'VBAT_RTC_UseLSIToClockRTTC_Test' to judge the LSI clock to 32.768KHz.
		The RTC Time is 2018 year November 11th 1 minute 0 second.*/
		TMC_VBAT_RTC_Alarm_Test();
	}
	if(Test == VBAT_SD_Demo_Test)
	{
		TMC_VBAT_SD_Test();
	}
	if(Test == VBAT_IWDT_Demo_Test)
	{
		TMC_VBAT_IWDT_Test();
	}
	if(Test == VBAT_VD_Demo_Test)
	{
		TMC_VBAT_VD_Test();
	}
	if(Test == VBAT_TD_Demo_Test)
	{
		TMC_VBAT_TD_Test();
	}
}

 /**
    * @brief  VBAT_DemoMain
    * @param  None
    * @retval None
 */
void VBAT_DemoMain(void)
{
	VBAT_Test();
}



