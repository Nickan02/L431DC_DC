

#include "PID.H"

// Error Element = {0, 0, 0};		//储存上次偏差值

// PID_InitTypeDef Voltage_Increase= {13.904,0,0,0,0,0,0};//14.2,0.0005,140.1,7.4

static float PID_Control(PID_InitTypeDef *Control_pid, float Input, float Point)
{
    float Target_Error,
        Realize;

    Target_Error = Point - Input;                           //计算偏差
    Control_pid->Integral += Control_pid->I * Target_Error; //求出偏差的积分

    if (Control_pid->Integral > Control_pid->limit)
        Control_pid->Integral = Control_pid->Integral;
    else
        Control_pid->Integral = -Control_pid->Integral;

    Realize = Control_pid->P * Target_Error +                                // P
              Control_pid->Integral +                                        // I
              Control_pid->D * (Target_Error - Control_pid->Previous_Error); // D
                                                                             // P*偏差积分 +I*偏差积分 +D * （偏差 - 上一次偏差）
    Control_pid->Previous_Error = Target_Error; //保存上一次偏差
    return Realize;                             //输出
}

//															输入值						限幅最大最小值									输出值							目标值

uint16_t PID_Error_Deal(double Average_Vol, uint16_t MAX, uint16_t MIN, uint16_t Output_Duty, float Target_Voltage, PID_InitTypeDef *PID_Parameter)
{
    //    static uint16_t	Output_Duty = 500;
    double Error_Voltage,
        Now_Voltage;
    Error_Voltage = Target_Voltage - Average_Vol;
    Now_Voltage = PID_Control(PID_Parameter, Error_Voltage, 0);
    Output_Duty = Output_Duty - Now_Voltage; /*	BUCK	*/
    if (Output_Duty < MIN)
    {
        Output_Duty = MIN;
    }
    else if (Output_Duty > MAX)
    {
        Output_Duty = MAX;
    }
    return Output_Duty;
}