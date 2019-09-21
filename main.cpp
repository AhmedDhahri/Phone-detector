#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include<iostream>

#define ANGULAR_STEP 2
#define SCALING_STEP 0.1
#define SLIDING_STEP 2

using namespace cv;
using namespace std;

typedef struct{
int h;
int w;
Mat m;
}ImRot;
typedef struct{
int h;
int w;
int x;
int y;
}Box;

Mat Window(Mat m,int x, int y, int h, int w);
Mat get_Gray_Frame(VideoCapture c);
Mat thershHolding(Mat m, int th);
VideoCapture init();
ImRot Rotate(Mat mask, double angle);
Mat Scale(Mat m, double r);
double cmp_img(Mat m1, Mat m2);
Mat draw_Box(Mat m,Box b);
void check_end();

int main(int argc, char** argv){
    VideoCapture cap = init();
    Mat mask = imread("mask.jpg",CV_LOAD_IMAGE_COLOR);

    while(true){
        Mat w = thershHolding(get_Gray_Frame(cap),90);


        for(double i = 0.5;i<=2;i+=SCALING_STEP){
            Mat scaled = Scale(mask, i);
            for(int j = 0;j<=180;j+=ANGULAR_STEP){
                ImRot ir = Rotate(scaled, j);
                Mat rotated = ir.m;
                Size s;
                s.height = ir.h;
                s.width = ir.w;
                for(int n1 = 0;n1<=480-w;n1+=SLIDING_STEP){
                    for(int n2 = 0;n2<=640-h;n2+=SLIDING_STEP){

                    }
                }
            }
        }

        Box b;b.h = 200;b.w = 200;b.x = 100;b.y = 100;
        w = draw_Box(w,b);
        imshow("Video", w);
        //imshow("Video", get_Gray_Frame(cap));
        check_end();
    }
    return 0;
}

Mat Window(Mat m,int x, int y, int h, int w){
    Rect rc;
    rc.x = x;
    rc.y = y;
    rc.height = h;
    rc.width = w;
    return m(rc).clone();
}
Mat get_Gray_Frame(VideoCapture c){
    Mat frame;
    c
     >> frame;
    Mat frameg;
    cvtColor( frame, frameg, CV_BGR2GRAY );
    return frameg;
}
Mat thershHolding(Mat m, int th){
    Mat f;
    threshold( m, f, th, 255,0);
    return f;
}
VideoCapture init(){
    VideoCapture cap;
    if(!cap.open(0))
        exit(1);
    return cap;
}
ImRot Rotate(Mat mask, double angle){
    Point2f center((mask.cols-1)/2.0, (mask.rows-1)/2.0);
    Mat rot = getRotationMatrix2D(center, angle, 1.0);
    Rect2f bbox = RotatedRect(Point2f(), mask.size(), angle).boundingRect2f();
    rot.at<double>(0,2) += bbox.width/2.0 - mask.cols/2.0;
    rot.at<double>(1,2) += bbox.height/2.0 - mask.rows/2.0;
    Mat f;
    warpAffine(mask, f, rot, bbox.size());
    ImRot ir;
    ir.m = f;
    ir.h = bbox.size().height;
    ir.w = bbox.size().width;
    return ir;
    }
Mat Scale(Mat m, double r){
    Mat f;
    resize(m, f, Size(), r, r);
    return f;
}
double cmp_img(Mat m1, Mat m2){
    double e = 0;
    Mat a;
    absdiff(m1, m2, a);
    int cells = a.cols*a.rows;
    double s = sum(a)[0];
    e = s/(cells*255);
    return e;
}
Mat draw_Box(Mat m,Box b){
    Mat f = m;
    Rect rect(b.x, b.y, b.w, b.h);
    rectangle(f, rect, Scalar(150,0,165,0),2,8,0);
    return f;
}
void check_end(){
    if(waitKey(10) == 27) exit(1);
}
