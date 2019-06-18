#include "base_protocol.h"
#include <QDebug>

QByteArray BaseProtocol::toData()
{
    QByteArray real_data = setRealData();
    return getHead()
           .append(real_data.length() + getCmd().length())
           .append(getCmd())
           .append(real_data)
           .append(getCRC(real_data))
           .append(getEnd());
}

QByteArray BaseProtocol::getHead()
{
    return QByteArray::fromHex("68 88 88 88 88 88 88 68 A0");
}

QByteArray BaseProtocol::getEnd()
{
    return QByteArray::fromHex("16");
}

QByteArray BaseProtocol::getCmd()
{
    QByteArray rtn_data;
    QByteArray data;

    switch (m_type) {
    case LOGIN_DATA:
        data = QByteArray::fromHex("EA 00");
        break;
    case REALTIME_DATA:
        data = QByteArray::fromHex("EA 03");
        break;
    }
    for(int i= data.length()-1; i >= 0; i--) {
        char c = data.at(i) + 0x33;
        rtn_data.append(c & 0xff);
    }
    return rtn_data;

}

QByteArray BaseProtocol::getCRC(QByteArray data)
{
    //to do
    return QByteArray::fromHex("00");
}


