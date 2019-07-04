#include "modbus_node.h"
#include <QDataStream>

ModbusNode::ModbusNode()
{
}

QByteArray ModbusNode::toData(quint8 index) const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::ReadWrite);
    stream << (quint8)(index + 0x33)
           << (quint8)((value2 & 0x00ff) + 0x33)
           << (quint8)((value2 >> 8) + 0x33)
           << (quint8)((value1 & 0x00ff) + 0x33)
           << (quint8)((value1 >> 8) + 0x33);
    return data;
}

bool NodeMapToData(const ModbusNodeMap &map, QByteArray &data)
{
    if (!map.size()) {
        return false;
    }
    data.clear();
    QMapIterator<int, ModbusNode> i(map);
    while (i.hasNext()) {
        i.next();
        ModbusNode modbus_node = i.value();
        data.append(modbus_node.toData(i.key()));
    }
    return true;
}
