/**
  ******************************************************************************
  * @file    DRV8303.c
  * @author  Leo
  * @date    2023/10/18
  * @version v1
  * @brief
  ******************************************************************************
  */

#include "bsp_DRV8303.h"
#include "spi.h"
#include "bsp_led.h"

uint16_t SPI_WRITE_MODE1=0;
uint16_t SPI_WRITE_MODE2=0;
uint16_t ReadReg=0;


/**
  * @brief  初始化DRV8303的GPIO口
  * @param  无
  * @retval 无
  */
void DRV8303_EN_GATE_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    DRV8303_EN_GATE1_GPIO_CLK();
    DRV8303_EN_GATE2_GPIO_CLK();

    GPIO_InitStruct.Pin   = DRV8303_EN_GATE1_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(DRV8303_EN_GATE1_GPIO_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.Pin   = DRV8303_EN_GATE2_GPIO_PIN;
    HAL_GPIO_Init(DRV8303_EN_GATE2_GPIO_PORT,&GPIO_InitStruct);
}

/**
  * @brief  使能DRV8303_EN_GATE1
  * @param  无
  * @retval 无
  */
void DRV8303_EN_GATE1_HIGH(void)
{
    HAL_GPIO_WritePin(DRV8303_EN_GATE1_GPIO_PORT, DRV8303_EN_GATE1_GPIO_PIN, 1);
}

/**
  * @brief  使能DRV8303_EN_GATE2
  * @param  无
  * @retval 无
  */
void DRV8303_EN_GATE2_HIGH(void)
{
    HAL_GPIO_WritePin(DRV8303_EN_GATE2_GPIO_PORT, DRV8303_EN_GATE2_GPIO_PIN, 1);
}


/**
  * @brief  8303写入数据,参数1为控制寄存器1需要写入数据，参数2为控制寄存器2写入数据
  * @param  无
  * @retval 无
  */
void DRV8303_WriteWord(uint16_t TxData1,uint16_t TxData2)
{
    uint16_t Control_Reg = 0;
    SPI_WRITE_MODE1=TxData1;
    Control_Reg=(TxData1 & 0x7800)>>11;
    if(Control_Reg == 0x0002)
    {
        DRV8303_SPI_ReadWrite(DRV8303_SPI1, SPI_WRITE_MODE1);

        DRV8303_SPI_ReadWrite(DRV8303_SPI3, SPI_WRITE_MODE1);
    }

    SPI_WRITE_MODE2=TxData2;
    Control_Reg=(TxData2 & 0x7800)>>11;
    if(Control_Reg == 0x0003)
    {
        DRV8303_SPI_ReadWrite(DRV8303_SPI1, SPI_WRITE_MODE2);

        DRV8303_SPI_ReadWrite(DRV8303_SPI3, SPI_WRITE_MODE2);
    }
}

/**
  * @brief  8303初始化
  * @param  无
  * @retval 无
  */
void DRV8303_Init(void)
{
    /** SPI初始化 */
    DRV8303_SPI_Init();
    /** 初始化使能引脚 */
    DRV8303_EN_GATE_GPIO_Init();
    /** 使能DRV8303 */
    DRV8303_EN_GATE1_HIGH();
    DRV8303_EN_GATE2_HIGH();

    HAL_Delay(500);

    SPI_WRITE_MODE1 = DRV8303_Write_Mode|DRV8303_Control_Reg1|DRV8303_GATE_CURRENT_0_25|DRV8303_GATE_RESET_Normal
                      |DRV8303_PWM_MODE_6|DRV8303_OCP_MODE_OC_Latch_Shut_Down|DRV8303_OC_ADJ_SET_0_926;

    SPI_WRITE_MODE2 = DRV8303_Write_Mode|DRV8303_Control_Reg2|DRV8303_OCTW_MODE_Report_OT_OC|DRV8303_GAIN_40
               |DRV8303_DC_CAL_CH1_A1_INPUT|DRV8303_DC_CAL_CH2_A2_INPUT|DRV8303_OC_TOFF_Cycle_by_Cycle;

    DRV8303_WriteWord(SPI_WRITE_MODE1,SPI_WRITE_MODE2);

    /** 用来读控制寄存器1的值 */
    ReadReg = DRV8303_Read_Mode|DRV8303_Control_Reg1;

//    uint16_t Re_values1 = DRV8303_SPI_ReadWrite(DRV8303_SPI1, ReadReg);
//    if(0x1012 == Re_values1)
//    {
//        LED1_ON;
//    }
//
//    uint16_t Re_values2 = DRV8303_SPI_ReadWrite(DRV8303_SPI3, ReadReg);
//    if(0x1012 == Re_values2)
//    {
//        LED2_ON;
//    }
//
//    /** 用来读控制寄存器2的值 */
//    ReadReg=DRV8303_Read_Mode|DRV8303_Control_Reg2;
//    uint16_t Re_values3 = DRV8303_SPI_ReadWrite(DRV8303_SPI1, ReadReg);
//    if(0x1808 == Re_values3)
//    {
//        LED3_ON;
//    }
//
//    uint16_t Re_values4 = DRV8303_SPI_ReadWrite(DRV8303_SPI3, ReadReg);
//    if(0x1808 == Re_values4)
//    {
//        LED4_ON;
//    }
}
