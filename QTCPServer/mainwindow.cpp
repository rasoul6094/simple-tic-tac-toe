#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_server = new QTcpServer();
    turn=0;
    if(m_server->listen(QHostAddress::Any, 8080))
    {
        connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
        connect(m_server, &QTcpServer::newConnection, this, &MainWindow::newConnection);
        ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);

}
}

    MainWindow::~MainWindow()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete ui;
}

void MainWindow::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());

}


void MainWindow::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    displayMessage(QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor()),socket);
}

void MainWindow::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer = socket->readAll();



    if(socket)
    {
        QString message = QString(buffer);
        emit newMessage(message,socket);
        return;
    }


}

void MainWindow::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        displayMessage(QString("INFO :: A client has just left the room").arg(socket->socketDescriptor()),socket);
        connection_set.remove(*it);
    }

    socket->deleteLater();
    if(connecting>0)
    connecting--;
    qDebug()<<connecting;
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "QTCPServer", "The host was not found. Please check the host name and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "QTCPServer", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
    default:
        QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
        QMessageBox::information(this, "QTCPServer", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MainWindow::sendMessage(QTcpSocket* socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = ui->lineEdit_message->text();

            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);

            socketStream.setVersion(QDataStream::Qt_5_15);
            socketStream << byteArray;
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void MainWindow::displayMessage(const QString& str ,QTcpSocket* socket )
{
    ui->textBrowser_receivedMessages->append(str);
    QStringList words = str.split(' ');

    if(words[0]=="register")
    {
        QString username = words[1];
        QString password = words[2];

        bool regSuccessful = false;
        if (m_user[username] == password )
            regSuccessful = true;

        if(username!="" && username!="" && !regSuccessful)
        {
            m_user.insert(username,password);
            socket->write("regOk");
        }
        else
            socket->write("regNo");
    }
    else if (words[0]=="login")
    {
        QString username = words[1];
        QString password = words[2];

        bool loginSuccessful = false;
        if (m_user[username] == password )
            loginSuccessful = true;

        if(loginSuccessful)
        {
            connecting++;
            qDebug()<<connecting;
            if(connecting==1)
            {
            player1=socket;
            socket->write("logOk1");
            }
            else if(connecting==2)
            {
            player2= socket;
            socket->write("logOk2");
            }

        }
        else
            socket->write("logNo");
    }
    else if(words[0]=="X")
    {
        turn=1;
        player1->write("your turn");

    }
    else if(words[0]=="move")
    {
        if(turn==1){
            turn=2;
            player2->write(str.toUtf8());
        }
        else if(turn==2){
            turn=1;
            player1->write(str.toUtf8());
        }
    }
    else if(str=="finish")
    {
        foreach (QTcpSocket* gamer, connection_set)
        {
            gamer->write("finish");
        }

    }

}


