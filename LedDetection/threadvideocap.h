#ifndef THREADVIDEOCAP
#define THREADVIDEOCAP

#include <opencv2/opencv.hpp>
#include <QThread>

using namespace cv;
using namespace std;

class ThreadVideoCap : public QThread{

public:
    void run();

signals:
    void ready(Mat frame);

};

#endif // THREADVIDEOCAP

