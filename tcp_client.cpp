#include "tcp_client.h"
#include <QDebug>

TcpClient::TcpClient(QObject *parent) :
    QObject(parent), p_socket(new QTcpSocket(parent))
{
    connect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}


void TcpClient::connectToHost(void)
{
    p_socket->connectToHost(m_host, m_port);
}

void TcpClient::stopConnect()
{
    p_socket->disconnectFromHost();
}

bool TcpClient::isConnected()
{
    return p_socket->state() == QAbstractSocket::ConnectedState;
}

bool TcpClient::sendData(QByteArray data)
{
    if (p_socket->isValid()) {
        p_socket->write(data);
        return true;
    }
    return false;
}

void TcpClient::error(QAbstractSocket::SocketError errorNum)
{
    qDebug()<< errorNum << p_socket->errorString();
    emit hasError(errorNum, p_socket->errorString());
}
