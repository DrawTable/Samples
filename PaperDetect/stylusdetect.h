#ifndef STYLUSDETECT
#define STYLUSDETECT

#include "videocontroller.h"

class StylusDetect: public VideoController{

    Mat bgr, orig, frame, trackGreen, trackRed, trackLight;
    RotatedRect lightRoi;
    enum LEDS {LED_GREEN, LED_RED};

    int lowH[2] = {46, 120};  // low HUE    106
    int lowS[2] = {40, 132};  // low SATURATION 116
    int lowV[2] = {0, 115};  // high VALUE 145

    int highH[2] = {87, 179}; // high HUE
    int highS[2] = {255, 255}; // high SATURATION
    int highV[2] = {255, 255}; // high VALUE

    int const DEBUG = false;

public:
    StylusDetect(const char* file = nullptr): VideoController(file){
        if(DEBUG){
            namedWindow("trackGreen", WINDOW_KEEPRATIO);
            namedWindow("trackRed", WINDOW_KEEPRATIO);
            namedWindow("trackLight", WINDOW_KEEPRATIO);
        }
    }

    // VideoController interface
    void optimizeThreshold(Mat &frame);
protected:
    void handleFrame(Mat &frame);
    void processLedTracking();
    Point findObjectPosition(Mat &imgThresholded, bool trackLight = false);
    void drawRotatedRect(Mat &frame, RotatedRect &rect);
};

#endif // STYLUSDETECT

