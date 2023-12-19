//
// Created by uoc11 on 2023/12/11.
//
#include "bsp_adc.h"
#include "bsp_svpwm.h"

ADC_HandleTypeDef MOTOR1_ADC_Handle;
ADC_InjectionConfTypeDef MOTOR1_ConfigInjected;

int16_t ADC_U_BUFFER[1000]={0};
int16_t ADC_V_BUFFER[1000]={0};

static void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    MOTOR1_CURR_U_ADC_GPIO_CLK_ENABLE();
    MOTOR1_CURR_V_ADC_GPIO_CLK_ENABLE();

    MOTOR2_CURR_U_ADC_GPIO_CLK_ENABLE();
    MOTOR2_CURR_V_ADC_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Pin = MOTOR1_CURR_U_ADC_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL ;
    HAL_GPIO_Init(MOTOR1_CURR_U_ADC_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = MOTOR1_CURR_V_ADC_GPIO_PIN;
    HAL_GPIO_Init(MOTOR1_CURR_V_ADC_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = MOTOR2_CURR_U_ADC_GPIO_PIN;
    HAL_GPIO_Init(MOTOR2_CURR_U_ADC_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = MOTOR2_CURR_V_ADC_GPIO_PIN;
    HAL_GPIO_Init(MOTOR2_CURR_V_ADC_GPIO_PORT, &GPIO_InitStructure);

    HAL_NVIC_SetPriority(MOTOR_ADC_IRQn ,0,0);
    HAL_NVIC_EnableIRQ(MOTOR_ADC_IRQn );
}

static void ADC_Mode_Config(void)
{
    MOTOR1_ADC_CLK_ENABLE();
    /**配置ADC的全局特性，包括时钟，分辨率，数据对齐方向，转换通道数量 */
    MOTOR1_ADC_Handle.Instance                   = MOTOR1_ADC;
    MOTOR1_ADC_Handle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;    /**ADC时钟84MHz/4=21MHz*/
    MOTOR1_ADC_Handle.Init.Resolution            = ADC_RESOLUTION_12B;          /**12位分辨率（转换时间15个时钟周期）*/
    MOTOR1_ADC_Handle.Init.ScanConvMode          = ENABLE;                      /**扫描模式*/
    MOTOR1_ADC_Handle.Init.ContinuousConvMode    = DISABLE;                     /**连续采样*/
    MOTOR1_ADC_Handle.Init.DiscontinuousConvMode = DISABLE;                     /**不连续采样*/
    MOTOR1_ADC_Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;   /**无外部触发*/
    MOTOR1_ADC_Handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;                /**软件启动*/
    MOTOR1_ADC_Handle.Init.DataAlign             = ADC_DATAALIGN_LEFT ;              /**右对齐，硬石源码是左对齐*/
    MOTOR1_ADC_Handle.Init.NbrOfConversion       = 1;
    MOTOR1_ADC_Handle.Init.DMAContinuousRequests = DISABLE;                         /**DMA传输请求*/
    MOTOR1_ADC_Handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;             /**单次转换完成标记*/
    HAL_ADC_Init(&MOTOR1_ADC_Handle);

    MOTOR1_ConfigInjected.InjectedChannel           = MOTOR1_CURR_U_ADC_CHY;
    MOTOR1_ConfigInjected.InjectedRank              = 1;                          /**采样顺序*/
    MOTOR1_ConfigInjected.InjectedNbrOfConversion   = 2;                          /**总的转换通道数量*/
    MOTOR1_ConfigInjected.InjectedSamplingTime      = ADC_SAMPLETIME_3CYCLES;     /**采样时间，3个周期，单次转换时间是15个周期*/
    MOTOR1_ConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONVEDGE_RISING;    /**外部信号上升沿触发（ADC外部）*/
    MOTOR1_ConfigInjected.ExternalTrigInjecConv     = ADC_EXTERNALTRIGINJECCONV_T1_CC4;        /**触发源：TIM1  CH4比较事件*/
    MOTOR1_ConfigInjected.AutoInjectedConv          = DISABLE;                                /**自动注入*/
    MOTOR1_ConfigInjected.InjectedDiscontinuousConvMode = ENABLE;                             /**不连续采样*/
    MOTOR1_ConfigInjected.InjectedOffset            = 0;                                       /**数据偏移值*/
    HAL_ADCEx_InjectedConfigChannel(&MOTOR1_ADC_Handle, &MOTOR1_ConfigInjected);

    MOTOR1_ConfigInjected.InjectedChannel = MOTOR1_CURR_V_ADC_CHY;
    MOTOR1_ConfigInjected.InjectedRank    = 2;                                               /**采样顺序，其他配置与U相采集一致*/
    HAL_ADCEx_InjectedConfigChannel(&MOTOR1_ADC_Handle, &MOTOR1_ConfigInjected);

    HAL_ADCEx_InjectedStart(&MOTOR1_ADC_Handle);


}

void ADC_Init(void)
{
    ADC_GPIO_Init();
    ADC_Mode_Config();
}

void ADC_Data_Read(void)
{
    uint16_t  i=0;
    ADC_U_BUFFER[++i]= MOTOR1_ADC ->JDR1;
    if(i>1000){i=0;}
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if( hadc->Instance == MOTOR1_ADC)
    {
        ADC_Data_Read();
        SVPWM_Mode();
    }
}