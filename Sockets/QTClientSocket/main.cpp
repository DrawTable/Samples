#include <QCoreApplication>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Controller ctrl;
    ctrl.start();
    return a.exec();
}

