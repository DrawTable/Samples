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
    int const RECORD = false;

    VideoWriter outputVideo;

public:
    StylusDetect(const char* file = nullptr): VideoController(file){
        if(DEBUG){
            namedWindow("trackGreen", WINDOW_KEEPRATIO);
            namedWindow("trackRed", WINDOW_KEEPRATIO);
            namedWindow("trackLight", WINDOW_KEEPRATIO);
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
    Point findObjectPosition(Mat &imgThresholded, bool trackLight = false);
    void drawRotatedRect(Mat &frame, RotatedRect &rect);
};

#endif // STYLUSDETECT

