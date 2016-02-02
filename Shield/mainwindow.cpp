#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include<QtNetwork/QNetworkInterface>
#include<QSqlRecord>
#include <QFileDialog>
#include<QSqlQuery>
#include<QDateTime>
#include<QTimer>
#include <QTextDocument>
#include <QScrollBar>
#include <QIntValidator>


#define TextColor QColor(255,255,255)
#define Plot_NoColor QColor(0,0,0,0)

#define Plot1_DotColor QColor(5,189,251)
#define Plot1_LineColor QColor(41,138,220)
#define Plot1_BGColor QColor(41,138,220,80)

#define Plot2_DotColor QColor(236,110,0)
#define Plot2_LineColor QColor(246,98,0)
#define Plot2_BGColor QColor(246,98,0,80)

#define TextWidth 1
#define LineWidth 2
#define DotWidth 8

#define Plot1_Count 20
#define Plot2_Count 20

#define Plot1_MaxY 100
#define Plot2_MaxY 100

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QMovie *movie = new QMovie("../1424878294.gif");
 //   ui->testlabel->setMovie(movie);
  //  qDebug()<<"..rrrr.."<<movie->isValid();//测试是否加在成功
  //  movie->start();
    initData();
    this->InitForm();
    this->InitPlot();
    this->InitPlot1();
    this->InitPlot2();
    this->LoadPlot1();
    this->LoadPlot2();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initData()
{
      setWindowFlags(Qt::FramelessWindowHint);
      ui->pushButton->setChecked(true);
      serial = new QSerialPort(this);
      //portname
      foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
          QStringList list;
          list << info.portName();
          ui->comboBox->addItem(list.first(),list);
      }
      connect(ui->linkBtn,SIGNAL(clicked()),this,SLOT(linkSerial()));
       //状态栏[0]
       model = new QSqlTableModel(this);
       model->setTable("userbase");
       model->select();
       QString str = model->record(0).value(1).toString();
       qDebug() << "debug" << str;
       QString addr = QNetworkInterface().allAddresses().at(1).toString();
       ui->Daddr->setText(tr("登录地址：%1").arg(addr));
       ui->Dname->setText(tr("当前用户名：%1").arg(str));
       //定时器
       QTimer *timer =  new QTimer(this);
       connect(timer,SIGNAL(timeout()), this,SLOT(displayBox()));
       timer->start(1000);
       //[0!]
        setNomalStyle();
}
void MainWindow::linkSerial()
{
    qDebug() << "port" << ui->comboBox->currentText();

    serial->setPortName(ui->comboBox->currentText());
     mythread = new myThread(this);
     mythread->run(serial);
     connect(serial,SIGNAL(readyRead()),this,SLOT(printfData()));
     connect(ui->sendpushButton,SIGNAL(clicked()),this,SLOT(getData()));
     connect(this,SIGNAL(sendData(QByteArray)),this,SLOT(writeData(QByteArray)));
     setNomalStyle();
     //曲线图
       //gai
       ui->gqs->setStyleSheet("image: url(:/sysButton/img/sysButton/open.png);");


}

//动态图表[0]
void MainWindow::InitForm()
{
//    currentIndex = 0;
    //初始化随机种子！
   // QTime time = QTime::currentTime();
    //qsrand(time.msec()+time.second() * 1000);

    //初始化动态曲线定时器
 //   plot_timer = new QTimer(this);
   // connect(plot_timer,SIGNAL(timeout()),this,SLOT(LoadPlot2()));
  //  connect(plot_timer,SIGNAL(timeout()),this,SLOT(LoadPlot1()));
  //  plots.append(ui->plot1);
 //   plots.append(ui->plot2);
    connect(this,SIGNAL(xsx()),this,SLOT(LoadPlot2()));
     connect(this,SIGNAL(xsx()),this,SLOT(LoadPlot1()));
     plots.append(ui->plot1);
     plots.append(ui->plot2);
}

