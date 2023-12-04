#ifndef CPROJECT_BSP_KEY_H
#define CPROJECT_BSP_KEY_H

#include "stm32f4xx.h"
#include "main.h"

/** 引脚定义 */
/*******************************************************/
#define KEY1_PIN                  GPIO_PIN_1
#define KEY1_GPIO_PORT            GPIOB
#define KEY1_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

#define KEY2_PIN                  GPIO_PIN_11
#define KEY2_GPIO_PORT            GPIOF
#define KEY2_GPIO_CLK_ENABLE()    __GPIOF_CLK_ENABLE()

#define KEY3_PIN                  GPIO_PIN_13
#define KEY3_GPIO_PORT            GPIOF
#define KEY3_GPIO_CLK_ENABLE()    __GPIOF_CLK_ENABLE()

#define KEY4_PIN                  GPIO_PIN_14
#define KEY4_GPIO_PORT            GPIOF
#define KEY4_GPIO_CLK_ENABLE()    __GPIOF_CLK_ENABLE()

#define KEY5_PIN                  GPIO_PIN_15
#define KEY5_GPIO_PORT            GPIOF
#define KEY5_GPIO_CLK_ENABLE()    __GPIOF_CLK_ENABLE()



/** 按键按下标置宏
   * 若按键按下为高电平，把宏设置成KEY_ON= 0 ，KEY_OFF = 1 即可
   */
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif //CPROJECT_BSP_KEY_H
