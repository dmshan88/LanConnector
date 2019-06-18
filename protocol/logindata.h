#ifndef LOGINDATA_H
#define LOGINDATA_H
#include "base_protocol.h"

class LoginData : public BaseProtocol
{
public:
    LoginData(PROTOCOL_TYPE type = LOGIN_DATA) : BaseProtocol(type){}
    void setId(int id){m_device_id = id;}
    int getId(void) {return m_device_id;}

protected:
    virtual QByteArray setRealData();
    QByteArray getIdData(void);
    QByteArray getRandData(void);
private:
    int m_device_id;
};

#endif // LOGINDATA_H
