#include "settings.h"


Settings* Settings::instance_ = 0;
Settings* Settings::Instance()
{
    if (!instance_) {
        instance_ = new Settings();
    }
    return instance_;
}

QString Settings::getMAC()
{
    return setting_->value("Device/mac").toString();
}

void Settings::setMAC(QString mac)
{
    setting_->setValue("Device/mac", mac);
    //to do
}

QString Settings::getIP()
{
    return setting_->value("Device/ip").toString();
}

void Settings::setIP(QString ip)
{
    setting_->setValue("Device/ip", ip);
    //to do
}

QString Settings::getMask()
{
    return setting_->value("Device/mask").toString();
}

void Settings::setMask(QString ip)
{
    setting_->setValue("Device/mask", ip);
    //to do
}
QString Settings::getGate()
{
    return setting_->value("Device/gate").toString();
}

void Settings::setGate(QString ip)
{
    setting_->setValue("Device/gate", ip);
    //to do
}

QString Settings::getDeviceVersion()
{
    return setting_->value("Device/version").toString();
}

HostPortMap Settings::getServerParam() const
{
    HostPortMap map;
    for(int i = 0; i < getServerCount(); i++) {
        QString host = setting_->value("Server/host" + QString::number(i + 1), "").toString();
        quint16 port = setting_->value("Server/port" + QString::number(i + 1), 0).toInt();
        map.insert(i + 1, HostPort(host, port));
    }
    return map;
}

void Settings::setServerParam(const HostPortMap &map)
{
    QMapIterator<int, HostPort> i(map);
    while (i.hasNext()) {
        i.next();
        int index = i.key();
        HostPort host_port = i.value();
        if (index > 8 || index < 1) {
            continue;
        }
        setting_->setValue("Server/host"+ QString::number(index), host_port.getHost());
        setting_->setValue("Server/port" + QString::number(index), host_port.getPort());
    }
}

bool Settings::setServerCount(int count)
{
    if (count > 8 || count < 1) {
        return false;
    }
    setting_->setValue("Server/count", count);
    return true;
}

bool Settings::setNodeCount(quint8 count)
{
    if (count > 8) {
        return false;
    }
    setting_->setValue("Node/count", count);
    return true;
}

quint8 Settings::getNodeAddress(int index)
{
    int address = 0;
    setting_->beginGroup("Node");

    if (setting_->contains("address" + QString::number(index))
    ) {
        address = setting_->value("address" + QString::number(index)).toInt();
    }
    setting_->endGroup();
    return address;
}

bool Settings::setNodeAddress(int index, quint8 address)
{
    if (index > 8 || index < 1) {
        return false;
    }
    setting_->beginGroup("Node");
    setting_->setValue("address"+ QString::number(index), address);
    setting_->endGroup();
    return true;
}

Settings::Settings()
{

    setting_ = new QSettings("./setting.ini", QSettings::IniFormat);

    if (!setting_->contains("Server/count")) {
        HostPortMap map;
        map.insert(1, HostPort("127.0.0.1", 2404));
        map.insert(2, HostPort("", 0));
        map.insert(3, HostPort("", 0));
        map.insert(4, HostPort("", 0));
        map.insert(5, HostPort("", 0));
        map.insert(6, HostPort("", 0));
        map.insert(7, HostPort("", 0));
        map.insert(8, HostPort("", 0));
        setServerParam(map);
        setServerCount(1);
    }

    if (!setting_->contains("Device/id")) {
        setDeviceId(0);
        setMAC("00 00 00 00 00 00");
        setIPType(Settings::STATIC);
        setIP("192.168.1.100");
        setGate("192.168.1.1");
        setMask("255.255.255.0");
        setDeviceName("MY-DEVICE");
        setCOM("COM3");
        setBaud(4800);
        setting_->setValue("Device/version", "79 1f");
    }

    if (!setting_->contains("Node/count")) {
        setNodeCount(0);
    }

}
