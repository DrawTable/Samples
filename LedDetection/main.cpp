#include "leddetector.h"

void testVideo(){

    VideoCapture cap("/Users/paulnta/Desktop/test.m4v");
    if(!cap.isOpened())
        return;

    waitKey(1);

    LedDetector* ld = LedDetector::getInstance();
    Mat frame;
    for(;;){

        cap >> frame;
        if(frame.empty())
            return;

        ld->setImage(frame);
        ld->debugLedDetection();

        if(waitKey(1) == 27)
            break;
    }

}

int main()
{

    Mat src = imread("/Users/paulnta/Desktop/Test_LedDetect/5.png");
    LedDetector* ld = LedDetector::getInstance();
    ld->setImage(src);
    ld->debugLedDetection();
    waitKey(0);

//    testVideo();
    return 0;
}

