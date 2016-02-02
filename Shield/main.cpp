#include "mainwindow.h"
#include "dialog_login.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("shield");
    db.setDatabaseName("SHIELD.db");
    db.setUserName("root");
    db.setPassword("123456");
    if(db.open())
    {

    //建表
    QSqlQuery query1;
    bool success=query1.exec("CREATE TABLE [userbase] ( [userId] INT(20) NOT NULL, [username] VARCHAR(40) NOT NULL, [passwd] VARCHAR(40), [limits] VARCHAR(20), PRIMARY KEY([userId]))");
    if(success)
        qDebug()<<QObject::tr("数据库表创建成功！\n");
    else
        qDebug()<<QObject::tr("数据库表创建失败或已存在！\n");
    //查询
    QSqlQuery query;
    success=query.exec("INSERT INTO userbase (userId, username, passwd, limits) "
                      "VALUES (001, 'root', 'cdjx1234', 'xxxxxx')");

    if(!success)
    {
        QSqlError lastError=query.lastError();
        qDebug()<<lastError.driverText()<<QString(QObject::tr("插入失败"));
    }
    else
        qDebug()<<QObject::tr("插入数据成功！\n");

//    Dialog_login w;
//    w.show();
    MainWindow w;
    w.show();
    return a.exec();
    }
    else
    {
        qDebug()<<QObject::tr("错误！ \n");
    }
}
