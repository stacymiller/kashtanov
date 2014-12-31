#include "myclientsocket.h"

MyClientSocket::MyClientSocket() {
    datastr = new char[sizeof(modelData)];
    offset = 0;
//    connect(this, SIGNAL(readyToModel()), this, SLOT(answer()));
}

void MyClientSocket::readDataFromClient(){
    qDebug() << QString::fromUtf8("Entered readDataFromClient()");
    if (offset < sizeof(modelData)) {
        qDebug() << QString::fromUtf8("Reading data from client");
        QByteArray curRes = mySocket->readAll();
        int n = curRes.length();
        memcpy(datastr + offset, curRes.data(), n);
        offset += n;
        qDebug() << "Read " + QString::number(n) + " bytes of " + QString::number(sizeof(modelData)) + "; total offset=" +QString::number(offset);
    }
    if (offset >= sizeof(modelData)){
        qDebug() << "Ended reading data";
        data = (modelData *)datastr;
        answer();
    }
}

void MyClientSocket::sendAnswerToClient(){
    qDebug() << QString::fromUtf8("Sending answer to client");
    stream = new QTextStream(this->mySocket);
    qDebug() << QString::fromUtf8("Opened stream");
    stream->setAutoDetectUnicode(true);
    answerText = worker->answer;
    qDebug() << "Got answer ready: " + answerText;
    (*stream) << answerText;
    qDebug() << QString::fromUtf8("Sent answer to stream");
    mySocket->close();
    qDebug() << QString::fromUtf8("Goodbye!");
}

void MyClientSocket::answer(){
    if (offset >= sizeof(modelData)){
        offset=0;
        worker = new QModelThread(data);
        connect(worker, SIGNAL(modelDone()), this, SLOT(sendAnswerToClient()));
//        connect(worker, SIGNAL(modelDone()), this, SLOT(testSlot()));
        worker->start();
    }
}

void MyClientSocket::testSlot(){
    qDebug() << QString::fromUtf8("Reached testSlot");
}

void MyClientSocket::setSocket(QTcpSocket * socket) {
    this->mySocket = socket;
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readDataFromClient()));
}
