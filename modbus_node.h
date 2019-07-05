#ifndef MODBUS_NODE_H
#define MODBUS_NODE_H
#include <QByteArray>
#include <QMap>
#include <QVector>

typedef quint8 ModbusAddress;
typedef QVector<ModbusAddress> ModbusAddressVector;

class ModbusNode
{
public:
    ModbusNode();
    ModbusAddress address;
    quint16 value1;
    quint16 value2;
//    bool is_timeout;

    QByteArray toData(const ModbusAddress &index) const;

};
typedef QMap<int, ModbusNode> ModbusNodeMap;
bool NodeMapToData(const ModbusNodeMap &map, QByteArray &data);

#endif // MODBUS_NODE_H