void MainWindow::InitPlot()
{
    //设置纵坐标名称
    plots[0]->yAxis->setLabel("温度值(单位:℃)");
    plots[1]->yAxis->setLabel("湿度值(单位:%)");
    //设置纵坐标范围
    plots[0]->yAxis->setRange(0, Plot1_MaxY);
    plots[1]->yAxis->setRange(0, Plot2_MaxY);

    foreach (QCustomPlot * plot, plots) {
        //设置坐标颜色/坐标名称颜色
        plot->yAxis->setLabelColor(TextColor);
        plot->xAxis->setTickLabelColor(TextColor);
        plot->yAxis->setTickLabelColor(TextColor);
        plot->xAxis->setBasePen(QPen(TextColor, TextWidth));
        plot->yAxis->setBasePen(QPen(TextColor, TextWidth));
        plot->xAxis->setTickPen(QPen(TextColor, TextWidth));
        plot->yAxis->setTickPen(QPen(TextColor, TextWidth));
        plot->xAxis->setSubTickPen(QPen(TextColor, TextWidth));
        plot->yAxis->setSubTickPen(QPen(TextColor, TextWidth));

        //设置画布背景色
        QLinearGradient plotGradient;
        plotGradient.setStart(0, 0);
        plotGradient.setFinalStop(0, 350);
        plotGradient.setColorAt(0, QColor(80, 80, 80));
        plotGradient.setColorAt(1, QColor(50, 50, 50));
        plot->setBackground(plotGradient);

        //设置坐标背景色
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(80, 80, 80));
        axisRectGradient.setColorAt(1, QColor(30, 30, 30));
        plot->axisRect()->setBackground(axisRectGradient);

        //设置图例提示位置及背景色
        plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
        plot->legend->setBrush(QColor(255, 255, 255, 100));

        plot->replot();
    }
}

void MainWindow::InitPlot1()
{
    plots[0]->addGraph();
    plots[0]->graph(0)->setName("温度值(单位:%)");
    plots[0]->graph(0)->setPen(QPen(Plot1_LineColor, LineWidth));
    plots[0]->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle,
                        QPen(Plot1_DotColor, LineWidth),
                        QBrush(Plot1_DotColor), DotWidth));

    //设置动态曲线的横坐标格式及范围
    plots[0]->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plots[0]->xAxis->setDateTimeFormat("HH:mm:ss");
    plots[0]->xAxis->setAutoTickStep(false);
    plots[0]->xAxis->setTickStep(2.5);
    plots[0]->xAxis->setRange(0, Plot1_Count, Qt::AlignRight);
}

void MainWindow::InitPlot2()
{
    plots[1]->addGraph();
    plots[1]->graph(0)->setName("湿度值(单位:%)");
    plots[1]->graph(0)->setPen(QPen(Plot2_LineColor, LineWidth));
    plots[1]->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssCircle,
                        QPen(Plot2_DotColor, LineWidth),
                        QBrush(Plot2_DotColor), DotWidth));

    //设置动态曲线的横坐标格式及范围
    plots[1]->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plots[1]->xAxis->setDateTimeFormat("HH:mm:ss");
    plots[1]->xAxis->setAutoTickStep(false);
    plots[1]->xAxis->setTickStep(1.5);
    plots[1]->xAxis->setRange(0, Plot2_Count, Qt::AlignRight);
}

void MainWindow::LoadPlot1()
{
    bool ok;
    plot1_key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
//    plot1_value = qrand() % Plot2_MaxY;
    plot1_value = ui->lineEditA->text().toDouble(&ok);
    plots[0]->graph(0)->addData(plot1_key, plot1_value);
    plots[0]->graph(0)->removeDataBefore(plot1_key - Plot1_Count - 1);
    plots[0]->xAxis->setRange(plot1_key, Plot1_Count , Qt::AlignRight);
    plots[0]->replot();
}

