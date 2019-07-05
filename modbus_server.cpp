#include "modbus_server.h"
#include <QDebug>
//#include <QTimer>

ModbusServer *ModbusServer::instance_ = 0;

ModbusServer *ModbusServer::Instance()
{
    if (!instance_) {
        instance_ = new ModbusServer();
    }
    return instance_;
}

ModbusServer::ModbusServer()
{
    collector_ = 0;
}

void ModbusServer::CollectNodeAlternate()
{
    static int last_index = -1;
    if (!GetNodeChannelCount() || !collector_)
        return;
    ModbusAddress address = 0;
    int current_index = last_index;
    do {
        current_index++;
        if (current_index >=  GetNodeChannelCount())
            current_index = 0;
        if (current_index == last_index && current_index)
            return;

        address = node_channels_.at(current_index);

    } while (address == 0);

    ModbusNode node;
    if (collector_->CollectNode(address, node)) {
        collected_nodes_.insert(current_index + 1, node);
    }
    last_index = current_index;
}
