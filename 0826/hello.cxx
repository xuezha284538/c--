// hello.cxx
#include <iostream>
#include "opencv4/opencv2/opencv.hpp"
using namespace std;
using namespace cv;
int main()
{
    
    // imread,imwrite 读 写图片
    // Mat
    Mat img = imread("./lena.jpg");
    // 是否为空
    if (img.empty())
    {
        cout << "error" << endl;
        return -1;
    }
    // 显示图形
    imshow("lena", img);
    // 等待键盘事件
    waitKey(0);
    // 关闭所有窗口
    destroyAllWindows();
    return 0;
}