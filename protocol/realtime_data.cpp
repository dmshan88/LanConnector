#include "realtime_data.h"
#include <QDebug>
#include <QDataStream>


bool RealtimeData::SetNodeData(const ModbusNodeMap &node_map)
{
    return NodeMapToData(node_map, node_data_);
}

QByteArray RealtimeData::GetRealData(void) const
{
    qDebug()<< "RealtimeData::GetRealData(void)";
    QByteArray data;

    data.append(node_data_);
    data.append(QByteArray::fromHex("37 34 33 33 33 33 33 33 33 33 33 33 33 33 33 33"));
    data.append(GetDeviceIdData());
    qDebug() << "RealtimeData::GetRealData" << data.toHex();
    return data;
}
