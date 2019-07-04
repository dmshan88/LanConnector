#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>
#include "host_port.h"

class Settings
{
public:
    static Settings* Instance();
    int getDeviceId(void){return setting_->value("Device/id").toInt();}
    void setDeviceId(int id){setting_->setValue("Device/id", id);}
    void setCOM(QString com) {setting_->setValue("Device/com", com);}
    QString getCOM(void) {return setting_->value("Device/com").toString();}
    int getBaud(void) {return setting_->value("Device/baud").toInt();}
    void setBaud(int baud){setting_->setValue("Device/baud", baud);}
    QString getMAC(void);
    void setMAC(QString mac);
    enum IP_TYPE{STATIC, DYNAMIC};
    void setIPType(IP_TYPE type){setting_->setValue("Device/iptype", type);}
    IP_TYPE getIPType(void) {return (IP_TYPE) setting_->value("Device/iptype").toInt();}
    QString getIP(void);
    void setIP(QString ip);
    QString getMask(void);
    void setMask(QString ip);
    QString getGate(void);
    void setGate(QString ip);
    QString getDeviceName(){return setting_->value("Device/name").toString();}
    void setDeviceName(QString name){return setting_->setValue("Device/name", name);}
    QString getDeviceVersion(void);


    HostPortMap getServerParam(void) const;
    void setServerParam(const HostPortMap &map);
    int getServerCount(void) const {return setting_->value("Server/count").toInt();}
    bool setServerCount(int count);


    quint8 getNodeCount(void) {return setting_->value("Node/count").toInt();}
    bool setNodeCount(quint8 count);
    quint8 getNodeAddress(int index);
    bool setNodeAddress(int index, quint8 address);

private:
    Settings();

    static Settings *instance_;
    QSettings *setting_;
};

#endif // SETTINGS_H
