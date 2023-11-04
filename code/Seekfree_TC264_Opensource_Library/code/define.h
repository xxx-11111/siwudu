/*
 * define.h
 *
 *  Created on: 2023年11月3日
 *      Author: 陈思跃
 */

#ifndef CODE_DEFINE_H_
#define CODE_DEFINE_H_
#include "zf_common_headfile.h"
//------------------------------------------------------------------------------------------------------------------
// 宏定义简介     图像部分
//------------------------------------------------------------------------------------------------------------------
#define image_heigh 120//在头文件里也有
#define image_widt 188//在头文件也有
#define find_count 700//最多找到的次数
#define threshold_max   255*5////定义膨胀和腐蚀的阈值区间
#define threshold_min   255*2////定义膨胀和腐蚀的阈值区间
#define zuoshang1 ((dir_l[i - 5] == 6||dir_l[i - 3]==6) && dir_l[i - 1] == 6 && dir_l[i] == 5 && (dir_l[i + 1] == 5 || dir_l[i + 1] == 4))
#define zuoxia1 (dir_l[i - 1] == 4 && dir_l[i] == 3 && dir_l[i + 1] == 2 && dir_l[i + 2] == 2&& dir_l[i + 3] == 2)
#define zuoxia2  (dir_l[i - 1] == 4 && dir_l[i] == 2 && (dir_l[i + 1] == 2 || dir_l[i + 1] == 3))
#define zuoxia3  (dir_l[i - 1] == 4 && dir_l[i] == 4 && (dir_l[i + 1] == 3 || dir_l[i + 1] == 2))
#define zuoxia4  (dir_l[i - 1] == 4 && dir_l[i] == 3 && (dir_l[i + 1] == 2 || dir_l[i + 1] == 3))
#define youshang1 ((dir_r[i - 2] == 6||dir_r[i - 2] == 7) && dir_r[i - 1] == 6 && dir_r[i] == 5 && (dir_r[i + 1] == 5 || dir_r[i + 1] == 4) && (dir_r[i + 2]==4||dir_r[i + 2]==5) )
#define youxia1  (dir_r[i - 1] == 4 && dir_r[i] == 3 && (dir_r[i + 1] == 2 || dir_r[i + 1] == 3 || dir_r[i + 1] == 1))
#define youxia2  (dir_r[i-1] == 4 && dir_r[i] == 4 && (dir_r[i + 1] == 2 && dir_r[i + 2] == 2))
#define pilot_row 50

//------------------------------------------------------------------------------------------------------------------
// 宏定义简介     led灯部分
//------------------------------------------------------------------------------------------------------------------
#define led_2 P22_2
#define led_3 P22_3
#define led_4 P22_4


//------------------------------------------------------------------------------------------------------------------
// 宏定义简介     编码器部分
//------------------------------------------------------------------------------------------------------------------
#define ENCODER_RIGHT   (TIM5_ENCODER)
#define ENCODER_RIGHT_A (TIM5_ENCODER_CH1_P10_3)
#define ENCODER_RIGHT_B (TIM5_ENCODER_CH2_P10_1)

#define ENCODER_LEFT   (TIM2_ENCODER)
#define ENCODER_LEFT_A (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_LEFT_B (TIM2_ENCODER_CH2_P33_6)

//------------------------------------------------------------------------------------------------------------------
// 宏定义简介     电机部分
//------------------------------------------------------------------------------------------------------------------
#define  MOTOR_LEFT_1   ATOM0_CH5_P02_5
#define  MOTOR_LEFT_2   P02_7//ATOM0_CH7_P02_7
#define  MOTOR_RIGHT_1  ATOM0_CH4_P02_4
#define  MOTOR_RIGHT_2  P02_6//ATOM0_CH6_P02_6

#define  MOTOR_PWM_MAX      9000                        //电机PWM限幅

#endif /* CODE_DEFINE_H_ */
