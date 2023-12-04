
#include "bsp_led.h"
#include "bsp_usart.h"

UART_HandleTypeDef UartHandle;

uint8_t	 data_buff[max_length];
uint8_t	 rd_p,wr_p;

/**
  * @brief  用来初始化USART串口的配置，其中，配置GPIO口，配置UASRT的功能
	*	@note 	无
  * @retval 无
  */

void USART_Config(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    USART_CLK_ENABLE();

    USART_RX_GPIO_CLK_ENABLE();
    USART_TX_GPIO_CLK_ENABLE();

/**USART1 GPIO Configuration
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX
  */
    /** 配置Tx引脚为复用功能  */
    GPIO_InitStruct.Pin   = USART_TX_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART_TX_AF;
    HAL_GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);
    /** 配置Rx引脚为复用功能 */
    GPIO_InitStruct.Pin = USART_RX_PIN;
    GPIO_InitStruct.Alternate = USART_RX_AF;
    HAL_GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);
    /** 配置串口USART 模式 */
    UartHandle.Instance	=	USART_SEL;
    UartHandle.Init.WordLength 	=	UART_WORDLENGTH_8B; 	/** 对USART_CR1中的M位进行编程以定义字长 */
    UartHandle.Init.BaudRate = USART_BAUDRATE;				/** 选择所需的波特率 */
    UartHandle.Init.StopBits = UART_STOPBITS_1;				/** 对USART_CR2中的停止位数量进行编程 */
    UartHandle.Init.Parity	 =	UART_PARITY_NONE;
    UartHandle.Init.Mode	 =	 UART_MODE_TX_RX;
    HAL_UART_Init(&UartHandle);						/** 只是开启了USART_UE */
    /** 配置串口接收中断 */
    __HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);

    HAL_NVIC_SetPriority(USART_IRQ, 0, 2);
    HAL_NVIC_EnableIRQ(USART_IRQ);
}

/**
  * @brief  这个函数是用来将uasrt检测到的数据推进data_buff里面
	*	@param 	第一个参数指向data_buff
	*	@param 	第二个参数指向RDR寄存器收集到一个byte的数据
	*	@note 	无
  * @retval 返回1时，说明了存入的数据和读出的数据同步处理完了，即存入多少数据，已经读完多少数据；
						返回0时，说明了存入了数据的，但没有将这些存入的数据解析完
  */
uint8_t PushArr(uint8_t *arr,uint8_t data)
{
    uint8_t index;
    index = (wr_p+1)%max_length;
    if(index == rd_p)return 1;
    arr[wr_p]=data;
    wr_p = index;
    return 0;
}

/**
  * @brief  这个函数是用来将data_buff的帧数据读取到用来协议解析的函数中
	*	@param 	第一个参数指向data_buff
	*	@param 	第二个参数指向用来存放一帧数据的数组中
	*	@note 	无
  * @retval 返回1时，说明了存入的数据和读出的数据同步处理完了，即存入多少数据就已经读完多少数据一帧一帧地处理；
						返回0时，说明了存入了数据的，但没有将这些存入的数据解析完
  */
uint8_t PopArr(uint8_t *arr ,uint8_t *data)
{
    if(rd_p == wr_p)return 1;
    *data = arr[rd_p];
    rd_p= (rd_p+1)%max_length;
    return 0;
}

/**
  * @brief  这个函数是用来获取数据帧的校验和
	*	@param 	数组
	*	@param 	数组大小
  * @retval 返回1表示奇数个1，返回0表示偶数个1
  */
uint8_t calculateChecksum(unsigned char *data) {
    int onesCount = 0;

    for (int i = 0; i < 7; i++) {
        unsigned char byte = data[i];

        for (int j = 0; j < 8; j++) {
            onesCount += (byte >> j) & 1;
        }
    }

    return onesCount % 2;  /** 返回1表示奇数个1，返回0表示偶数个1 */
}

/**
  * @brief  这个函数是用来获取数据帧的CRC-16校验值
	*	@param 	7个字节大小的数组
  * @retval CRC校验值
  */

