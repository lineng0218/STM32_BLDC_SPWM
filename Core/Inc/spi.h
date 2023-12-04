/**
  ******************************************************************************
  * @file    spi.h
  * @author  Leo
  * @date    2023/10/17
  * @version v1
  * @brief
  ******************************************************************************
  */

#ifndef __SPI_H__
#define __SPI_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal_gpio.h"

/** 使能SPI1和SPI3的时钟 */
#define DRV8303_SPI1_CLK_ENABLE()               __HAL_RCC_SPI1_CLK_ENABLE()
#define DRV8303_SPI3_CLK_ENABLE()               __HAL_RCC_SPI3_CLK_ENABLE()

/** 定义SPI1的GPIO接口 */
#define DRV8303_SPI1                            SPI1

#define DRV8303_SPI1_GPIO_SCK_PIN               GPIO_PIN_5
#define DRV8303_SPI1_GPIO_SCK_PORT              GPIOA
#define DRV8303_SPI1_SCK_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define DRV8303_SPI1_SCK_AF                     GPIO_AF5_SPI1

#define DRV8303_SPI1_GPIO_MISO_PIN              GPIO_PIN_6
#define DRV8303_SPI1_GPIO_MISO_PORT             GPIOA
#define DRV8303_SPI1_MISO_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()
#define DRV8303_SPI1_MISO_AF                    GPIO_AF5_SPI1

#define DRV8303_SPI1_GPIO_MOSI_PIN              GPIO_PIN_7
#define DRV8303_SPI1_GPIO_MOSI_PORT             GPIOA
#define DRV8303_SPI1_MOSI_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()
#define DRV8303_SPI1_MOSI_AF                    GPIO_AF5_SPI1

#define DRV8303_SPI1_GPIO_NSS_PIN               GPIO_PIN_10
#define DRV8303_SPI1_GPIO_NSS_PORT              GPIOD
#define DRV8303_SPI1_NSS_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE()

#define DRV8303_SPI1_NSS_LOW()                  HAL_GPIO_WritePin(DRV8303_SPI1_GPIO_NSS_PORT,DRV8303_SPI1_GPIO_NSS_PIN,0)
#define DRV8303_SPI1_NSS_HIGH()                 HAL_GPIO_WritePin(DRV8303_SPI1_GPIO_NSS_PORT,DRV8303_SPI1_GPIO_NSS_PIN,1)



/** 定义SPI3的GPIO接口 */
#define DRV8303_SPI3                            SPI3

#define DRV8303_SPI3_GPIO_SCK_PIN               GPIO_PIN_3
#define DRV8303_SPI3_GPIO_SCK_PORT              GPIOB
#define DRV8303_SPI3_SCK_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define DRV8303_SPI3_SCK_AF                     GPIO_AF6_SPI3

#define DRV8303_SPI3_GPIO_MISO_PIN              GPIO_PIN_4
#define DRV8303_SPI3_GPIO_MISO_PORT             GPIOB
#define DRV8303_SPI3_MISO_GPIO_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define DRV8303_SPI3_MISO_AF                    GPIO_AF6_SPI3

#define DRV8303_SPI3_GPIO_MOSI_PIN              GPIO_PIN_5
#define DRV8303_SPI3_GPIO_MOSI_PORT             GPIOB
#define DRV8303_SPI3_MOSI_GPIO_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define DRV8303_SPI3_MOSI_AF                    GPIO_AF6_SPI3

#define DRV8303_SPI3_GPIO_NSS_PIN               GPIO_PIN_10
#define DRV8303_SPI3_GPIO_NSS_PORT              GPIOG
#define DRV8303_SPI3_NSS_GPIO_CLK_ENABLE()      __GPIOG_CLK_ENABLE()

#define DRV8303_SPI3_NSS_LOW()                  HAL_GPIO_WritePin(DRV8303_SPI3_GPIO_NSS_PORT,DRV8303_SPI3_GPIO_NSS_PIN,0)
#define DRV8303_SPI3_NSS_HIGH()                 HAL_GPIO_WritePin(DRV8303_SPI3_GPIO_NSS_PORT,DRV8303_SPI3_GPIO_NSS_PIN,1)


extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;
void DRV8303_SPI_Init(void);
uint16_t SPI_Write_Read_Data(SPI_TypeDef* SPIx, uint16_t txData);
uint16_t DRV8303_SPI_ReadWrite(SPI_TypeDef* SPIx, uint16_t TxData);

#endif /* __SPI_H__ */

