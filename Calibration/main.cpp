#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

// Scalar(51,25,203) to Scalar(103,151,246)
string windowName = "mainWindow";
string screenWindowName = "screen";
string grayScaleWindow = "grayScale";
string contouredWindow = "contours";
string edgedWindow = "edged";
Mat orig_img;
bool objectSelected = false;
RNG rng;

int currentImage = 0;
string images[] = {
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration01.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration02.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration03.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration04.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration05.JPG",
    "/Users/paulnta/Documents/HEIG4/PDG/tests/calibration06.JPG"
};

Vec3b hsvMin = {179, 255, 255}; // default value for hsv min
Vec3b hsvMax = {0, 0, 0};       // default valude for hsv max

int blurSize = 5;
int maxBlur = 20;
int lowThreshold = 20;
int maxThreshold = 100;
int ratioThreshold = 3;
int apertureSize = 3;

void detectScreenByColor();
void detectByContours();
void optimizeThreshold(Mat &frame);
void detectGrayScale(int, void*);
vector<Point> getContours(Mat& img);

void setHsvMin(Vec3b pixel){
    for(int i=0; i < 3; i++)
        hsvMin[i] = (pixel[i] < hsvMin[i] ? pixel[i] : hsvMin[i]);
}

void setHsvMax(Vec3b pixel){
    for(int i=0; i < 3; i++)
        hsvMax[i] = (pixel[i] > hsvMax[i] ? pixel[i] : hsvMax[i]);
}

// GET HSV RANGE
static void onMouse( int event, int x, int y, int, void* )
{
    if(event == cv::EVENT_LBUTTONDOWN){
        ++currentImage;
        objectSelected = true;
        orig_img = imread(images[currentImage % 6]);
        resize(orig_img, orig_img,Size(orig_img.rows * 0.4, orig_img.cols * 0.4));
        detectGrayScale(0,0);
    }
//    if(event == cv::EVENT_LBUTTONUP)
//        objectSelected = false;

//    if(objectSelected){
//        Vec3b pixel = hsv.at<Vec3b>(Point(x,y));
//        setHsvMin(pixel);
//        setHsvMax(pixel);
//        qDebug("Min: Scalar(%d,%d,%d), Max: Scalar(%d,%d,%d)", hsvMin[0], hsvMin[1], hsvMin[2],  hsvMax[0], hsvMax[1], hsvMax[2]);
//        detectScreenByColor();
//        detectByContours();
//    }
}

bool contour_compare_area(const vector<Point> c1, const vector<Point> c2){
    return contourArea(c1) > contourArea(c2);
}

//void detectByContours(){

////    Canny(orig_img, edged, 80, 300, 3);
////    inRange(hsv, hsvMin, hsvMax, edged);
//    inRange(hsv, Scalar(51,25,203), Scalar(103,151,246), edged);
//    optimizeThreshold(edged);
//    contoured = Mat::zeros(orig_img.rows, orig_img.cols, CV_8UC3);

//    vector<vector<Point>> contours;
//    vector<Point> maxContours;
//    vector<Point> screenApprox;

//    findContours(edged, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//    qDebug() << "nb_contours : " << contours.size();
//    if(contours.size() == 0){return;}
//    std::sort(contours.begin(), contours.end(), contour_compare_area);
//    maxContours = contours.at(0);
//    approxPolyDP(contours.at(0), screenApprox, 0.1* arcLength(contours.at(0),true), true);
//    qDebug() << "nb point approx" << screenApprox.size();

//    for(int i=0; i < contours.size(); i++){
//        drawContours(edged, contours, i, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), 2);
//    }

//    polylines(contoured, screenApprox, true, Scalar(0,255,0), 1);
//    polylines(contoured, maxContours, true, Scalar(0,0,255),2);

//    imshow(contouredWindow, contoured);
//    imshow(edgedWindow, edged);
//    imshow(windowName, orig_img);
//}

//vector<Point> getContours(Mat& img){
//    vector<vector<Point>> contours;
//    findContours(img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//    std::sort(contours.begin(), contours.end(), contour_compare_area);
//    for(int i=0; i < contours.size(); i++){
//        qDebug() << contourArea(contours.at(i));
//        drawContours(contoured, contours, i, Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255)), 2);
//    }

//    return contours.at(0);
//}

//void detectScreenByColor(){

////    inRange(hsv, hsvMin, hsvMax, screen);
//    inRange(hsv, Scalar(51,25,203), Scalar(103,151,246), screen);

//    //morphological opening (remove small objects from the foreground)
//    optimizeThreshold(hsv);

//    imshow(screenWindowName, screen);
//    imshow(windowName, orig_img);
//}

void detectGrayScale(int, void*){
    qDebug() << "detectGrayScale";
    blurSize = blurSize == 0 ? 1 : blurSize;

    Mat result, grayscale, contoured;
    result = grayscale = contoured = Mat::zeros(orig_img.rows, orig_img.cols, CV_8UC3);

    cvtColor(orig_img, grayscale, CV_BGR2GRAY);
    blur(grayscale, result, Size(blurSize,blurSize));
    Canny(result, result, lowThreshold, lowThreshold*ratioThreshold, apertureSize);
    contoured = Mat::zeros(orig_img.rows, orig_img.cols, CV_8UC3);

    vector<vector<Point>> contours;
    findContours(result, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    std::sort(contours.begin(), contours.end(), contour_compare_area);
    for(int i=0; i < contours.size(); i++){
        drawContours(orig_img, contours,i, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)),1);
    }

    imshow(windowName, orig_img);
    imshow(grayScaleWindow, result);
}

int main(int argc, char *argv[])
{
    namedWindow(windowName, WINDOW_KEEPRATIO);
    namedWindow(grayScaleWindow, WINDOW_KEEPRATIO);
    namedWindow(contouredWindow, WINDOW_KEEPRATIO);
    createTrackbar("Blur", grayScaleWindow, &blurSize, maxBlur,detectGrayScale);
    createTrackbar("Threshold", grayScaleWindow, &lowThreshold, maxThreshold,detectGrayScale);
    setMouseCallback(windowName, onMouse, 0);

    orig_img = imread("/Users/paulnta/Documents/HEIG4/PDG/tests/calibration00.JPG");
    resize(orig_img, orig_img,Size(orig_img.rows * 0.2, orig_img.cols * 0.2));

    detectGrayScale(0,0);

    waitKey(0);
    return 0;
}

void optimizeThreshold(Mat &frame){

    //morphological opening (remove small objects from the foreground)
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate( frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    GaussianBlur(frame, frame, Size(3,3), 0);
}

