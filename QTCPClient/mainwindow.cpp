#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include "board.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // API FOR GET TIME

  QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url(QString("https://worldtimeapi.org/api/timezone/Asia/Tehran"));
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=]()mutable
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(this ,"error" ,"API error , please check your network");
            qDebug()<<"API Error: " << reply->errorString();
        }
        else
        {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();
            time = jsonObj.value("datetime").toString();
            ui->time_lbl->setText(time.mid(0,18).replace('T',"    "));
        }

    });

    // CONECTING TO GAME SERVER


    socket = new QTcpSocket(this);

    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);

    socket->connectToHost(QHostAddress::LocalHost,8080);

    if(socket->waitForConnected())
        ui->statusBar->showMessage("Connected to Server");
    else{
        QMessageBox::critical(this,"QTCPClient", QString("The following error occurred: %1.").arg(socket->errorString()));
        exit(EXIT_FAILURE);
    }

    game = new Board(socket);

}

MainWindow::~MainWindow()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

void MainWindow::readSocket()
{
    QByteArray buffer = socket->readAll();
    QString str = QString(buffer);

    if(str=="start")
        game->setLabale("start");

    else if(socket)
    {
        emit newMessage(buffer);
        return;
    }


}

void MainWindow::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;

    ui->statusBar->showMessage("Disconnected!");
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "QTCPClient", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "QTCPClient", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
        default:
            QMessageBox::information(this, "QTCPClient", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MainWindow::on_reg_btn_clicked()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            if(ui->user_led->text()!="" )
            {
                if(ui->pass_led->text()!="" )
                {

                QString password = ui->pass_led->text();
            QString username = ui->user_led->text();
            QString str =( "register " + username + " " + password);

            qDebug()<<str;


            QByteArray byteArray = str.toUtf8();
            socket->write(byteArray);


            ui->pass_led->clear();
            ui->user_led->clear();
                }
                else
                {
                QMessageBox::critical(this,"register error","please fill password first");
                return;
                }
            }

            else
            {
            QMessageBox::critical(this,"register error","please fill username first");
                return;
            }
        }
        else
            QMessageBox::critical(this,"QTCPClient","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPClient","Not connected");
}


void MainWindow::displayMessage(const QString& str)
{
    QStringList words = str.split(' ');

    if(str=="regOk")
        QMessageBox::information(this,"Register","Register was ok");
    else if (str=="regNo")
        QMessageBox::critical(this,"Register","Register falied");
    else if(str=="logOk1")
    {
        QMessageBox::information(this,"Login","loged in");
        game->setLabale("waiting for player2");
        game->show();
        this->hide();
    }
    else if(str=="logOk2")
    {
        QMessageBox::information(this,"Login","loged in");
        game->setLabale("not your turn");
        socket->write("X turn");
        game->show();
        this->hide();
    }

    else if(str=="logNo")
        QMessageBox::critical(this,"Login","login falied");

    else if(str=="your turn")
    {
        game->setLabale("your turn");
    }
    else if(words[0]=="move")
    {
        game->setLabale("your turn");
        game->newMove(str);
    }
    else if(str=="finish")
    {
        game->turn=false;
    }

}




void MainWindow::on_log_btn_clicked()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            if(ui->user_led->text()!="" )
            {
                if(ui->pass_led->text()!="" )
                {

                QString password = ui->pass_led->text();
                QString username = ui->user_led->text();
                QString str =( "login " + username + " " + password);

                qDebug()<<str;


                QByteArray byteArray = str.toUtf8();
                socket->write(byteArray);


                ui->pass_led->clear();
                ui->user_led->clear();
                }
                else
                {
                QMessageBox::critical(this,"Login error","please fill password first");
                return;
                }
            }

            else
            {
                QMessageBox::critical(this,"Login error","please fill username first");
                return;
            }
        }
        else
            QMessageBox::critical(this,"QTCPClient","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPClient","Not connected");


}

