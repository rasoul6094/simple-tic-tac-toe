#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <QSet>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class mySocket : public QObject
{
    Q_OBJECT

public:
    explicit mySocket(QObject *parent = nullptr);
    void addValue(QTcpSocket* value);
    QSet<QTcpSocket*> connection_set;
    void initTurn();

signals:
    void containerSizeReached();

private:
//    QSet<QTcpSocket*> connection_set;

};

#endif // MYSOCKET_H
