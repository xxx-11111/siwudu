/*
 * IMAGE.c
 *
 *  Created on: 2023��11��3��
 *      Author: ��˼Ծ
 */

#include "zf_common_headfile.h"
#include "IMAGE.h"
#include "define.h"

//------------------------------------------------------------------------------------------------------------------
// �������     ȡ����ֵ
// ����˵��     ��Ҫȡ����ֵ��in_t�Ͳ���
// ���ز���     ȡ����ֵ
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
int my_abs(int value)
{
if (value >= 0) return value;
else return -value;
}


//------------------------------------------------------------------------------------------------------------------
// �������     �޷�
// ����˵��     x:�Ƚ�ֵ
// ����˵��     a:��ֵ
// ����˵��     b:��ֵ
// ���ز���     �޷����
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
uint8 limit_a_b(uint8 x, uint8 a, uint8 b)
{
    if (x < a) x = a;
    if (x > b) x = b;
    return x;
}


//------------------------------------------------------------------------------------------------------------------
// �������     ��򷨶�ֵ��
// ����˵��     uint8���͵Ķ�ά����
// ���ز���     ��ֵ
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
int otsu(uint8 image[image_heigh][image_widt])
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
// �������     ��ͼ������˲�
// ����˵��     uint8���͵Ķ�ά����
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void image_filter(uint8 image[image_heigh][image_widt])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
{
    int i, j;
    int num = 0;


    for (i = 1; i < 120 - 1; i++)
    {
        for (j = 1; j < (188 - 1); j++)
        {
            //ͳ�ư˸����������ֵ
            num =
                image[i - 1][j - 1] +image[i - 1][j] + image[i - 1][j + 1]
                + image[i][j - 1] + image[i][j + 1]
                + image[i + 1][j - 1] + image[i + 1][j] + image[i + 1][j + 1];
            if (num >= threshold_max &&image[i][j] == 0)
            {
                image[i][j] = 255;//��  ���Ը�ɺ궨�壬�������
            }
            if (num <= threshold_min && image[i][j] == 255)
            {
                image[i][j] = 0;//��
            }
        }
    }
}


