#include <QCoreApplication>
/*
#include <QTcpSocket>
#include <QTimer>
*/
#include "tcp_client.h"
#include "protocol/login_data.h"
#include "protocol/realtime_data.h"
#include <QDebug>
#include "settings.h"
#include "main_task.h"
#include "protocol/udp_protocol.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Settings::Instance();
    /*
    setting->setNodeCount(2);
    setting->setNodeAddress(1,11);
    setting->setNodeAddress(2,22);
    setting->setDeviceId(10013194);
    */
    udp_protocol udp;
    MainTask *task = MainTask::Instance();
    task->SetStartFlag(true);
    task->StartTimer();


    return a.exec();
}
