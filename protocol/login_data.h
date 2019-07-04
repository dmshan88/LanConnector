#ifndef LOGIN_DATA_H
#define LOGIN_DATA_H
#include "base_tcp_protocol.h"

class LoginData : public BaseTcpProtocol
{
public:
    LoginData(PROTOCOL_TYPE type = LOGIN_DATA) : BaseTcpProtocol(type){}

    void SetDeviceId(int device_id){device_id_ = device_id;}
//    int getDeviceId(void) {return device_id_;}

protected:
    virtual QByteArray GetRealData(void) const;
    const QByteArray GetDeviceIdData(void) const;

private:
    const QByteArray GetRandomData(void) const;

    int device_id_;
};

#endif // LOGIN_DATA_H
