#ifndef MODBUSRTU_SERVER_H
#define MODBUSRTU_SERVER_H

#include <QObject>
#include "modbus.h"
#include <QVector>

class ModbusRtuServer : public QObject
{
    Q_OBJECT
public:
    static ModbusRtuServer *Instance();

    struct Modbus_Node{
        int address;
        quint16 value1;
        quint16 value2;
        bool is_timeout;
    };

    bool addNodeAddress(int index, int address);
    void setNodeValue(int index, Modbus_Node node);
    void setNodeCount(int count);
    int getNodeCount(void) {return m_nodelist.count();}
    Modbus_Node getNode(int index);
    void setRTU(QString com = "COM3", int baud = 4800);

signals:

public slots:

private slots:
    void timeout();
private:
    bool getValue(int address,quint16 &value1,quint16 &value2);

    explicit ModbusRtuServer(QObject *parent = 0);
    static ModbusRtuServer *m_instance;
    modbus_t *ctx;
    bool is_ok;
    bool is_busy;
    const static int NODE_COUNT;
    QVector<Modbus_Node> m_nodelist;


};

#endif // MODBUSRTU_SERVER_H
