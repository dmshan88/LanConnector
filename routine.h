#ifndef ROUTINE_H
#define ROUTINE_H

//#include <settings.h>
#include <QObject>
#include <QMap>
#include <QList>
#include "protocol/realtimedata.h"
#include "tcp_client.h"
class Routine : public QObject
{
    Q_OBJECT
public:
    static Routine *Instance();
    void start(void) {m_flag = true;}
    void stop(void) {m_flag = false;}
    void setServerClients(void);
signals:

public slots:
    void SendRealTimeData();
private:

    void connectToHost(int index);
    RealTimeData getRealTimeData();

    explicit Routine(QObject *parent = 0);
    static Routine *m_instance;

    //Settings *m_setting;
    QMap<int, TcpClient*> m_client_map;
    //RealTimeData m_realtime_data;
    int m_deviceid;
    bool m_flag;

};

#endif // ROUTINE_H
