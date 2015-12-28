#include "controller.h"



Controller::Controller(){
    Worker* worker = new Worker;
    worker->moveToThread(&thread);
    connect(&thread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), &thread, SLOT(quit()));
    connect(&thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(&thread, SIGNAL(finished()), QCoreApplication::instance(), SLOT(quit()));
    connect(this, SIGNAL(mouseMove(int,int)), worker, SLOT(mouseMouve(int,int)));
    connect(this, SIGNAL(mousePressed()), worker, SLOT(mousePressed()));
    connect(this, SIGNAL(mouseReleased()), worker, SLOT(mouseReleased()));
    connect(this, SIGNAL(quit()), worker , SLOT(quit()) );

}
