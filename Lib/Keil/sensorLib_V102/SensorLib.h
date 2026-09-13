/**
  ******************************************************************************
  * @file	SENSOR.h
  * @author  TMC Team
  * @version V3.1.0
  * @date	06/19/2009
  * @brief   This file contains all the functions of Sensors.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HERE IN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSOR_H_
#define __SENSOR_H_

#ifdef __cplusplus
 extern "C" {
#endif

	 

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

/*参考时钟分频系数*/
typedef enum
{
	REFERENCE_CLOCK_DIV_NO = 0,
	REFERENCE_CLOCK_DIV_2 = 1,
	REFERENCE_CLOCK_DIV_4 = 2,
	REFERENCE_CLOCK_DIV_8 = 3,
} REFCLK_Div_Typedef;

/*待测时钟源*/
typedef enum
{
	LSE = 0,
	LSI = 1,
} TSCLK_Sel_Typedef;

/*参考时钟源选择*/
typedef enum
{
	PLL_L = 0,
	HSE = 1,
} REFCLK_Sel_Typedef;

typedef struct 
{
	REFCLK_Div_Typedef REFCLKDIV;
	TSCLK_Sel_Typedef TSCLKSEL;
	REFCLK_Sel_Typedef REFCLKSEL;
	FunctionalState HFDEN;
	FunctionalState LFDEN;
	
} FD3_InitTypeDef;

/*FD3状态寄存器标志位*/
typedef enum
{
	FD3_FLAG_LOW_ALM = 1,        //低频报警状态，1报警，0未报警
	FD3_FLAG_HIGH_ALM = 2,       //高频报警状态，1报警，0未报警
	FD3_FLAG_CHK_OK = 4,         //时钟开启状态，该位为1为已开启且频率在检测范围内，0为未开启或者频率不正确。
	FD3_FLAG_REDUNDANCY_CHK_ALM = 16,    //寄存器冗余报警
} FD3_Sts_Typedef;

/*FD3中断寄存器标志位*/
typedef enum
{
	FD3_INT_LOW_ALM = 1,        //低频报警中断
	FD3_INT_HIGH_ALM = 2,       //高频报警中断
	FD3_INT_READY = 4,          //时钟准备就绪中断
} FD3_Int_Typedef;


/*FD2状态寄存器标志位*/
typedef enum
{
	FD2_FLAG_LOW_ALM = 1,            //低频报警状态，1报警，0未报警
	FD2_FLAG_HIGH_ALM = 2,           //高频报警状态，1报警，0未报警
} FD2_Sts_Typedef;

/*FD2状态寄存器标志位*/
typedef enum
{
	FD2_INT_LOW_ALM = 1,            //低频报警中断
	FD2_INT_HIGH_ALM = 2,           //高频报警中断
} FD2_Int_Typedef;

typedef enum
{
	VBATVD_FLAG_LOW_ALM = 1,         //低压报警状态，1报警，0未报警
	VBATVD_FLAG_HIGH_ALM = 2,        //高压报警状态，1报警，0未报警
}
VBATVD_Sts_Typedef;  

typedef enum
{
	VBATTD_FLAG_LOW_ALM = 1,         //低温报警状态，1报警，0未报警
	VBATTD_FLAG_HIGH_ALM = 2,        //高温报警状态，1报警，0未报警
}
VBATTD_Sts_Typedef;

typedef enum
{
	VBATTD_INT_LOW_ALM = 1,         //低温报警中断
	VBATTD_INT_HIGH_ALM = 2,        //高温报警中断
}
VBATTD_Int_Typedef;

typedef enum
{
	VBATVD_INT_LOW_ALM = 1,         //低温报警中断
	VBATVD_INT_HIGH_ALM = 2,        //高温报警中断
}
VBATVD_Int_Typedef;




void _GetSensorLibVersion(unsigned char *Version);

/*******************************LD********************************/
void _LD_Cmd(FunctionalState NewState);

/******************************FD2********************************/
void _FD2_HighFreConfigValue(unsigned long HighFreFactor);
void _FD2_LowFreConfigValue(unsigned long LowFreFactor);
void _FD2_LowFreDetect_CMD(FunctionalState NewState);
void _FD2_HighFreDetect_CMD(FunctionalState NewState);
FlagStatus _FD2_GetStatus(FD2_Sts_Typedef Flags);
void _FD2_ClearStatus(FD2_Sts_Typedef Flags);
void _FD2_ConfigIT(FD2_Int_Typedef Flags, FunctionalState NewState);

/******************************FD3********************************/
void _FD3_Init(FD3_InitTypeDef *FD3_InitStrct);
void _FD3_HighFreConfigValue(unsigned long HighFreFactor);
void _FD3_LowFreConfigValue(unsigned long LowFreFactor);
FlagStatus _FD3_GetStatus(FD3_Sts_Typedef Flags);
void _FD3_ClearStatus(FD3_Sts_Typedef Flags);
void _FD3_ConfigIT(FD3_Int_Typedef Flags, FunctionalState NewState);

/******************************ASH********************************/
void _ASH_ConfigIT(FunctionalState NewState);
FlagStatus _ASH_GetStatus(void);
void _ASH_ConfigIT(FunctionalState NewState);
void _ASH_Cmd(FunctionalState NewState);
/******************************EGS********************************/
void _EGS_ConfigIT(FunctionalState NewState);
FlagStatus _EGS_GetStatus(void);
void _EGS_ClearStatus(void);

/******************************VBATTD*****************************/
void _VBATTD_ClearStatus(VBATTD_Sts_Typedef Flags);
FlagStatus _VBATTD_GetStatus(VBATTD_Sts_Typedef Flags);
void _VBATTD_ConfigIT(VBATTD_Int_Typedef Flags, FunctionalState NewState);
void _VBATTD_Cmd(VBATTD_Sts_Typedef Flags, FunctionalState NewState);

/******************************VBATVD*****************************/
void _VBATVD_ClearStatus(VBATVD_Sts_Typedef Flags);
FlagStatus _VBATVD_GetStatus(VBATVD_Sts_Typedef Flags);
void _VBATVD_ConfigIT(VBATVD_Int_Typedef Flags, FunctionalState NewState);
void _VBATVD_Cmd(VBATVD_Sts_Typedef Flags, FunctionalState NewState);

/******************************VBAT WP****************************/
void PWR_VB_WriteProtect(FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif   //_SENSOR_H_

/************************ (C) COPYRIGHT TMC *****END OF FILE***********************/



