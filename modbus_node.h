#ifndef MODBUS_NODE_H
#define MODBUS_NODE_H
#include <QByteArray>
#include <QMap>
class ModbusNode
{
public:
    ModbusNode();
    int address;
    quint16 value1;
    quint16 value2;
//    bool is_timeout;

    QByteArray toData(quint8 index) const;

};
typedef QMap<int, ModbusNode> ModbusNodeMap;
bool NodeMapToData(const ModbusNodeMap &map, QByteArray &data);

#endif // MODBUS_NODE_H