//------------------------------------------------------------------------------------------------------------------
// �������     ��Χ���ڿ�
// ����˵��     uint8���͵Ķ�ά����
// ���ز���     void
// ��ע��Ϣ
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
// �������     Ѱ�Ҵ�����
// ����˵��     start_row����ʼ������
//             image[120][188]:�����ͼ��
//             *start_point_l���������ָ�룬�����ַ����
//             *start_point_r���ұ�����ָ�룬�����ַ����
// ���ز���     void
// ��ע��Ϣ     ��ֵ���󣬴ӵ����м��������ҵ�һ���ڵ�
//------------------------------------------------------------------------------------------------------------------
unsigned int get_start_point(unsigned int start_row, uint8 image[120][188],int* start_point_l,int* start_point_r)
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
        if (image[start_row][i] == 255 && image[start_row][i - 1] == 0)
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
        if (image[start_row][i] == 255 && image[start_row][i + 1] == 0)
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
// �������     �������ҵ�
// ����˵��     break_number�������Ҫѭ���Ĵ���
//             (*image)[image_w]�������ҵ�Ķ�ֵͼ������,�����������Ƽ���
//             *l_stastic��ͳ��������ݣ�����ҵ���������
//             *r_stastic��ͳ���ұ����ݣ�����ҵ���������
//             l_start_x�������������
//             l_start_y��������������
//             r_start_x���ұ���������
//             r_start_y���ұ����������
//             high_max��ѭ���������õ�����߸߶�
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void Eight_fields(int break_number, uint8(*image)[image_widt], int* l_stastic, int* r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, int* high_max,
        uint8 points_l[find_count][2], uint8 points_r[find_count][2], uint8 dir_r[find_count], uint8 dir_l[find_count]
)
{
    int i = 0, j = 0;
    //��߱���
    uint8 search_filds_l[8][2] = { {  0 } };
    int index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = { 0 };
    int l_data_statics;//ͳ�����
    //����˸�����
    static int seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�����˳ʱ��

    //�ұ߱���
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//���������
    int index_r = 0;//�����±�
    uint8 temp_r[8][2] = { {  0 } };
    int r_data_statics;//ͳ���ұ�
    //����˸�����
    static int seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
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
    while (break_number--)
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
            /*  printf("index_lΪ%d\n", index_l);*/
                dir_l[l_data_statics - 1] = (uint8)(i);//��¼��������������ǰ�Ѿ��ӹ�1��
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
            *high_max = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�
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
            printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���... \n");
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
                dir_r[r_data_statics - 1] = (uint8)(i);//��¼��������
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
// �������     �Ӱ�����߽�����ȡ��Ҫ�ı���
// ����˵��     total���������ҵ��ĵ������
//             border���洢�߽������
//             points���ҵ��ĵ�Ĵ�������
// ���ز���     void
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void get_left(int total,uint8 border[120],uint8 points[find_count][2])
{
    int i = 0;
    int j = 0;
    int h = 0;
    //��ʼ��
    for (i = 0; i < 120; i++)
    {
        border[i] = 1;
    }
    h = 110;
    //���
    for (j = 0; j < total; j++)//֮ǰ����ҵ�������ѭ������
    {
        //printf("%d\n", j);
        if (points[j][1] == h)//�����ҵ���ߴ��ĵ�
        {
            border[h] = points[j][0] + 1;//��߽���������һ��Ϊ��ǰ��xֵ+ƫ����
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
// �������     ��С���˷���б��
// ����˵��     begin����ʼ������
//             end����ֹ������
//             * border���߽索�������
// ���ز���     б��
// ��ע��Ϣ
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
// �������     ����ؾ��б��
// ����˵��     begin����ʼ������
//             end����ֹ������
//             * slope_rate��б��
//             * intercept ���ؾ�
// ���ز���     void
// ��ע��Ϣ
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



//------------------------------------------------------------------------------------------------------------------
// �������     ���������б��
// ����˵��     start����ʼ����
//             end����������
//             * border���߽�����
//             * intercept��б��ָ��
// ���ز���     float��б��
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
float calculate_xielv(uint8 start, uint8 end, uint8* border, float* intercept)
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
// �������     �ж�Ԫ��
// ����˵��     *l_border����߽索�������
//             *r_border���ұ߽索�������
//             *total_num_l���������ҵ�����ߵĵ��������ָ��
//             *total_num_r���������ҵ����ұߵĵ��������ָ��
//             *dir_l����������ߵ���������Ĵ�������
//             *dir_r���������ұߵ���������Ĵ�������
//             (*points_l)[2]������������ҵ��ĵ�Ĵ�������
//             (*points_r)[2]���������ұ��ҵ��ĵ�Ĵ�������
//             uint8(*image)[188]����Ҫ�����ͼ��Ķ�ά����
// ���ز���     void
// ��ע��Ϣ
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

        if ((zuoxia1||zuoxia2||zuoxia3||zuoxia4) && (points_l[i][1] >= 60))
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

    if (break_num_l_y && break_num_r_y &&!(break_num_below_r_y))//����ʮ��ǰ����(ֻ���ұ����㣩��û�����£�
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

    if (break_num_below_r_y && break_num_r_y && break_num_below_r_y && !(break_num_l_y))//����ʮ��ǰ�������ұ���������£�
    {
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
        start = break_num_below_l_y + 2 ;
        start = limit_a_b(start, 0, 120);
        end = break_num_below_l_y +15;
        calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
        printf("\n������С���˷����ߣ�");
        printf("\nslope_l_rate:%lf\n intercept_l:%lf\n", slope_l_rate, intercept_l);
        for (i = break_num_r_y - 2; i < break_num_below_r_y; i++)
        {
            l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
            l_border[i] = limit_a_b(l_border[i], 2, 186);//�޷�
        }
    }

    if (break_num_below_r_y && break_num_r_y && break_num_below_r_y && break_num_l_y )//����ʮ��ǰ�������ĵ㶼�У�
    {
        printf("\nע�⣺�ĵ㶼��\n");
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

