//
// Created by uoc11 on 2023/12/11.
//

#ifndef CPROJECT_BSP_ADC_H
#define CPROJECT_BSP_ADC_H

#include "stm32f4xx.h"
#include "main.h"

/** MOTOR1的ADC*/
#define MOTOR1_ADC                                   ADC3
#define MOTOR1_ADC_CLK_ENABLE()                      __HAL_RCC_ADC1_CLK_ENABLE()
#define MOTOR1_CURR_U_ADC_CHY                        ADC_CHANNEL_9
#define MOTOR1_CURR_V_ADC_CHY                        ADC_CHANNEL_14

/** MOTOR1注入通道采集相电流(Ia_1,Ib_1)GPIO*/



#endif //CPROJECT_BSP_ADC_H
