// vd.cxx
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
    // 视频采集
    VideoCapture capture;
    // capture.open(0,CAP_ANY);
    if (!capture.open(1, CAP_ANY))
    {
        cout << "open error" << endl;
        return -1;
    }
    // 图像容器,存储一帧画面
    Mat frame;
    Mat f, can, f1;
    // 解码,获得下一帧
    while (capture.read(frame))
    {
        putText(frame, "2026/8/27", Point(405, 15), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
        imshow("video", frame);
        // GaussianBlur(frame,f,Size(3,3),0,0);
        bilateralFilter(frame, f1, 5, 75, 75);
        cvtColor(f1, f, COLOR_BGR2GRAY);
        // threshold(f,f,127,255,THRESH_BINARY);
        putText(f, "2026/8/27", Point(405, 15), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
        imshow("2", f);
        Canny(f, can, 64, 128);
        putText(can, "2026/8/27", Point(405, 15), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
        imshow("1", can);
        if (waitKey(70) == 'q')
            break;
    }

    capture.release();

    destroyAllWindows();
    return 0;
}
