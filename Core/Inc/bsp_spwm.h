//
// Created by uoc11 on 2023/12/4.
//

#ifndef CPROJECT_BSP_SPWM_H
#define CPROJECT_BSP_SPWM_H

#include "stm32f4xx_hal.h"
#include "bsp_motor_tim.h"

#define PI          (3.1415926f) // ¦Ð
#define PI_X2       (2.0f*PI)    // 2¦Ð


#define  MODULATED_FREQ_Hz    5
// ÔØ²¨(Èý½Ç²¨)ÆµÂÊ
#define  CARRIER_FREQ_Hz      (float)(1.68e8/(MOTOR1_PWM_PRESCALER_COUNT+1))/(2*(MOTOR1_PWM_PERIOD_COUNT +1))//

#define  RATIO                (float)(CARRIER_FREQ_Hz/MODULATED_FREQ_Hz)


#define SIN_AMPMAX             255

typedef enum {
    CCW,
    CW,
}MotorDir_Typedef;

void config_Sinusoidal( float Fre);
void TuneSinAmp( uint16_t Amplitude);
void set_MotorDir(MotorDir_Typedef Dir);
#endif //CPROJECT_BSP_SPWM_H
