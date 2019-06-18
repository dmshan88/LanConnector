#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>

class Settings
{
public:
    static Settings* Instance();
    int getDeviceId(void){return m_setting->value("Device/id").toInt();}
    void setDeviceId(int id){m_setting->setValue("Device/id", id);}
    QString getMAC(void);
    void setMAC(QString mac);
    enum IP_TYPE{STATIC, DYNAMIC};
    void setIPType(IP_TYPE type){m_setting->setValue("Device/iptype", type);}
    IP_TYPE getIPType(void) {return (IP_TYPE) m_setting->value("Device/iptype").toInt();}
    QString getIP(void);
    void setIP(QString ip);
    QString getMask(void);
    void setMask(QString ip);
    QString getGate(void);
    void setGate(QString ip);
    QString getDeviceName(){return m_setting->value("Device/name").toString();}
    void setDeviceName(QString name){return m_setting->setValue("Device/name", name);}
    QString getDeviceVersion(void);


    int getServerCount(void) {return m_setting->value("Server/count").toInt();}
    bool setServerCount(int count);
    void getServer(int index, QString &host, quint16 &port);
    bool setServer(int index, QString host, quint16 port);

    int getNodeCount(void) {return m_setting->value("Node/count").toInt();}
    bool setNodeCount(int count);
    int getNodeAddress(int index);
    bool setNodeAddress(int index, int address);

private:
    Settings();

    static Settings *m_instance;
    QSettings *m_setting;
};

#endif // SETTINGS_H