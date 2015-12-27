#ifndef WORKER
#define WORKER

#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextStream>

class Worker: public QObject{
    Q_OBJECT
    QTcpSocket* socket;

public slots:

    void doWork(){
        socket = new QTcpSocket(this);
        socket->connectToHost("localhost",5003);

        connect(socket, SIGNAL(connected()), this, SLOT(connected()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showErrors(QAbstractSocket::SocketError)));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    }

    void connected(){
        qDebug() << "connected";

        for(int i=0; i < 10; i++){
            sendData(QString("data %1 \n").arg(i).toUtf8());
            sleep(1);
        }

        sendData("STOP");
        emit finished();
    }

    void showErrors(QAbstractSocket::SocketError err){
        qDebug() << err;
    }

    void readyRead(){
        QString line = socket->readLine();
        qDebug() << "Received: " << line;
    }

    void sendData(const char* data){
        socket->write(data);
        socket->flush();
    }

    void bytesWritten(qint64 n){
        qDebug("%lld bytes written",n);
    }

signals:
    void finished();
    void connectionReady();
};

#endif // WORKER

