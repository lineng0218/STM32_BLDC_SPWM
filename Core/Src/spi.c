/**
  ******************************************************************************
  * @file    spi.c
  * @author  Leo
  * @date    2023/10/17
  * @version v1
  * @brief
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "stm32f4xx_hal_spi.h"

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;

/**
  * @brief  配置SPI1的GPIO口
  * @param  无
  * @retval 无
  */
void SPI1_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct_NSS;

    DRV8303_SPI1_CLK_ENABLE();
    DRV8303_SPI1_SCK_GPIO_CLK_ENABLE();
    DRV8303_SPI1_MISO_GPIO_CLK_ENABLE();
    DRV8303_SPI1_MOSI_GPIO_CLK_ENABLE();
    DRV8303_SPI1_NSS_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin   = DRV8303_SPI1_GPIO_SCK_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = DRV8303_SPI1_SCK_AF;
    HAL_GPIO_Init(DRV8303_SPI1_GPIO_SCK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = DRV8303_SPI1_GPIO_MISO_PIN;
    HAL_GPIO_Init(DRV8303_SPI1_GPIO_MISO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = DRV8303_SPI1_GPIO_MOSI_PIN;
    HAL_GPIO_Init(DRV8303_SPI1_GPIO_MOSI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct_NSS.Pin = DRV8303_SPI1_GPIO_NSS_PIN;
    GPIO_InitStruct_NSS.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_NSS.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct_NSS.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DRV8303_SPI1_GPIO_NSS_PORT, &GPIO_InitStruct_NSS);

    DRV8303_SPI1_NSS_HIGH();
}


/**
  * @brief  配置SPI3的GPIO口
  * @param  无
  * @retval 无
  */
void SPI3_GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct_NSS;

    DRV8303_SPI3_CLK_ENABLE();
    DRV8303_SPI3_SCK_GPIO_CLK_ENABLE();
    DRV8303_SPI3_MISO_GPIO_CLK_ENABLE();
    DRV8303_SPI3_MOSI_GPIO_CLK_ENABLE();
    DRV8303_SPI3_NSS_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin   = DRV8303_SPI3_GPIO_SCK_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = DRV8303_SPI3_SCK_AF;
    HAL_GPIO_Init(DRV8303_SPI3_GPIO_SCK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = DRV8303_SPI3_GPIO_MISO_PIN;
    HAL_GPIO_Init(DRV8303_SPI3_GPIO_MISO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = DRV8303_SPI3_GPIO_MOSI_PIN;
    HAL_GPIO_Init(DRV8303_SPI3_GPIO_MOSI_PORT, &GPIO_InitStruct);

    /***/
    GPIO_InitStruct_NSS.Pin = DRV8303_SPI3_GPIO_NSS_PIN;
    GPIO_InitStruct_NSS.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_NSS.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct_NSS.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DRV8303_SPI3_GPIO_NSS_PORT, &GPIO_InitStruct_NSS);

    DRV8303_SPI3_NSS_HIGH();
}

/**
  * @brief  配置SPI1的模式
  * @param  无
  * @retval 无
  */
void DRV8303_SPI1_Config(void)
{
    DRV8303_SPI1_CLK_ENABLE();

    hspi1.Instance = DRV8303_SPI1;
    /** 在 SPI 通信中，数据可以在全双工模式或半双工模式下传输。全双工模式允许同时进行发送和接收操作，
     * 而半双工模式则只允许在一个时刻进行发送或接收操作。这个参数允许您选择合适的双向模式，以满足通信需求和设备规格。根据需要，它可以是 @ref SPI_Direction 中定义的值之一。*/
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    /** 指定 SPI 的操作模式 */
    hspi1.Init.Mode      = SPI_MODE_MASTER;
    /** 指定用于 CRC 计算的多项式，必须是介于 1 到 65535 之间的奇数。 */
    hspi1.Init.CRCPolynomial = 15;
    /** 用于指定 SPI 数据的大小，即每个数据传输的位数。 */
    hspi1.Init.DataSize  = SPI_DATASIZE_16BIT;
    /** 用于指定数据传输是从最高有效位（MSB）还是最低有效位（LSB）开始。 */
    hspi1.Init.FirstBit  = SPI_FIRSTBIT_MSB;
    /** 用于指定波特率预分频值，该值将用于配置传输和接收的 SCK 时钟（时钟频率）。 */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    /** 用于指定 NSS（Slave Select）信号在 SPI（串行外设接口）配置中的管理方式。NSS 变量指定 NSS 信号是由硬件（NSS 引脚）管理还是由软件使用 SSI（Slave Select In）位进行管理。*/
    hspi1.Init.NSS       = SPI_NSS_SOFT;
    /** 用于指定 SPI 串行时钟的稳态（极性）。 */
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    /** 用于指定在数据位捕获过程中的时钟信号的活动边沿。在 SPI 通信中，数据的传输通常发生在时钟信号的上升沿或下降沿。这个参数允许您选择适当的时钟边沿来捕获数据位，以满足通信需求和设备规格 */
    hspi1.Init.CLKPhase  =  SPI_PHASE_2EDGE;
    HAL_SPI_Init(&hspi1);
    /** 激活SPI */
    __HAL_SPI_ENABLE(&hspi1);
}

/**
  * @brief  配置SPI3的模式
  * @param  无
  * @retval 无
  */
void DRV8303_SPI3_Config(void)
{
    DRV8303_SPI3_CLK_ENABLE();

    hspi3.Instance = DRV8303_SPI3;
    /** 在 SPI 通信中，数据可以在全双工模式或半双工模式下传输。全双工模式允许同时进行发送和接收操作，
     * 而半双工模式则只允许在一个时刻进行发送或接收操作。这个参数允许您选择合适的双向模式，以满足通信需求和设备规格。根据需要，它可以是 @ref SPI_Direction 中定义的值之一。*/
    hspi3.Init.Direction = SPI_DIRECTION_2LINES;
    /** 指定 SPI 的操作模式 */
    hspi3.Init.Mode      = SPI_MODE_MASTER;
    /** 指定用于 CRC 计算的多项式，必须是介于 1 到 65535 之间的奇数。 */
    hspi3.Init.CRCPolynomial = 15;
    /** 用于指定 SPI 数据的大小，即每个数据传输的位数。 */
    hspi3.Init.DataSize  = SPI_DATASIZE_16BIT;
    /** 用于指定数据传输是从最高有效位（MSB）还是最低有效位（LSB）开始。 */
    hspi3.Init.FirstBit  = SPI_FIRSTBIT_MSB;
    /** 用于指定波特率预分频值，该值将用于配置传输和接收的 SCK 时钟（时钟频率）。 */
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    /** 用于指定 NSS（Slave Select）信号在 SPI（串行外设接口）配置中的管理方式。NSS 变量指定 NSS 信号是由硬件（NSS 引脚）管理还是由软件使用 SSI（Slave Select In）位进行管理。*/
    hspi3.Init.NSS       = SPI_NSS_SOFT;
    /** 用于指定 SPI 串行时钟的稳态（极性）。 */
    hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
    /** 用于指定在数据位捕获过程中的时钟信号的活动边沿。在 SPI 通信中，数据的传输通常发生在时钟信号的上升沿或下降沿。这个参数允许您选择适当的时钟边沿来捕获数据位，以满足通信需求和设备规格 */
    hspi3.Init.CLKPhase  =  SPI_PHASE_2EDGE;

    HAL_SPI_Init(&hspi3);

    /** 激活SPI */
    __HAL_SPI_ENABLE(&hspi3);
}


/**
  * @brief  初始化SPI
  * @param  无
  * @retval 无
  *
*/
void DRV8303_SPI_Init(void)
{
    SPI1_GPIO_INIT();
    SPI3_GPIO_INIT();
    DRV8303_SPI1_Config();
    DRV8303_SPI3_Config();

}


/**
  * @brief  SPI读写数据
  * @param  这是SPI选择句柄
  * @param  uint16_t指针,这是要发送的数据
  * @retval uint16_t，返回的数据
  *
*/
uint16_t SPI_Write_Read_Data(SPI_TypeDef* SPIx, uint16_t txData)
{
    uint16_t rxData;

    /** 此标志用于检查传输缓冲区是否为空，以确定是否可以将数据传输到SPI总线。 */
    while(SPI_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET){}
    SPI_I2S_SendData(SPIx,txData);

    while (SPI_GetFlagStatus(SPIx,SPI_FLAG_RXNE) == RESET){}
    rxData = SPI_I2S_ReceiveData(SPIx);
    return rxData;
}


/**
  * @brief  DRV8303读写数据的口
  * @param  这是SPI选择句柄
  * @param  uint16_t指针,这是要发送的数据
  * @retval uint16_t，返回的数据
  *
*/
uint16_t DRV8303_SPI_ReadWrite(SPI_TypeDef* SPIx, uint16_t TxData)
{
    uint16_t Data;
    if(DRV8303_SPI1 == SPIx)
    {
        DRV8303_SPI1_NSS_LOW();
        SPI_Write_Read_Data(DRV8303_SPI1, TxData);
        DRV8303_SPI1_NSS_HIGH();
        HAL_Delay(10);
        DRV8303_SPI1_NSS_LOW();
        Data = SPI_Write_Read_Data(DRV8303_SPI1, TxData);
        DRV8303_SPI1_NSS_HIGH();
        return Data;
    }
    else{
        DRV8303_SPI3_NSS_LOW();
        SPI_Write_Read_Data(DRV8303_SPI3, TxData);
        DRV8303_SPI3_NSS_HIGH();
        HAL_Delay(10);
        DRV8303_SPI3_NSS_LOW();
        Data = SPI_Write_Read_Data(DRV8303_SPI3, TxData);
        DRV8303_SPI3_NSS_HIGH();
        return Data;
    }
}

