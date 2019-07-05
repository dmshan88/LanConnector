#include <QCoreApplication>
#include "tcp_client.h"
#include "protocol/login_data.h"
#include "protocol/realtime_data.h"
#include "settings.h"
#include "main_task.h"
#include "protocol/udp_protocol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Settings::Instance();
    udp_protocol udp;
    MainTask *task = MainTask::Instance();
    task->SetStartFlag(true);
    task->StartTimer();

    return a.exec();
}
