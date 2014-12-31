#include <QDebug>
#include "qmodelthread.h"
#include "model.h"

QModelThread::QModelThread(modelData * d)
{
    data = d;
}

void QModelThread::run(){
    answer = simulation(data);
    qDebug() << QString::fromUtf8("Emitting signal");
    emit modelDone();
    qDebug() << QString::fromUtf8("Emitted signal");
}
