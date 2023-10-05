#include "mySocket.h"

mySocket::mySocket(QObject *parent) : QObject(parent)
{
    connect(this, &mySocket::containerSizeReached, this, &mySocket::initTurn);
}

void mySocket::addValue(QTcpSocket* value)
{
    connection_set.insert(value);

    // Check if the container size is equal to 2
    if (connection_set.size() == 2)
    {
        // Emit the signal when the container size reaches 2
        emit containerSizeReached();
    }
}

void mySocket::initTurn()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->write("start");
        socket->waitForBytesWritten(30000);
    }
}
