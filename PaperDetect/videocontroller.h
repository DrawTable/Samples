#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QDebug>
#include <QThread>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class VideoController
{

public:

    VideoController();
    void start();
    void togglePause();
    void setSource(const char* file = nullptr);
    ~VideoController();
    bool inline isPaused() {return pause;}

    int frameNum;

protected:

    bool webCam = false;
    bool estimatePenPos = false;
    int const DEBUG = true;
    int const RECORD = false;
    int PROFILING = false;
    bool stop = false;
    bool pause = false;
    VideoCapture* cap;
    string windowName;
    virtual void handleFrame(Mat &frame);

    void startVideo();
    void startWebCam();
signals:

public slots:
};

#endif // VIDEOCONTROLLER_H