uint16_t calculateCRC16(uint8_t* arr, int length) {
    uint16_t crc = 0xFFFF;
    uint16_t poly = 0x8005;

    for (int i = 0; i < length; i++) {
        crc ^= (arr[i] << 8);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc & 0xFFFF;
}

/**
 * @brief   查找帧头
 *          直接操作读指针
 * @return  0：没有找到帧头，1:找到了帧头，并且当前rd_p指向0x55的下一个位置即命令代码
 */
static uint8_t recvBuff_Find_Header(void)
{
    uint8_t index;
    index = rd_p;
    /** 如果相等，则读指针和写指针指向同一位置,这里要先将他注释掉 */
    if(rd_p == wr_p) return 0;

    if(data_buff[index] != 0xAA)
    {
        rd_p = (index +1) % max_length;
        return 0;
    }
    else
    {
        /** 对应如果找到aa后，将指针加1去到下一个位 */
        index = (rd_p + 1) % max_length;
        if(data_buff[index] != 0x55)
        {
            /** 当前位找不到，加1寻找指向下一个 */
            rd_p = (index +1) % max_length;
            return 0;
        }
        else
        {
            /** 找到了当前55，所以包头的位置锁定，将读指针指向命令位 */
            rd_p = (index +1) % max_length;
            return 1;
        }
    }
}

/**
 * @brief   函数用于按反序合并两个字节为一个16位整数
 * @return  返回一个两个字节的数据
 */
uint16_t mergeBytesToUint16(uint8_t data[2]) {
    uint16_t mergedData = 0;

    for (int i = 0; i < 4; i++) {
        mergedData |= ((uint16_t)data[i] << (i * 8));
    }
    return mergedData;
}

/**
 * @brief   输入一个指针数据，用来处理参数的函数，将指针对应的数据提取四个字节出来，返回的是速度值，可能有负数。
 * @para    处理的指针数据
 * @return  返回一个四字节的数据，
 */
uint64_t mergeParametersToUint64(void) {
    /** 读取八个字节参数码，注意大端序或小端序 */
    uint64_t mergedData = 0;
    for (int i = 0; i < 8; i++) {
        rd_p = (rd_p + 1) % 128;  // 更新 rd_p
        /** 将每个字节位移到正确的位置并合并到 mergedData
         这里假设数据是按照小端序存储的，即低位字节在低地址
         如果数据是大端序，请相应调整位移量 (7 - i) * 8 */
        mergedData |= ((uint64_t)data_buff[rd_p] << (i * 8));
    }
    return mergedData;
}

/**
 * @brief   查找帧头,验证数据的正确性即验证CRC的值
 * @return  返回一个标志状态，状态代表找到帧头以及数据正确并且通过，1代表通过，0代表错误
 */
static uint8_t protocol_Check_header_CRC(void)
{
    uint8_t tmp_arr[11];
    uint8_t tmp_crc[2];
    uint16_t cal_crc16;
    uint16_t send_crc16;
    uint8_t pack_header_flag;
    uint8_t header_CRC_Ready_flat;
    pack_header_flag = recvBuff_Find_Header();
    uint8_t tmp_ptr;
    /** 没找到数据头 */
    if(0 == pack_header_flag)
    {
        return header_CRC_Ready_flat = 0;
    }
    else
    {
        /** 将指向命令的的rd_p保护起来，用于下面的直接使用 */
        tmp_ptr = rd_p;
        tmp_arr[0] = 0xAA;
        tmp_arr[1] = 0x55;
        /** 第三个位置放入命令码8位，因为在寻找包头的时候寻找结束后当前指针指向，命令码的位置 */
        tmp_arr[2] = data_buff[tmp_ptr];

        /** 对数据进行crc校验，如果校验结果正确提取出命令以及参数
        for（init;condition;increment ）循环的控制流是，首先先执行init的值，然后进行条件的判断，判断通过，执行主体for内的内同，然后再到increment中进行操作 */
        for(int i = 3; i < 11; i++)
        {
            tmp_ptr = (tmp_ptr+1) % max_length;
            tmp_arr[i] = data_buff[tmp_ptr];
        }
        /** 假设这个for循环后，返回的是参数的最后一个值
        将原始的数据包头、命令码、参数值进行计算CRC码 */
        cal_crc16 = calculateCRC16(tmp_arr,11);
        /** 取出CRC校验码 */
        for (int j = 0; j < 2; j++)
        {
            tmp_ptr = (tmp_ptr+1) % max_length;
            tmp_crc[j] = data_buff[tmp_ptr];
        }
        /** 如今tmp_ptr指向校验码高位
        将其反向合并成16位的数据 */
        send_crc16 = mergeBytesToUint16(tmp_crc);
        /** 如果发送过来的校验值不等于我收到数据后计算的校验值，则这帧数据为错误的数据，丢弃 */
        if (send_crc16 != cal_crc16)
        {

            /** 将数据指向下一组数据 */
            tmp_ptr = (tmp_ptr+1) % max_length;
            rd_p = tmp_ptr;
            return 0;
        }
        else
        {
            /** 指向这帧数据的命令字节，由于前面我已经将rd_p保护起来，如果执行到这行命令，则代表验证通过 */
            return 1;
        }
    }
}

/**
 * @brief   接收的数据处理
 * @param   void
 * @return  -1：没有找到一个正确的命令.
 */
int8_t receiving_process(void)
{
    uint8_t cmd_type = CMD_NONE;
    uint8_t header_CRC_Ready_flat;
    uint32_t tmp_para;            /** 注意，参数的有可能是负数，如果是负数时，传输过来的数据是补码 */


    while (1)
    {
        header_CRC_Ready_flat = protocol_Check_header_CRC();
        if (1 != header_CRC_Ready_flat)
        {
            return -1;
        }
            /** 已经找到了帧头以及校对了CRC,此时，rd_p指向当前数据帧的命令位 */
        else
        {

            cmd_type = data_buff[rd_p];
            switch (cmd_type)
            {
                case SetPIDValue_CMD:
                {
                    LED1_ON;
                    tmp_para = mergeParametersToUint64();
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }
                    break;

                case SpeedGo_CMD:
                {
                    LED2_ON;
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }
                    break;

                case LocationGo_CMD:
                {
                    LED3_ON;
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }

                case TurnLeft_CMD:
                {
                    LED4_ON;
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }
                    break;

                case Stop_CMD:
                {
                    LED1_ON;
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }
                    break;

                case TurnRight_CMD:
                {
                    LED2_ON;
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }
                    break;

                case Reset_CMD:
                {
                    LED3_ON;
                    /** 处理完一帧数据后将rd_p移到下一个数据帧开头 */
                    rd_p = (rd_p+3) % max_length;
                }
                    break;


                default:
                    return -1;
            }

        }
    }
}

/**
  * @brief 设置上位机的速度值/位置值
  * @param cmd：命令
  * @param data：参数指针
  * @retval 无
  */
void set_computer_Speed_Location_value(uint8_t cmd, int32_t data)
{
    uint8_t tmp_arr[9]; /** 定义并初始化数组 */
    uint16_t crc16 = 0;

    tmp_arr[0] = 0xAA;
    tmp_arr[1] = 0x55;
    tmp_arr[2] = cmd;
    tmp_arr[3] = data & 0xFF;          /** 提取最低有效字节 */
    tmp_arr[4] = (data >> 8) & 0xFF;   /**提取次低有效字节 */
    tmp_arr[5] = (data >> 16) & 0xFF;  /** 提取次高有效字节 */
    tmp_arr[6] = (data >> 24) & 0xFF;  /** 提取最高有效字节 */

    /** 计算crc16 */
    crc16 = calculateCRC16(tmp_arr,7);
    tmp_arr[7] = crc16 & 0xFF;         /** 提取CRC16的最低有效字节 */
    tmp_arr[8] = (crc16 >> 8) & 0xFF;  /** 提取CRC16的次低有效字节 */


    HAL_UART_Transmit(&UartHandle, tmp_arr, sizeof(tmp_arr), 1000);

}

/**********************************************************************************************/