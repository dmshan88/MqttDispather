#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "MQTTAsync.h"
class MqttClient
{
public:

    static MqttClient *Instance();
    void Connect();
    void Subscribe(void);
    void Send(const char *topic, MQTTAsync_message *message);

protected:
    MqttClient();
    ~MqttClient();
private:
    void InitClient(void);

    MQTTAsync_willOptions will_;
    MQTTAsync_connectOptions conn_opts_;
    MQTTAsync client_;

    static MqttClient *_instance;
};

#endif // MQTT_CLIENT_H
