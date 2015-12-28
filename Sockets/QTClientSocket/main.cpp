#include <QCoreApplication>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Controller ctrl;
    ctrl.start();
    ctrl.mouseMove(100, 30);
    QThread::sleep(1);
    ctrl.mousePressed();
    QThread::sleep(1);
    ctrl.mouseReleased();
    QThread::sleep(1);
    ctrl.mouseMove(100, 60);
    QThread::sleep(1);
    ctrl.mousePressed();
    QThread::sleep(1);
    ctrl.mouseReleased();
    QThread::sleep(1);
    ctrl.quit();

    return a.exec();
}

