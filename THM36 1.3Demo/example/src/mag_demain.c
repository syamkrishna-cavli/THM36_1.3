/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mag_demain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
 
#include "thm36_1.3.h"
#include "rcc.h"
#include "adc.h"
#include "mag_drv.h"

uint8_t g_mag_run;
MAG_DecodeTypeDef g_mag_track;
uint8_t MAG_DemoMain(void)
{
	uint8_t mag_result;
	MAG_InitTypeDef mag_init_para;
	MAG_TimeStampDef mag_time_stamp;

	RCC_AHBClockCmd(RCC_AHBPeriph_DMA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);
	RCC_ADCClockConfig(RCC_ADC_CLKSRC_PLL_L, ENABLE);

		mag_init_para.Chip	= 0;   //THM36 1.3
		mag_init_para.Divsion = 1;   //ADC devision.
		mag_init_para.Buad	= 30;  //ADC sample cycle.
		mag_init_para.AMP_cm  = ADC_AMP_CENTRAL_LEVEL_50_PERCENT;  //ADC center level.
		mag_init_para.AMP_adj = ADC_AMP_MAG_44;  //ADC magnification times.
		mag_init_para.Threshold = 0x15;
		mag_init_para.Autothen  = 0;
		mag_init_para.Peelen	= 1;  //Enable peel function.
		mag_init_para.Peelth	= 0xff;
		mag_init_para.Fileren   = 1;  //Enable digital filtering.
		mag_init_para.Filercfg  = 3;
		MAG_Init(mag_init_para);  //Initialization.
	
	g_mag_run = 1;
	while(g_mag_run)
	{
		if(MAG_SwipeDetect())		  //Swipe detecting.
		{
			MAG_ReadTimeStamp(&mag_time_stamp);  
			memset(g_mag_track.track1_buf, 0 , sizeof(g_mag_track.track1_buf));
			memset(g_mag_track.track2_buf, 0 , sizeof(g_mag_track.track2_buf));
			memset(g_mag_track.track3_buf, 0 , sizeof(g_mag_track.track3_buf));
			mag_result = MAG_DecodeTrack(&g_mag_track);  //Decode.

			if(mag_result)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

