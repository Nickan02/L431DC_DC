/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPWM_H
#define __SPWM_H
#include "stdint.h"
#include "main.h"
#include "tim.h"
extern TIM_HandleTypeDef htim1;
#define doublepi 								6.283185307   		// 2pi
#define Time_CLK 								(80000000/(Prescal+1))			// 最好能被80MHz整除,168/8 = 21MHz;
#define N												512
#define Sin_F										50		// 500Hz的SPWM波-》500hz正弦波
#define Prescal             	  (4-1)
// extern float n1_Temp,n2_Temp;


// float n1_RET(float AN_RET);
// float n2_RET(float M,float AN_RET);
void SinWave(void);
uint16_t TIM1_Per_Set(float Sin_FRQ);
//
/*静态函数，只在SPWM.C调用*/
static void MX_TIM1_Init(void);
static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
#endif /* __SPWM_H */


