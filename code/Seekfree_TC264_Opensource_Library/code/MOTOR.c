/*
 * MOTOR.c
 *
 *  Created on: 2023年11月4日
 *      Author: 陈思跃
 */
#include "define.h"
#include "motor.h"


//------------------------------------------------------------------------------------------------------------------
// 函数简介     编码器初始化
// 参数说明     void
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void EncoderInit(void)
{
    encoder_quad_init(ENCODER_RIGHT,ENCODER_RIGHT_A,ENCODER_RIGHT_B);       //初始化右边编码器模块
    encoder_quad_init(ENCODER_LEFT,ENCODER_LEFT_A,ENCODER_LEFT_B);          //初始化左边编码器模块
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     电机初始化
// 参数说明     void
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void MotorInit(void)
{
    pwm_init(MOTOR_LEFT_1,12500,0);                   //初始化左电机
    gpio_init(MOTOR_LEFT_2, GPO, 0, GPO_PUSH_PULL);
    pwm_init(MOTOR_RIGHT_1,12500,0);                  //初始化右电机
    gpio_init(MOTOR_RIGHT_2, GPO, 0, GPO_PUSH_PULL);
}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     电机赋p_wm波
// 参数说明     pwm_left:输入左电机的p_wm值
//             pwm_right：输入右电机的p_wm值
// 返回参数     void
// 备注信息     占空比为百分之（pwm_right）/PWM_DUTY_MAX*100
//             PWM_DUTY_MAX=10000
//             本版本为固定50占空比+输入p_wm的一半
//             当输入10000时，为50+10000/2*100/10000=100
//------------------------------------------------------------------------------------------------------------------
void MotorSetPWM(int pwm_left,int pwm_right)
{
    //对p_wm进行限幅
    if(pwm_left>MOTOR_PWM_MAX)
        pwm_left = MOTOR_PWM_MAX;
    else if(pwm_left<-MOTOR_PWM_MAX)
        pwm_left = -MOTOR_PWM_MAX;
    if(pwm_right>MOTOR_PWM_MAX)
        pwm_right = MOTOR_PWM_MAX;
    else if(pwm_right<-MOTOR_PWM_MAX)
        pwm_right = -MOTOR_PWM_MAX;
    //左电机
    gpio_set_level(MOTOR_LEFT_2, 1);
    pwm_set_duty(MOTOR_LEFT_1,5000+(int)(pwm_left/2));
    //右电机
    gpio_set_level(MOTOR_RIGHT_2, 1);
    pwm_set_duty(MOTOR_RIGHT_1,5000-(int)(pwm_right/2));
}
