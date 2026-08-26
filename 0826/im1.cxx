#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char const *argv[])
{
    Mat m1 = imread("lena.jpg", IMREAD_COLOR);
    // Mat m2 = imread("lena.jpg", IMREAD_GRAYSCALE);
    // Mat m3 = imread("lena.jpg", IMREAD_REDUCED_COLOR_2);
    // Mat m4;
    // //更改大小为120*120
    // resize(m1,m4,Size(120,120));
    // 缩放0.5倍
    // resize(m1,m4,Size(),0.5,0.5,INTER_AREA);
    // 缩放2倍
    // resize(m1,m4,Size(),2,2,INTER_AREA);

    // imwrite("m4.png",m4);
    // imshow("m4",m4);
    // // if (m.empty())
    // {
    //     return -1;
    // }

    // rectangle(m1, Rect(244, 244, 50, 50), Scalar(0, 0, 0), 2);
    // rectangle(m1, Rect(310, 249, 50, 50), Scalar(0, 0, 0), 2);
    // circle(m1, Point(317, 317), 15, Scalar(0, 0, 255), -1);
    // putText(m1, "2026/8/26", Point(m1.rows / 2 - 50, 20), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 255, 255));

    Mat mask(m1.size(), m1.type());
    mask.setTo(Scalar(0, 22, 22));
    Mat light,dark;
    subtract(m1,mask,dark);
    add(m1,mask,light);

    imshow("1",m1);
    imshow("2",mask);
    imshow("3",dark);
    imshow("4",light);
    
    waitKey(0); // 等待按键，否则窗口不刷新
    // imshow("m1", m1);
    //     //裁剪一块区域
    //     Rect r(0,0,m1.rows/3,m1.cols/3);
    //     //poi
    //     Mat poi=m1(r);
    //     imshow("poi",poi);

    // // imshow("m2", m2);
    // imshow("m3", m3);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
