#ifndef STYLUSDETECT
#define STYLUSDETECT

#include "videocontroller.h"

class StylusDetect: public VideoController{

    Mat bgr, orig, frame, trackGreen, trackRed;
    typedef enum {LED_GREEN, LED_RED} LED_TYPE;

    RotatedRect lightRoi[2];
    Mat trackLight[2];
    int trackLightHue[2];

    int const windowTrackLightSize = 60;

    Scalar lowHSV[2];
    Scalar highHSV[2];

    VideoWriter outputVideo;

public:
    StylusDetect(const char* file = nullptr): VideoController(file){

        /*
         * Configurations HSV
         */
        lowHSV[LED_GREEN] = Scalar(46,40,0);
        highHSV[LED_GREEN] = Scalar(87,255,255);

        lowHSV[LED_RED] = Scalar(164, 98, 94);
        highHSV[LED_RED] = Scalar(174,211,145);

        trackLightHue[LED_GREEN] = 42;
        trackLightHue[LED_RED] = 94;


        if(DEBUG){
            namedWindow("trackGreen", WINDOW_KEEPRATIO);
            namedWindow("trackRed", WINDOW_KEEPRATIO);
            namedWindow("trackLightGreen", WINDOW_KEEPRATIO);
            namedWindow("trackLightRed", WINDOW_KEEPRATIO);
            namedWindow("trackLightRedHSV", WINDOW_KEEPRATIO);
            namedWindow("trackLightGreenHSV", WINDOW_KEEPRATIO);
        }
    }

protected:
    void optimizeThreshold(Mat &frame);
    virtual void handleFrame(Mat &frame);
    void processLedTracking();
    Point findObjectPosition(Mat &imgThresholded, LED_TYPE led, bool trackLightRed = false);
    void drawRotatedRect(Mat &frame, RotatedRect &rect);
    void calculatePenPos(const Point &red, const Point &green);
    void drawCross(Mat &frame, Point center, Scalar color, int size);
};

#endif // STYLUSDETECT

