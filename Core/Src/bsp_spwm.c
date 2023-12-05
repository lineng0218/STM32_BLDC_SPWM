//
// Created by uoc11 on 2023/12/4.
//
#include "bsp_spwm.h"
#include "math.h"

float  Volt_Freq   = 0.2;     //V/F 控制量
int32_t Accel      = 1;       //加速度

extern  int32_t  SamplePoint;
extern  uint8_t SinTable[256];
int32_t  SinAmp   = 24;    // 输出正弦波得幅值控制变量
int32_t  HalfMax  = 0x0834;  // 定时器周期一半
int16_t  Sin1Dir  = 1;       // 正弦波1正负半轴控制
int16_t  Sin2Dir  = 1;       // 正弦波2正负半轴控制
int16_t  Sin3Dir  = -1;      // 正弦波3正负半轴控制

/** 电机方向控制变量*/
__IO uint16_t sin1TableIndex = 0;
__IO uint16_t sin2TableIndex = 170;  // 相位差120°，一个正弦周期512个数据点，512/3=170.6
__IO uint16_t sin3TableIndex = 340;  // 相位差120°，一个正弦周期512个数据点，341=（170.6*2）
MotorDir_Typedef MotorDir = CW;      // 电机方向

/** 调整正弦波幅值，正弦波幅值A=(SinAmp/1024)*sinx */
void TuneSinAmp( uint16_t Amplitude)
{
    if( Amplitude >= 1024)
        SinAmp = 1024;
    else
        SinAmp = Amplitude;
}
/**调整正弦波频率，查表法只能通过改变载波频率来调整频率 */
void TuneSinFreq( float Frequency)
{
    uint32_t tmpARR = 0;
    float TIM_SrcFreq  = 1.68e8;
    float tmpPSC = 0 ;
    //每个脉冲就是一个数据点，一个周期512个数据点
    float tmpPulseFreq = Frequency*SamplePoint*2;
    if(Frequency == 0 )
        return ;
    do
    {
        tmpARR = TIM_SrcFreq/(tmpPSC+1.0f)/tmpPulseFreq;
        tmpARR>>=1;
        tmpPSC++;
        if( tmpPSC>65535 )
        {
            return;
        }
    }while( tmpARR>65535);
    __HAL_TIM_SET_PRESCALER (&motor1_htimx_bldcm, tmpPSC-1);
    __HAL_TIM_SET_AUTORELOAD(&motor1_htimx_bldcm, tmpARR);
    HalfMax = tmpARR>>1;
}

/** 设置电机方向*/
void set_MotorDir(MotorDir_Typedef Dir)
{
    uint16_t tmp = sin1TableIndex;
    if( Dir == CCW )
    {
        sin2TableIndex = (tmp+171)%(2*SamplePoint);         //相位差120°，正弦表数据为差171
        sin3TableIndex = (sin2TableIndex+171)%(2*SamplePoint);// 相位差120°，正弦表数据为差341
    }
    else
    {
        sin3TableIndex = (tmp+171)%(2*SamplePoint);
        sin2TableIndex = (sin3TableIndex+171)%(2*SamplePoint);
    }
}

/** 设置调制波波形频率*/
void config_Sinusoidal( float Fre)
{
    uint16_t Amp;

    if( Fre>0 )
    {
        TuneSinFreq(Fre);
        Amp = Fre*Volt_Freq;
        if(Amp>24)
            Amp = 24;
        set_MotorDir(CW);
    }
    else if(Fre<0)
    {
        Fre = -Fre;
        TuneSinFreq(Fre);
        Amp = Fre*Volt_Freq;
        if(Amp>24)
            Amp = 24;
        set_MotorDir(CCW);
    }else
    {
        Amp = 0;
    }
    SinAmp = Amp ;
}
/**正弦函数映射，放大n倍 */
static uint16_t map(uint16_t sinx, uint16_t out_max)
{
    return sinx * out_max/SIN_AMPMAX;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//  LED2_ON;
    int32_t CCR1 = 0;
    int32_t CCR2 = 0;
    int32_t CCR3 = 0;

    CCR1 = map(SinTable[sin1TableIndex&0xFF], HalfMax);
    CCR1 = HalfMax + (Sin1Dir * CCR1);
    CCR1 = (SinAmp*CCR1)/24;

    CCR2 = map(SinTable[sin2TableIndex&0xFF], HalfMax);
    CCR2 = HalfMax + (Sin2Dir * CCR2);
    CCR2 = (SinAmp*CCR2)/24;

    CCR3 = map(SinTable[sin3TableIndex&0xFF], HalfMax);
    CCR3 = HalfMax + (Sin3Dir * CCR3);
    CCR3 = (SinAmp*CCR3)/24;

            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, (uint16_t)CCR1);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, (uint16_t)CCR2);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, (uint16_t)CCR3);

    sin1TableIndex ++;
    sin2TableIndex ++;
    sin3TableIndex ++;

    /* Õý¸º°ëÖáÇÐ»» */
    if( sin1TableIndex >= SamplePoint ) Sin1Dir = -1;
    else Sin1Dir = 1;
    if( sin2TableIndex >= SamplePoint ) Sin2Dir = -1;
    else Sin2Dir = 1;
    if( sin3TableIndex >= SamplePoint ) Sin3Dir = -1;
    else Sin3Dir = 1;

    if( sin1TableIndex >= 2*SamplePoint)
        sin1TableIndex = 0;
    if( sin2TableIndex >= 2*SamplePoint)
        sin2TableIndex = 0;
    if( sin3TableIndex >= 2*SamplePoint)
        sin3TableIndex = 0;
//  LED2_OFF;
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