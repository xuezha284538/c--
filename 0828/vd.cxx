// 视频录制 :采集 存储
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
int main(int argc, char const *argv[])
{
    VideoCapture capture;

    if (!capture.open(1,CAP_ANY))
    {
        cout << "error" << endl;
        return -1;
    }
    Mat frame, prevframe, gray,gray1, prevgray, diff;

    capture.read(prevframe);

    cvtColor(prevframe, prevgray, COLOR_BGR2GRAY);
    while (1)
    {
        capture.read(frame);
        if (frame.empty())
        {
            return -1;
        }
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(7, 7), 0);
        // medianBlur(gray,gray,5);
        // bilateralFilter(gray,gray1,60,75,75);
        absdiff(gray, prevgray, diff);
        threshold(diff, diff, 17, 255, THRESH_BINARY);

        //降噪
        Mat k=getStructuringElement(MORPH_RECT,Size(3,3));
        erode(diff,diff,k);
        dilate(diff,diff,k);

        vector<vector<Point>> cont;
        findContours(diff, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (auto &c : cont)
        {
            if (contourArea(c) < 900)
                continue;
            Rect box = boundingRect(c);
            rectangle(frame, box, Scalar(255, 0, 0), 2);
        }
        prevgray = gray.clone();
        imshow("video", frame);
        imshow("bin", diff);
        if (waitKey(50) == 'q')
            break;
            
    }
    capture.release();
    destroyAllWindows();
    return 0;
}
