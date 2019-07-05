#include "modbus_collector.h"

ModbusCollector::ModbusCollector()
{
}
void ModbusCollector::SetSerialParam(const char *com, int baud)
{
    ctx = modbus_new_rtu(com, baud, 'N', 8, 1);
    if (modbus_connect(ctx) == -1) {
        modbus_free(ctx);
        is_ok = false;
    } else {
        is_ok = true;
    }
    is_busy = false;
}

bool ModbusCollector::CollectNode(const ModbusAddress &address, ModbusNode &node)
{
    if (!is_ok) {
        return false;
    }
    if (is_busy) {
        return false;
    }
    if (!address) {
        return false;
    }
    is_busy = true;
    modbus_set_slave(ctx, address);
    quint16 value[2] = {0,0};
    int ret = modbus_read_registers(ctx, 0, 2, value);
    node.address = address;
    node.value1 = value[0];
    node.value2 = value[1];
    is_busy = false;
    return ret > 0;

}
