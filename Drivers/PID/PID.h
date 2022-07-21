
#ifndef __PID_H_
#define __PID_H_
#include "main.h"

/*
P:比例度(即比例带)越大，控制器的放大倍数越小，被控参数的曲线越平稳；
			比例度越小，控制器的放大倍数越大，被控参数的曲线越波动。

I:调整积分作用的实质就是改变控制器输出的变化速率，
这个速率是通过积分作用的输出等于比例作用的输出所需的一段时问来衡量的。
积分时间小，表示积分速度大，积分作用就强；反之，积分时问大，则积分作用就弱。
如果积分时间无穷大，表示没有积分作用，控制器就成为纯比例控制器。


D:当输入阶跃信号后，微分器一开始输出的最大变化值与微分作用消失后的输出变化的比值就是微分放大倍数Kd，
即微分增益，微分増益的单位是时间，设置微分时间(或者微分增益)为零会取消微分的功能。



*/




typedef struct 
{ 
		float P;//比例	
		float	I;		//积分
		float	D;		//微分			
		float	limit;
		float Integral;				//积分
		float Last_Error;		//储存上次的未达到目标的值
		float Previous_Error;		//上一次的误差
}PID_InitTypeDef;

//typedef struct
//{
//  float Integral;				//积分
//  float Last_Error;		//储存上次的未达到目标的值
//  float Previous_Error;		//上一次的误差
//}Error;


//float PID_Control(Error *Element_sptr, PID *Control_pid, float Input, float Point);
uint16_t PID_Error_Deal(double Average_Vol,uint16_t MAX,uint16_t MIN,uint16_t	Output_Duty,float Target_Voltage,PID_InitTypeDef *PID_Parameter);
#endif

