#include "main_task.h"
#include <QTimer>

#include "protocol/realtime_data.h"
#include "settings.h"

MainTask *MainTask::instance_ = 0;
MainTask *MainTask::Instance()
{
    if (!instance_) {
        instance_ = new MainTask();
    }
    return instance_;
}

MainTask::MainTask() : modbus_server_(ModbusServer::Instance()),
    timer_(new MainTimer())
{
    Settings *setting = Settings::Instance();

    ResetTcpClients();

    ModbusCollector *com_port = new ModbusCollector();
    com_port->SetSerialParam(setting->getCOM().toLatin1().data(), setting->getBaud());
    modbus_server_->SetCollector(com_port);

    SetModbusNodes();
}

void MainTask::ResetTcpClients()
{
    SetStartFlag(false);

    ReleaseTcpClientMap(tcp_clients_);
    InitTcpClientMap(tcp_clients_, Settings::Instance()->getServerParam());
}

void MainTask::SetModbusNodes()
{
    Settings *setting = Settings::Instance();

    int node_count = setting->getNodeCount();
    int node_address_array[node_count];
    for (int i = 0; i < node_count; i++) {
        node_address_array[i] = setting->getNodeAddress(i + 1);
    }
    modbus_server_->SetNodeChannels(node_address_array, node_count);
}

void MainTask::SendRealTimeData()
{
    if (!GetStartFlag()) {
        return;
    }
    RealtimeData realtime_data;
    realtime_data.SetDeviceId(Settings::Instance()->getDeviceId());
    bool ok = realtime_data.SetNodeData(modbus_server_->GetCollectedNodes());
    if (!ok) {
        qDebug() << " MainTask::SendRealTimeData() not ok";
        return;
    }
    QByteArray data = realtime_data.GetData();
    qDebug() << "MainTask::SendRealTimeData()" << data.toHex();
    foreach (TcpClient* client, tcp_clients_) {
        if (client->isConnected()) {
            client->sendData(data);
        } else {
            client->connectToHost();
        }
    }
}
