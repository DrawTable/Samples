#ifndef CONTROLLER
#define CONTROLLER

#include <QThread>
#include <QCoreApplication>
#include <QObject>
#include "worker.h"

class Controller: public QObject{
    Q_OBJECT

    QThread thread;
public:
    Controller(){
        Worker* worker = new Worker;
        worker->moveToThread(&thread);
        connect(&thread, SIGNAL(started()), worker, SLOT(doWork()));
        connect(worker, SIGNAL(finished()), &thread, SLOT(quit()));
        connect(&thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(&thread, SIGNAL(finished()), QCoreApplication::instance(), SLOT(quit()));
    }

    void start(){
        thread.start();
    }


};
#endif // CONTROLLER

