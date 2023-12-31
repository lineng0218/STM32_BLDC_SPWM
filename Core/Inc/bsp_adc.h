//
// Created by uoc11 on 2023/12/11.
//

#ifndef CPROJECT_BSP_ADC_H
#define CPROJECT_BSP_ADC_H

#include "stm32f4xx.h"
#include "main.h"

/** MOTOR1的ADC*/
#define MOTOR1_ADC                                   ADC3
#define MOTOR1_ADC_CLK_ENABLE()                      __HAL_RCC_ADC3_CLK_ENABLE()
#define MOTOR1_CURR_U_ADC_CHY                        ADC_CHANNEL_9
#define MOTOR1_CURR_V_ADC_CHY                        ADC_CHANNEL_14

/** MOTOR1的ADC*/
#define MOTOR2_ADC                                   ADC3
#define MOTOR2_ADC_CLK_ENABLE()                      __HAL_RCC_ADC3_CLK_ENABLE()
#define MOTOR2_CURR_U_ADC_CHY                        ADC_CHANNEL_15
#define MOTOR2_CURR_V_ADC_CHY                        ADC_CHANNEL_4

#define MOTOR_ADC_IRQn                               ADC_IRQn
#define MOTOR_ADC_IRQHandler                         ADC_IRQHandler

/** MOTOR1注入通道采集相电流(Ia_1,Ib_1)GPIO*/
#define MOTOR1_CURR_U_ADC_GPIO_PORT                  GPIOF
#define MOTOR1_CURR_U_ADC_GPIO_PIN                   GPIO_PIN_3
#define MOTOR1_CURR_U_ADC_GPIO_CLK_ENABLE()          __GPIOF_CLK_ENABLE()

#define MOTOR1_CURR_V_ADC_GPIO_PORT                  GPIOF
#define MOTOR1_CURR_V_ADC_GPIO_PIN                   GPIO_PIN_4
#define MOTOR1_CURR_V_ADC_GPIO_CLK_ENABLE()          __GPIOF_CLK_ENABLE()

/** MOTOR2注入通道采集相电流(Ia_2,Ib_2)GPIO*/
#define MOTOR2_CURR_U_ADC_GPIO_PORT                  GPIOF
#define MOTOR2_CURR_U_ADC_GPIO_PIN                   GPIO_PIN_5
#define MOTOR2_CURR_U_ADC_GPIO_CLK_ENABLE()          __GPIOF_CLK_ENABLE()

#define MOTOR2_CURR_V_ADC_GPIO_PORT                  GPIOF
#define MOTOR2_CURR_V_ADC_GPIO_PIN                   GPIO_PIN_6
#define MOTOR2_CURR_V_ADC_GPIO_CLK_ENABLE()          __GPIOF_CLK_ENABLE()

void ADC_Init(void);
void ADC_Data_Read(void);

#endif //CPROJECT_BSP_ADC_H
