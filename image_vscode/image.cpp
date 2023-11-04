#include <iostream>
#include <opencv2/opencv.hpp>
//#include "b.cpp"
#include <string>
#include "image.h"
using namespace cv;
using namespace std;
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
// 函数简介     限幅
// 参数说明     比较值，下值，上值
// 返回参数     结果值
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int limit_a_b(int x, int a, int b)
{
	if (x < a) x = a;
	if (x > b) x = b;
	return x;
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     取绝对值
// 参数说明     计算值
// 返回参数     结果值
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int my_abs(int value)
{
if (value >= 0) return value;
else return -value;
}
//extern int init[120][188];
/*
函数名称：void get_start_point(uint8 start_row)
功能说明：
参数说明：输入任意行数
函数返回：无
修改时间：2022年9月8日
备    注：
example：  get_start_point(image_h-2)
 */

 //------------------------------------------------------------------------------------------------------------------
 // 函数简介     寻找两个边界的边界点作为八邻域循环的起始点
 // 参数说明     初始行，图像数组，左边起点数组，右边起点数组
 // 返回参数     1：至少找到一个;   0:都没找到
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
unsigned int get_start_point(unsigned int start_row, uchar init[120][188],int* start_point_l,int* start_point_r)
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
		if (init[start_row][i] == 255 && init[start_row][i - 1] == 0)
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
		if (init[start_row][i] == 255 && init[start_row][i + 1] == 0)
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
// 函数简介     寻找最优阈值
// 参数说明     图像数组
// 返回参数     阈值
// 备注信息
//------------------------------------------------------------------------------------------------------------------

int otsu(uchar init[120][188])
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
            int pixel = init[y][x];
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

        w1 = double(n1) / double(height * width);
        w2 = double(n2) / double(height * width);

        mean1 = double(sum1) / double(n1);
        mean2 = double(sum2) / double(n2);

        var_between = w1 * w2 * pow(mean1 - mean2, 2);

        for (int j = 0; j < i; j++) {
            var_within += pow(j - mean1, 2) * hist[j];
        }
        for (int j = i; j < 256; j++) {
            var_within += pow(j - mean2, 2) * hist[j];
        }
        if (var_between > var_max) {
            var_max = var_between;
            threshold = i;
        }
    }
    return (int)threshold;
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     利用阈值二值化
// 参数说明     图像高度，图像宽度，阈值，图像数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Binaryzation(int height,int width,int threshold, uchar init[120][188])
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (init[y][x] > threshold) { init[y][x] = 255; }
            else { init[y][x] = 0; }
        }
    }
}





 //------------------------------------------------------------------------------------------------------------------
 // 函数简介     八领域找点
 // 参数说明     break_flag_r			：最多需要循环的次数
