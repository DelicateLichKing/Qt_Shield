#include "dialog_login.h"
#include "ui_dialog_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>


Dialog_login::Dialog_login(QWidget *parent) :
    DropShadowWidget(parent),
    ui(new Ui::Dialog_login)
{
    ui->setupUi(this);
    //登录不可用
    ui->loginButton->setEnabled(false);
    //lineEdite
    ui->userlineEdit->setPlaceholderText("请输入用户名");
    ui->passwdlineEdit->setPlaceholderText("请输入密码");
    //密码显示模式
    ui->passwdlineEdit->setEchoMode(QLineEdit::Password);
    //信号与槽
    connect(ui->loginButton,SIGNAL(clicked()),this,SLOT(loginbtn()));
    connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(exitbtn()));
    //登录按钮可用
    connect(ui->userlineEdit,SIGNAL(textChanged(QString)),this,SLOT(loginbtnSet(QString)));
    connect(ui->passwdlineEdit,SIGNAL(textChanged(QString)),this,SLOT(loginbtnSet(QString)));
}

Dialog_login::~Dialog_login()
{
    delete ui;
}
void Dialog_login::loginbtn()
{
    bool flag = false;
    if(!this->judgeEmpty())
    {
        ui->passwdlineEdit->clear();
        return;
    }
        QSqlQuery q("SELECT * FROM userbase");
        QSqlRecord rec=q.record();
        //QSQLite
         int nameCol = rec.indexOf("passwd"); // index of the field "name"
            qDebug() << "Number: " << nameCol;
       while(q.next())
       {
         qDebug() << "1: " << q.value(1).toString();
         qDebug() << "2: " << q.value(2).toString();
         if(q.value(1).toString() == ui->userlineEdit->text()&&
                q.value(2). toString()==ui->passwdlineEdit->text())
         {
             flag = true;
             break;
         }
       }
        if(flag)
        {
            mainwindow = new MainWindow;
            mainwindow->show();
            this->clearAll();
            this->hide();
            return;
        }
        else
         {
            QMessageBox::information(this,"提示","error",QMessageBox::Yes);
            return;
        }
    QMessageBox::information(this,"提示","error2",QMessageBox::Yes);
    this->clearAll();
    return;
}
void Dialog_login::exitbtn()
{
    this->close();
}
void Dialog_login::loginbtnSet(QString)
{
    ui->loginButton->setEnabled(true);
}
void Dialog_login::clearAll()
{
    ui->userlineEdit->clear();
    ui->passwdlineEdit->clear();
}


bool Dialog_login::judgeEmpty()
{
    if(ui->userlineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","用户名不能为空");
        return false;
    }
    if(ui->passwdlineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空");
        return false;
    }
    else
        return true;
}
