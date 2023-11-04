/*
 * IMAGE.c
 *
 *  Created on: 2023年11月3日
 *      Author: 陈思跃
 */

#include "zf_common_headfile.h"
#include "IMAGE.h"
#include "define.h"

//------------------------------------------------------------------------------------------------------------------
// 函数简介     取绝对值
// 参数说明     需要取绝对值的in_t型参数
// 返回参数     取绝对值
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int my_abs(int value)
{
if (value >= 0) return value;
else return -value;
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     限幅
// 参数说明     x:比较值
// 参数说明     a:下值
// 参数说明     b:上值
// 返回参数     限幅结果
// 备注信息
//------------------------------------------------------------------------------------------------------------------
uint8 limit_a_b(uint8 x, uint8 a, uint8 b)
{
    if (x < a) x = a;
    if (x > b) x = b;
    return x;
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     大津法二值化
// 参数说明     uint8类型的二维数组
// 返回参数     阈值
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int otsu(uint8 image[image_heigh][image_widt])
{
    // 大津法算法参数
    int width = 188;
    int height = 120;
    int hist[256] = { 0 };//直方图分布图
    double threshold = 0.0;
    double var_max = 0.0;

    // 统计像素值直方图
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel = image[y][x];
            hist[pixel]++;
        }
    }
    for (int i = 1; i < 255; i++) {
        int n1 = 0;
        int n2 = 0;
        int sum1 = 0;
        int sum2 = 0;
        double w1 = 0.0;
        double w2 = 0.0;
        double mean1 = 0.0;
        double mean2 = 0.0;
        double var_between = 0.0;
        double var_within = 0.0;

        for (int j = 0; j < i; j++) {
            n1 += hist[j];
            sum1 += j * hist[j];
        }

        for (int j = i; j < 256; j++) {
            n2 += hist[j];
            sum2 += j * hist[j];
        }

        if (n1 == 0 || n2 == 0) {
            continue;
        }

        w1 = (double)(n1) / (double)(height * width);
        w2 = (double)(n2) / (double)(height * width);

        mean1 = (double)(sum1) / (double)(n1);
        mean2 = (double)(sum2) / (double)(n2);

        var_between = w1 * w2 * pow(mean1 - mean2, 2);

        for (int j = 0; j < i; j++) {
            var_within += pow(j - mean1, 2) * hist[j];
        }
        for (int j = i; j < 256; j++) {
            var_within += pow(j - mean2, 2) * hist[j];
        }
        if (var_between > var_max) {
            var_max = var_between;
            threshold = (float)i;
        }
    }

    for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (image[y][x] > threshold) { image[y][x] = 255; }
                else { image[y][x] = 0; }
            }
        }
    return (int)threshold;
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     对图像进行滤波
// 参数说明     uint8类型的二维数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void image_filter(uint8 image[image_heigh][image_widt])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
    int i, j;
    int num = 0;


    for (i = 1; i < 120 - 1; i++)
    {
        for (j = 1; j < (188 - 1); j++)
        {
            //统计八个方向的像素值
            num =
                image[i - 1][j - 1] +image[i - 1][j] + image[i - 1][j + 1]
                + image[i][j - 1] + image[i][j + 1]
                + image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1];
            if (num >= threshold_max &&image[i][j] == 0)
            {
                image[i][j] = 255;//白  可以搞成宏定义，方便更改
            }
            if (num <= threshold_min && image[i][j] == 255)
            {
                image[i][j] = 0;//黑
            }
        }
    }
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     外围画黑框
// 参数说明     uint8类型的二维数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void image_draw_rectan(uint8 image[image_heigh][image_widt])
{

    int i = 0;
    for (i = 0; i < 120; i++)
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][188 - 1] = 0;
        image[i][188 - 2] = 0;

    }
    for (i = 0; i < 188; i++)
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;

    }
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     寻找大津法起点
// 参数说明     start_row：开始的行数
//             image[120][188]:传入的图像
//             *start_point_l：左边起点的指针，传入地址即可
//             *start_point_r：右边起点的指针，传入地址即可
// 返回参数     void
// 备注信息     二值化后，从底行中间往左右找第一个黑点
//------------------------------------------------------------------------------------------------------------------
unsigned int get_start_point(unsigned int start_row, uint8 image[120][188],int* start_point_l,int* start_point_r)
{
    unsigned int i = 0, l_found = 0, r_found = 0;
    //清零
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y

    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y

    //从中间往左边，先找起点
    for (i = 188 / 2; i > 2; i--)
    {
        start_point_l[0] = i-1;//列
        start_point_l[1] = start_row;//行
        if (image[start_row][i] == 255 && image[start_row][i - 1] == 0)
        {
            //printf("找到左边起点image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }

    for (i = 188 / 2; i < 187; i++)
    {
        start_point_r[0] = i+1;//x
        start_point_r[1] = start_row;//y
        if (image[start_row][i] == 255 && image[start_row][i + 1] == 0)
        {
            //printf("找到右边起点image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }

    if (l_found && r_found)return 1;
    else {
        //printf("未找到起点\n");
        return 0;
    }
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     八邻域找点
// 参数说明     break_number：最多需要循环的次数
//             (*image)[image_w]：进行找点的二值图像数组,填入数组名称即可
//             *l_stastic：统计左边数据，输出找到左点的数量
//             *r_stastic：统计右边数据，输出找到左点的数量
//             l_start_x：左边起点横坐标
//             l_start_y：左边起点纵坐标
//             r_start_x：右边起点横坐标
//             r_start_y：右边起点纵坐标
//             high_max：循环结束所得到的最高高度
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Eight_fields(int break_number, uint8(*image)[image_widt], int* l_stastic, int* r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, int* high_max,
        uint8 points_l[find_count][2], uint8 points_r[find_count][2], uint8 dir_r[find_count], uint8 dir_l[find_count]
)
{
    int i = 0, j = 0;
    //左边变量
    uint8 search_filds_l[8][2] = { {  0 } };
    int index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = { 0 };
    int l_data_statics;//统计左边
    //定义八个邻域
    static int seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是顺时针

    //右边变量
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//中心坐标点
    int index_r = 0;//索引下标
    uint8 temp_r[8][2] = { {  0 } };
    int r_data_statics;//统计右边
    //定义八个邻域
    static int seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是逆时针

    l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来


    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

    printf("\n函数内的左边起始点是(%d,%d)\n", center_point_l[0], center_point_l[1]);
    printf("函数内的右边起始点是(%d,%d)\n", center_point_r[0], center_point_r[1]);

    //开启邻域循环
    while (break_number--)
    {
        /*---------------------------------------------对矩阵进行初始化---------------------------------------*/
        //左边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//索引加一


        //右边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        /*printf("\n第一轮的右边中心点为（%d，%d）\n", points_r[r_data_statics][0], points_r[r_data_statics][1]);*/

        /*------------------------------------------左边界巡线------------------------------------------------------*/
        index_l = 0;//左边界标志位置零
        for (i = 0; i < 8; i++)//可以临时储存八次，使用前清零
        {
            temp_l[i][0] = 0;
            temp_l[i][1] = 0;
        }
        /*printf("\n(1)%d\n", image[3][111]);
        printf("(2)%d\n", image[4][111]);
        printf("(3)%d\n", image[4][110]);
        printf("(4)%d\n", image[4][109]);
        printf("(5)%d\n", image[3][109]);*/
        //在八邻域内判断边界点
        for (i = 0; i < 8; i++)//八个领域进行判断
        {
            //坐标点为（y，x）顺时针从黑跳到白
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                /*printf("\n大循环为%d，第%d次的前数为%d\n", break_flag, i + 1, image[search_filds_l[i][1]][search_filds_l[i][0]]);
                printf("大循环为%d，第%d次的后数为%d\n", break_flag, i + 1, image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]]);*/
                temp_l[index_l][0] = search_filds_l[(i)][0];//八邻域找到的边界（黑点）的x坐标赋给临时变量
                temp_l[index_l][1] = search_filds_l[(i)][1];//八邻域找到的边界（黑点）的y坐标赋给临时变量
                index_l++;//找到边界标志位
            /*  printf("index_l为%d\n", index_l);*/
                dir_l[l_data_statics - 1] = (uint8)(i);//记录生长方向（索引此前已经加过1）
            }
            if (index_l)//如果边界标志位非0（八个邻域内最少找到一个边界点）
            {
                //更新坐标点
                center_point_l[0] = temp_l[0][0];//将临时坐标赋给中心点
                center_point_l[1] = temp_l[0][1];//将临时坐标赋给中心点
                //找到八个邻域中确认的中心点最左下的部分
                for (j = 0; j < index_l; j++)
                {
                    if (center_point_l[1] > temp_l[j][1])
                    {
                        center_point_l[0] = temp_l[j][0];//x
                        center_point_l[1] = temp_l[j][1];//y
                    }
                }
            }
        }
        //左边或右边连续是同一个点
        if ((points_r[r_data_statics][0] == points_r[r_data_statics - 1][0] && points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            || (points_l[l_data_statics - 1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics - 1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics - 1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics - 1][1] == points_l[l_data_statics - 3][1]))
        {
            printf("三次进入同一个点，退出\n");
            break;
        }
        if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2))
        {
            printf("\n左右相遇退出\n");
            *high_max = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
            //printf("\n在y=%d处退出\n",*hightest);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
            /*printf("\n如果左边比右边高了，左边等待右边\n");
            printf("那右边是%d\n",points_r[r_data_statics][1]);
            printf("而左边是%d\n", points_l[l_data_statics-1][1]);*/
            continue;//如果左边比右边高了，左边等待右边
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
        {
            printf("\n左边开始向下了，等待右边，等待中... \n");
            center_point_l[0] = points_l[l_data_statics - 1][0];//x
            center_point_l[1] = points_l[l_data_statics - 1][1];//y
            l_data_statics--;
        }
        r_data_statics++;//索引加一
        index_r = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_r[i][0] = 0;//先清零，后使用
            temp_r[i][1] = 0;//先清零，后使用
        }
        //右边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//索引加一
                dir_r[r_data_statics - 1] = (uint8)(i);//记录生长方向
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {
                //更新坐标点
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }
            }
        }
    }

    //取出循环次数
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     从八邻域边界里提取需要的边线
// 参数说明     total：八领域找到的点的数量
//             border：存储边界的数组
//             points：找到的点的储存数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void get_left(int total,uint8 border[120],uint8 points[find_count][2])
{
    int i = 0;
    int j = 0;
    int h = 0;
    //初始化
    for (i = 0; i < 120; i++)
    {
        border[i] = 1;
    }
    h = 110;
    //左边
    for (j = 0; j < total; j++)//之前左边找到几个就循环几次
    {
        //printf("%d\n", j);
        if (points[j][1] == h)//遍历找到最高处的点
        {
            border[h] = points[j][0] + 1;//左边界数组的最高一行为当前的x值+偏移量
        }
        else continue; //每行只取一个点，没找到就跳出循环继续找
        h--;//找到的情况下将高度降下
        if (h == 0)//降到最后一行，退出
        {
            break;//到最后一行退出
        }
    }
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     最小二乘法求斜率
// 参数说明     begin：开始的行数
//             end：终止的行数
//             * border：边界储存的数组
// 返回参数     斜率
// 备注信息
//------------------------------------------------------------------------------------------------------------------
float Slope_Calculate(uint8 begin, uint8 end, uint8* border)
{
    float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
    int i = 0;
    float result = 0;

    static float resultlast;

    for (i = begin; i < end; i++)
    {
        xsum += i;
        ysum += border[i];
        xysum += i * (border[i]);
        x2sum += i * i;
    }

    if ((end - begin) * x2sum - xsum * xsum) //判断除数是否为零
    {
        result = ((end - begin) * xysum - xsum * ysum) / ((end - begin) * x2sum - xsum * xsum);
        resultlast = result;
    }
    else
    {
        result = resultlast;
    }
    return result;
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     计算截距和斜率
// 参数说明     begin：开始的行数
//             end：终止的行数
//             * slope_rate：斜率
//             * intercept ：截距
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void calculate_s_i(uint8 start, uint8 end, uint8* border, float* slope_rate, float* intercept)
{
    int i, num = 0;
    int xsum = 0, ysum = 0;
    float y_average, x_average;

    num = 0;
    xsum = 0;
    ysum = 0;
    y_average = 0;
    x_average = 0;
    for (i = start; i < end; i++)
    {
        xsum += i;
        ysum += border[i];
        num++;
    }
    //计算各个平均数
    if (num)
    {
        x_average = (float)(xsum / num);
        y_average = (float)(ysum / num);

    }
    /*计算斜率*/
    *slope_rate = Slope_Calculate(start, end, border);//斜率
    *intercept = y_average - (*slope_rate) * x_average;//截距
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     计算两点间斜率
// 参数说明     start：开始行数
//             end：结束行数
//             * border：边界数组
//             * intercept：斜率指针
// 返回参数     float型斜率
// 备注信息
//------------------------------------------------------------------------------------------------------------------
float calculate_xielv(uint8 start, uint8 end, uint8* border, float* intercept)
{
    float xielv = 0;

    xielv = (float)( (border[start] - border[end])*100 / (start - end));
    *intercept = border[start] - xielv * start/100;
    //printf("\nx差值:%d\t", start - end);
    //printf("y差值:%d\t", (border[start] - border[end]));
    //printf("斜率为:%lf\n", xielv);
    //printf("截距为:%lf\n", *intercept);
    return xielv;
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     判断元素
// 参数说明     *l_border：左边界储存的数组
//             *r_border：右边界储存的数组
//             *total_num_l：八领域找到的左边的点的数量的指针
//             *total_num_r：八领域找到的右边的点的数量的指针
//             *dir_l：八领域左边的生长方向的储存数组
//             *dir_r：八领域右边的生长方向的储存数组
//             (*points_l)[2]：八领域左边找到的点的储存数组
//             (*points_r)[2]：八领域右边找到的点的储存数组
//             uint8(*image)[188]：需要处理的图像的二维数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void cross_fill_ing(uint8* l_border, uint8* r_border, int total_num_l, int total_num_r,
    int* dir_l, int* dir_r, int(*points_l)[2], int(*points_r)[2], uint8(*image)[image_widt])
{
    int i;
    int break_num_l_x = 0;
    int break_num_r_x = 0;
    int break_num_l_y = 0;
    int break_num_r_y = 0;
    int break_num_below_l_x = 0;
    int break_num_below_r_x = 0;
    int break_num_below_l_y = 0;
    int break_num_below_r_y = 0;
    uint8 start, end;
    float slope_l_rate = 0, intercept_l = 0;
    float slope_r_rate = 0, intercept_r = 0;


    /*for (int i = 0; i < 120; i++)
    {
        printf("函数内左边的第%d行是（%d,%d)\n", i, i, l_border[i]);
    }*/
    for (i = 0; i < total_num_l; i++)//循环左边找到的点
    {
        if ((zuoshang1) && (points_l[i][1] >= 20))
        {
            printf("\nzuoshang:%d", points_l[i][1]);
            break_num_l_y = points_l[i][1] - 2;//传递y坐标
            break_num_l_x = l_border[break_num_l_y];
            printf("\n左边上面找到了:(");
            printf("%d,", break_num_l_y);
            printf("%d)\n", break_num_l_x);
            break;
        }
    }
    image[break_num_l_y][break_num_l_x] = 180;
    image[break_num_l_y + 1][break_num_l_x + 1] = 180;
    image[break_num_l_y - 1][break_num_l_x - 1] = 180;
    image[break_num_l_y + 1][break_num_l_x - 1] = 180;
    image[break_num_l_y - 1][break_num_l_x + 1] = 180;

    for (i = 0; i < total_num_l; i++)//循环左边找到的点
    {

        if ((zuoxia1||zuoxia2||zuoxia3||zuoxia4) && (points_l[i][1] >= 60))
        {
            printf("\nzuoxia:%d", points_l[i][1]);
            break_num_below_l_y = points_l[i][1] ;//传递y坐标
            break_num_below_l_x = l_border[break_num_below_l_y];
            printf("\n左边下面找到了:(");
            printf("%d,", break_num_below_l_y);
            printf("%d)\n", break_num_below_l_x);
            break;
        }
    }
    image[break_num_below_l_y][break_num_below_l_x] = 180;
    image[break_num_below_l_y + 1][break_num_below_l_x + 1] = 180;
    image[break_num_below_l_y - 1][break_num_below_l_x - 1] = 180;
    image[break_num_below_l_y + 1][break_num_below_l_x - 1] = 180;
    image[break_num_below_l_y - 1][break_num_below_l_x + 1] = 180;

    for (i = 0; i < total_num_r; i++)
    {
        /*printf("dir_r[%d]为%d\t", i, dir_r[i]);
        printf("i为%d时，dir_r[i - 1] =%d，dir_r[i] =%d，dir_r[i + 1] =%d\n", i, dir_r[i - 1], dir_r[i], dir_r[i + 1]);*/
        if (youshang1&& (points_r[i][1] >= 20))
        {
            printf("\nyoushang:%d", points_r[i][1]);
            break_num_r_y = points_r[i][1] - 2;//传递y坐标
            break_num_r_x = r_border[break_num_r_y];
            printf("\n右边上面找到了:(");
            printf("%d,", break_num_r_y);
            printf("%d)\n", break_num_r_x);
            break;
        }
    }
    image[break_num_r_y][break_num_r_x] = 180;
    image[break_num_r_y + 1][break_num_r_x + 1] = 180;
    image[break_num_r_y - 1][break_num_r_x - 1] = 180;
    image[break_num_r_y + 1][break_num_r_x - 1] = 180;
    image[break_num_r_y - 1][break_num_r_x + 1] = 180;


    for (i = 0; i < total_num_r; i++)//循环左边找到的点
    {
        if ((youxia1||youxia2) && (points_r[i][1] >= 20))
        {
            printf("\nyouxia:%d", points_r[i][1]);
            break_num_below_r_y = points_r[i][1] ;//传递y坐标
            break_num_below_r_x = r_border[break_num_below_r_y];
            printf("\n右边下面找到了:(");
            printf("%d,", break_num_below_r_y);
            printf("%d)\n", break_num_below_r_x);
            break;
        }
    }
    image[break_num_below_r_y][break_num_below_r_x] = 180;
    image[break_num_below_r_y + 1][break_num_below_r_x + 1] = 180;
    image[break_num_below_r_y - 1][break_num_below_r_x - 1] = 180;
    image[break_num_below_r_y + 1][break_num_below_r_x - 1] = 180;
    image[break_num_below_r_y - 1][break_num_below_r_x + 1] = 180;

    if (break_num_l_y && break_num_r_y &&!(break_num_below_r_y))//满足十字前特征(只有右边两点）（没有左下）
    {
        printf("\n注意：只有右边两点\n");
        //计算斜率
        start = break_num_r_y - 15;//起点
        start = limit_a_b(start, 0, 120);//限幅
        end = break_num_r_y - 5;//终点
        calculate_s_i(start, end, r_border, &slope_r_rate, &intercept_r);
        printf("slope_r_rate:%lf\n intercept_r:%lf\n", slope_r_rate, intercept_r);
        for (i = break_num_r_y - 5; i < 120 - 1; i++)
        {
            r_border[i] = slope_r_rate * (i)+intercept_r;
            r_border[i] = limit_a_b(r_border[i], 2, 186);
        }
    }

    if (break_num_below_r_y && break_num_r_y && break_num_below_r_y && !(break_num_l_y))//满足十字前特征（右边两点和左下）
    {
        printf("\n注意：右边两点和左下\n");
        //计算右边斜率，利用两点间斜率补线
        start = break_num_r_y - 1;
        start = limit_a_b(start, 0, 120);
        end = break_num_below_r_y + 1;
        slope_r_rate = calculate_xielv(start, end, r_border, &intercept_r);
        printf("\n右边两点间斜率补线：");
        printf("\nstart:%d\t end:%d", start, end);
        printf("\n右上x：%d,右下x：%d\n", r_border[break_num_r_y], r_border[break_num_below_r_y]);
        printf("xielv:%lf\t intercept_l:%lf\n", slope_r_rate, intercept_r);
        for (i = break_num_r_y - 3; i < break_num_below_r_y + 5; i++)
        {
            r_border[i] = slope_r_rate * (i) / 100 + intercept_r;//y = kx+b
            /*printf("y:%d   x:%d\n", i, r_border[i]);*/
            r_border[i] = limit_a_b(r_border[i], 2, 186);//限幅
        }

        //左下利用最小二乘法进行拟和
        start = break_num_below_l_y + 2 ;
        start = limit_a_b(start, 0, 120);
        end = break_num_below_l_y +15;
        calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
        printf("\n左下最小二乘法补线：");
        printf("\nslope_l_rate:%lf\n intercept_l:%lf\n", slope_l_rate, intercept_l);
        for (i = break_num_r_y - 2; i < break_num_below_r_y; i++)
        {
            l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
            l_border[i] = limit_a_b(l_border[i], 2, 186);//限幅
        }
    }

    if (break_num_below_r_y && break_num_r_y && break_num_below_r_y && break_num_l_y )//满足十字前特征（四点都有）
    {
        printf("\n注意：四点都有\n");
        start = break_num_l_y - 1;
        start = limit_a_b(start, 0, 120);
        end = break_num_below_l_y + 1;
        slope_l_rate = calculate_xielv(start, end, l_border, &intercept_l);
        printf("\n左边两点间斜率补线：");
        printf("\nstart_y:%d\t end_y:%d", start, end);
        printf("\n左上x：%d\t 左下x：%d\n", l_border[break_num_l_y], l_border[break_num_below_l_y]);
        printf("xielv:%lf\t intercept_l:%lf\n", slope_l_rate, intercept_l);
        for (i = break_num_l_y - 3; i < break_num_below_l_y + 5; i++)
        {
            l_border[i] = slope_l_rate * (i) / 100 + intercept_l;//y = kx+b
            l_border[i] = limit_a_b(l_border[i], 2, 186);//限幅
        }


        start = break_num_r_y - 1;
        start = limit_a_b(start, 0, 120);
        end = break_num_below_r_y + 1;
        slope_r_rate = calculate_xielv(start, end, r_border, &intercept_r);
        printf("\n右边两点间斜率补线：");
        printf("\nstart:%d\t end:%d", start, end);
        printf("\n右上x：%d,右下x：%d\n", r_border[break_num_r_y], r_border[break_num_below_r_y]);
        printf("xielv:%lf\t intercept_l:%lf\n", slope_r_rate, intercept_r);
        for (i = break_num_r_y - 3; i < break_num_below_r_y + 5; i++)
        {
            r_border[i] = slope_r_rate * (i) / 100 + intercept_r;//y = kx+b
            r_border[i] = limit_a_b(r_border[i], 2, 186);//限幅
        }


    }

    for (i = 0; i < 188; i++)
    {
        image[pilot_row][i] = 230;
    }
    printf("\n领航线左边为%d", l_border[pilot_row]);
    printf("\n领航线右边为%d", r_border[pilot_row]);
    int pilot_min = ((r_border[pilot_row] - l_border[pilot_row]) / 2) + l_border[pilot_row];
    printf("\n领航线中点为%d", pilot_min);
    image[pilot_row][93] = 120;
    image[pilot_row][pilot_min] = 0;
    printf("\n差值为%d", 93 - pilot_min);

}

