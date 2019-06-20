#include <QCoreApplication>
/*
#include <QTcpSocket>
#include <QTimer>
*/
#include "tcp_client.h"
#include "protocol/logindata.h"
#include "protocol/realtimedata.h"
#include <QDebug>
#include "settings.h"
#include "routine.h"
#include "protocol/udp_protocol.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Settings *setting = Settings::Instance();
    /*
    setting->setNodeCount(2);
    setting->setNodeAddress(1,11);
    setting->setNodeAddress(2,22);
    setting->setDeviceId(10013194);
    */
    udp_protocol udp;
    Routine *routine = Routine::Instance();
    routine->start();


    return a.exec();
}
