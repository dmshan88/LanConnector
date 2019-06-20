#include "realtimedata.h"
#include <QDebug>
#include <QDataStream>
#include <QByteArray>

QByteArray RealTimeData::setRealData()
{
    QByteArray data;
    foreach (NodeData node, m_nodelist) {
        data.append(nodeToData(node));
    }
    data.append(QByteArray::fromHex("37 34 33 33 33 33 33 33 33 33 33 33 33 33 33 33"));
    data.append(getIdData());
    qDebug() << data.toHex();
    return data;
}

QByteArray RealTimeData::nodeToData(RealTimeData::NodeData node)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::ReadWrite);
    stream << (quint8)(node.nodeId + 0x33)
           << (quint8)(node.temL + 0x33)
           << (quint8)(node.temH + 0x33)
           << (quint8)(node.humL + 0x33)
           << (quint8)(node.humH + 0x33);
    return data;
}
