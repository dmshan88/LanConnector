#include "login_data.h"
#include <QString>
//#include <QDebug>
#include <QDateTime>

QByteArray LoginData::GetRealData(void) const
{
    QByteArray data;
    data.append(GetRandomData()).append(GetDeviceIdData());
    return data;
}

const QByteArray LoginData::GetDeviceIdData() const
{
    //to do
    QString str = QString::number(device_id_).rightJustified(8, '0', true);
    QByteArray data;
    for (int pos = str.length() - 2; pos >=0; pos -= 2) {
        bool ok;
        int part = 0;
        part = str.mid(pos, 2).toInt(&ok, 16) + 0x33;

        data.append(part);
    }
    return data;
}

const QByteArray LoginData::GetRandomData() const
{
    QByteArray data;
    data.setNum(QDateTime::currentDateTimeUtc().toTime_t() & 0xffff);
    return data.leftJustified(4, '0', true);
}
