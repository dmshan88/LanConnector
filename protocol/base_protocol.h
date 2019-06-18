#ifndef BASE_PROTOCOL_H
#define BASE_PROTOCOL_H
#include <QByteArray>

class BaseProtocol
{
public:
    enum PROTOCOL_TYPE{LOGIN_DATA, REALTIME_DATA};
    BaseProtocol(PROTOCOL_TYPE type) : m_type(type){}

    QByteArray toData(void);
protected:
    QByteArray getHead();
    QByteArray getEnd();
    QByteArray getCmd();
    virtual QByteArray setRealData() = 0;

private:
    QByteArray getHexString(QByteArray bytes);
    QByteArray getCRC(QByteArray data);

    PROTOCOL_TYPE m_type;

};

#endif // BASE_PROTOCOL_H
