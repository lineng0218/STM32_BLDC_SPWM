//
// Created by uoc11 on 2023/12/12.
//
#include "bsp_svpwm.h"
#include "math.h"

PWM_Duty_CCR_Typedef   PWM_Channel;  //暂存三通道比较值
const int16_t hSin_Cos_Table[256]  = SIN_COS_TABLE;   //正余弦数值
int16_t DTheta = 0;                                   // 每个周期控制角度增量


//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//    Volt_Typedef V_alpha_beta = {0};
//    int16_t    sector   = 0;
//    uint16_t   uhindex  = 0;
//    int32_t    tmp_sin  = 0;
//    int32_t    tmp_cos  = 0;
//    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
//    {
//       /**目标转子频率/PWM频率，再乘以360°就是每个周期的旋转角度 */
//       /**0~360°对应0~65536，1 s16degree = 360°/65536 */
//       /** 使用定点运算，Q=15格式*/
//        DTheta += round(FREQ_RATIO * 65536);
//       /**等比例缩小，是的数据范围在10bit以内。其中高两位用于判断象限，低8位用于查表 */
//       /**对于角度值（°），判断象限可以除以90°，得到的整数部分只有0，1，2，3这4个数值 */
//        uhindex = (uint16_t)DTheta / 64;
//        /**对于s16degree，可以除以16384（64*256）。将s16degree等比例缩小以后，判断象限除以90°，右移8位，相当于除以256，例如92/90=1*/
//        sector = (uhindex & 0x300)>>8;
//
//        switch(sector)
//        {
//            // 0~90
//            case 0:
//                /**角度等比例缩小后，数值范围在0~1024（10bit以内），取低8位用于查表怕，相当于除以256求余，即是除以90°求余，如92%90=2，限制角度范围在0~90°*/
//                tmp_sin = (UREF * (int16_t)hSin_Cos_Table[( uint8_t )( uhindex )]);
//                tmp_cos = (UREF * hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
//                break;
//            case 1:
//                tmp_sin = (UREF * hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
//                tmp_cos = (UREF * -hSin_Cos_Table[( uint8_t )( uhindex )]);
//                break;
//            case 2:
//                tmp_sin = (UREF * -hSin_Cos_Table[( uint8_t )( uhindex )]);
//                tmp_cos = (UREF * -hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
//                break;
//            case 3:
//                tmp_sin = (UREF * -hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
//                tmp_cos = (UREF * hSin_Cos_Table[( uint8_t )( uhindex )]);
//                break;
//
//        }
//        V_alpha_beta.V_alpha = (int16_t) (tmp_sin >> 16);
//        V_alpha_beta.V_beta  = (int16_t) (tmp_cos >> 16);
//
//        PWMC_SetPhaseVoltage( V_alpha_beta );
//        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, PWM_Channel.CCR1);
//        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, PWM_Channel.CCR2);
//        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, PWM_Channel.CCR3);
////    printf("Ta = %d,Tb= %d,Tc= %d\n",PWM_Channel.CCR1,PWM_Channel.CCR2,PWM_Channel.CCR3);
//    }
//}

