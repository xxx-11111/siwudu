/*
 * define.h
 *
 *  Created on: 2023��11��3��
 *      Author: ��˼Ծ
 */

#ifndef CODE_DEFINE_H_
#define CODE_DEFINE_H_
#include "zf_common_headfile.h"
//------------------------------------------------------------------------------------------------------------------
// �궨����     ͼ�񲿷�
//------------------------------------------------------------------------------------------------------------------
#define image_heigh 120//��ͷ�ļ���Ҳ��
#define image_widt 188//��ͷ�ļ�Ҳ��
#define find_count 700//����ҵ��Ĵ���
#define threshold_max   255*5////�������ͺ͸�ʴ����ֵ����
#define threshold_min   255*2////�������ͺ͸�ʴ����ֵ����
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
// �궨����     led�Ʋ���
//------------------------------------------------------------------------------------------------------------------
#define led_2 P22_2
#define led_3 P22_3
#define led_4 P22_4


//------------------------------------------------------------------------------------------------------------------
// �궨����     ����������
//------------------------------------------------------------------------------------------------------------------
#define ENCODER_RIGHT   (TIM5_ENCODER)
#define ENCODER_RIGHT_A (TIM5_ENCODER_CH1_P10_3)
#define ENCODER_RIGHT_B (TIM5_ENCODER_CH2_P10_1)

#define ENCODER_LEFT   (TIM2_ENCODER)
#define ENCODER_LEFT_A (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_LEFT_B (TIM2_ENCODER_CH2_P33_6)

//------------------------------------------------------------------------------------------------------------------
// �궨����     �������
//------------------------------------------------------------------------------------------------------------------
#define  MOTOR_LEFT_1   ATOM0_CH5_P02_5
#define  MOTOR_LEFT_2   P02_7//ATOM0_CH7_P02_7
#define  MOTOR_RIGHT_1  ATOM0_CH4_P02_4
#define  MOTOR_RIGHT_2  P02_6//ATOM0_CH6_P02_6

#define  MOTOR_PWM_MAX      9000                        //���PWM�޷�

#endif /* CODE_DEFINE_H_ */
