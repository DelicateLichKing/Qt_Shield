#ifndef MYTHREAD
#define MYTHREAD
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>


//! [0]
class myThread : public QThread
{
    Q_OBJECT

public:
    myThread(QObject *parent = 0);
    ~myThread();
    void run(QSerialPort *serial);
signals:
    void sendData(const QString &d);
private slots:
     void readData(const QByteArray &d);
private:
    QString portName;
};
//! [0]
#endif // MYTHREAD

