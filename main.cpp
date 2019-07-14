#include <QCoreApplication>
//#include "mqtt_dispather.h"
#include "tools.h"
#include "mqtt_client.h"
#include <settings.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMsgHandler(CustomMessageOutput);

    Settings::Instance();
    qputenv("MQTT_C_CLIENT_TRACE", "client.log");
    qputenv("MQTT_C_CLIENT_TRACE_LEVEL","PROTOCOL");
    MqttClient::Instance()->Connect();
//    MqttDispather::Instance()->Start();

    return a.exec();
}
