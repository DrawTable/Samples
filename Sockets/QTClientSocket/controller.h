    #ifndef CONTROLLER
#define CONTROLLER

#include <QThread>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include "worker.h"

class Controller: public QObject{
    Q_OBJECT

    QThread thread;
    Worker* worker;
public:
    Controller();

    void start(){
        thread.start();
        sleep(2);
        QTimer::singleShot(2000, this, SLOT(sendData()));
    }

signals:
    void dataReady(const char*);
public slots:

    void sendData(){
        emit dataReady("HELLO FROM OUTSIDE");
    }




signals:
    void mouseMove(int x, int y);
    void mousePressed();
    void mouseReleased();
    void quit();


};
#endif // CONTROLLER

