/*
 * IMAGE.h
 *
 *  Created on: 2023年11月3日
 *      Author: 陈思跃
 */


#ifndef CODE_IMAGE_H_
#define CODE_IMAGE_H_

#include "zf_common_headfile.h"
#include "define.h"

uint8 limit_a_b(uint8 x, uint8 a, uint8 b);
int my_abs(int value);
int otsu(uint8 image[image_heigh][image_widt]);
void image_filter(uint8 image[image_heigh][image_widt]);
void image_draw_rectan(uint8 image[image_heigh][image_widt]);
unsigned int get_start_point(unsigned int start_row, uint8 image[120][188],int* start_point_l,int* start_point_r);
void Eight_fields(int break_number, uint8(*image)[image_widt], int* l_stastic, int* r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, int* high_max,
        uint8 points_l[find_count][2], uint8 points_r[find_count][2], uint8 dir_r[find_count], uint8 dir_l[find_count]
);
void get_left(int total,uint8 border[120],uint8 points[find_count][2]);
float Slope_Calculate(uint8 begin, uint8 end, uint8* border);
float calculate_xielv(uint8 start, uint8 end, uint8* border, float* intercept);
void calculate_s_i(uint8 start, uint8 end, uint8* border, float* slope_rate, float* intercept);
void cross_fill_ing(uint8* l_border, uint8* r_border, int total_num_l, int total_num_r,
    int* dir_l, int* dir_r, int(*points_l)[2], int(*points_r)[2], uint8(*image)[image_widt]);

#endif /* CODE_IMAGE_H_ */
