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
    sim.cpp \
    model.cpp \
    R32m.cpp \
    qmodelthread.cpp \
    myclientsocket.cpp

HEADERS  += mainwindow.h \
    sim.h \
    model.h \
    RAND.H \
    modeldata.h \
    qmodelthread.h \
    myclientsocket.h

FORMS    += mainwindow.ui
