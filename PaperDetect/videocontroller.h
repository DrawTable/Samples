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

    ~VideoController();

protected:
    VideoCapture* cap;
    string windowName;
    virtual void handleFrame(Mat &frame);

signals:

public slots:
};

#endif // VIDEOCONTROLLER_H
