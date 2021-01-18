//g++ -o main main.cpp /home/zhb/Desktop/qqw/process_image.cpp `pkg-config --cflags --libs opencv`
#include <iostream>
#include <opencv2/opencv.hpp>
#include<vector>
#include <opencv2/highgui/highgui.hpp>
#include"image_process.h"
#include<string>
using namespace std;
using namespace cv;
int main(){
    Mat image;
    Mat image_result;
    VideoWriter video("164.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30.0, Size(1920, 1080));
    VideoCapture capture("6.mp4");
    Mat frame;
    if(!capture.isOpened()) {
        cout << "can not open video" << endl;
        return -1;
    }
    while(capture.isOpened()){
        capture>>frame;
        image_process image2(frame);
        Mat image_result2;
        image_result2 = image2.process();
        imshow("result_video",image_result2);
        resize(image_result2, image_result2, Size(1920, 1080));
        video << image_result2;
        waitKey(1);
    }
}
