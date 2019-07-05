#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QTcpSocket>
#include "host_port.h"
class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    void setHostPort(const HostPort &host_port) {host_port_ = host_port;}
    void connectToHost(void);
    void stopConnect(void);
    bool isConnected(void);
signals:
    void hasError(QAbstractSocket::SocketError errorNum, QString errorMessage);
public slots:
    bool sendData(QByteArray data);

private slots:
    void error(QAbstractSocket::SocketError errorNum);

private:
   HostPort host_port_;
   QTcpSocket *socket_;

};

typedef QMap<int, TcpClient*> TcpClientMap;
void InitTcpClientMap(TcpClientMap &client_map, const HostPortMap &host_port_map);
void ReleaseTcpClientMap(TcpClientMap &client_map);

#endif // TCP_CLIENT_H
