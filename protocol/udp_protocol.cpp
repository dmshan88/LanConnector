#include "udp_protocol.h"
#include "settings.h"
#include "main_task.h"

udp_protocol::udp_protocol(QObject *parent) :
    QObject(parent), m_udp(new QUdpSocket(parent))
{
    //m_udp->bind(QHostAddress::Any, 1901);
    m_udp->bind((QHostAddress)"192.168.0.100", 1901);
    connect(m_udp, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_udp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

}

void udp_protocol::readData()
{
    while (m_udp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udp->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udp->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        if (sender == QHostAddress(Settings::Instance()->getIP())) {
            continue;
        }
        qDebug() << "receive datagram" <<  sender << senderPort << datagram.toHex();

        QByteArray mac_data = Settings::Instance()->getMAC().toLatin1();
        const char *mac_ch = mac_data.constData();
        QByteArray cmd_data = datagram.left(4);
        if (cmd_data == QByteArray::fromHex("aa 00 01 02") && datagram.size() == 5) {
            qDebug() << "receive find device";
            broadcastData(getSearchReturn());
        } else if (cmd_data == QByteArray::fromHex("aa 00 07 03") && datagram.size() == 11) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "get server param";
                broadcastData(getServerParam());
            }
        } else if (cmd_data == QByteArray::fromHex("aa 00 07 06") && datagram.size() == 11) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "reset to default";
            }
        } else if (cmd_data == QByteArray::fromHex("aa 01 2b 04") && datagram.size() == 303) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "set server param";
                setServerParam(datagram.right(293));
                MainTask::Instance()->ResetTcpClients();
                MainTask::Instance()->SetStartFlag(true);

            }
        } else if (cmd_data == QByteArray::fromHex("aa 00 0b 0a") && datagram.size() == 15) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "get device param";
                broadcastData(getDeviceParam());
            }
        } else if (cmd_data == QByteArray::fromHex("aa 00 67 0a") && datagram.size() == 107) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "set device param";
               setDeviceParam(datagram.right(97));
            }
        }

    }
}

void udp_protocol::error(QAbstractSocket::SocketError errorCode)
{
    qDebug() << errorCode << m_udp->errorString();;
}

void udp_protocol::setServerParam(QByteArray data)
{
    Settings *setting = Settings::Instance();
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    qint8 iptype;
    quint32 ip;
    quint32 gate;
    quint32 mask;
    quint8 server_count;
    stream  >> iptype >> ip >> gate >> mask;
    stream.skipRawData(6);
    stream >> server_count;
    HostPortMap host_port_map;

    for (int i=0; i < 8; i++) {
        stream.skipRawData(2);
        quint16 port;
        stream >> port;
        char host[30];
        stream.readRawData(host, 30);
        host_port_map.insert(i+1, HostPort(host, port));
    }

    setting->setIPType(iptype == 0 ? Settings::DYNAMIC : Settings::STATIC);
    setting->setIP(QHostAddress(ip).toString());
    setting->setGate(QHostAddress(gate).toString());
    setting->setMask(QHostAddress(mask).toString());
    setting->setServerParam(host_port_map);
    setting->setServerCount(server_count);
    /*
    for (int i=0; i < 8; i++) {
        setting->setHostPort(i+1, HostPort(host[i], port[i]));
    }
    */

}

void udp_protocol::setDeviceParam(QByteArray data)
{
    QString idstr = data.mid(6,1).toHex()
                    + data.mid(5,1).toHex()
                    + data.mid(4,1).toHex()
                    + data.mid(3,1).toHex();

    qDebug() << data.toHex() << idstr;
    Settings *setting = Settings::Instance();
//    bool ok;
    int id = idstr.toInt();

    quint8 node_count;
    quint8 node_address[8];
    QDataStream stream(&data, QIODevice::ReadOnly);
//    stream.setByteOrder(QDataStream::LittleEndian);
    stream.skipRawData(3);
    stream.skipRawData(4);
    stream.skipRawData(14);
    stream >> node_count;
    for (int i=0; i<8; i++) {
        stream >> node_address[i];
    }
    setting->setDeviceId(id);
    setting->setNodeCount(node_count);
    for (int i=0; i<8; i++) {
        setting->setNodeAddress(i+1, node_address[i]);
    }
}

