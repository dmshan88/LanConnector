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
      char nodeId;
      char temH;
      char temL;
      char humH;
      char humL;
    };
    void addNode(NodeData node){m_nodelist.append(node);}

protected:
    virtual QByteArray setRealData();

private:
    QByteArray nodeToData(NodeData data);
    QList<NodeData> m_nodelist;
};

#endif // REALTIMEDATA_H
