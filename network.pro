#-------------------------------------------------
#
# Project created by QtCreator 2011-10-28T11:20:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    model.h

FORMS    += mainwindow.ui
