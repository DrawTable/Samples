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
    while(!stop){

        if(!pause){
            // Read next frame
            success = cap->read(frame);
        }

        if(!success){
            cout << "fail to read frame" << endl;
            break;
        }

        int start = getTickCount();
        // virtual function, subclasses can handle each frame
        double height = frame.rows * 0.7;
        double width = frame.cols * 0.7;

//        resize(frame, frame, Size(width, height));

        handleFrame(frame);
        int end = getTickCount();

        if(PROFILING){
            qDebug() << "handle frame : " << (end-start) / double(getTickFrequency()) *1000 << "ms";
        }

        int key = waitKey(10);
        switch (key) {
        case 27:
            this->stop = true;
            break;
        case int('p'):
            waitKey(0);
            break;
        case int('m'):
            estimatePenPos = !estimatePenPos;
            break;
        case int('s'):
            PROFILING = !PROFILING;
            break;
        default:
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

