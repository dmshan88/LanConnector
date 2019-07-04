#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "main_timer.h"
#include "tcp_client.h"
#include "modbus_server.h"

class MainTask
{
public:
    static MainTask *Instance();
    void SetStartFlag(bool ok) {start_flag_ = ok;}
    bool GetStartFlag(void) const {return start_flag_;}

    void StartTimer(void){timer_->Start();}

    void ResetTcpClients(void);
    void SendRealTimeData();

    void SetModbusNodes(void);

private:

    MainTask();
    static MainTask *instance_;

    TcpClientMap tcp_clients_;
    bool start_flag_;
    ModbusServer *modbus_server_;
    MainTimer *timer_;

};

#endif // MAIN_TASK_H
