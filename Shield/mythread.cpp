#include"mythread.h"
#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <QDebug>

QT_USE_NAMESPACE

myThread::myThread(QObject *parent)
    : QThread(parent)
{
}

myThread::~myThread()
{

}
//! [2] //! [3]

//! [4]
void myThread::run(QSerialPort *serial)
{


    if(serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "COM2" ;
    }

}

void myThread::readData(const QByteArray &d)
{
    QSerialPort serial;
    serial.setPortName(portName);

    if(serial.open(QIODevice::ReadWrite))
    {
         qDebug() <<  "isopen";
        serial.write(QByteArray(d));
        qDebug() << QString(d);
    }
    else
    {
        qDebug() <<  "isclose";
    }
    /*
    QByteArray data;
    data =  serial.readAll();
    qDebug() <<  "ces" << QString(data);
    emit sendData(QString(data));*/
}
