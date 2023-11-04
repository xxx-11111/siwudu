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
// �������     �޷�
// ����˵��     �Ƚ�ֵ����ֵ����ֵ
// ���ز���     ���ֵ
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
int limit_a_b(int x, int a, int b)
{
	if (x < a) x = a;
	if (x > b) x = b;
	return x;
}


//------------------------------------------------------------------------------------------------------------------
// �������     ȡ����ֵ
// ����˵��     ����ֵ
// ���ز���     ���ֵ
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
int my_abs(int value)
{
if (value >= 0) return value;
else return -value;
}
//extern int init[120][188];
/*
�������ƣ�void get_start_point(uint8 start_row)
����˵����
����˵����������������
�������أ���
�޸�ʱ�䣺2022��9��8��
��    ע��
example��  get_start_point(image_h-2)
 */

 //------------------------------------------------------------------------------------------------------------------
 // �������     Ѱ�������߽�ı߽����Ϊ������ѭ������ʼ��
 // ����˵��     ��ʼ�У�ͼ�����飬���������飬�ұ��������
 // ���ز���     1�������ҵ�һ��;   0:��û�ҵ�
 // ��ע��Ϣ
 //------------------------------------------------------------------------------------------------------------------
unsigned int get_start_point(unsigned int start_row, uchar init[120][188],int* start_point_l,int* start_point_r)
{
	unsigned int i = 0, l_found = 0, r_found = 0;
	//����
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y

	//���м�����ߣ��������
	for (i = 188 / 2; i > 2; i--)
	{
		start_point_l[0] = i-1;//��
		start_point_l[1] = start_row;//��
		if (init[start_row][i] == 255 && init[start_row][i - 1] == 0)
		{
			//printf("�ҵ�������image[%d][%d]\n", start_row,i);
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
			//printf("�ҵ��ұ����image[%d][%d]\n",start_row, i);
			r_found = 1;
			break;
		}
	}

	if (l_found && r_found)return 1;
	else {
		//printf("δ�ҵ����\n");
		return 0;
	}
}


//------------------------------------------------------------------------------------------------------------------
// �������     Ѱ��������ֵ
// ����˵��     ͼ������
// ���ز���     ��ֵ
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------