void SVPWM_Mode(void )
{
    Volt_Typedef V_alpha_beta = {0};
    int16_t    sector   = 0;
    uint16_t   uhindex  = 0;
    int32_t    tmp_sin  = 0;
    int32_t    tmp_cos  = 0;
        /**目标转子频率/PWM频率，再乘以360°就是每个周期的旋转角度 */
        /**0~360°对应0~65536，1 s16degree = 360°/65536 */
        /** 使用定点运算，Q=15格式*/
        DTheta += round(FREQ_RATIO * 65536);
        /**等比例缩小，是的数据范围在10bit以内。其中高两位用于判断象限，低8位用于查表 */
        /**对于角度值（°），判断象限可以除以90°，得到的整数部分只有0，1，2，3这4个数值 */
        uhindex = (uint16_t)DTheta / 64;
        /**对于s16degree，可以除以16384（64*256）。将s16degree等比例缩小以后，判断象限除以90°，右移8位，相当于除以256，例如92/90=1*/
        sector = (uhindex & 0x300)>>8;

        switch(sector)
        {
            // 0~90
            case 0:
                /**角度等比例缩小后，数值范围在0~1024（10bit以内），取低8位用于查表怕，相当于除以256求余，即是除以90°求余，如92%90=2，限制角度范围在0~90°*/
                tmp_sin = (UREF * (int16_t)hSin_Cos_Table[( uint8_t )( uhindex )]);
                tmp_cos = (UREF * hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
                break;
            case 1:
                tmp_sin = (UREF * hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
                tmp_cos = (UREF * -hSin_Cos_Table[( uint8_t )( uhindex )]);
                break;
            case 2:
                tmp_sin = (UREF * -hSin_Cos_Table[( uint8_t )( uhindex )]);
                tmp_cos = (UREF * -hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
                break;
            case 3:
                tmp_sin = (UREF * -hSin_Cos_Table[( uint8_t )( 0xFFu - ( uint8_t )( uhindex ) )]);
                tmp_cos = (UREF * hSin_Cos_Table[( uint8_t )( uhindex )]);
                break;

        }
        V_alpha_beta.V_alpha = (int16_t) (tmp_sin >> 16);
        V_alpha_beta.V_beta  = (int16_t) (tmp_cos >> 16);

        PWMC_SetPhaseVoltage( V_alpha_beta );
        __HAL_TIM_SET_COMPARE(&motor1_htimx_bldcm, TIM_CHANNEL_1, PWM_Channel.CCR1);
        __HAL_TIM_SET_COMPARE(&motor1_htimx_bldcm, TIM_CHANNEL_2, PWM_Channel.CCR2);
        __HAL_TIM_SET_COMPARE(&motor1_htimx_bldcm, TIM_CHANNEL_3, PWM_Channel.CCR3);
//    printf("Ta = %d,Tb= %d,Tc= %d\n",PWM_Channel.CCR1,PWM_Channel.CCR2,PWM_Channel.CCR3);
}





uint16_t PWMC_SetPhaseVoltage( Volt_Typedef Valfa_beta )
{
    int32_t wX, wY, wZ, wUAlpha, wUBeta, wTimePhA, wTimePhB, wTimePhC;

    wUAlpha = Valfa_beta.V_alpha * ( int32_t )T_SQRT3;
    wUBeta = -( Valfa_beta.V_beta * ( int32_t )( PWM_PERIOD_CYCLES ) ) * 2;

    wX = wUBeta;
    wY = ( wUBeta + wUAlpha ) / 2;
    wZ = ( wUBeta - wUAlpha ) / 2;

    if ( wY < 0 )
    {
        if ( wZ < 0 )
        {
            //SECTOR_5;
            wTimePhA = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( wY - wZ ) / ( int32_t )262144 );
            wTimePhB = wTimePhA + wZ / 131072;
            wTimePhC = wTimePhA - wY / 131072;

        }
        else /* wZ >= 0 */
        if ( wX <= 0 )
        {
            //SECTOR_4;
            wTimePhA = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( wX - wZ ) / ( int32_t )262144 );
            wTimePhB = wTimePhA + wZ / 131072;
            wTimePhC = wTimePhB - wX / 131072;
        }
        else /* wX > 0 */
        {
            //SECTOR_3;
            wTimePhA = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( wY - wX ) / ( int32_t )262144 );
            wTimePhC = wTimePhA - wY / 131072;
            wTimePhB = wTimePhC + wX / 131072;
        }
    }
    else /* wY > 0 */
    {
        if ( wZ >= 0 )
        {
            //SECTOR_2;
            wTimePhA = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( wY - wZ ) / ( int32_t )262144 );
            wTimePhB = wTimePhA + wZ / 131072;
            wTimePhC = wTimePhA - wY / 131072;
        }
        else /* wZ < 0 */
        if ( wX <= 0 )
        {
            //SECTOR_6;
            wTimePhA = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( wY - wX ) / ( int32_t )262144 );
            wTimePhC = wTimePhA - wY / 131072;
            wTimePhB = wTimePhC + wX / 131072;
        }
        else /* wX > 0 */
        {
            //SECTOR_1;
            wTimePhA = ( int32_t )( PWM_PERIOD_CYCLES ) / 4 + ( ( wX - wZ ) / ( int32_t )262144 );
            wTimePhB = wTimePhA + wZ / 131072;
            wTimePhC = wTimePhB - wX / 131072;
        }
    }

    PWM_Channel.CCR1 = wTimePhA;
    PWM_Channel.CCR2 = wTimePhB;
    PWM_Channel.CCR3 = wTimePhC;
    return 0;
}