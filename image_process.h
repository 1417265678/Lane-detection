#ifndef PROJECT1_IMAGE_PROCESS_H
#define PROJECT1_IMAGE_PROCESS_H
#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class image_process {
public:
    Mat image_src;
    Mat image_dst;
    image_process(Mat image);

    Mat process();
    ~image_process();

};
#endif