#include "base_tcp_protocol.h"

QByteArray BaseTcpProtocol::GetData() const
{
    QByteArray real_data = GetRealData();
    QByteArray data;

    data.append(GetHeadData())
        .append(real_data.length() + GetCmdData().length())
        .append(GetCmdData())
        .append(real_data)
        .append(GetCrcData())
        .append(GetEndData());
    return data;
}

QByteArray BaseTcpProtocol::GetHeadData(void) const
{
    return QByteArray::fromHex("68 88 88 88 88 88 88 68 A0");
}

QByteArray BaseTcpProtocol::GetEndData(void) const
{
    return QByteArray::fromHex("16");
}

QByteArray BaseTcpProtocol::GetCmdData(void) const
{
    QByteArray data;
    switch (type_) {
    case LOGIN_DATA:
        data =  QByteArray::fromHex("33 1d");
        break;
    case REALTIME_DATA:
        data = QByteArray::fromHex("36 1d");
        break;
    default:
        data = QByteArray::fromHex("33 33");
    }
    return data;
    /*
    QByteArray rtn_data;
    QByteArray data;

    switch (type_) {
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
*/
}

QByteArray BaseTcpProtocol::GetCrcData(void) const
{
    //to do
    return QByteArray::fromHex("00");
}


