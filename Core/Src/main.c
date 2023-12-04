/**
  ******************************************************************************
  * @file   : main.c
  * @brief  : Main program body
  * question: 如果cmake不见了，在file->setting->cmake 中的右上角reload一下即可
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "spi.h"
#include "gpio.h"
#include "bsp_DRV8303.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_motor_tim.h"
#include "bsp_usart.h"
#include "bsp_spwm.h"

//void MX_FREERTOS_Init(void);

extern float  Volt_Freq;
extern  int32_t  Accel;


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    float freq = 5;    // ³õÊ¼ÆµÂÊÖµ
    float max =  60.0f;
    HAL_Init();
    /** 初始化系统时钟为168MHz */
    SystemClock_Config();
    /** LED 灯初始化 */
    LED_GPIO_Config();
    /** 初始化 DRV8303 */
    DRV8303_Init();
    /** 初始化按键G PIO */
    Key_GPIO_Config();
    /** 初始化电机 */
    /** 初始化USART 配置模式为 115200 8-N-1，中断接收 */
//    USART_Config();
    TIMx_Configuration();

    /** 开启定时器通道1输出PWM */
    HAL_TIM_PWM_Start(&motor1_htimx_bldcm,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&motor1_htimx_bldcm, TIM_CHANNEL_1);

    /** 开启定时器通道2输出PWM */
    HAL_TIM_PWM_Start(&motor1_htimx_bldcm,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&motor1_htimx_bldcm, TIM_CHANNEL_2);

    /** 开启定时器通道3输出PWM */
    HAL_TIM_PWM_Start(&motor1_htimx_bldcm,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&motor1_htimx_bldcm, TIM_CHANNEL_3);

    HAL_TIM_Base_Start_IT(&motor1_htimx_bldcm);

    HAL_Delay(3000);
    while (1)
    {
        HAL_Delay(10);
        config_Sinusoidal( freq += Accel );
        if( (freq>=60) || ( freq<=-60 ) )
        {
            Volt_Freq = 0.2f;
        }
        if( (freq >= max ) || (freq <= -max))
        {
            freq -= Accel;
        }
    }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001)
    {
        /* Enable the Flash prefetch */
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}
/****************************END OF FILE***************************/

