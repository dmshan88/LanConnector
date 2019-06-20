#include "modbusrtu_server.h"
#include <QDebug>
#include <QTimer>

ModbusRtuServer *ModbusRtuServer::m_instance = 0;
const int ModbusRtuServer::NODE_COUNT = 8;
ModbusRtuServer *ModbusRtuServer::Instance()
{
    if (!m_instance) {
        m_instance = new ModbusRtuServer();
    }
    return m_instance;
}


ModbusRtuServer::ModbusRtuServer(QObject *parent) :
    QObject(parent)
{
    //setRTU();

    setNodeCount(NODE_COUNT);
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start();
}

bool ModbusRtuServer::addNodeAddress(int index, int address)
{
    if (index < 0 || index >= m_nodelist.size()) {
        return false;
    }

    foreach (Modbus_Node old_node, m_nodelist) {
        if (old_node.address == address && address) {
            return false;
        }
    }
    m_nodelist[index].address = address;
    m_nodelist[index].is_timeout = true;
    m_nodelist[index].value1 = 0;
    m_nodelist[index].value2 = 0;
    return true;
}

void ModbusRtuServer::setNodeValue(int index, ModbusRtuServer::Modbus_Node node)
{
    m_nodelist[index].is_timeout = node.is_timeout;
    m_nodelist[index].value1 = node.value1;
    m_nodelist[index].value2 = node.value2;
}

void ModbusRtuServer::setNodeCount(int count)
{
    m_nodelist.resize(count);
    for (int i = 0; i < m_nodelist.count(); i++) {
        addNodeAddress(i, 0);
    }
}

ModbusRtuServer::Modbus_Node ModbusRtuServer::getNode(int index)
{
    /*
     Modbus_Node node;
     node.is_timeout = true;
     node.address = 0;
     node.value1 = 0;
     node.value2 = 0;
     return m_nodelist.value(index, node);
*/
     return m_nodelist.at(index);
}

void ModbusRtuServer::timeout()
{
    static int node_index = 0;
    Modbus_Node node;
    int start = node_index;

    do {
        node =  m_nodelist.at(node_index);
        node_index++;
        if (node_index >= m_nodelist.size())
            node_index = 0;
        if (start == node_index && start) {
            return;
        }

    } while(node.address == 0);


    node.is_timeout = !getValue(node.address, node.value1, node.value2);
    setNodeValue(start,node);
}

void ModbusRtuServer::setRTU(QString com, int baud)
{
    ctx = modbus_new_rtu(com.toLatin1().data(), baud, 'N', 8, 1);
    if (modbus_connect(ctx) == -1) {
        modbus_free(ctx);
//        return false;
        is_ok = false;
    }
    is_busy = false;
//    return true;
    is_ok = true;
}

bool ModbusRtuServer::getValue(int address, quint16 &value1, quint16 &value2)
{
    if (!is_ok) {
        return false;
    }
    if (is_busy) {
        return false;
    }
    is_busy = true;
    modbus_set_slave(ctx, address);
    quint16 value[2] = {0,0};
    int ret = modbus_read_registers(ctx, 0, 2, value);
    value1 = value[0];
    value2 = value[1];
    is_busy = false;
    return ret > 0;

}
