/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mag_drv.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef _MAG_DRV_H_
#define _MAG_DRV_H_

#include <stdint.h>

#define MAX_PEAK_NUM   1200
#define TRACK1_CHARACTER_LENGTH 79
#define TRACK2_CHARACTER_LENGTH 40
#define TRACK3_CHARACTER_LENGTH 107

typedef struct
{
	uint8_t		Chip;			/*THM36 - 0, THM36S - 1 */
	uint8_t		Divsion;		/*ADC时钟分频系数*/
	uint16_t  	Buad;			/*ADC采样周期*/
	uint8_t		AMP_cm;			/*放大器中心电平与电源电压的比值*/
	uint8_t		AMP_adj;		/*放大器放大倍数*/
	uint8_t		Threshold;		/*极值搜索阈值*/
	uint8_t		Autothen;		/*使能阈值自适应，0-不使能，1-使能*/
	uint8_t	 Peelen;			/*使能削顶削底功能，0-不使能，1-使能*/
	uint8_t	 Peelth;			/*削顶削底阈值*/
	uint8_t	 Fileren;		/*使能数字滤波功能，0-不使能，1-使能*/
	uint8_t	 Filercfg;		/*数字滤波参数设置，0-连续2个数据平均，1-连续3个数据平均，2-连续4个数据平均，3-连续5个数据平均*/
} MAG_InitTypeDef;

typedef struct
{
	uint8_t track1_buf[TRACK1_CHARACTER_LENGTH];		 /*磁道1解码数据*/

	uint8_t track2_buf[TRACK2_CHARACTER_LENGTH];		 /*磁道2解码数据*/

	uint8_t track3_buf[TRACK3_CHARACTER_LENGTH];		 /*磁道3解码数据*/

} MAG_DecodeTypeDef;

typedef struct
{
	uint32_t *channel1_ptr;		 /*通道1波峰波谷和时间戳数据*/

	uint32_t *channel2_ptr;		 /*通道2波峰波谷和时间戳数据*/

	uint32_t *channel3_ptr;		 /*通道3波峰波谷和时间戳数据*/

} MAG_TimeStampDef;

void MAG_Init(MAG_InitTypeDef mag_init);  /*磁条卡初始化*/
uint8_t MAG_SwipeDetect(void);			/*磁条卡检测刷卡*/
uint8_t MAG_ReadTimeStamp(MAG_TimeStampDef *time_data);/*磁条卡读时间戳*/
uint8_t MAG_DecodeTrack(MAG_DecodeTypeDef *mag_data);  /*磁条卡解码卡号*/


#endif /* MAG_DRV_H_ */

/******************* (C) COPYRIGHT 2020 TMC company *****END OF FILE****/