//				(*image)[image_w]		：需要进行找点的图像数组，必须是二值图, 填入数组名称即可
//				 * l_stastic			：统计左边数据，用来输入初始数组成员的序号和取出循环次数
//				 * r_stastic			：统计右边数据，用来输入初始数组成员的序号和取出循环次数
//				 l_start_x				：左边起点横坐标
//			  	 l_start_y				：左边起点纵坐标
//			  	 r_start_x				：右边起点横坐标
//			 	 r_start_y				：右边起点纵坐标
//				 hightest				：循环结束所得到的最高高度
//				 int points_l[find_count][2]：左边的点
//				 int points_r[find_count][2]：右边的点
//				 int dir_r[find_count]：右边生长方向
//				 int dir_l[find_count]：左边生长方向
 // 返回参数     void
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
void Eight_fields(int break_flag, uchar(*image)[188], int* l_stastic, int* r_stastic, int l_start_x, int l_start_y, int r_start_x, int r_start_y, int* hightest,
	int points_l[find_count][2], int points_r[find_count][2], int dir_r[find_count], int dir_l[find_count]
)
{
	int i = 0, j = 0;
	//左边变量
	int search_filds_l[8][2] = { {  0 } };
	int index_l = 0;
	int temp_l[8][2] = { {  0 } };
	int center_point_l[2] = { 0 };
	int l_data_statics;//统计左边
	//定义八个邻域
	static int seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是顺时针

	//右边变量
	int search_filds_r[8][2] = { {  0 } };
	int center_point_r[2] = { 0 };//中心坐标点
	int index_r = 0;//索引下标
	int temp_r[8][2] = { {  0 } };
	int r_data_statics;//统计右边
	//定义八个邻域
	static int seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
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
	while (break_flag--)
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
			/*	printf("index_l为%d\n", index_l);*/
				dir_l[l_data_statics - 1] = (i);//记录生长方向（索引此前已经加过1）
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
			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
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
		/*	printf("\n左边开始向下了，等待右边，等待中... \n");*/
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
				dir_r[r_data_statics - 1] = (i);//记录生长方向
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
// 函数简介     给图像画一个黑框
// 参数说明     图像数组
 // 返回参数     void
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
void image_draw_rectan(uchar(*image)[188])
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
// 函数简介     从八邻域边界里提取需要的边线
// 参数说明     单边点数量，左边边界数组，左边找到的点
 // 返回参数     void
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
void get_left(int total_L,int l_border[120],int points_l[500][2])
{
	int i = 0;
	int j = 0;
	int h = 0;
	//初始化
	for (i = 0; i < 120; i++)
	{
		l_border[i] = 1;
	}
	h = 110;
	//左边
	for (j = 0; j < total_L; j++)//之前左边找到几个就循环几次
	{
		//printf("%d\n", j);
		if (points_l[j][1] == h)//遍历找到最高处的点
		{
			l_border[h] = points_l[j][0] + 1;//左边界数组的最高一行为当前的x值+偏移量
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
// 函数简介     从八邻域边界里提取需要的边线
// 参数说明     单边点数量，右边边界数组，右边找到的点
 // 返回参数     void
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
void get_right(int total_R,int r_border[120], int points_r[500][2])
{
	int temp_r[120] = { 0 };
	int i = 0;
	int j = 0;
	int h = 0;
	for (i = 0; i < 120; i++)
	{
		r_border[i] = 187;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
	}
	h = 110;
	//右边
	for (j = 0; j < total_R; j++)
	{
		/*printf("points_r[j][1]是%d,h是%d\n", points_r[j][1], h);*/
		if (points_r[j][1] == h)
		{
			r_border[h] = points_r[j][0] - 1;
		}
		else continue;//每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0)break;//到最后一行退出
	}

	/*for (int i = 0; i < 120; i++)
	{
		temp_r[i] = r_border[120 - i];

	}
	for (int i = 0; i < 120; i++)
	{
		r_border[i] = temp_r[i];
	}*/

}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     膨胀和腐蚀
// 参数说明     图像数组
 // 返回参数     void
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
//定义膨胀和腐蚀的阈值区间
#define threshold_max	255*5//此参数可根据自己的需求调节
#define threshold_min	255*2//此参数可根据自己的需求调节
void image_filter(uchar(*bin_image)[188])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
	int i, j;
	int num = 0;


	for (i = 1; i < 120 - 1; i++)
	{
		for (j = 1; j < (188 - 1); j++)
		{
			//统计八个方向的像素值
			num =
				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
				+ bin_image[i][j - 1] + bin_image[i][j + 1]
				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];
			if (num >= threshold_max && bin_image[i][j] == 0)
			{
				bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改
			}
			if (num <= threshold_min && bin_image[i][j] == 255)
			{
				bin_image[i][j] = 0;//黑
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     最小二乘法求斜率
// 参数说明     开始行，结束行，边界数组
 // 返回参数     斜率
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
float Slope_Calculate(int begin, int end, int* border)
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
// 函数简介     求斜率，截距
// 参数说明     开始行，结束行，边界数组，斜率地址，截距地址
 // 返回参数    void
 // 备注信息
 //------------------------------------------------------------------------------------------------------------------
void calculate_s_i(int start, int end, int* border, float* slope_rate, float* intercept)
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

float calculate_xielv(int start, int end, int* border, float* intercept)
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
// 函数简介     左右边斜率补线函数
// 参数说明     左边界数组，右边界数组，左边点数量，右边点数量，左边生长方向，右边生长方向，左边点数组，右边点数组，图像数组，转化数组
// 返回参数     斜率
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void cross_fill_ing(int* l_border, int* r_border, int total_num_l, int total_num_r,
	int* dir_l, int* dir_r, int(*points_l)[2], int(*points_r)[2], uchar(*image)[188])
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
	int start, end;
	float slope_l_rate = 0, intercept_l = 0;
	float slope_r_rate = 0, intercept_r = 0;

	int lost_r_flag = 0;
	int lost_r = 0;
	int lost_l_flag = 0;
	int lost_l = 0;
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
		
		if ((zuoxia1||zuoxia2||zuoxia3||zuoxia4) && (points_l[i][1] >= 50))
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

	if (break_num_r_y && break_num_below_r_y &&!(break_num_below_l_y) && !(break_num_l_y))//满足十字前特征(只有右边两点）（没有左下）
	{
		for (i = break_num_r_y ; i < break_num_below_r_y; i++)
		{
			if (r_border[i] == 185)
			{
				lost_r ++ ;
			}
		}
		if (lost_r >= 4)
		{
			lost_r_flag = 1;
		}
		if (lost_r_flag)
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
	}

	if (break_num_below_r_y && break_num_r_y && break_num_below_l_y && !(break_num_l_y))//满足十字前特征（右边两点和左下）
	{
		for (i = break_num_r_y; i < break_num_below_r_y; i++)//丢线判断
		{
			if (r_border[i] == 185)
			{
				lost_r++;
			}
		}

		if (lost_r >= 4)
		{
			lost_r_flag = 1;
		}
		if (lost_r_flag)
		{
			printf("\nlost_r:%d", lost_r);
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
			start = break_num_below_l_y + 2;
			start = limit_a_b(start, 0, 120);
			end = break_num_below_l_y + 15;
			calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
			printf("\n左下最小二乘法补线：");
			printf("\nslope_l_rate:%lf\n intercept_l:%lf\n", slope_l_rate, intercept_l);
			for (i = break_num_r_y - 2; i < break_num_below_r_y; i++)
			{
				l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
				l_border[i] = limit_a_b(l_border[i], 2, 186);//限幅
			}
		}
	}

	if (break_num_below_r_y && break_num_r_y && break_num_below_r_y && break_num_l_y )//满足十字前特征（四点都有）
	{
		printf("\n注意：四点都有\n");
		for (i = break_num_r_y; i < break_num_below_r_y; i++)
		{
			if (r_border[i] == 185)
			{
				lost_r++;
			}
		}
		if (lost_r >= 4)
		{
			lost_r_flag = 1;
		}
		for (i = break_num_l_y; i < break_num_below_l_y; i++)
		{
			if (l_border[i] == 185)
			{
				lost_l++;
			}
		}
		if (lost_l >= 4)
		{
			lost_l_flag = 1;
		}
		if (lost_r_flag&& lost_l_flag)
		{
			printf("\nlost_r:%d", lost_r);
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

//------------------------------------------------------------------------------------------------------------------
// 函数简介     将图像转到数组里面
// 参数说明     图像变量，图像数组
// 返回参数     图像变量
// 备注信息
//------------------------------------------------------------------------------------------------------------------

cv::Mat save_to_array(cv::Mat& mat_img,uchar(*image)[188])
{
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			image[i][j] = mat_img.at<uchar>(i, j);
		}
	}
	return mat_img;
}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     将数组转到图片里面
// 参数说明     图像变量，图像数组
// 返回参数     图像变量
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void arrar_to_image(cv::Mat& mat_img, uchar(*image)[188])
{
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			mat_img.at<uchar>(i, j) = image[i][j];
		}
	}
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     展示图片
// 参数说明     字符串（边框名称），图像变量
// 返回参数     图像变量
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void showing_image(const std::string& str, cv::Mat& mat_img)
{
	namedWindow(str, WINDOW_NORMAL);
	resizeWindow(str, chuankou_x, chuankou_y);
	imshow(str, mat_img);
}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印生长方向
// 参数说明     左边找到点数量，右边找到点数量，左边生长方向，右边生长方向
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_growth_direction(int data_stastics_l, int data_stastics_r,int* dir_l, int* dir_r )
{
	for (int i = 0; i < data_stastics_l; i++)
	{
		printf("左边的第%d个生长方向为%d\n", i, dir_l[i]);
	}
	for (int i = 0; i < data_stastics_r; i++)
	{
		printf("右边的第%d个生长方向为%d\n", i, dir_r[i]);
	}

}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印右边生长方向
// 参数说明     右边找到点数量，右边生长方向
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_r_growth_direction(int data_stastics_r, int* dir_r, int(*points_r)[2])
{
	for (int i = 0; i < data_stastics_r; i++)
	{
		printf("右边的第%d个生长方向为%d  ", i, dir_r[i]);
		printf("%d  ", points_r[i][1]);
		printf("%d\n", points_r[i][0]);
	}
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印左边生长方向
// 参数说明     左边找到点数量，左边生长方向
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_l_growth_direction(int data_stastics_l, int* dir_l,int(*points_l)[2])
{
	for (int i = 0; i < data_stastics_l; i++)
	{
		printf("左边的第%d个生长方向为%d  ", i, dir_l[i]);
		printf("%d  ", points_l[i][1]);
		printf("%d\n", points_l[i][0]);
	}
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印八领域找到的右点
// 参数说明     右边找到点数量，右边点数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_r_point(int data_stastics_r, int(*points_r)[2])
{
	for (int i = 0; i < data_stastics_r; i++)
	{
		printf("右边的第%d个边界点是（%d,%d)\n", i, points_r[i][0], points_r[i][1]);
	}
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印八领域找到的左点
// 参数说明     左边找到点数量，左边点数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_l_point(int data_stastics_l, int(*points_l)[2])
{
	for (int i = 0; i < data_stastics_l; i++)
	{
		printf("左边的第%d个边界点是（%d,%d)\n", i, points_l[i][0], points_l[i][1]);
	}
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印巡线找到的右边界
// 参数说明     右边界数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_alo_r_point(int* points_r)
{
	for (int i = 0; i < 120; i++)
	{
		printf("右边的第%d行是（%d,%d)\n", i,i, points_r[i]);
	}
}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印巡线找到的左边界
// 参数说明     左边界数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void printf_alo_l_point(int* points_l)
{
	for (int i = 0; i < 120; i++)
	{
		printf("左边的第%d行是（%d,%d)\n", i,i, points_l[i]);
	}
}




//------------------------------------------------------------------------------------------------------------------
// 函数简介     画出所有找到的点以及初始点
// 参数说明     左边八领域找到点数量，右边八领域找到点数量，左边八领域找到的点数组，右边八领域找到的点数组，图像数组，左边开始点数组，右边开始点数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void show_all_point(int data_stastics_l, int data_stastics_r, int(*points_l)[2], int(*points_r)[2], uchar(*all_boundary)[188],int* start_point_l, int* start_point_r)
{
	int ge_10_r = 0;
	int ge_10_l = 0;
	for (int k = 0; k < data_stastics_l; k++)
	{
		int temp_l_x = 0;
		int temp_l_y = 0;
		temp_l_x = points_l[k][0];
		temp_l_y = points_l[k][1];
		for (int i = 0; i < 120; i++)
		{
			for (int j = 0; j < 188; j++)
			{
				if (i == temp_l_y && j == temp_l_x)
				{
					all_boundary[i][j] = 70;
					ge_10_l++;
					if (ge_10_l % 5 == 0) {
						all_boundary[i][j] = 170;
						ge_10_l=0;
					}
				}
				
			}
		}
	}
	for (int k = 0; k < data_stastics_r; k++)
	{
		int temp_r_x = 0;
		int temp_r_y = 0;
		temp_r_x = points_r[k][0];
		temp_r_y = points_r[k][1];
		for (int i = 0; i < 120; i++)
		{
			for (int j = 0; j < 188; j++)
			{
				if (i == temp_r_y && j == temp_r_x)
				{
					all_boundary[i][j] = 70;
					ge_10_r++;
					if (ge_10_r % 5 == 0) {
						all_boundary[i][j] = 170;
						ge_10_r = 0;
					}
				}
			}
		}
	}
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			if (j == start_point_r[0] && i == start_point_r[1])
			{
				all_boundary[i][j] = 180;
			}
			if (j == start_point_l[0] && i == start_point_l[1])
			{
				all_boundary[i][j] = 180;
			}
		}
	}
}





//------------------------------------------------------------------------------------------------------------------
// 函数简介     左右寻完线开始画线
// 参数说明     左边界数组，右边界数组，图像数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void alone_line(int* r_border, int* l_border, uchar(*Final_version)[188])
{
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			if (j == r_border[i])
			{
				Final_version[i][j] = 255;
			}
		}
	}
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			if (j == l_border[i])
			{
				Final_version[i][j] = 255;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------
// 函数简介     打印左右边界点
// 参数说明     左边界数组，右边界数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void show_alone_line(int* r_border, int* l_border)
{
	//右线
	for (int i = 0; i < 120; i++)
	{
		printf("右线为(%d,%d)\n", i, r_border[i]);
	}
	for (int i = 0; i < 120; i++)
	{
		printf("左线为(%d,%d)\n", i, l_border[i]);
	}
}


//------------------------------------------------------------------------------------------------------------------
// 函数简介     计算中线
// 参数说明     中线数组，左边界数组，右边界数组，图像数组
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void show_minline(int* center_line, int* r_border, int* l_border, uchar(*Final_version)[188])
{
	for (int i = 0; i < 120; i++)
	{
		center_line[i] = (r_border[i] + l_border[i]) / 2;
	}
	//画中线
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			if (j == center_line[i])
			{
				Final_version[i][j] = 255;
			}
		}
	}

}
//------------------------------------------------------------------------------------------------------------------
// 函数简介     替换字符串里面的数字
// 参数说明     数字，字符串
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void modify_num(int num, std::string& filename)
{
	// 将数字转换为字符串
	std::string new_num = std::to_string(num);

	// 查找数字的开始位置
	std::size_t pos = filename.find_first_of("0123456789");

	// 获取原数字的长度
	std::size_t num_len = filename.find_first_not_of("0123456789", pos) - pos;

	// 替换数字
	filename.replace(pos, num_len, new_num);
}



//------------------------------------------------------------------------------------------------------------------
// 函数简介     将字符串赋给窗口名称
// 参数说明     字符串，图像变量
// 返回参数     void
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void readImage(std::string& filename, cv::Mat& output) {
	output = imread(filename, IMREAD_GRAYSCALE);
}