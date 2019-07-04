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

void ModbusServer::SetNodeChannels(int *address_array, int count)
{
    node_channels_.resize(count);

    for (int i = 0; i < count; i++) {
        node_channels_[i] = node_channels_.contains(address_array[i]) ? 0 : address_array[i];
    }
    collected_nodes_.clear();
}

void ModbusServer::CollectNodeAlternate()
{
    static int last_index = -1;
    if (!GetNodeChannelCount() || !collector_)
        return;
    int address = 0;
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
    qDebug() << "node_channels_:" << node_channels_ << current_index << last_index;
    qDebug() << "ModbusServer::CollectNodeAlternate()" << address;
    if (collector_->CollectNode(address, node)) {
        qDebug() << "ModbusServer::CollectNodeAlternate() collect ok";
        collected_nodes_.insert(current_index + 1, node);
    }
    last_index = current_index;
}
