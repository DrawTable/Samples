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


    int const DEBUG = true;
    int const RECORD = false;

    VideoWriter outputVideo;

public:
    StylusDetect(const char* file = nullptr): VideoController(file){

        lowHSV[LED_GREEN] = Scalar(46,40,0);
        highHSV[LED_GREEN] = Scalar(87,255,255);

        lowHSV[LED_RED] = Scalar(120,132,115);
        highHSV[LED_RED] = Scalar(179,255,255);

        trackLightHue[LED_GREEN] = 42;
        trackLightHue[LED_RED] = 94;


        if(DEBUG){
            namedWindow("trackGreen", WINDOW_KEEPRATIO);
            namedWindow("trackRed", WINDOW_KEEPRATIO);
            namedWindow("trackLightGreen", WINDOW_KEEPRATIO);
            namedWindow("trackLightRed", WINDOW_KEEPRATIO);
        }
        if(RECORD){

            Size S = Size((int) cap->get(CV_CAP_PROP_FRAME_WIDTH), (int) cap->get(CV_CAP_PROP_FRAME_HEIGHT));

            outputVideo.open("out.avi",
                             VideoWriter::fourcc('I','4','2','0'), //cap->get(CV_CAP_PROP_FOURCC) ,
                             cap->get(CV_CAP_PROP_FPS),S, true);

            if (!outputVideo.isOpened()){
                std::cout << "!!! Output video could not be opened" << std::endl;
                return;
            }

            qDebug() << "output video:" << outputVideo.isOpened() << S.height << "x" << S.width << cap->get(CV_CAP_PROP_FPS) << "fps";
        }
    }

protected:
    void optimizeThreshold(Mat &frame);
    virtual void handleFrame(Mat &frame);
    void processLedTracking();
    Point findObjectPosition(Mat &imgThresholded, LED_TYPE led, bool trackLightRed = false);
    void drawRotatedRect(Mat &frame, RotatedRect &rect);
};

#endif // STYLUSDETECT

