#include "main_timer.h"
#include <QDebug>
#include "main_task.h"

const int MainTimer::kDefaultInterval = 100;
MainTimer::MainTimer(QObject *parent) :
    QObject(parent), timer_(new QTimer(this))
{
    SetInterval(kDefaultInterval);
    connect(timer_, SIGNAL(timeout()), this, SLOT(SlotTimeout()));
}

void MainTimer::SlotTimeout()
{
    static int counter = 0;

    ModbusServer::Instance()->CollectNodeAlternate();
    if (counter == 10) {
        MainTask::Instance()->SendRealTimeData();
    }

    counter ++;
    if (counter > 10) {
        counter = 0;
    }
}
