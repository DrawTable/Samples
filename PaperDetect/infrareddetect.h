#ifndef INFRAREDDETECT_H
#define INFRAREDDETECT_H

#include "videocontroller.h"

class InfraredDetect : public VideoController
{
    typedef vector<Point> Contour;
    typedef vector<Contour> Contours;

    Mat orig, frame, hsv, thresholdLight, thresholdLed, contoured;
    Mat mLight, mLed, mMerge, test;

    string windowDebug, windowThreshold, windowThresholdLed, windowContours;
    int lowH = 56, lowS = 68, lowV = 68, highH = 148, highS = 255, highV = 255;
    int lowHLed = 0, lowSLed = 0, lowVLed = 155, highHLed = 255, highSLed = 155, highVLed = 255;

    static InfraredDetect* instance;

    InfraredDetect(): VideoController(){
        // windows
        windowDebug = "debug";
        windowThreshold = "threshold";
        windowThresholdLed = "thresholdLed";
        windowContours = "contours";

//        namedWindow(windowDebug, WINDOW_KEEPRATIO);
        namedWindow(windowThreshold, WINDOW_KEEPRATIO);
//        namedWindow(windowContours, WINDOW_KEEPRATIO);
        namedWindow(windowThresholdLed, WINDOW_KEEPRATIO);

        // trackbars
        createTrackbar("lowH",  windowThreshold, &lowH, 179, onTrackBarChanged);
        createTrackbar("lowS",  windowThreshold, &lowS, 255, onTrackBarChanged);
        createTrackbar("lowV",  windowThreshold, &lowV, 255, onTrackBarChanged);
        createTrackbar("highH", windowThreshold, &highH, 179, onTrackBarChanged);
        createTrackbar("highS", windowThreshold, &highS, 255, onTrackBarChanged);
        createTrackbar("highV", windowThreshold, &highV, 255, onTrackBarChanged);

        createTrackbar("lowH",  windowThresholdLed, &lowHLed, 179, onTrackBarChanged);
        createTrackbar("lowS",  windowThresholdLed, &lowSLed, 255, onTrackBarChanged);
        createTrackbar("lowV",  windowThresholdLed, &lowVLed, 255, onTrackBarChanged);
        createTrackbar("highH", windowThresholdLed, &highHLed, 179, onTrackBarChanged);
        createTrackbar("highS", windowThresholdLed, &highSLed, 255, onTrackBarChanged);
        createTrackbar("highV", windowThresholdLed, &highVLed, 255, onTrackBarChanged);
    }

public:

    static InfraredDetect* getInstance(){
        if(instance == nullptr)
            instance = new InfraredDetect();
        return instance;
    }

protected:

    void handleFrame(Mat &frame)
    {
        this->orig = frame;
        cvtColor(frame, hsv, CV_BGR2HSV);
        processThreshold();
        showImages();
    }

    void showImages(){
        imshow(windowDebug, hsv);
        imshow(windowContours, mMerge);
        imshow(windowThreshold, mLight);
        imshow(windowThresholdLed, mLed);
        imshow(windowName, this->frame);
    }

    void findObjectPosition(Mat& thresholded){
        // calculate the exact position of the light
        Moments m = moments(thresholded);
        double mArea = m.m00;
        Point lightCenter(double(m.m10/mArea), double(m.m01/mArea));

        if(lightCenter.x > 0 && lightCenter.y > 0){
            drawCross(frame, lightCenter, Scalar(0,255,0), 20);
        }
    }

    void drawCross(Mat &frame, Point center, Scalar color, int size){
        line(frame, Point(center.x, center.y -size), Point(center.x,center.y + size ), color, 2);
        line(frame, Point(center.x-size, center.y), Point(center.x + size,center.y), color, 2);
    }

    static void onTrackBarChanged(int, void*){
        // processThreshold is heavy
        // so we do it only when video is paused
        // if not we will see change in the next frame
        InfraredDetect* _this = getInstance();
        if(_this->isPaused()){
            _this->processThreshold();
            _this->showImages();
        }
    }

    void processThresholdLight(){
        inRange(hsv, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), thresholdLight);
        optimizeThreshold(thresholdLight);
    }

    void processThresholdLed(){
        inRange(hsv, Scalar(lowHLed, lowSLed, lowVLed), Scalar(highHLed, highSLed, highVLed), thresholdLed);
        optimizeThreshold(thresholdLed);
    }

    void processThreshold(){
        orig.copyTo(this->frame);
        processThresholdLed();
        processThresholdLight();
        processContours();
    }

    void processContours(){

        mLight = Mat::zeros(this->frame.rows, this->frame.cols, CV_8UC3);
        mLed = Mat::zeros(this->frame.rows, this->frame.cols, CV_8UC3);

        contoured = Mat::zeros(this->frame.rows, this->frame.cols, CV_8UC3);
        Contours contoursLight, contoursLed;
        findContours(thresholdLight, contoursLight, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        findContours(thresholdLed, contoursLed, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        fillPoly(mLight,contoursLight, Scalar(0,0,255));
        fillPoly(mLed,contoursLed, Scalar(0,0,255));
        bitwise_and(mLight, mLed, mMerge);
        cvtColor(mMerge, mMerge, CV_BGR2GRAY);
        findObjectPosition(mMerge);
    }

    Scalar getLowHSVRange(){ return Scalar(lowH, lowS, lowV);}
    Scalar getHighHSVRange(){ return Scalar(highH, highS, highV);}
    Mat& getHsvMat() { return hsv; }
    Mat& getThresholdedMat() { return thresholdLight; }

    void optimizeThreshold(Mat& imgThresholded){
        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    }
};

#endif // INFRAREDDETECT_H
