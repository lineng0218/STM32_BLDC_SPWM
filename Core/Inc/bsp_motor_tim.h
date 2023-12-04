#ifndef CPROJECT_BSP_MOTOR_TIM_H
#define CPROJECT_BSP_MOTOR_TIM_H

#include "stm32f4xx.h"
#include "main.h"

/*****************************电机接口1宏定义*******************************************/
#define MOTOR1_TIM                      TIM1
#define MOTOR1_TIM_CLK_ENABLE()         __TIM1_CLK_ENABLE()
#define MOTOR1_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM1_CLK_DISABLE()

#define MOTOR1_TIM_IRQn                   TIM1_UP_TIM10_IRQn
#define MOTOR1_TIM_OC_IRQHANDLER          TIM1_UP_TIM10_IRQHandler
extern TIM_HandleTypeDef                  motor1_htimx_bldcm;

/** 累计 TIM_Period个后产生一个更新或者中断
	当定时器从0计数到5599，即为5600次，为一个定时周期 */
#define MOTOR1_PWM_PERIOD_COUNT         (4200-1)

/** 高级控制定时器时钟源TIMxCLK = HCLK = 168MHz
	 设定定时器频率为=TIMxCLK/(PWM_PRESCALER_COUNT+1)/PWM_PERIOD_COUNT = 15KHz*/
#define MOTOR1_PWM_PRESCALER_COUNT      (0)

#define MOTOR1_TIM_REPETITIONCOUNTER    1

/** TIM1 通道1 输出引脚 */
#define MOTOR1_OCPWM1_PIN               GPIO_PIN_8
#define MOTOR1_OCPWM1_GPIO_PORT         GPIOA
#define MOTOR1_OCPWM1_GPIO_CLK_ENABLE() __GPIOA_CLK_ENABLE()
#define MOTOR1_OCPWM1_AF                GPIO_AF1_TIM1

/** TIM1 通道2 输出引脚 */
#define MOTOR1_OCPWM2_PIN               GPIO_PIN_9
#define MOTOR1_OCPWM2_GPIO_PORT         GPIOA
#define MOTOR1_OCPWM2_GPIO_CLK_ENABLE() __GPIOA_CLK_ENABLE()
#define MOTOR1_OCPWM2_AF                GPIO_AF1_TIM1

/** TIM1 通道3 输出引脚 */
#define MOTOR1_OCPWM3_PIN               GPIO_PIN_10
#define MOTOR1_OCPWM3_GPIO_PORT         GPIOA
#define MOTOR1_OCPWM3_GPIO_CLK_ENABLE() __GPIOA_CLK_ENABLE()
#define MOTOR1_OCPWM3_AF			    GPIO_AF1_TIM1

/** TIM1 通道1 互补输出引脚 */
#define MOTOR1_OCNPWM1_PIN               GPIO_PIN_13
#define MOTOR1_OCNPWM1_GPIO_PORT      	 GPIOB
#define MOTOR1_OCNPWM1_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()
#define MOTOR1_OCNPWM1_AF			     GPIO_AF1_TIM1

/** TIM1 通道2 互补输出引脚 */
#define MOTOR1_OCNPWM2_PIN            	 GPIO_PIN_14
#define MOTOR1_OCNPWM2_GPIO_PORT      	 GPIOB
#define MOTOR1_OCNPWM2_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()
#define MOTOR1_OCNPWM2_AF			     GPIO_AF1_TIM1

/** TIM1 通道3 互补输出引脚 */
#define MOTOR1_OCNPWM3_PIN            	 GPIO_PIN_15
#define MOTOR1_OCNPWM3_GPIO_PORT      	 GPIOB
#define MOTOR1_OCNPWM3_GPIO_CLK_ENABLE() __GPIOB_CLK_ENABLE()
#define MOTOR1_OCNPWM3_AF				 GPIO_AF1_TIM1


void TIMx_Configuration(void);



#endif //CPROJECT_BSP_MOTOR_TIM_H
