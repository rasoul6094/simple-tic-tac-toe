#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QMetaType>
#include <QMap>
#include <QSet>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newMessage(QString,QTcpSocket* socket);
private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);

    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str,QTcpSocket* socket);
    void sendMessage(QTcpSocket* socket);

private:
    Ui::MainWindow *ui;
    int connecting = 0;
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;
    QMap<QString, QString> m_user;
    QTcpSocket* player1;
    QTcpSocket* player2;
    int turn;

};

#endif // MAINWINDOW_H
