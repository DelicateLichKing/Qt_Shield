#-------------------------------------------------
#
# Project created by QtCreator 2016-01-11T10:35:35
#
#-------------------------------------------------

QT       += core gui sql network serialport axcontainer printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shield
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_login.cpp \
    drop_shadow_widget.cpp \
    mythread.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    drop_shadow_widget.h \
    dialog_login.h \
    mythread.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    dialog_login.ui

DISTFILES += \
    Shield.pro.user

RESOURCES += \
    Resources/shield_res.qrc