void MainWindow::LoadPlot2()
{
    bool ok;
    plot2_key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
   // plot2_value = qrand() % Plot2_MaxY;
    plot2_value = ui->lineEditB->text().toDouble(&ok);
    plots[1]->graph(0)->addData(plot2_key, plot2_value);
    plots[1]->graph(0)->removeDataBefore(plot2_key - Plot2_Count - 1);
    plots[1]->xAxis->setRange(plot2_key, Plot2_Count , Qt::AlignRight);
    plots[1]->replot();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false : true);
    if (value) {
        plots[0]->graph(0)->setBrush(QBrush(Plot1_BGColor));
        plots[1]->graph(0)->setBrush(QBrush(Plot2_BGColor));
    } else {
        plots[0]->graph(0)->setBrush(QBrush(Plot_NoColor));
        plots[1]->graph(0)->setBrush(QBrush(Plot_NoColor));
    }
    plots[0]->replot();
    plots[1]->replot();
}

//[0!]


void MainWindow::displayBox()
{
    QTime time1 = QTime::currentTime();
    ui->Dtime->setText(tr("时间：%1").arg(time1.toString()));
}

//写数据[0]
void MainWindow::getData()
{
    QByteArray d ;
    d = ui->sendlineEdit->text().toLocal8Bit();
    emit sendData(d);
    qDebug() << QString(d);
}
void MainWindow::writeData(const QByteArray &d)
{
    serial->write(d);
}
//[0!]
//读数据[0]
void MainWindow::printfData()
{
    bool ok;
    const char *datastart = "ffff";
    QString data1;
    QString data2;
    int data3;
    int datastartlen = strlen(datastart);
    ui->lineEditA->clear();
    ui->lineEditB->clear();
    QByteArray data = serial->readAll();
    if(data.startsWith(datastart))
    {
        data1 = data.mid(datastartlen,2);
        data2 = data.mid(datastartlen+2,2);
        data3 =  data.mid(datastartlen+4,2).toInt(&ok,10);
    }
    else
    {
      qDebug()<<"data_x";
    }
    QString str1 = QString(data1);
    QString str2 = QString(data2);
    ui->lineEditA->insert(str1);
    ui->lineEditB->insert(str2);
    ui->progressBarA->setValue(data3);
    emit xsx();
}
//[0!]
//移动窗口[0]
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(m_pixmapBg.rect(), m_pixmapBg);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    this->move(e->globalPos() - m_pointStart);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    m_pointPress = e->globalPos();
    m_pointStart = m_pointPress - this->pos();

}
//[0！]
//
void MainWindow::setNomalStyle()
{
    QFile styleSheet(":/qss/shield.qss");
    if (!styleSheet.open(QIODevice::ReadOnly))
    {
        qWarning("Can't open the style sheet file.");
        return;
    }
    qApp->setStyleSheet(styleSheet.readAll());
}
//Table[0]
void MainWindow::userTable()
{
    model->setTable("userbase");
    model->select();
    ui->usertableView->setModel(model);
    ui->usertableView->horizontalHeader()->show();
    ui->usertableView->horizontalHeader()->setSectionsClickable(false );
    ui->usertableView->verticalHeader()->show();
    ui->usertableView->setColumnWidth(0,50);
    ui->usertableView->setColumnWidth(1,100);
    ui->usertableView->setColumnWidth(2,100);
    ui->usertableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void MainWindow::warningTable()
{
    model->setTable("warning");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->show();
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,50);
    ui->tableView->setColumnWidth(4,100);
    ui->tableView->setColumnWidth(5,100);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//nobb
}

//Table[0!]
//查询[0]
void MainWindow::on_find_clicked()
{
    QString name = ui->findEdit->text();
    QString dtStart = ui->dateTimeStart->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString dtEnd = ui->dateTimeEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    model->setTable("Warning");
    //sql
    model->setFilter(QObject::tr("date between '%1' and '%2'").arg(dtStart).arg(dtEnd));
    model->select();
    //事件查询
 if(!name.isEmpty())
 {
    model->setFilter(QObject::tr("name='%1'").arg(name));
    model->select();
  }
    ui->tableView->setModel(model);
}
//查询[0!]
//表格操作[0]
void MainWindow::on_usertableView_clicked(const QModelIndex &index)
{
    ui->usertableView->selectRow(index.row());//整行
}

