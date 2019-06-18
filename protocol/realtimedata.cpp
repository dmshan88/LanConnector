#include "realtimedata.h"
#include <QDebug>



QByteArray RealTimeData::setRealData()
{
    QByteArray data;
    foreach (NodeData node, m_nodelist) {
        data.append(nodeToData(node));
    }
    //qDebug() << "rr" << m_nodelist.length() << data;
    data.append(QByteArray::fromHex("37 34 33 33 33 33 33 33 33 33 33 33 33 33 33 33"));
    data.append(getIdData());
    qDebug() << data.toHex();
    return data;
}

QByteArray RealTimeData::nodeToData(RealTimeData::NodeData data)
{
    //QByteArray data;
    //data.fromHex("34 4f 34 0a 34");
    return QByteArray::fromHex("34 4f 34 0a 34");
}
