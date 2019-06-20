#include "settings.h"


Settings* Settings::m_instance = 0;
Settings* Settings::Instance()
{
    if (!m_instance) {
        m_instance = new Settings();
    }
    return m_instance;
}

QString Settings::getMAC()
{
    return m_setting->value("Device/mac").toString();
}

void Settings::setMAC(QString mac)
{
    m_setting->setValue("Device/mac", mac);
    //to do
}

QString Settings::getIP()
{
    return m_setting->value("Device/ip").toString();
}

void Settings::setIP(QString ip)
{
    m_setting->setValue("Device/ip", ip);
    //to do
}

QString Settings::getMask()
{
    return m_setting->value("Device/mask").toString();
}

void Settings::setMask(QString ip)
{
    m_setting->setValue("Device/mask", ip);
    //to do
}
QString Settings::getGate()
{
    return m_setting->value("Device/gate").toString();
}

void Settings::setGate(QString ip)
{
    m_setting->setValue("Device/gate", ip);
    //to do
}

QString Settings::getDeviceVersion()
{
    return m_setting->value("Device/version").toString();
}

bool Settings::setServerCount(int count)
{
    if (count > 8 || count < 1) {
        return false;
    }
    m_setting->setValue("Server/count", count);
    return true;
}

bool Settings::setNodeCount(int count)
{
    if (count > 8 || count < 0) {
        return false;
    }
    m_setting->setValue("Node/count", count);
    return true;
}

int Settings::getNodeAddress(int index)
{
    int address = 0;
    m_setting->beginGroup("Node");

    if (m_setting->contains("address" + QString::number(index))
    ) {
        address = m_setting->value("address" + QString::number(index)).toInt();
    }
    m_setting->endGroup();
    return address;
}

bool Settings::setNodeAddress(int index, int address)
{
    if (index > 8 || index < 1) {
        return false;
    }
    m_setting->beginGroup("Node");
    m_setting->setValue("address"+ QString::number(index), address);
    m_setting->endGroup();
    return true;
}

void Settings::getServer(int index, QString &host, quint16 &port)
{
    host = "";
    port = 0;
    m_setting->beginGroup("Server");

    if (m_setting->contains("host" + QString::number(index))
        && m_setting->contains("port" + QString::number(index))
    ) {
        host = m_setting->value("host" + QString::number(index)).toString();
        port = m_setting->value("port" + QString::number(index)).toInt();
    }
    m_setting->endGroup();
}

bool Settings::setServer(int index, QString host, quint16 port)
{
    if (index > 8 || index < 1) {
        return false;
    }
    m_setting->beginGroup("Server");
    m_setting->setValue("host"+ QString::number(index), host);
    m_setting->setValue("port" + QString::number(index), port);
    m_setting->endGroup();
    return true;
}

Settings::Settings()
{

    m_setting = new QSettings("./setting.ini", QSettings::IniFormat);

    if (!m_setting->contains("Server/count")) {
        setServerCount(1);
        setServer(1, "127.0.0.1", 2404);
        setServer(2, "", 0);
        setServer(3, "", 0);
        setServer(4, "", 0);
        setServer(5, "", 0);
        setServer(6, "", 0);
        setServer(7, "", 0);
        setServer(8, "", 0);

    }

    if (!m_setting->contains("Device/id")) {
        setDeviceId(0);
        setMAC("00 00 00 00 00 00");
        setIPType(Settings::STATIC);
        setIP("192.168.1.100");
        setGate("192.168.1.1");
        setMask("255.255.255.0");
        setDeviceName("MY-DEVICE");
        setCOM("COM3");
        setBaud(4800);
        m_setting->setValue("Device/version", "79 1f");
    }

    if (!m_setting->contains("Node/count")) {
        setNodeCount(0);
    }

}
