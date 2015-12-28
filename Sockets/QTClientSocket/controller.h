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
    Controller(){
        qDebug() << "Controller contruct: " << QThread::currentThreadId() << endl;
        worker = new Worker;
        worker->moveToThread(&thread);
        connect(&thread, SIGNAL(started()), worker, SLOT(doWork()));
        connect(worker, SIGNAL(finished()), &thread, SLOT(quit()));
        connect(&thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(&thread, SIGNAL(finished()), QCoreApplication::instance(), SLOT(quit()));
        connect(this, SIGNAL(dataReady(const char*)), worker, SLOT(sendData(const char*)));
    }

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



};
#endif // CONTROLLER

