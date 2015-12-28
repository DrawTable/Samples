#include "worker.h"


void Worker::doWork(){
    socket = new QTcpSocket(this);
    socket->connectToHost("localhost",5003);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showErrors(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}


void Worker::connected(){
    qDebug() << "connected";
}


void Worker::showErrors(QAbstractSocket::SocketError err){
    qDebug() << err;
}


void Worker::readyRead(){
    QString line = socket->readLine();
    qDebug() << "Received: " << line;
}


void Worker::sendData(const char *data){
    socket->write(data);
    socket->flush();
}

void Worker::mouseMouve(int x, int y){

    sendData(QString("move:%1:%2\n").arg(x).arg(y).toUtf8());
}

void Worker::mousePressed(){
    sendData(QString("press\n").toUtf8());
}

void Worker::mouseReleased(){
    sendData(QString("release\n").toUtf8());
}

void Worker::quit(){
    sendData((QString("quit\n").toUtf8()));
    emit finished();
}