QByteArray udp_protocol::getSearchReturn()
{
    Settings *setting = Settings::Instance();

    QByteArray ipdata;
    QDataStream stream(&ipdata, QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << ((QHostAddress)setting->getIP()).toIPv4Address();


    QByteArray mac_data = setting->getMAC().toLatin1();
    const char *mac_ch = mac_data.constData();

    QByteArray version_data = setting->getDeviceVersion().toLatin1();
    const char *ver_ch = version_data.constData();
    return QByteArray::fromHex("55 00 2c 02 00 28 00 00")
        .append(ipdata)
        .append(QByteArray::fromHex(mac_ch))
        .append(QByteArray::fromHex(ver_ch))
        .append(QByteArray::fromHex("01 00"))
        .append(setting->getDeviceName().leftJustified(21, 0, true))
        .append(QByteArray::fromHex("46"));

}

QByteArray udp_protocol::getServerParam()
{
    Settings *setting = Settings::Instance();

    QByteArray mac_data = setting->getMAC().toLatin1();
    const char *mac_ch = mac_data.constData();

    QByteArray ip_data;
    QDataStream ip_stream(&ip_data, QIODevice::ReadWrite);
    ip_stream.setByteOrder(QDataStream::LittleEndian);
    ip_stream << ((QHostAddress)setting->getIP()).toIPv4Address();

    QByteArray mask_data;
    QDataStream mask_stream(&mask_data, QIODevice::ReadWrite);
    mask_stream.setByteOrder(QDataStream::LittleEndian);
    mask_stream << ((QHostAddress)setting->getMask()).toIPv4Address();

    QByteArray gate_data;
    QDataStream gate_stream(&gate_data, QIODevice::ReadWrite);
    gate_stream.setByteOrder(QDataStream::LittleEndian);
    gate_stream << ((QHostAddress)setting->getGate()).toIPv4Address();

    QByteArray host_data_array;
    HostPortMap host_port_map = setting->getServerParam();
    for (int i = 1; i <= 8; i++) {
        quint16 port = 0;
        QString host = "";
        if (host_port_map.contains(i)) {
            port = host_port_map.value(i).getPort();
            host = host_port_map.value(i).getHost();
        }
        QByteArray host_data;
        if (!host.isEmpty() && port > 0 && i <= setting->getServerCount()) {

            QDataStream host_stream(&host_data, QIODevice::ReadWrite);
            host_stream.setByteOrder(QDataStream::LittleEndian);
            host_stream << (quint16) 0 << port;
            host_data.append(host.leftJustified(30, 0, true));
        } else {
            host_data.append(host.leftJustified(34, 0, true));
        }
        host_data_array.append(host_data);
    }

    return QByteArray::fromHex("55 01 2b 03")
        .append(QByteArray::fromHex(mac_ch))
        .append(setting->getIPType() == Settings::DYNAMIC ? QByteArray::fromHex("08") : QByteArray::fromHex("80"))
        .append(ip_data)
        .append(gate_data)
        .append(mask_data)
        .append(QByteArray::fromHex("ff ff ff ff ff ff"))
        .append(QByteArray::fromHex("08"))  //mac connection
        .append(host_data_array)
        .append(QByteArray::fromHex("46"));
}

QByteArray udp_protocol::getDeviceParam()
{
    //return QByteArray::fromHex("5500600b 000eead49d10 7e10 08 94310110 03003c000500 01 c0120000080001 02150b030405060708 e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e7030000e703000090c84b");
    Settings *setting = Settings::Instance();

    QByteArray mac_data = setting->getMAC().toLatin1();
    const char *mac_ch = mac_data.constData();
    QByteArray id_data = QByteArray::number(setting->getDeviceId()).leftJustified(8, '0', true);
    QByteArray id_data_rev = id_data.mid(6,2)
            .append(id_data.mid(4, 2))
            .append(id_data.mid(2, 2))
            .append((id_data.mid(0,2)));
    const char *id_ch = id_data_rev.constData();
    //qDebug() << id_data << id_data_rev << id_ch;
    QByteArray node_data = QByteArray::fromHex("03 01 02 03 04 05 06 07 08");
    QDataStream stream(&node_data, QIODevice::ReadWrite);
    quint8 nodecount = setting->getNodeCount();
    stream << nodecount;
    for (int i = 0; i < nodecount; i++) {
        stream << setting->getNodeAddress(i + 1);
    }
    return QByteArray::fromHex("55 00 60 0b")
            .append(QByteArray::fromHex(mac_ch))
            .append(QByteArray::fromHex("7e 10"))
            .append(QByteArray::fromHex("08"))
            .append(QByteArray::fromHex(id_ch))
            .append(QByteArray::fromHex("03 00 3c 00 05 00"))
            .append(QByteArray::fromHex("01"))
            .append(QByteArray::fromHex("c0 12 00 00 08 00 01"))
            .append(node_data)
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("e7 03 00 00 e7 03 00 00"))
            .append(QByteArray::fromHex("83 2d a4"))
            ;
}

void udp_protocol::broadcastData(QByteArray data)
{
    m_udp->writeDatagram(data, QHostAddress::Broadcast, 1901);
    qDebug() << "broadcastdata:" << data.toHex();
}
