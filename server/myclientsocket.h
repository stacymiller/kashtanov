#ifndef QCLIENTSOCKET_H
#define QCLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "modeldata.h"
#include "qmodelthread.h"

class MyClientSocket : public QObject
{
    Q_OBJECT
public:
    MyClientSocket();
    void setSocket(QTcpSocket *);
    QTcpSocket * mySocket;
public slots:
    void readDataFromClient();
    void answer();
    void sendAnswerToClient();
    void testSlot();
private:
    modelData * data;
    QString answerText;
    QTextStream * stream;
    QModelThread * worker;

    char * datastr;
    int offset;
signals:
    void readyToModel();
};

#endif // QCLIENTSOCKET_H
