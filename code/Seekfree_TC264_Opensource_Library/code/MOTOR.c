/*
 * MOTOR.c
 *
 *  Created on: 2023��11��4��
 *      Author: ��˼Ծ
 */
#include "define.h"
#include "motor.h"


//------------------------------------------------------------------------------------------------------------------
// �������     ��������ʼ��
// ����˵��     void
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void EncoderInit(void)
{
    encoder_quad_init(ENCODER_RIGHT,ENCODER_RIGHT_A,ENCODER_RIGHT_B);       //��ʼ���ұ߱�����ģ��
    encoder_quad_init(ENCODER_LEFT,ENCODER_LEFT_A,ENCODER_LEFT_B);          //��ʼ����߱�����ģ��
}


//------------------------------------------------------------------------------------------------------------------
// �������     �����ʼ��
// ����˵��     void
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void MotorInit(void)
{
    pwm_init(MOTOR_LEFT_1,12500,0);                   //��ʼ������
    gpio_init(MOTOR_LEFT_2, GPO, 0, GPO_PUSH_PULL);
    pwm_init(MOTOR_RIGHT_1,12500,0);                  //��ʼ���ҵ��
    gpio_init(MOTOR_RIGHT_2, GPO, 0, GPO_PUSH_PULL);
}

//------------------------------------------------------------------------------------------------------------------
// �������     �����p_wm��
// ����˵��     pwm_left:����������p_wmֵ
//             pwm_right�������ҵ����p_wmֵ
// ���ز���     void
// ��ע��Ϣ     ռ�ձ�Ϊ�ٷ�֮��pwm_right��/PWM_DUTY_MAX*100
//             PWM_DUTY_MAX=10000
//             ���汾Ϊ�̶�50ռ�ձ�+����p_wm��һ��
//             ������10000ʱ��Ϊ50+10000/2*100/10000=100
//------------------------------------------------------------------------------------------------------------------
void MotorSetPWM(int pwm_left,int pwm_right)
{
    //��p_wm�����޷�
    if(pwm_left>MOTOR_PWM_MAX)
        pwm_left = MOTOR_PWM_MAX;
    else if(pwm_left<-MOTOR_PWM_MAX)
        pwm_left = -MOTOR_PWM_MAX;
    if(pwm_right>MOTOR_PWM_MAX)
        pwm_right = MOTOR_PWM_MAX;
    else if(pwm_right<-MOTOR_PWM_MAX)
        pwm_right = -MOTOR_PWM_MAX;
    //����
    gpio_set_level(MOTOR_LEFT_2, 1);
    pwm_set_duty(MOTOR_LEFT_1,5000+(int)(pwm_left/2));
    //�ҵ��
    gpio_set_level(MOTOR_RIGHT_2, 1);
    pwm_set_duty(MOTOR_RIGHT_1,5000-(int)(pwm_right/2));
}
