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
    setting->setNodeCount(1);
    setting->setNodeAddress(1,22);
    setting->setDeviceId(10013194);

    udp_protocol udp;
    Routine *routine = Routine::Instance();
    routine->start();

    //qDebug() << udp.getSearchReturn().toHex();
    /*
    Routine *routine = Routine::Instance();
    */
    /*
    Settings *setting = Settings::Instance();
    Settings *setting2 = Settings::Instance();
    if (setting == setting2) {
        qDebug() << "same";
    } else {
        qDebug() << "not same";
    }
    QString host = "";
    int port = 0;
    setting->getServer(1,host, port);
    qDebug() << host << port;
    */
    /*
    TcpClient *client = new TcpClient();
    client->connectToHost("localhost", 2404);
    qDebug("connect");

    LoginData logindata(10013194);
    qDebug() << logindata.toData().toHex();
    RealTimeData realtimedata(10013194);
    qDebug() << realtimedata.toData().toHex();
    RealTimeData::NodeData node;
    realtimedata.addNode(node);

    client->sendData(realtimedata.toData());
*/


    /*
    QTcpSocket *client = new QTcpSocket();


    client->connectToHost("localhost", 1234);
    client->waitForConnected();
    int delay = 1000;
    while(client->state() == QAbstractSocket::ConnectedState) {
        qDebug("hahah");
        char data[] = {0x68, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x68, 0xa0, 0x1b, 0x36, 0x1d, 0x34, 0x4f, 0x34, 0x0a, 0x34, 0x37, 0x34, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xc7, 0x64, 0x34, 0x43, 0xda, 0x16};
        client->write(data, sizeof(data)/sizeof(data[0]));
        QEventLoop eventloop;

        QTimer::singleShot(delay, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    qDebug("netstat error:" + client->state());
    client->disconnectFromHost();
*/
    return a.exec();
}
