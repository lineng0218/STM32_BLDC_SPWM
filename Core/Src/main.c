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
#include "bsp_svpwm.h"

//void MX_FREERTOS_Init(void);

extern float  Volt_Freq;
extern  int32_t  Accel;


/**半周期正弦波，幅值256，频率为1Hz */
uint8_t SinTable[256]={// 0 ~ ¦Ð
        0x00, 0x03, 0x06, 0x09, 0x0C, 0x0F, 0x12, 0x15,
        0x18, 0x1C, 0x1F, 0x22, 0x25, 0x28, 0x2B, 0x2E,
        0x31, 0x34, 0x37, 0x3A, 0x3D, 0x40, 0x44, 0x47,
        0x4A, 0x4D, 0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E,
        0x61, 0x64, 0x67, 0x6A, 0x6D, 0x6F, 0x72, 0x75,
        0x78, 0x7A, 0x7D, 0x80, 0x83, 0x85, 0x88, 0x8B,
        0x8D, 0x90, 0x92, 0x95, 0x97, 0x9A, 0x9C, 0x9F,
        0xA1, 0xA4, 0xA6, 0xA8, 0xAB, 0xAD, 0xAF, 0xB2,
        0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBF, 0xC1, 0xC3,
        0xC5, 0xC7, 0xC9, 0xCA, 0xCC, 0xCE, 0xD0, 0xD2,
        0xD4, 0xD5, 0xD7, 0xD9, 0xDA, 0xDC, 0xDD, 0xDF,
        0xE0, 0xE2, 0xE3, 0xE5, 0xE6, 0xE7, 0xE9, 0xEA,
        0xEB, 0xEC, 0xED, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3,
        0xF4, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF8, 0xF9,
        0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC, 0xFD, 0xFD,
        0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
        0xFF, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
        0xFD, 0xFD, 0xFD, 0xFC, 0xFC, 0xFB, 0xFB, 0xFA,
        0xFA, 0xF9, 0xF8, 0xF8, 0xF7, 0xF6, 0xF5, 0xF4,
        0xF4, 0xF3, 0xF2, 0xF1, 0xF0, 0xEF, 0xED, 0xEC,
        0xEB, 0xEA, 0xE9, 0xE7, 0xE6, 0xE5, 0xE3, 0xE2,
        0xE0, 0xDF, 0xDD, 0xDC, 0xDA, 0xD9, 0xD7, 0xD5,
        0xD4, 0xD2, 0xD0, 0xCE, 0xCC, 0xCA, 0xC9, 0xC7,
        0xC5, 0xC3, 0xC1, 0xBF, 0xBC, 0xBA, 0xB8, 0xB6,
        0xB4, 0xB2, 0xAF, 0xAD, 0xAB, 0xA8, 0xA6, 0xA4,
        0xA1, 0x9F, 0x9C, 0x9A, 0x97, 0x95, 0x92, 0x90,
        0x8D, 0x8B, 0x88, 0x85, 0x83, 0x80, 0x7D, 0x7A,
        0x78, 0x75, 0x72, 0x6F, 0x6D, 0x6A, 0x67, 0x64,
        0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52, 0x4F, 0x4D,
        0x4A, 0x47, 0x44, 0x40, 0x3D, 0x3A, 0x37, 0x34,
        0x31, 0x2E, 0x2B, 0x28, 0x25, 0x22, 0x1F, 0x1C,
        0x18, 0x15, 0x12, 0x0F, 0x0C, 0x09, 0x06, 0x03,
};

int32_t  SamplePoint = sizeof(SinTable)/sizeof(SinTable[0]); //标准正弦波点数

/**定点数据Q=15,0~32766对应浮点数据0~0.99996948 */


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
    LED1_ON;
    /** 初始化 DRV8303 */
    DRV8303_Init();
    /** 初始化按键G PIO */
    Key_GPIO_Config();
    /** 初始化电机 */
    /** 初始化USART 配置模式为 115200 8-N-1，中断接收 */
//    USART_Config();
    TIMx_Configuration();

    HAL_Delay(1000);
    while (1)
    {

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

