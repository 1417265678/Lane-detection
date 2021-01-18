#include "image_process.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

//构造函数
image_process::image_process(Mat image):image_src(image){}
//成员函数
Mat image_process::process(){
    //*************reading image******************
    Mat image;
    image = image_src ;
    if(image.empty()){
        cout <<"reading error"<<endl;
    }

    //***************gray image*******************
    Mat image_gray;
    cvtColor(image,image_gray, CV_BGR2GRAY);
    //imshow("1",image_gray);

    //************gaussian smoothing**************
    Mat image_gau;
    GaussianBlur(image_gray, image_gau, Size(5,5),0,0); 

    //******************canny*********************
    Mat image_canny;
    Canny(image_gau, image_canny,100, 200, 3);

    //**************interesting aera*************
    Mat dstImg;
    Mat mask = Mat::zeros(image_canny.size(), CV_8UC1);
    Point PointArray[4];
    PointArray[1] = Point(0.38*mask.cols,0.65*mask.rows); // 0.3 0.5
    PointArray[2] = Point(0.6*mask.cols,0.65*mask.rows); // 0.7 0.5
    PointArray[3] = Point(0.85*mask.cols,0.93*mask.rows); //0.1 0.8
    PointArray[0] = Point(0.15*mask.cols,0.93*mask.rows);  //0.9 0.8
    /*PointArray[0] = Point(0.2*mask.cols,0.8*mask.rows); // 0.3 0.5
    PointArray[1] = Point(0.4*mask.cols,0.5*mask.rows); // 0.7 0.5
    PointArray[2] = Point(0.6*mask.cols,0.5*mask.rows); //0.1 0.8
    PointArray[3] = Point(0.8*mask.cols,0.8*mask.rows);  //0.9 0.8*/
    fillConvexPoly(mask,PointArray,4,Scalar(255));
    bitwise_and(mask,image_canny,dstImg);
    imshow("2",mask);
    imshow("3",dstImg);

//************************houghline*******************累计概率霍夫变换
    vector<Vec4i> lines;
    int rho = 1;
    double theta = CV_PI/180;
    int threshold = 30;
    int min_line_len = 0; //***100
    int max_line_gap = 100;
    HoughLinesP(dstImg,lines,rho,theta,threshold,min_line_len,max_line_gap);
    //cout<<lines[1]<<endl;
//***************draw line update********************************
    Mat image_draw = Mat::zeros(image_canny.size(),CV_8UC3);
    vector<int> right_x, right_y, left_x, left_y;
    double slope_right_sum;
    double b_right_sum ;
    double slope_left_sum ;
    double b_left_sum ;
    double slope_right_mean;
    double slope_left_mean;
    double b_right_mean;
    double b_left_mean;
    vector<double> slope_right, slope_left,b_right, b_left;
    for(size_t i= 0;i<lines.size();i++){
        Vec4i L;
        double slope,b;
        L = lines[i];
        slope = (L[3]-L[1])*1.0/(L[2]-L[0]);
        b = L[1]-L[0]*slope;

        if (slope >=0.2/*0.2*/){
            slope_right.push_back(slope);
            b_right.push_back(b);
            //right_x.push_back((L[0],L[2]));
            //right_y.push_back((L[1],L[3]));

          }
        if (slope <= -0.15)/*else*/{
            slope_left.push_back(slope);
            b_left.push_back(b);
            // left_x.push_back((L[0],L[2]));
            // right_y.push_back((L(1),L[3]));

            }


        }
    slope_right_sum = accumulate(slope_right.begin(), slope_right.end(),0.0);
    b_right_sum = accumulate(b_right.begin(), b_right.end(),0.0);
    slope_left_sum = accumulate(slope_left.begin(),slope_left.end(),0.0);
    b_left_sum = accumulate(b_left.begin(),b_left.end(),0.0);
    slope_right_mean = slope_right_sum/slope_right.size();
    slope_left_mean = slope_left_sum/slope_left.size();
    b_right_mean = b_right_sum/b_right.size();
    b_left_mean = b_left_sum/b_left.size();
    cout <<"slope_right: "<<slope_right_sum<<endl;
    double y1r = 0.65*mask.rows;
    double y2r = 1.0*mask.rows;
    double y1l = 1.0*mask.rows;
    double y2l = 0.65*mask.rows;
    double x1r = (y1r - b_right_mean) / slope_right_mean;
    double x2r = (y2r - b_right_mean) / slope_right_mean;
    double x1l = (y1l - b_left_mean) / slope_left_mean;
    double x2l = (y2l - b_left_mean) / slope_left_mean;
       /*double x1r = 630; //550;
       double x2r = 850; //850;
       double x1l = 240; //120;
       double x2l = 540; //425;
       int y1r = slope_right_mean * x1r + b_right_mean;
       int y2r = slope_right_mean * x2r + b_right_mean;
       int y1l = slope_left_mean * x1l + b_left_mean;
       int y2l = slope_left_mean * x2l + b_left_mean;*/
      if(x1r>100&&x1l>100){
      
      line(image_draw, Point(x1r,y1r),Point(x2r,y2r),Scalar(0,0,255),20,CV_AA);
      line(image_draw, Point(x1l,y1l),Point(x2l,y2l),Scalar(0,0,255),20,CV_AA);}
//*************mix two image*************************
    Mat image_mix = Mat::zeros(image_canny.size(),CV_8UC3);
    addWeighted(image_draw,1,image,1,0.0,image_mix);
//**************out put****************************
    //return image_mix;

    //VideoWriter video("testtest.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30.0, Size(1920, 1080));

    //String img_path = "E:\\软件所资料\\科创项目材料\\图片\\无人机视频所有图片\\";
    //vector<String> img;
    //resize(image_mix, image_mix, Size(1920, 1080));
    //video << image_mix;
    return image_mix;
}
//析构函数
image_process::~image_process() {}
