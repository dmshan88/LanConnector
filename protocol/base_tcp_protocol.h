#ifndef BASE_TCP_PROTOCOL_H
#define BASE_TCP_PROTOCOL_H
#include <QByteArray>

class BaseTcpProtocol
{
public:
    enum PROTOCOL_TYPE{LOGIN_DATA, REALTIME_DATA};
    explicit BaseTcpProtocol(PROTOCOL_TYPE type) : type_(type){}

    QByteArray GetData(void) const;
protected:
    virtual QByteArray GetRealData(void) const = 0;

private:
    QByteArray GetCmdData(void) const;
    QByteArray GetEndData(void) const;
    QByteArray GetHeadData(void) const;
    QByteArray GetCrcData(void) const;

    PROTOCOL_TYPE type_;
};

#endif // BASE_TCP_PROTOCOL_H
