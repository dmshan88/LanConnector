#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H

#include <QVector>
#include "modbus_node.h"
#include "modbus_collector.h"

class ModbusServer
{
public:
    static ModbusServer *Instance();

    int GetNodeChannelCount(void) const {return node_channels_.count();}
    void SetNodeChannels(int *address_array, int count);

    void SetCollector(ModbusCollector *collector) {collector_ = collector;}

    ModbusNodeMap GetCollectedNodes(void) const {return collected_nodes_;}
    void CollectNodeAlternate(void);

private:

    ModbusServer();
    static ModbusServer *instance_;

    QVector<int> node_channels_;
    ModbusCollector *collector_;
    ModbusNodeMap collected_nodes_;

};

#endif // MODBUS_SERVER_H
