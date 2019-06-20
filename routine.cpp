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
    QObject(parent), m_modbus(ModbusRtuServer::Instance())
{
    Settings *setting = Settings::Instance();
    m_deviceid = setting->getDeviceId();
    m_client_map.clear();
    setServerClients();
    m_modbus->setRTU(setting->getCOM(), setting->getBaud());
    setNodes();
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

void Routine::setNodes()
{
    Settings *setting = Settings::Instance();
    m_modbus->setNodeCount(setting->getNodeCount());
    for (int i = 0; i< setting->getNodeCount() ; i++) {
        m_modbus->addNodeAddress(i, setting->getNodeAddress(i + 1));
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
    Settings *setting = Settings::Instance();
    RealTimeData realtime_data;
    realtime_data.setId(m_deviceid);
    for (int i = 0; i < setting->getNodeCount(); i++) {
        ModbusRtuServer::Modbus_Node modbus_node = m_modbus->getNode(i);
        RealTimeData::NodeData node;
        node.nodeId = i + 1;
        node.humH = (modbus_node.value1 & 0xff00) >> 8;
        node.humL = (modbus_node.value1 & 0x00ff) >> 0;
        node.temH = (modbus_node.value2 & 0xff00) >> 8;
        node.temL = (modbus_node.value2 & 0x00ff) >> 0;
        realtime_data.addNode(node);
    }
    return realtime_data;
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


