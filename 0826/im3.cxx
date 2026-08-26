#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char const *argv[])
{
    Mat m1=imread("lena.jpg");
    Mat m2,m3,m5,m7;
    blur(m1,m3,Size(3,3));//做均值滤波
    blur(m1,m5,Size(5,5));//做均值滤波
    blur(m1,m7,Size(17,17));//做均值滤波

imshow("s1",m1);
imshow("s3",m3);
imshow("s5",m5);
imshow("s7",m7);
waitKey(0);
destroyAllWindows();

    return 0;
}

