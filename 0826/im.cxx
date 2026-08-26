#include<iostream>
#include<opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
    /* code */
    //图形容器 iamge container
    //0~255
    //rgb 
    //opencv bgr blue green red
    Mat m1(400,400,CV_8UC3,Scalar(0,255,255));

    Mat m2(Size(400,400),CV_8UC1);
    m2.setTo(Scalar(200));

    cout<<m1<<endl;
    //写到文件中保存
    imwrite("m2.png",m2);
   // imshow("m1.png",m1);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