int otsu(uchar init[120][188])
{
    // ����㷨����
    int width = 188;
    int height = 120;
    int hist[256] = { 0 };//ֱ��ͼ�ֲ�ͼ
    double threshold = 0.0;
    double var_max = 0.0;

    // ͳ������ֱֵ��ͼ
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
// �������     ������ֵ��ֵ��
// ����˵��     ͼ��߶ȣ�ͼ���ȣ���ֵ��ͼ������
// ���ز���     void
// ��ע��Ϣ
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
 // �������     �������ҵ�
 // ����˵��     break_flag_r			�������Ҫѭ���Ĵ���
//				(*image)[image_w]		����Ҫ�����ҵ��ͼ�����飬�����Ƕ�ֵͼ, �����������Ƽ���
//				 * l_stastic			��ͳ��������ݣ����������ʼ�����Ա����ź�ȡ��ѭ������
//				 * r_stastic			��ͳ���ұ����ݣ����������ʼ�����Ա����ź�ȡ��ѭ������
//				 l_start_x				�������������
//			  	 l_start_y				��������������
//			  	 r_start_x				���ұ���������
//			 	 r_start_y				���ұ����������
//				 hightest				��ѭ���������õ�����߸߶�
//				 int points_l[find_count][2]����ߵĵ�
//				 int points_r[find_count][2]���ұߵĵ�
//				 int dir_r[find_count]���ұ���������
//				 int dir_l[find_count]�������������
 // ���ز���     void
 // ��ע��Ϣ
 //------------------------------------------------------------------------------------------------------------------
void Eight_fields(int break_flag, uchar(*image)[188], int* l_stastic, int* r_stastic, int l_start_x, int l_start_y, int r_start_x, int r_start_y, int* hightest,
	int points_l[find_count][2], int points_r[find_count][2], int dir_r[find_count], int dir_l[find_count]
)
{
	int i = 0, j = 0;
	//��߱���
	int search_filds_l[8][2] = { {  0 } };
	int index_l = 0;
	int temp_l[8][2] = { {  0 } };
	int center_point_l[2] = { 0 };
	int l_data_statics;//ͳ�����
	//����˸�����
	static int seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//�����˳ʱ��

	//�ұ߱���
	int search_filds_r[8][2] = { {  0 } };
	int center_point_r[2] = { 0 };//���������
	int index_r = 0;//�����±�
	int temp_r[8][2] = { {  0 } };
	int r_data_statics;//ͳ���ұ�
	//����˸�����
	static int seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//�������ʱ��

	l_data_statics = *l_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������
	r_data_statics = *r_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������


	//��һ�θ��������  ���ҵ������ֵ������
	center_point_l[0] = l_start_x;//x
	center_point_l[1] = l_start_y;//y
	center_point_r[0] = r_start_x;//x
	center_point_r[1] = r_start_y;//y

	printf("\n�����ڵ������ʼ����(%d,%d)\n", center_point_l[0], center_point_l[1]);
	printf("�����ڵ��ұ���ʼ����(%d,%d)\n", center_point_r[0], center_point_r[1]);

	//��������ѭ��
	while (break_flag--)
	{
		/*---------------------------------------------�Ծ�����г�ʼ��---------------------------------------*/
		//���
		for (i = 0; i < 8; i++)//����8F����
		{
			search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
			search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
		}
		//�����������䵽�Ѿ��ҵ��ĵ���
		points_l[l_data_statics][0] = center_point_l[0];//x
		points_l[l_data_statics][1] = center_point_l[1];//y
		l_data_statics++;//������һ


		//�ұ�
		for (i = 0; i < 8; i++)//����8F����
		{
			search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
			search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
		}
		//�����������䵽�Ѿ��ҵ��ĵ���
		points_r[r_data_statics][0] = center_point_r[0];//x
		points_r[r_data_statics][1] = center_point_r[1];//y

		/*printf("\n��һ�ֵ��ұ����ĵ�Ϊ��%d��%d��\n", points_r[r_data_statics][0], points_r[r_data_statics][1]);*/

		/*------------------------------------------��߽�Ѳ��------------------------------------------------------*/
		index_l = 0;//��߽��־λ����
		for (i = 0; i < 8; i++)//������ʱ����˴Σ�ʹ��ǰ����
		{
			temp_l[i][0] = 0;
			temp_l[i][1] = 0;
		}
		/*printf("\n(1)%d\n", image[3][111]);
		printf("(2)%d\n", image[4][111]);
		printf("(3)%d\n", image[4][110]);
		printf("(4)%d\n", image[4][109]);
		printf("(5)%d\n", image[3][109]);*/
		//�ڰ��������жϱ߽��
		for (i = 0; i < 8; i++)//�˸���������ж�
		{
			
			//�����Ϊ��y��x��˳ʱ��Ӻ�������
			if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
				&& image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
			{
				/*printf("\n��ѭ��Ϊ%d����%d�ε�ǰ��Ϊ%d\n", break_flag, i + 1, image[search_filds_l[i][1]][search_filds_l[i][0]]);
				printf("��ѭ��Ϊ%d����%d�εĺ���Ϊ%d\n", break_flag, i + 1, image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]]);*/
				temp_l[index_l][0] = search_filds_l[(i)][0];//�������ҵ��ı߽磨�ڵ㣩��x���긳����ʱ����
				temp_l[index_l][1] = search_filds_l[(i)][1];//�������ҵ��ı߽磨�ڵ㣩��y���긳����ʱ����
				index_l++;//�ҵ��߽��־λ
			/*	printf("index_lΪ%d\n", index_l);*/
				dir_l[l_data_statics - 1] = (i);//��¼��������������ǰ�Ѿ��ӹ�1��
			}

			if (index_l)//����߽��־λ��0���˸������������ҵ�һ���߽�㣩
			{
				//���������
				center_point_l[0] = temp_l[0][0];//����ʱ���긳�����ĵ�
				center_point_l[1] = temp_l[0][1];//����ʱ���긳�����ĵ�
				//�ҵ��˸�������ȷ�ϵ����ĵ������µĲ���
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

		//��߻��ұ�������ͬһ����
		if ((points_r[r_data_statics][0] == points_r[r_data_statics - 1][0] && points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
			&& points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
			|| (points_l[l_data_statics - 1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics - 1][0] == points_l[l_data_statics - 3][0]
				&& points_l[l_data_statics - 1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics - 1][1] == points_l[l_data_statics - 3][1]))
		{
			printf("���ν���ͬһ���㣬�˳�\n");
			break;
		}

		if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
			&& my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2))
		{
			printf("\n���������˳�\n");	
			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�
			//printf("\n��y=%d���˳�\n",*hightest);
			break;
		}

		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
		{
			/*printf("\n�����߱��ұ߸��ˣ���ߵȴ��ұ�\n");
			printf("���ұ���%d\n",points_r[r_data_statics][1]);
			printf("�������%d\n", points_l[l_data_statics-1][1]);*/
			continue;//�����߱��ұ߸��ˣ���ߵȴ��ұ�
		}

		if (dir_l[l_data_statics - 1] == 7
			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//��߱��ұ߸����Ѿ�����������
		{
		/*	printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���... \n");*/
			center_point_l[0] = points_l[l_data_statics - 1][0];//x
			center_point_l[1] = points_l[l_data_statics - 1][1];//y
			l_data_statics--;
		}


		r_data_statics++;//������һ

		index_r = 0;//�����㣬��ʹ��
		for (i = 0; i < 8; i++)
		{
			temp_r[i][0] = 0;//�����㣬��ʹ��
			temp_r[i][1] = 0;//�����㣬��ʹ��
		}

		//�ұ��ж�
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
				&& image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
			{
				temp_r[index_r][0] = search_filds_r[(i)][0];
				temp_r[index_r][1] = search_filds_r[(i)][1];
				index_r++;//������һ
				dir_r[r_data_statics - 1] = (i);//��¼��������
				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
			}
			if (index_r)
			{

				//���������
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


	//ȡ��ѭ������
	*l_stastic = l_data_statics;
	*r_stastic = r_data_statics;

	
}

//------------------------------------------------------------------------------------------------------------------
// �������     ��ͼ��һ���ڿ�
// ����˵��     ͼ������
 // ���ز���     void
 // ��ע��Ϣ
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
// �������     �Ӱ�����߽�����ȡ��Ҫ�ı���
// ����˵��     ���ߵ���������߽߱����飬����ҵ��ĵ�
 // ���ز���     void
 // ��ע��Ϣ
 //------------------------------------------------------------------------------------------------------------------
void get_left(int total_L,int l_border[120],int points_l[500][2])
{
	int i = 0;
	int j = 0;
	int h = 0;
	//��ʼ��
	for (i = 0; i < 120; i++)
	{
		l_border[i] = 1;
	}
	h = 110;
	//���
	for (j = 0; j < total_L; j++)//֮ǰ����ҵ�������ѭ������
	{
		//printf("%d\n", j);
		if (points_l[j][1] == h)//�����ҵ���ߴ��ĵ�
		{
			l_border[h] = points_l[j][0] + 1;//��߽���������һ��Ϊ��ǰ��xֵ+ƫ����
		}
		else continue; //ÿ��ֻȡһ���㣬û�ҵ�������ѭ��������
		h--;//�ҵ�������½��߶Ƚ���
		if (h == 0)//�������һ�У��˳�
		{
			break;//�����һ���˳�
		}
	}
}


//------------------------------------------------------------------------------------------------------------------
// �������     �Ӱ�����߽�����ȡ��Ҫ�ı���
// ����˵��     ���ߵ��������ұ߽߱����飬�ұ��ҵ��ĵ�
 // ���ز���     void
 // ��ע��Ϣ
 //------------------------------------------------------------------------------------------------------------------
void get_right(int total_R,int r_border[120], int points_r[500][2])
{
	int temp_r[120] = { 0 };
	int i = 0;
	int j = 0;
	int h = 0;
	for (i = 0; i < 120; i++)
	{
		r_border[i] = 187;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������
	}
	h = 110;
	//�ұ�
	for (j = 0; j < total_R; j++)
	{
		/*printf("points_r[j][1]��%d,h��%d\n", points_r[j][1], h);*/
		if (points_r[j][1] == h)
		{
			r_border[h] = points_r[j][0] - 1;
		}
		else continue;//ÿ��ֻȡһ���㣬û����һ�оͲ���¼
		h--;
		if (h == 0)break;//�����һ���˳�
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
// �������     ���ͺ͸�ʴ
// ����˵��     ͼ������
 // ���ز���     void
 // ��ע��Ϣ
 //------------------------------------------------------------------------------------------------------------------
//�������ͺ͸�ʴ����ֵ����
#define threshold_max	255*5//�˲����ɸ����Լ����������
#define threshold_min	255*2//�˲����ɸ����Լ����������
void image_filter(uchar(*bin_image)[188])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
{
	int i, j;
	int num = 0;


	for (i = 1; i < 120 - 1; i++)
	{
		for (j = 1; j < (188 - 1); j++)
		{
			//ͳ�ư˸����������ֵ
			num =
				bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
				+ bin_image[i][j - 1] + bin_image[i][j + 1]
				+ bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];
			if (num >= threshold_max && bin_image[i][j] == 0)
			{
				bin_image[i][j] = 255;//��  ���Ը�ɺ궨�壬�������
			}
			if (num <= threshold_min && bin_image[i][j] == 255)
			{
				bin_image[i][j] = 0;//��
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------
// �������     ��С���˷���б��
// ����˵��     ��ʼ�У������У��߽�����
 // ���ز���     б��
 // ��ע��Ϣ
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

	if ((end - begin) * x2sum - xsum * xsum) //�жϳ����Ƿ�Ϊ��
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
// �������     ��б�ʣ��ؾ�
// ����˵��     ��ʼ�У������У��߽����飬б�ʵ�ַ���ؾ��ַ
 // ���ز���    void
 // ��ע��Ϣ
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
	//�������ƽ����
	if (num)
	{
		x_average = (float)(xsum / num);
		y_average = (float)(ysum / num);

	}
	/*����б��*/
	*slope_rate = Slope_Calculate(start, end, border);//б��
	*intercept = y_average - (*slope_rate) * x_average;//�ؾ�
}

float calculate_xielv(int start, int end, int* border, float* intercept)
{
	float xielv = 0;

	xielv = (float)( (border[start] - border[end])*100 / (start - end));
	*intercept = border[start] - xielv * start/100;
	//printf("\nx��ֵ:%d\t", start - end);
	//printf("y��ֵ:%d\t", (border[start] - border[end]));
	//printf("б��Ϊ:%lf\n", xielv);
	//printf("�ؾ�Ϊ:%lf\n", *intercept);
	return xielv;
}



//------------------------------------------------------------------------------------------------------------------
// �������     ���ұ�б�ʲ��ߺ���
// ����˵��     ��߽����飬�ұ߽����飬��ߵ��������ұߵ�������������������ұ�����������ߵ����飬�ұߵ����飬ͼ�����飬ת������
// ���ز���     б��
// ��ע��Ϣ
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
		printf("��������ߵĵ�%d���ǣ�%d,%d)\n", i, i, l_border[i]);
	}*/
	for (i = 0; i < total_num_l; i++)//ѭ������ҵ��ĵ�
	{
		if ((zuoshang1) && (points_l[i][1] >= 20))
		{
			printf("\nzuoshang:%d", points_l[i][1]);
			break_num_l_y = points_l[i][1] - 2;//����y����
			break_num_l_x = l_border[break_num_l_y];
			printf("\n��������ҵ���:(");
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

	for (i = 0; i < total_num_l; i++)//ѭ������ҵ��ĵ�
	{
		
		if ((zuoxia1||zuoxia2||zuoxia3||zuoxia4) && (points_l[i][1] >= 50))
		{
			printf("\nzuoxia:%d", points_l[i][1]);
			break_num_below_l_y = points_l[i][1] ;//����y����
			break_num_below_l_x = l_border[break_num_below_l_y];
			printf("\n��������ҵ���:(");
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
		/*printf("dir_r[%d]Ϊ%d\t", i, dir_r[i]);
		printf("iΪ%dʱ��dir_r[i - 1] =%d��dir_r[i] =%d��dir_r[i + 1] =%d\n", i, dir_r[i - 1], dir_r[i], dir_r[i + 1]);*/
		if (youshang1&& (points_r[i][1] >= 20))
		{
			printf("\nyoushang:%d", points_r[i][1]);
			break_num_r_y = points_r[i][1] - 2;//����y����
			break_num_r_x = r_border[break_num_r_y];
			printf("\n�ұ������ҵ���:(");
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


	for (i = 0; i < total_num_r; i++)//ѭ������ҵ��ĵ�
	{
		if ((youxia1||youxia2) && (points_r[i][1] >= 20))
		{
			printf("\nyouxia:%d", points_r[i][1]);
			break_num_below_r_y = points_r[i][1] ;//����y����
			break_num_below_r_x = r_border[break_num_below_r_y];
			printf("\n�ұ������ҵ���:(");
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

	if (break_num_r_y && break_num_below_r_y &&!(break_num_below_l_y) && !(break_num_l_y))//����ʮ��ǰ����(ֻ���ұ����㣩��û�����£�
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
			printf("\nע�⣺ֻ���ұ�����\n");
			//����б��
			start = break_num_r_y - 15;//���
			start = limit_a_b(start, 0, 120);//�޷�
			end = break_num_r_y - 5;//�յ�
			calculate_s_i(start, end, r_border, &slope_r_rate, &intercept_r);
			printf("slope_r_rate:%lf\n intercept_r:%lf\n", slope_r_rate, intercept_r);
			for (i = break_num_r_y - 5; i < 120 - 1; i++)
			{
				r_border[i] = slope_r_rate * (i)+intercept_r;
				r_border[i] = limit_a_b(r_border[i], 2, 186);
			}
		}
	}

	if (break_num_below_r_y && break_num_r_y && break_num_below_l_y && !(break_num_l_y))//����ʮ��ǰ�������ұ���������£�
	{
		for (i = break_num_r_y; i < break_num_below_r_y; i++)//�����ж�
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
			printf("\nע�⣺�ұ����������\n");
			//�����ұ�б�ʣ����������б�ʲ���
			start = break_num_r_y - 1;
			start = limit_a_b(start, 0, 120);
			end = break_num_below_r_y + 1;
			slope_r_rate = calculate_xielv(start, end, r_border, &intercept_r);
			printf("\n�ұ������б�ʲ��ߣ�");
			printf("\nstart:%d\t end:%d", start, end);
			printf("\n����x��%d,����x��%d\n", r_border[break_num_r_y], r_border[break_num_below_r_y]);
			printf("xielv:%lf\t intercept_l:%lf\n", slope_r_rate, intercept_r);
			for (i = break_num_r_y - 3; i < break_num_below_r_y + 5; i++)
			{
				r_border[i] = slope_r_rate * (i) / 100 + intercept_r;//y = kx+b
				/*printf("y:%d   x:%d\n", i, r_border[i]);*/
				r_border[i] = limit_a_b(r_border[i], 2, 186);//�޷�
			}

			//����������С���˷��������
			start = break_num_below_l_y + 2;
			start = limit_a_b(start, 0, 120);
			end = break_num_below_l_y + 15;
			calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
			printf("\n������С���˷����ߣ�");
			printf("\nslope_l_rate:%lf\n intercept_l:%lf\n", slope_l_rate, intercept_l);
			for (i = break_num_r_y - 2; i < break_num_below_r_y; i++)
			{
				l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
				l_border[i] = limit_a_b(l_border[i], 2, 186);//�޷�
			}
		}
	}

	if (break_num_below_r_y && break_num_r_y && break_num_below_r_y && break_num_l_y )//����ʮ��ǰ�������ĵ㶼�У�
	{
		printf("\nע�⣺�ĵ㶼��\n");
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
			printf("\n��������б�ʲ��ߣ�");
			printf("\nstart_y:%d\t end_y:%d", start, end);
			printf("\n����x��%d\t ����x��%d\n", l_border[break_num_l_y], l_border[break_num_below_l_y]);
			printf("xielv:%lf\t intercept_l:%lf\n", slope_l_rate, intercept_l);
			for (i = break_num_l_y - 3; i < break_num_below_l_y + 5; i++)
			{
				l_border[i] = slope_l_rate * (i) / 100 + intercept_l;//y = kx+b
				l_border[i] = limit_a_b(l_border[i], 2, 186);//�޷�
			}

			start = break_num_r_y - 1;
			start = limit_a_b(start, 0, 120);
			end = break_num_below_r_y + 1;
			slope_r_rate = calculate_xielv(start, end, r_border, &intercept_r);
			printf("\n�ұ������б�ʲ��ߣ�");
			printf("\nstart:%d\t end:%d", start, end);
			printf("\n����x��%d,����x��%d\n", r_border[break_num_r_y], r_border[break_num_below_r_y]);
			printf("xielv:%lf\t intercept_l:%lf\n", slope_r_rate, intercept_r);
			for (i = break_num_r_y - 3; i < break_num_below_r_y + 5; i++)
			{
				r_border[i] = slope_r_rate * (i) / 100 + intercept_r;//y = kx+b
				r_border[i] = limit_a_b(r_border[i], 2, 186);//�޷�
			}
		}


	}


	for (i = 0; i < 188; i++)
	{
		image[pilot_row][i] = 230;
	}
	printf("\n�캽�����Ϊ%d", l_border[pilot_row]);
	printf("\n�캽���ұ�Ϊ%d", r_border[pilot_row]);
	int pilot_min = ((r_border[pilot_row] - l_border[pilot_row]) / 2) + l_border[pilot_row];
	printf("\n�캽���е�Ϊ%d", pilot_min);
	image[pilot_row][93] = 120;
	image[pilot_row][pilot_min] = 0;
	printf("\n��ֵΪ%d", 93 - pilot_min);

}

//------------------------------------------------------------------------------------------------------------------
// �������     ��ͼ��ת����������
// ����˵��     ͼ�������ͼ������
// ���ز���     ͼ�����
// ��ע��Ϣ
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
// �������     ������ת��ͼƬ����
// ����˵��     ͼ�������ͼ������
// ���ز���     ͼ�����
// ��ע��Ϣ
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
// �������     չʾͼƬ
// ����˵��     �ַ������߿����ƣ���ͼ�����
// ���ز���     ͼ�����
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void showing_image(const std::string& str, cv::Mat& mat_img)
{
	namedWindow(str, WINDOW_NORMAL);
	resizeWindow(str, chuankou_x, chuankou_y);
	imshow(str, mat_img);
}

//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡ��������
// ����˵��     ����ҵ����������ұ��ҵ���������������������ұ���������
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_growth_direction(int data_stastics_l, int data_stastics_r,int* dir_l, int* dir_r )
{
	for (int i = 0; i < data_stastics_l; i++)
	{
		printf("��ߵĵ�%d����������Ϊ%d\n", i, dir_l[i]);
	}
	for (int i = 0; i < data_stastics_r; i++)
	{
		printf("�ұߵĵ�%d����������Ϊ%d\n", i, dir_r[i]);
	}

}

//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡ�ұ���������
// ����˵��     �ұ��ҵ����������ұ���������
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_r_growth_direction(int data_stastics_r, int* dir_r, int(*points_r)[2])
{
	for (int i = 0; i < data_stastics_r; i++)
	{
		printf("�ұߵĵ�%d����������Ϊ%d  ", i, dir_r[i]);
		printf("%d  ", points_r[i][1]);
		printf("%d\n", points_r[i][0]);
	}
}



//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡ�����������
// ����˵��     ����ҵ��������������������
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_l_growth_direction(int data_stastics_l, int* dir_l,int(*points_l)[2])
{
	for (int i = 0; i < data_stastics_l; i++)
	{
		printf("��ߵĵ�%d����������Ϊ%d  ", i, dir_l[i]);
		printf("%d  ", points_l[i][1]);
		printf("%d\n", points_l[i][0]);
	}
}



//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡ�������ҵ����ҵ�
// ����˵��     �ұ��ҵ����������ұߵ�����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_r_point(int data_stastics_r, int(*points_r)[2])
{
	for (int i = 0; i < data_stastics_r; i++)
	{
		printf("�ұߵĵ�%d���߽���ǣ�%d,%d)\n", i, points_r[i][0], points_r[i][1]);
	}
}



//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡ�������ҵ������
// ����˵��     ����ҵ�����������ߵ�����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_l_point(int data_stastics_l, int(*points_l)[2])
{
	for (int i = 0; i < data_stastics_l; i++)
	{
		printf("��ߵĵ�%d���߽���ǣ�%d,%d)\n", i, points_l[i][0], points_l[i][1]);
	}
}


//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡѲ���ҵ����ұ߽�
// ����˵��     �ұ߽�����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_alo_r_point(int* points_r)
{
	for (int i = 0; i < 120; i++)
	{
		printf("�ұߵĵ�%d���ǣ�%d,%d)\n", i,i, points_r[i]);
	}
}

//------------------------------------------------------------------------------------------------------------------
// �������     ��ӡѲ���ҵ�����߽�
// ����˵��     ��߽�����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void printf_alo_l_point(int* points_l)
{
	for (int i = 0; i < 120; i++)
	{
		printf("��ߵĵ�%d���ǣ�%d,%d)\n", i,i, points_l[i]);
	}
}




//------------------------------------------------------------------------------------------------------------------
// �������     ���������ҵ��ĵ��Լ���ʼ��
// ����˵��     ��߰������ҵ����������ұ߰������ҵ�����������߰������ҵ��ĵ����飬�ұ߰������ҵ��ĵ����飬ͼ�����飬��߿�ʼ�����飬�ұ߿�ʼ������
// ���ز���     void
// ��ע��Ϣ
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
// �������     ����Ѱ���߿�ʼ����
// ����˵��     ��߽����飬�ұ߽����飬ͼ������
// ���ز���     void
// ��ע��Ϣ
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
// �������     ��ӡ���ұ߽��
// ����˵��     ��߽����飬�ұ߽�����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void show_alone_line(int* r_border, int* l_border)
{
	//����
	for (int i = 0; i < 120; i++)
	{
		printf("����Ϊ(%d,%d)\n", i, r_border[i]);
	}
	for (int i = 0; i < 120; i++)
	{
		printf("����Ϊ(%d,%d)\n", i, l_border[i]);
	}
}


//------------------------------------------------------------------------------------------------------------------
// �������     ��������
// ����˵��     �������飬��߽����飬�ұ߽����飬ͼ������
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void show_minline(int* center_line, int* r_border, int* l_border, uchar(*Final_version)[188])
{
	for (int i = 0; i < 120; i++)
	{
		center_line[i] = (r_border[i] + l_border[i]) / 2;
	}
	//������
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
// �������     �滻�ַ������������
// ����˵��     ���֣��ַ���
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void modify_num(int num, std::string& filename)
{
	// ������ת��Ϊ�ַ���
	std::string new_num = std::to_string(num);

	// �������ֵĿ�ʼλ��
	std::size_t pos = filename.find_first_of("0123456789");

	// ��ȡԭ���ֵĳ���
	std::size_t num_len = filename.find_first_not_of("0123456789", pos) - pos;

	// �滻����
	filename.replace(pos, num_len, new_num);
}



//------------------------------------------------------------------------------------------------------------------
// �������     ���ַ���������������
// ����˵��     �ַ�����ͼ�����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void readImage(std::string& filename, cv::Mat& output) {
	output = imread(filename, IMREAD_GRAYSCALE);
}