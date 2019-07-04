#include "tcp_client.h"
#include <QDebug>

TcpClient::TcpClient(QObject *parent) :
    QObject(parent), socket_(new QTcpSocket(parent))
{
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}


void TcpClient::connectToHost(void)
{
    socket_->connectToHost(host_port_.getHost(), host_port_.getPort());
}

void TcpClient::stopConnect()
{
    socket_->disconnectFromHost();
}

bool TcpClient::isConnected()
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

bool TcpClient::sendData(QByteArray data)
{
    if (socket_->isValid()) {
        socket_->write(data);
        return true;
    }
    return false;
}

void TcpClient::error(QAbstractSocket::SocketError errorNum)
{
    qDebug()<< errorNum << socket_->errorString();
    emit hasError(errorNum, socket_->errorString());
}


void InitTcpClientMap(TcpClientMap &client_map, const HostPortMap &host_port_map)
{
    client_map.clear();
    QMapIterator<int, HostPort> i(host_port_map);
    while (i.hasNext()) {
        i.next();
        TcpClient *client = new TcpClient();
        client_map.insert(i.key(), client);
        client->setHostPort(i.value());
        client->connectToHost();
    }
}


void ReleaseTcpClientMap(TcpClientMap &client_map)
{
    foreach (TcpClient* client, client_map) {
        client->stopConnect();
        client->deleteLater();
    }
}
