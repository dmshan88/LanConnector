#include "logindata.h"
#include <QString>
#include <QDebug>
#include <QDateTime>

QByteArray LoginData::setRealData()
{
    return getRandData().append(getIdData());
}

QByteArray LoginData::getIdData()
{
    QString str = QString::number(m_device_id).rightJustified(8, '0', true);
    QByteArray data;
    for (int pos = str.length() - 2; pos >=0; pos -= 2) {
        bool ok;
        int part = 0;
        part = str.mid(pos, 2).toInt(&ok, 16) + 0x33;

        data.append(part);

    }
    return data;
}

QByteArray LoginData::getRandData()
{
    QByteArray data;
    data.setNum(QDateTime::currentDateTimeUtc().toTime_t() & 0xffff);
    return data.leftJustified(4, '0', true);
}
