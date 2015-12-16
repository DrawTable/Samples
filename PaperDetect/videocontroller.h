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

    VideoController(const char* file = nullptr);

    void start();
    void togglePause();

    ~VideoController();

protected:

    bool estimatePenPos = false;
    int const DEBUG = false;
    int const RECORD = false;
    int PROFILING = false;
    bool stop = false;
    bool pause = false;

    VideoCapture* cap;
    string windowName;
    virtual void handleFrame(Mat &frame);

signals:

public slots:
};

#endif // VIDEOCONTROLLER_H
