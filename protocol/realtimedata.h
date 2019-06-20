#ifndef REALTIMEDATA_H
#define REALTIMEDATA_H

#include "logindata.h"
#include <QList>

class RealTimeData : public LoginData
{
public:
    RealTimeData(void) : LoginData(REALTIME_DATA){m_nodelist.clear();}
    struct NodeData
    {
      quint8 nodeId;
      quint8 temH;
      quint8 temL;
      quint8 humH;
      quint8 humL;
    };
    void addNode(NodeData node){m_nodelist.append(node);}

protected:
    virtual QByteArray setRealData();

private:
    QByteArray nodeToData(NodeData node);
    QList<NodeData> m_nodelist;
};

#endif // REALTIMEDATA_H
