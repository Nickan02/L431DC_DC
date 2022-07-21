
#ifndef __Filter_H_
#define __Filter_H_
#include "math.h"
#define Contains_Symbols 0 //当返回值有符号时为1 无符号为0(默认为0)
#if Contains_Symbols  
typedef signed char Actual_Out;     
#else  
typedef unsigned int Actual_Out; 
#endif

Actual_Out Clipping_filter(unsigned int Amplitude,int get_ad);//限幅滤波Amplitude值根据实际调
Actual_Out Debounce_filtering(unsigned int N,int get_ad);//消抖滤波
Actual_Out Limiting_Debounce_Filtering(unsigned int N,int A,int get_ad);//限幅消抖滤波
Actual_Out Median_filter(unsigned int N,int get_ad);//中位值滤波
Actual_Out Arithmetic_Average_Filtering(unsigned int N,int get_ad);//算数平均滤波
Actual_Out Recursive_averaging_filter(unsigned int A,int get_ad);//递推平均滤波
Actual_Out Median_Average_Filtering(unsigned int N,int get_ad);//中位值平均滤波
Actual_Out First_order_lag_filtering(float A,int get_ad);//一阶滞后滤波
Actual_Out First_order_lag_filtering1(float A,int get_ad);//一阶滞后滤波
Actual_Out First_order_lag_filtering2(float A,int get_ad);//一阶滞后滤波
Actual_Out Weighted_recursive_average_filtering(unsigned int N,int get_ad);//加权递推平均滤波
int ABS(int n);//取绝对值函数


#define FFT_N 16                                        //定义傅里叶变换的点数
#define PI 3.1415926                                    //定义圆周率值
struct compx { double real, imag; };                    //定义一个复数结构
extern struct compx Compx[];							//FFT输入和输出：从Compx[0]开始存放，根据大小自己定义
extern double SIN_TAB[];								//正弦信号表
extern void Refresh_Data(struct compx *xin, int id, double wave_data);
extern void create_sin_tab(double *sin_t);
extern void FFT(struct compx *xin);
extern void Get_Result(struct compx *xin, double sample_frequency);

#endif

