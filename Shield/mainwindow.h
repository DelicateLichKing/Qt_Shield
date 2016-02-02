#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"mythread.h"
#include"qcustomplot.h"
#include <QAxObject>
#include <QMainWindow>
#include<QSqlTableModel>
#include <QDesktopServices>
#include <QMessageBox>
#include <QtGui>
#include <QtCore>
#include <QFileDialog>

#include <QMovie>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
       QSerialPort *serial;
signals:
    void sendData(const QByteArray &d);
    void xsx();
private slots:
    //Searial
    void linkSerial();
    //
    void  LoadPlot1();
    void LoadPlot2();
    //表格输出
    void Table2Excel(QSqlTableModel *table,QString title);
    void printfData();
    void getData();
     void writeData(const QByteArray &d);
     void displayBox();
    //find
      void on_find_clicked();
    //table operate
        void on_usertableView_clicked(const QModelIndex &index);

     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

     void on_pushButton_3_clicked();

     void on_pushButton_4_clicked();

     void on_pushButton_5_clicked();

     void on_pushButton_6_clicked();

     void on_closeBtn_1_clicked();

     void on_minBtn_1_clicked();

     void on_opushButton_2_clicked();

     void on_insertpushButton_clicked();

     void on_deletepushButton_clicked();

     void on_checkBox_stateChanged(int arg1);

     void on_ckText_stateChanged(int arg1);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
//plot[0]
    int currentIndex;
    QTimer *plot_timer;
    QList <QCustomPlot *> plots;

    double plot1_key;
    double plot1_value;

    double plot2_key;
    double plot2_value;

    QPixmap m_pixmapBg;
    QPoint m_pointStart;
    QPoint m_pointPress;

    void InitForm();
    void InitPlot();
    void InitPlot1();
    void InitPlot2();
//plot[0!]
    void initData();
    void transaction();
    myThread *mythread;

    //自定义样式;
    void setNomalStyle();
    //
    QSqlTableModel *model;
    //Table
    void userTable();
    void warningTable();
};

#endif // MAINWINDOW_H
