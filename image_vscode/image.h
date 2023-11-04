#ifndef __image_H
#define __iamge_H
#define find_count 700
#define chuankou_x 1504
#define chuankou_y 960
#include <string>
unsigned int get_start_point(unsigned int start_row, uchar init[120][188], int* start_point_l, int* start_point_r);
int otsu(uchar init[120][188]);
void Binaryzation(int height, int width, int threshold, uchar init[120][188]);
int my_abs(int value);
void Eight_fields(int break_flag, uchar(*image)[188], int* l_stastic, int* r_stastic, int l_start_x, int l_start_y, int r_start_x, int r_start_y, int* hightest,
	int points_l[find_count][2], int points_r[find_count][2], int dir_r[find_count], int dir_l[find_count]
);
void image_draw_rectan(uchar(*image)[188]);
void get_right(int total_R, int r_border[120], int points_r[500][2]);
void get_left(int total_L, int l_border[120], int points_l[500][2]);
void image_filter(uchar(*bin_image)[188]);
void cross_fill_ing(int* l_border, int* r_border, int total_num_l, int total_num_r,
	int* dir_l, int* dir_r, int(*points_l)[2], int(*points_r)[2], uchar(*image)[188]);//Ê®×ÖÇ°
void calculate_s_i(int start, int end, int* border, float* slope_rate, float* intercept);
float Slope_Calculate(int begin, int end, int* border);
cv::Mat save_to_array(cv::Mat& mat_img, uchar(*image)[188]);
void arrar_to_image(cv::Mat& mat_img, uchar(*image)[188]);
void showing_image(const std::string& str, cv::Mat& mat_img);
void printf_r_growth_direction(int data_stastics_r, int* dir_r, int(*points_r)[2]);
void printf_l_growth_direction(int data_stastics_l, int* dir_l, int(*points_l)[2]);
void printf_r_point(int data_stastics_r, int(*points_r)[2]);
void printf_l_point(int data_stastics_l, int(*points_l)[2]);
void show_all_point(int data_stastics_l, int data_stastics_r, int(*points_l)[2], int(*points_r)[2], uchar(*all_boundary)[188], int* start_point_l, int* start_point_r);
void alone_line(int* r_border, int* l_border, uchar(*Final_version)[188]);
void printf_alo_r_point(int* points_r);
void printf_alo_l_point(int* points_l);
void show_minline(int* center_line, int* r_border, int* l_border, uchar(*Final_version)[188]);
float calculate_xielv(int start, int end, int* border, float* intercept);
void show_alone_line(int* r_border, int* l_border);
void modify_num(int num, std::string& filename);
void readImage(std::string& filename, cv::Mat& output);
#endif



