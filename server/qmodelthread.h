#ifndef QMODELTHREAD_H
#define QMODELTHREAD_H

#include <QObject>
#include <QThread>
#include "modeldata.h"

class QModelThread : public QThread {
    Q_OBJECT
private:
    modelData * data;
public:
    QModelThread(modelData * d);
    QString answer;
    void run();
signals:
    void modelDone();
};

#endif // QMODELTHREAD_H
