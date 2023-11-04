#include <iostream>
#include <opencv2/opencv.hpp>
#include"image.h"
#include <conio.h> 
#include "iomanip"
#include "sstream"
using namespace cv;
using namespace std;

int start_point_l[2] = { 0 };//左边起点的x，y值
int start_point_r[2] = { 0 };//右边起点的x，y值

#define find_count_max 700

//存放点的x，y坐标
int points_l[find_count_max][2] = { {  0 } };//左线
int points_r[find_count_max][2] = { {  0 } };//右线
int dir_r[find_count_max] = { 0 };//用来存储右边生长方向
int dir_l[find_count_max] = { 0 };//用来存储左边生长方向
int data_stastics_l = 0;//统计左边找到点的个数
int data_stastics_r = 0;//统计右边找到点的个数
int hightest = 0;//最高点

int r_border[120];//左线数组
int l_border[120];//右线数组
int center_line[120];//中线数组

int main()
{
    Mat src = Mat::zeros(120, 188, CV_8UC1);
    std::string filename = "P00222.bmp";
   
    int temp = 0;
    scanf("%d", &temp);
    std::string num_str = filename.substr(1, filename.find(".bmp") - 1); // 去除开头的 'P' 和后缀 '.bmp'
    int num = std::stoi(num_str); // 将数字字符串解析为整数
    num = temp; // 将数字加一
    std::stringstream ss; // 定义 stringstream 类型变量, 用来将数字转为字符串并添加前导零
    ss << std::setw(5) << std::setfill('0') << num;
    std::string new_num = ss.str(); // 将 stringstream 类型变量转为字符串
    filename.replace(filename.find_first_of("0123456789"), num_str.length(), new_num);

    readImage(filename, src);
  

    //储存原始图像的数组
    uchar init[120][188];
    //将图像储存在二维数组里面
    save_to_array(src, init);
    Mat dst1(120, 188, CV_8UC1);;
    //将二维数组转化为图片，验证数组是否正确
    arrar_to_image(dst1, init);
    //展示原始图片
    showing_image("原始图像", dst1);
    //大津法计算阈值
    int threshold = otsu(init);
    //根据阈值进行二值化
    Binaryzation(120, 188, threshold, init);
    //滤波
    image_filter(init);
    //套黑框
    image_draw_rectan(init);
    //处理效果
    Mat dst3 = Mat::zeros(120, 188, CV_8UC1);
    arrar_to_image(dst3, init);
    showing_image("二值化", dst3);
    //复制二值化图像
    uchar bin_image[120][188] = { 0 };
    for (int i = 0; i < 120; i++)
    {
        for (int j = 0; j < 188; j++)
        {
            bin_image[i][j] = init[i][j];
        }
    }
    //求开始点位置
    get_start_point(115, init, start_point_l, start_point_r);
    //打印初始点位置
    printf("左边初始点为（%d,%d）\n", start_point_l[0], start_point_l[1]);
    printf("右边初始点为（%d,%d）\n", start_point_r[0], start_point_r[1]);
    //八领域处理
    Eight_fields(find_count_max, init, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest, points_l, points_r, dir_r, dir_l);
    //打印找到的边界点个数
    printf("左边界一共找到%d\n", data_stastics_l);
    printf("右边界一共找到%d\n", data_stastics_r);


    //打印找到的点
    /* printf_r_point(data_stastics_r,points_r);*/
    //printf_l_point(data_stastics_l, points_l);
    //画出边界及初始点
    show_all_point(data_stastics_l, data_stastics_r, points_l, points_r, init, start_point_l, start_point_r);
    //所有边界效果展示
      Mat dst4 = Mat::zeros(120, 188, CV_8UC1);
    arrar_to_image(dst4, init);
    showing_image("所有边界点以及初始点", dst4);
    //巡线
    get_left(data_stastics_l, l_border, points_l);
    get_right(data_stastics_r, r_border, points_r);
    //打印左右边线
    /*show_alone_line(r_border, l_border);*/
    //打印生长方向,以及每个种子的行数，列数
    //printf_r_growth_direction(data_stastics_r, dir_r, points_r);
    printf_l_growth_direction(data_stastics_l, dir_l, points_l);
    //巡线完在新数组储存
    uchar alo_line[120][188] = { 0 };
    alone_line(r_border, l_border, alo_line);

    //将线画出
       Mat dst5 = Mat::zeros(120, 188, CV_8UC1);
    arrar_to_image(dst5, alo_line);
    showing_image("一行一点", dst5);
    //补线
    //cross_fill_front(l_border, r_border, data_stastics_l, data_stastics_r,
    //    dir_l, dir_r, points_l, points_r, alo_line,bin_image);//十字中
    cross_fill_ing(l_border, r_border, data_stastics_l, data_stastics_r,
        dir_l, dir_r, points_l, points_r, alo_line);//十字前
    //展示补线
    alone_line(r_border, l_border, alo_line);
    //补上中线
    /*show_minline(center_line, r_border, l_border, alo_line);*/
    Mat dst6 = Mat::zeros(120, 188, CV_8UC1);
    arrar_to_image(dst6, alo_line);
    showing_image("找突变点+补线", dst6);
    cv:: waitKey(0);
   
    return 0;
}