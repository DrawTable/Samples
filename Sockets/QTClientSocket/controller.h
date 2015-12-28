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
    Controller();

    void start(){
        thread.start();
    }



signals:
    void mouseMove(int x, int y);
    void mousePressed();
    void mouseReleased();
    void quit();


};
#endif // CONTROLLER

