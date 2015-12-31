#ifndef CONTROLLER
#define CONTROLLER

#include "threadvideocap.h"
#include "leddetector.h"

#include <QObject>

class Controller : public QObject{

    Q_OBJECT

    LedDetector* ld;

public:
    Controller(){
        ld = LedDetector::getInstance();
        ThreadVideoCap tvc;
//        connect(&tvc, SIGNAL(frameReady(Mat)), this, SLOT(handleFrame(Mat)));
        tvc.start();
    }

public slots:
    void handleFrame(Mat frame){
        Mat copy;
        frame.copyTo(copy);
        ld->setImage(copy);
        ld->debugLedDetection();
    }
};

#endif // CONTROLLER

