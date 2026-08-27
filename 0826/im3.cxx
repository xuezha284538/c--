#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char const *argv[])
{
    Mat m1 = imread("lena.jpg");
    Mat m2, m3, m5, m7;
    // blur(m1,m3,Size(3,3));//做均值滤波
    // blur(m1,m5,Size(5,5));//做均值滤波
    // blur(m1,m7,Size(17,17));//做均值滤波
    //高斯滤波
    // GaussianBlur(m1,m3,Size(3,3),0,0);//高斯滤波 边缘清晰
    // GaussianBlur(m1,m5,Size(5,5),0,0);
     GaussianBlur(m1,m7,Size(7,7),0,0);
    // 中值滤波 //细节抹除 像素差别过大会被中和
    // medianBlur(m1,m3,3);
    // medianBlur(m1,m5,9);
    // medianBlur(m1,m7,57);
    //双边滤波
    // bilateralFilter(m1,m3,3,75,75);
    // bilateralFilter(m1,m5,9,75,75);
    // bilateralFilter(m1,m7,15,75,75);

    // imshow("s1", m5);
    //转成灰度图
    Mat gray;
    cvtColor(m7,gray,COLOR_BGR2GRAY);
    imshow("gray",gray);
    Mat bin,bin2;
    //二值化
    // threshold(gray,bin,128,255,THRESH_BINARY);

    // imshow("bin",bin);
    // //大津法 自动匹配阈值
    // threshold(gray,bin2,0,255,THRESH_BINARY|THRESH_OTSU);
    // imshow("bin2",bin2);
    // imshow("s3", m3);
    // imshow("s5", m5);
    // imshow("s7", m7);
    //边缘检测
    Mat can;
    Canny(gray,can,64,128);
    imshow("bin",can);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
