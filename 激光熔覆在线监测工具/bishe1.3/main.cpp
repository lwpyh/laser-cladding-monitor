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
int medianBlur1 = 3;  //中值滤波初始值
Mat dstImage ;  
Mat frame;  //定义一个Mat变量，用于存储每一帧的图像
double time0;
#define LEN 128  
 int exposure=-1;
int g_nContrastValue=80; //滑块条对比度初值
int g_nBrightValue=80;  //滑块条亮度初值
Mat g_srcImage,g_dstImage;


 //-----------------------------------【全局函数声明部分】--------------------------------------
//     描述：全局函数声明
//-----------------------------------------------------------------------------------------------
static void ContrastAndBright(int, void *);
 static void on_medianBlur(int, void *); 
//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------

int main()
{
	 uchar lutData[256]; 
	//t=(2^i-1)/255;
    for ( int i = 0; i<256; i++)  
   {  
        //if (i > 170)  
	  // lutData[i] =1.65*(256-log(0.3*(256-i))*35)-172;//优化算法
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
        if (cvWaitKey(40) == 27)  //cvWaitKey的参数相当于多少ms一帧，现在是40ms一帧，1s25帧
            break;                //按ESC就退出
     pyrDown(Image,dst1,Size(Image.cols/2,Image.rows/2));//降采样,参数依次为源图像，目标图像，变换后为1/4大小 
	cvtColor(dst1,dst1,CV_BGR2GRAY);//变灰度图像
	namedWindow("output1");
    createTrackbar("Kernal Value:","output1",&medianBlur1,50,on_medianBlur);  
    on_medianBlur(medianBlur1,0);

//--------伪彩色处理------------
//	applyColorMap(dst2,dst,COLORMAP_JET);//伪彩色处理，模式为0-255传统模式
//	dstImage = dst.clone();
//	imshow("output2",dst);//伪彩色处理的效果
	LUT(dst2, lut, b);  //采用自己的算法的优化处理
    imshow("zijisuanfa2", b);  
    applyColorMap(b,dst3,COLORMAP_JET);//伪彩色处理，模式为0-255传统模式
	imshow("output3",dst3);//输出第二个优化结果
	waitKey(5);

//---------------创建滑块调节对比度和亮度-------------
   //改变控制台前景色和背景色
       //system("color5F"); 
 
       //读入此时这一帧的
   //    g_srcImage= dst3;
        //      if(!g_srcImage.data ) { printf("Oh，no，读取g_srcImage图片错误~！\n"); return false; }
    //   g_dstImage= Mat::zeros( dst.size(), dst.type() );
 
       //设定对比度和亮度的初值
       //g_nContrastValue=80;
       //g_nBrightValue=80;
 
       //创建窗口
	 //  namedWindow("【效果图窗口】");
 
       //创建轨迹条
      // createTrackbar("对比度：", "【效果图窗口】",&g_nContrastValue,300,ContrastAndBright );
     //  createTrackbar("亮   度：","【效果图窗口】",&g_nBrightValue,200,ContrastAndBright );
     //  createTrackbar("模糊度： ","【效果图窗口】",&medianBlur1,50,on_medianBlur);
       //调用回调函数
      // ContrastAndBright(g_nContrastValue,0);
     //  ContrastAndBright(g_nBrightValue,0);
     //  on_medianBlur(medianBlur1,0);


//---------当按下键盘上的q键时，截取一张图片-------------  
        if(waitKey(20) == 'q')  
        {  
            sprintf(SaveName , "D:/111/%5d.png" ,pictureNumber++);//设置图片的序号，名称  
            imwrite(SaveName , dst3);//保存图片 
        }  
        //imshow("视频窗口", Image);  //显示图片  
    }  
    return 0;  
}


//------高斯滤波轨迹条回调函数------------  
static void on_medianBlur(int, void *)  
{  
	int ksize=medianBlur1*2+1;
    medianBlur(dst1,dst2,ksize);//中值滤波取中间值，依次为源图像，目标图像，卷积核大小（掩模大小）一般取奇正数
	imshow("output1",dst2);
}

//---------------------改变图像对比度和亮度值的回调函数------------------------------
static void ContrastAndBright(int, void *)
{
 
       //创建窗口
      // namedWindow("【原始图窗口】", 1);
 
       //三个for循环，执行运算 g_dstImage(i,j) =a*g_srcImage(i,j) + b
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
 
       //显示图像
      // imshow("【原始图窗口】", g_srcImage);
       imshow("【效果图窗口】", g_dstImage);
}