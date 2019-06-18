#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    void setHost(QString host) {m_host = host;}
    void setPort(quint16 port) {m_port = port;}

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
   QString m_host;
   quint16 m_port;
   QTcpSocket *p_socket;

};

#endif // TCP_CLIENT_H
