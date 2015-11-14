#include "videocontroller.h"

VideoController::VideoController(const char* file)
{

    if(file != nullptr){
       // Start with an existing video
        cap = new VideoCapture(file);

    } else {

        // Start webcam
        cap = new VideoCapture(0);
    }

    if(!cap->isOpened()){
        cout << "Cannot open the web cam" << endl;
        return;
    }

    windowName = "MainWindow";
    namedWindow(windowName, WINDOW_NORMAL);
}


void VideoController::handleFrame(Mat& frame){
    imshow(windowName, frame);
}

void VideoController::start()
{
    Mat frame;
    while(true){

        // Read next frame
        bool success = cap->read(frame);
        if(!success){
            cout << "fail to read frame" << endl;
            break;
        }

        // virtual function, subclasses can handle each frame
        handleFrame(frame);

        if(waitKey(33) == 27){
            break;
        }
    }
}

VideoController::~VideoController()
{
  delete cap;
}

