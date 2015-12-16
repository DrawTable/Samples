
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string windowName = "mainwindow";
string resultWindow = "result";
Mat frame, hsv, thresholded;


void optimizeThreshold(Mat &frame);
void handleFrame(Mat &frame);
int main(int argc, char *argv[])
{

    string filename = "/Users/paulnta/Movies/hand.mov";

    namedWindow(windowName, WINDOW_KEEPRATIO);
    namedWindow(resultWindow, WINDOW_KEEPRATIO);

    Mat frame;
   VideoCapture cap(filename); //open camera no 1 external
   if (!(cap.grab() && cap.isOpened())) //check if external camera is working
   {
        cout << "Camera is not ready" << endl;
   }
   for (;;) //wait 30 millisecond and check for esc key to exit
   {
          cap.read(frame); //read image from camera and store it in frame variable
          if(frame.empty()){
              break;
          }

          handleFrame(frame);

        waitKey(33);
   }

    return  0;
}

bool contour_compare_area(const vector<Point> c1, const vector<Point> c2){
    return contourArea(c1) > contourArea(c2);
}

void handleFrame(Mat &frame){

    cout << "handleFrmae" << endl;
    cvtColor(frame, hsv, CV_BGR2HSV);
    inRange(hsv, Scalar(0,42,110), Scalar(18,104,255), thresholded);
    optimizeThreshold(thresholded);

    vector<vector<Point>> contours;
    vector<Point> handApprox;


     findContours(thresholded, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
     if(contours.size() > 0){
        sort(contours.begin(), contours.end(), contour_compare_area);

        Scalar color(255,0,255);
        drawContours(frame, contours, 0, color,1);

        vector<Point> handContour = contours.at(0);
        approxPolyDP(contours.at(0),handApprox, 0.02* arcLength(handContour,true), true);

        Mat approx = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
        polylines(approx, handApprox, true, Scalar(0,255,255), 2);
        imshow(resultWindow, approx);
    }



    imshow(windowName, frame);
}

void optimizeThreshold(Mat &frame){

    //morphological opening (remove small objects from the foreground)
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    GaussianBlur(thresholded, thresholded, Size(3,3), 0);
}
