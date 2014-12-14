#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    server_status=0;
}

void MainWindow::on_starting_clicked()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::Any, 33333) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        ui->textinfo->append(QString::fromUtf8("Server started"));
        qDebug() << QString::fromUtf8("Server started");
    }
}

void MainWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Server stopped"));
        qDebug() << QString::fromUtf8("Server stopped");
        server_status=0;
    }
}


void MainWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("New connection");
        ui->textinfo->append(QString::fromUtf8("New connection"));
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient())); // connects slot to signal, where SClients[idusersocs
                                                                                        // is a signal's emitter, SIGNAL(readyRead()) in an emitting
                                                                                        // signal, this - object that receives signal, SLOT(...)
                                                                                        // is a method that will be called to manage the signal
    }
}

void MainWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender(); //pointer to the object that called the slot is actually a pointer to QTcpSocket
    int idusersocs=clientSocket->socketDescriptor();
    QTextStream os(clientSocket); // textsrteam that operates on device
    os.setAutoDetectUnicode(true);
    QString s = clientSocket->readAll();
    QString ans = simulation();
//    ui->textinfo->append("\nAnswer to client: \"Hello!\"");
//    os << "HTTP/1.0 200 Ok\r\n"
//          "Content-Type: text/html; charset=\"utf-8\"\r\n"
//          "\r\n"
//          "<h1>"<< "Hello!" <<"</h1>\n"
//          << QDateTime::currentDateTime().toString() << "\n";
    os << ans;
//    ui->textinfo->append("ReadClient:"+s+"\n\r");
    ui->textinfo->append(ans);
    clientSocket->close();
    SClients.remove(idusersocs);
}
