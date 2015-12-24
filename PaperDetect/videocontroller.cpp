#include "videocontroller.h"

VideoController::VideoController()
{
    windowName = "MainWindow";
    namedWindow(windowName, WINDOW_NORMAL);
}

void VideoController::setSource(const char* file){
    waitKey(33);
    if(file != nullptr){
       // Start with an existing video
        cap = new VideoCapture(file);
        cout << "opening a video file" << endl;
    } else {
        // Start webcam
        cap = new VideoCapture(0);
        webCam = true;
    }

    if(!cap->isOpened()){
        cout << "Cannot open the web cam" << endl;
        return;
    }
}

void VideoController::handleFrame(Mat& frame){
    imshow(windowName, frame);
}

void VideoController::startWebCam(){
    cout  << "startWebCam" << endl;
    bool success;
    Mat frame;
    VideoCapture capture(0);

    for(;;){
        success = capture.read(frame);
        if(!success){
            cout << "unable to read frame" << endl;
            return;
        }
        handleFrame(frame);
        if(waitKey(33) == 27){
            break;
        }
    }
}

void VideoController::startVideo(){
    bool success;
    Mat frame;
    while(!stop){

        if(!pause){
            // Read next frame
            success = cap->read(frame);
        }

        // fail to read frame (end of video ? or error ?)
        if(!success){
            cap->set(CV_CAP_PROP_POS_MSEC, 0);
            success = cap->read(frame);
            if(!success){
                cout << "unable to read frame" << endl;
                break;
            }else {
                cout << "end of video" << endl;
            }
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

        int key = waitKey(33);
        switch (key) {
        case 27:
            this->stop = true;
            break;
        case int('p'):
            pause = true;
            waitKey(0);
            pause = false;
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

void VideoController::start()
{
    if(webCam){
        startWebCam();
    } else {
        startVideo();
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

