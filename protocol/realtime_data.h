#ifndef REALTIME_DATA_H
#define REALTIME_DATA_H

#include "login_data.h"
#include <modbus_node.h>

class RealtimeData : public LoginData
{
public:
    RealtimeData() : LoginData(REALTIME_DATA){node_data_.clear();}

    bool SetNodeData(const ModbusNodeMap &node_map);

protected:
    virtual QByteArray GetRealData(void) const;

private:

    QByteArray node_data_;
};

#endif // REALTIME_DATA_H
