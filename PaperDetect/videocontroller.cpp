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
    bool success;
    Mat frame;
    while(true){

        if(!pause){
            // Read next frame
            success = cap->read(frame);
        }

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

void VideoController::togglePause()
{
    this->pause = !this->pause;
}

VideoController::~VideoController()
{
  delete cap;
}

