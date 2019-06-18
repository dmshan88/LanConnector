#include "udp_protocol.h"
#include "settings.h"
#include "routine.h"

udp_protocol::udp_protocol(QObject *parent) :
    QObject(parent), m_udp(new QUdpSocket(parent))
{
    m_udp->bind(QHostAddress::Any, 1901);
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
                qDebug() << "receive get param";
                broadcastData(getDeviceParam());
            }
        } else if (cmd_data == QByteArray::fromHex("aa 00 07 06") && datagram.size() == 11) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "reset to default";
            }
        } else if (cmd_data == QByteArray::fromHex("aa 01 2b 04") && datagram.size() == 303) {
            if (datagram.mid(4,6) == QByteArray::fromHex(mac_ch)) {
                qDebug() << "set param";
                setParam(datagram.right(293));
                Routine::Instance()->setServerClients();
                Routine::Instance()->start();

            }
        }

    }
}

void udp_protocol::error(QAbstractSocket::SocketError errorCode)
{
    qDebug() << errorCode << m_udp->errorString();;
}

void udp_protocol::setParam(QByteArray data)
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
    quint16 port[8];
    QString host[8];
    for (int i=0; i < 8; i++) {
        stream.skipRawData(2);
        stream >> port[i];
        char str[30];
        stream.readRawData(str, 30);
        host[i].append(str);
    }

    setting->setIPType(iptype == 0 ? Settings::DYNAMIC : Settings::STATIC);
    setting->setIP(QHostAddress(ip).toString());
    setting->setGate(QHostAddress(gate).toString());
    setting->setMask(QHostAddress(mask).toString());
    setting->setServerCount(server_count);
    for (int i=0; i < 8; i++) {
        setting->setServer(i+1, host[i], port[i]);
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

QByteArray udp_protocol::getDeviceParam()
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

    QByteArray host_dataarray;
    for (int i = 1; i <= 8; i++) {
        quint16 port = 0;
        QString host = "";
        setting->getServer(i, host, port);
        QByteArray host_data;
        if (!host.isEmpty() && port > 0 && i <= setting->getServerCount()) {

            QDataStream host_stream(&host_data, QIODevice::ReadWrite);
            host_stream.setByteOrder(QDataStream::LittleEndian);
            host_stream << (quint16) 0 << port;
            host_data.append(host.leftJustified(30, 0, true));
        } else {
            host_data.append(host.leftJustified(34, 0, true));
        }
        host_dataarray.append(host_data);
    }
    return QByteArray::fromHex("55 01 2b 03")
        .append(QByteArray::fromHex(mac_ch))
        .append(setting->getIPType() == Settings::DYNAMIC ? QByteArray::fromHex("08") : QByteArray::fromHex("80"))
        .append(ip_data)
        .append(gate_data)
        .append(mask_data)
        .append(QByteArray::fromHex("ff ff ff ff ff ff"))
        .append(QByteArray::fromHex("08"))  //mac connection
        .append(host_dataarray)
        .append(QByteArray::fromHex("46"));
}

void udp_protocol::broadcastData(QByteArray data)
{
    m_udp->writeDatagram(data, QHostAddress::Broadcast, 1901);
    qDebug() << "broadcastdata:" << data.toHex();
}
