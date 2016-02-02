#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include <QDialog>
#include<QSqlTableModel>
#include "mainwindow.h"
#include "drop_shadow_widget.h"

namespace Ui {
class Dialog_login;
}

class Dialog_login : public DropShadowWidget
{
    Q_OBJECT

public:
    explicit Dialog_login(QWidget *parent = 0);
    ~Dialog_login();
    void clearAll();
    bool judgeEmpty();

private:
    Ui::Dialog_login *ui;
    QSqlTableModel *model;
    MainWindow *mainwindow;


private slots:
    void exitbtn();
    void loginbtn();
    void loginbtnSet(QString);
};

#endif // DIALOG_LOGIN_H

