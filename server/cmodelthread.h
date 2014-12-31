#ifndef CMODELTHREAD_H
#define CMODELTHREAD_H

#include <QThread>


class CModelThread : public QThread
{
    Q_OBJECT
protected:
    void run();
//    QTextStream answerStream;
public:
    CModelThread(modelData d, int id);
    CModelThread();
    ~CModelThread();
    modelData data;
    int userid;
};

#endif // CMODELTHREAD_H
