#ifndef UDP_PROTOCOL_H
#define UDP_PROTOCOL_H

#include <QUdpSocket>

class udp_protocol : public QObject
{
    Q_OBJECT
public:
    explicit udp_protocol(QObject *parent = 0);

    QByteArray getSearchReturn(void);
    QByteArray getServerParam(void);
    QByteArray getDeviceParam(void);

signals:

public slots:
private slots:
    void readData(void);
    void error(QAbstractSocket::SocketError errorCode);

private:
    void setServerParam(QByteArray data);
    void setDeviceParam(QByteArray data);
    void broadcastData(QByteArray data);
    QUdpSocket *m_udp;

};

#endif // UDP_PROTOCOL_H