//表格操作[0!]

//导出表格类[0]
void MainWindow::Table2Excel(QSqlTableModel *table, QString title)
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Excel 文件(*.xls *.xlsx)"));
    if (fileName!="")
    {
        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)","true");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
            int i,j,colcount=table->columnCount();
            QAxObject *cell,*col;
            //标题行
            cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", title);
            cell->querySubObject("Font")->setProperty("Size", 18);
            //调整行高
            worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108);//xlCenter
            range->setProperty("VerticalAlignment", -4108);//xlCenter
            //列标题
            for(i=0;i<colcount;i++)
            {
                QString columnName;
                columnName.append(QChar(i  + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                col->setProperty("ColumnWidth", ui->usertableView->columnWidth(i)/6);
                cell=worksheet->querySubObject("Cells(int,int)", 2, i+1);
                columnName=model->headerData(i,Qt::Horizontal).toString();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }
            //数据区
            for(i=0;i<table->rowCount();i++){
                for (j=0;j<colcount;j++)
                {
                    QString str2 = table->record(i).value(j).toString();
                    qDebug() << "str2:" << str2;
                    worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", str2);
                }
            }
            //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount - 1 + 'A');
            lrange.append(QString::number(table->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel=NULL;
            if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else
        {
            QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
        }
    }
}

//[0!]
//按钮[0]
void MainWindow::on_pushButton_clicked()
{
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
  ui->stackedWidget->setCurrentIndex(2);
  warningTable();
}

void MainWindow::on_pushButton_4_clicked()
{
  ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_5_clicked()
{
  ui->stackedWidget->setCurrentIndex(4);
    userTable();
}

void MainWindow::on_pushButton_6_clicked()
{
  ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_closeBtn_1_clicked()
{
    this->close();
}

void MainWindow::on_minBtn_1_clicked()
{
    this->showMinimized();
}

void MainWindow::on_opushButton_2_clicked()
{
    model->setTable("userbase");
    model->select();
    Table2Excel(model,"账户管理");
}

void MainWindow::on_insertpushButton_clicked()
{
    //插入

    QSqlQuery q;
    q.prepare("insert into userbase values (?, ?, ?, ?)");
    QVariantList userId;
    userId << 4 << 5 << 6 << 7;
    q.addBindValue(userId);

    QVariantList username;
    username << "Harald" << "Boris" << "Trond" <<"Luyc"<< QVariant(QVariant::String);
    q.addBindValue(username);

    QVariantList passwd;
    passwd << "33333333" <<" 444444" << "55555555" <<"66666666" << QVariant(QVariant::String);
    q.addBindValue(passwd);

    QVariantList limits;
    limits <<" __xx_x" << "__x__x" << "xxxx__" << "xxxxx_" << QVariant(QVariant::String);
    q.addBindValue(limits);

    if (!q.execBatch())
        qDebug() << "q.lastError()";

    model->setTable("userbase");
    model->select();
    ui->usertableView->setModel(model);
}

void MainWindow::on_deletepushButton_clicked()
{
    //删除
    int curRow = ui->usertableView->currentIndex().row();
//    QModelIndex index = ui->usertableView->currentIndex();
    int userID = curRow+1;
    qDebug() << "id:" << userID;
    model->removeRow(curRow);
   model->setTable("userbase");
   model->select();
   ui->usertableView->setModel(model);
}
//[0!]



void MainWindow::on_ckText_stateChanged(int arg1)
{
    bool value = (arg1 == 0 ? false : true);
    if (value) {
        plots[0]->legend->setVisible(true);
        plots[1]->legend->setVisible(true);
    } else {
        plots[0]->legend->setVisible(false);
        plots[1]->legend->setVisible(false);
    }

    foreach (QCustomPlot * plot, plots) {
        plot->replot();
    }
}
