#ifndef HOST_PORT_H
#define HOST_PORT_H
#include <QString>
#include <QMap>
class HostPort
{
public:
    HostPort(){}
    HostPort(QString host, quint16 port): host_(host), port_(port){}
    void setHost(QString host) {host_ = host;}
    QString getHost(void) const {return host_;}
    void setPort(quint16 port) {port_ = port;}
    quint16 getPort(void) const {return port_;}
private:
    QString host_;
    quint16 port_;
};

typedef QMap<int, HostPort> HostPortMap;
#endif // HOST_PORT_H
