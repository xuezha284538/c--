// 视频录制 :采集 存储
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
int main(int argc, char const *argv[])
{
    VideoCapture capture;
    VideoWriter out;
    if (!capture.open(1, CAP_ANY))
    {
        cout << "error" << endl;
        return -1;
    }
    // 1. 存储路径
    // 2. 四字符的编码器代号
    // 3. 帧数
    // 4. 图片大小 尺寸
    // 5. 有颜色默认true 灰白false
    string path = "out.mp4";
    // string path = "out1.avi";
    // string path = "264.mp4";
    // string path = "265.mp4";
    // int fource = VideoWriter::fourcc('a', 'v', 'c', '1');//h.264
    // int fource = VideoWriter::fourcc('X', 'V', 'I', 'D');//avi格式
    // int fource = VideoWriter::fourcc('h', 'e', 'v', '1');//h.265
    int fource = VideoWriter::fourcc('m', 'p', '4', 'v'); //mp4
    
    int fps = 10;
    //分辨率超出后 不匹配 不兼容播放不了 
    int w = capture.get(CAP_PROP_FRAME_WIDTH), h = capture.get(CAP_PROP_FRAME_HEIGHT);
    Size size(w, h);
    if (!out.open(path, fource, fps, size,0))
    {
        cout << "error out.open" << endl;
        return -1;
    }
    Mat frame,gray;
    int i=0;
    while (i<200)
    {
        capture.read(frame);
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        out.write(gray);
        imshow("video", frame);
        if (waitKey(100) == 'q')
            break;
            i++;
    }
    out.release();
    capture.release();
    destroyAllWindows();
    return 0;
}
