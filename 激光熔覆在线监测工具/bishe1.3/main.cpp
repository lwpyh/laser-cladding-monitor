#include <opencv2\opencv.hpp>  
#include <cv.h>  
#include <cxcore.h>  
#include <signal.h>  
#include <stdlib.h>  
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <iostream>  
using namespace cv;  
using namespace std;

Mat src,dst,dst1,dst2,b,dst3,dst4;  
Mat Image;  
int medianBlur1 = 3;  //��ֵ�˲���ʼֵ
Mat dstImage ;  
Mat frame;  //����һ��Mat���������ڴ洢ÿһ֡��ͼ��
double time0;
#define LEN 128  
 int exposure=-1;
int g_nContrastValue=80; //�������Աȶȳ�ֵ
int g_nBrightValue=80;  //���������ȳ�ֵ
Mat g_srcImage,g_dstImage;


 //-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
//     ������ȫ�ֺ�������
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void *);
 static void on_medianBlur(int, void *); 
//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-------------------------------------------------------------------------------------------------

int main()
{
	 uchar lutData[256]; 
	//t=(2^i-1)/255;
    for ( int i = 0; i<256; i++)  
   {  
        //if (i > 170)  
	  // lutData[i] =1.65*(256-log(0.3*(256-i))*35)-172;//�Ż��㷨
	    //lutData[i] =7.25*(256-log(0.3*(256-i))*9.0)-1573;
	    lutData[i]=46*(log(i));
    }  
	char SaveName[LEN];  
    int pictureNumber = 1;
    Mat lut(1, 256, CV_8UC1, lutData);

    VideoCapture capture("c://Video.mp4");
    Mat frame;
    while (capture.isOpened())
    {
        capture >> frame;
		Image=Mat(frame);
        imshow("capture", frame);
        if (cvWaitKey(40) == 27)  //cvWaitKey�Ĳ����൱�ڶ���msһ֡��������40msһ֡��1s25֡
            break;                //��ESC���˳�
     pyrDown(Image,dst1,Size(Image.cols/2,Image.rows/2));//������,��������ΪԴͼ��Ŀ��ͼ�񣬱任��Ϊ1/4��С 
	cvtColor(dst1,dst1,CV_BGR2GRAY);//��Ҷ�ͼ��
	namedWindow("output1");
    createTrackbar("Kernal Value:","output1",&medianBlur1,50,on_medianBlur);  
    on_medianBlur(medianBlur1,0);

//--------α��ɫ����------------
//	applyColorMap(dst2,dst,COLORMAP_JET);//α��ɫ����ģʽΪ0-255��ͳģʽ
//	dstImage = dst.clone();
//	imshow("output2",dst);//α��ɫ�����Ч��
	LUT(dst2, lut, b);  //�����Լ����㷨���Ż�����
    imshow("zijisuanfa2", b);  
    applyColorMap(b,dst3,COLORMAP_JET);//α��ɫ����ģʽΪ0-255��ͳģʽ
	imshow("output3",dst3);//����ڶ����Ż����
	waitKey(5);

//---------------����������ڶԱȶȺ�����-------------
   //�ı����̨ǰ��ɫ�ͱ���ɫ
       //system("color5F"); 
 
       //�����ʱ��һ֡��
   //    g_srcImage= dst3;
        //      if(!g_srcImage.data ) { printf("Oh��no����ȡg_srcImageͼƬ����~��\n"); return false; }
    //   g_dstImage= Mat::zeros( dst.size(), dst.type() );
 
       //�趨�ԱȶȺ����ȵĳ�ֵ
       //g_nContrastValue=80;
       //g_nBrightValue=80;
 
       //��������
	 //  namedWindow("��Ч��ͼ���ڡ�");
 
       //�����켣��
      // createTrackbar("�Աȶȣ�", "��Ч��ͼ���ڡ�",&g_nContrastValue,300,ContrastAndBright );
     //  createTrackbar("��   �ȣ�","��Ч��ͼ���ڡ�",&g_nBrightValue,200,ContrastAndBright );
     //  createTrackbar("ģ���ȣ� ","��Ч��ͼ���ڡ�",&medianBlur1,50,on_medianBlur);
       //���ûص�����
      // ContrastAndBright(g_nContrastValue,0);
     //  ContrastAndBright(g_nBrightValue,0);
     //  on_medianBlur(medianBlur1,0);


//---------�����¼����ϵ�q��ʱ����ȡһ��ͼƬ-------------  
        if(waitKey(20) == 'q')  
        {  
            sprintf(SaveName , "D:/111/%5d.png" ,pictureNumber++);//����ͼƬ����ţ�����  
            imwrite(SaveName , dst3);//����ͼƬ 
        }  
        //imshow("��Ƶ����", Image);  //��ʾͼƬ  
    }  
    return 0;  
}


//------��˹�˲��켣���ص�����------------  
static void on_medianBlur(int, void *)  
{  
	int ksize=medianBlur1*2+1;
    medianBlur(dst1,dst2,ksize);//��ֵ�˲�ȡ�м�ֵ������ΪԴͼ��Ŀ��ͼ�񣬾���˴�С����ģ��С��һ��ȡ������
	imshow("output1",dst2);
}

//---------------------�ı�ͼ��ԱȶȺ�����ֵ�Ļص�����------------------------------
static void ContrastAndBright(int, void *)
{
 
       //��������
      // namedWindow("��ԭʼͼ���ڡ�", 1);
 
       //����forѭ����ִ������ g_dstImage(i,j) =a*g_srcImage(i,j) + b
       for(int y = 0; y < g_srcImage.rows; y++ )
       {
              for(int x = 0; x < g_srcImage.cols; x++ )
              {
                     for(int c = 0; c < 3; c++ )
                     {
                            g_dstImage.at<Vec3b>(y,x)[c]= saturate_cast<uchar>( (g_nContrastValue*0.01)*(g_srcImage.at<Vec3b>(y,x)[c] ) + g_nBrightValue );
                     }
              }
       }
 
       //��ʾͼ��
      // imshow("��ԭʼͼ���ڡ�", g_srcImage);
       imshow("��Ч��ͼ���ڡ�", g_dstImage);
}