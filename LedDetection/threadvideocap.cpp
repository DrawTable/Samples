#include "threadvideocap.h"

void ThreadVideoCap::run(){
    VideoCapture cap("/Users/paulnta/Desktop/test.m4v");
    if(!cap.isOpened())
        return;
    waitKey(1);
    Mat frame;
    for(;;){
        cap >> frame;
        if(frame.empty())
            return;
//        emit ready(frame);
        waitKey(1);
    }
}
