#ifndef MODBUS_COLLECTOR_H
#define MODBUS_COLLECTOR_H

#include "modbus.h"
#include "modbus_node.h"
class ModbusCollector
{
public:
    ModbusCollector();

    void SetSerialParam(const char* com, int baud = 4800);

    bool CollectNode(int address, ModbusNode &node);
private:

    modbus_t *ctx;
    bool is_ok;
    bool is_busy;
};

#endif // MODBUS_COLLECTOR_H
