//
// Created by uoc11 on 2023/12/4.
//
#include "bsp_spwm.h"
#include "math.h"

float UVPhase      = PI_X2_DIV3;
float UWPhase      = -PI_X2_DIV3;
float Modulation   = 400;     //载波比 载波/调制波
float Amplitude    = 0;       //调制度，调制波幅值/载波幅值
float Pi_divN      = 0;
float  Volt_Freq   = 0.2;     //V/F 控制量
int32_t Accel      = 1;       //加速度


void config_Sinusoidal( float Fre)
{
    float Amp;
    if(Fre > 0.0f)
    {
        Amp = Fre*Volt_Freq;
        if(Amp>24.0f)
            Amp = 24.0f;

        Amplitude  = Amp/24.0f;  // <0 Amplitude  < 1;
        Modulation = CARRIER_FREQ_Hz/Fre;
        Pi_divN    = PI_X2/Modulation;

        UVPhase = +PI_X2_DIV3;
        UWPhase = -PI_X2_DIV3;
    }
    else if( Fre < 0.0f )
    {
        Fre = -Fre;
        Amp = Fre*Volt_Freq;
        if(Amp>24.0f)
            Amp = 24.0f;

        Amplitude  = Amp/24.0f;
        Modulation = CARRIER_FREQ_Hz/Fre;
        Pi_divN    = PI_X2/Modulation;
        UVPhase = -PI_X2_DIV3;
        UWPhase = +PI_X2_DIV3;
    }
    else
    {
        Modulation = 0;
        Pi_divN    = 0;
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    uint16_t CCR1  = 0;
    uint16_t CCR2  = 0;
    uint16_t CCR3  = 0;
    static float i = 0;

    float tmp = Pi_divN*i;

    CCR1 = TIM_HALF_PERIOD*( Amplitude + Amplitude*sinf( tmp) );
    CCR2 = TIM_HALF_PERIOD*( Amplitude + Amplitude*sinf( tmp + UVPhase) );
    CCR3 = TIM_HALF_PERIOD*( Amplitude + Amplitude*sinf( tmp + UWPhase) );

    __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, CCR1);
    __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, CCR2);
    __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, CCR3);
    i++;
    if(i >= Modulation)
        i=0;
}


void HAL_SYSTICK_Callback()
{
    static uint32_t i = 0;
    i++;
    if( i>=10000 )
    {
        i = 0;
//    Accel = -Accel;
       Accel = 0;
    }
}