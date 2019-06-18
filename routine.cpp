#include "routine.h"
#include <QTimer>
#include "protocol/logindata.h"
#include "settings.h"
Routine *Routine::m_instance = 0;
Routine *Routine::Instance()
{
    if (!m_instance) {
        m_instance = new Routine();
    }
    return m_instance;
}



Routine::Routine(QObject *parent) :
    QObject(parent)
{
    m_deviceid = Settings::Instance()->getDeviceId();
    m_client_map.clear();
    setServerClients();
    QTimer *timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), this, SLOT(SendRealTimeData()));
    timer->start();
}

void Routine::setServerClients()
{
    m_flag = false;
    foreach (TcpClient* old_client, m_client_map) {
        old_client->stopConnect();
        old_client->deleteLater();
    }
    m_client_map.clear();
    Settings *setting = Settings::Instance();
    for(int i = 1; i <= setting->getServerCount(); i++) {
        TcpClient *client= new TcpClient(this);
        m_client_map.insert(i, client);
        connectToHost(i);
    }
}

void Routine::connectToHost(int index)
{
    QString host;
    quint16 port;
    Settings::Instance()->getServer(index, host, port);
    if(!host.isEmpty() && port && m_client_map.contains(index)) {
        TcpClient *client = m_client_map.value(index);
        client->setHost(host);
        client->setPort(port);
        client->connectToHost();
    }
}

RealTimeData Routine::getRealTimeData()
{
    //to do
    RealTimeData data;
    data.setId(m_deviceid);
    RealTimeData::NodeData node;
    data.addNode(node);
    return data;
}

void Routine::SendRealTimeData()
{
    if (!m_flag) {
        return;
    }
    foreach (TcpClient* client, m_client_map) {
        if (client->isConnected()) {
            client->sendData(getRealTimeData().toData());
        } else {
            client->connectToHost();
        }
    }
}


