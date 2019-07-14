#ifndef MQTT_CALLBACK_H
#define MQTT_CALLBACK_H

#include "mqtt_client.h"
class MqttCallback
{
public:
    MqttCallback();

    void static onConnectFailure(void* context, MQTTAsync_failureData* response);
    void static onConnectSuccess(void* context, MQTTAsync_successData* response);
    void static OnConnected(void *context, char *cause);
    void static onConnectionLost(void* context, char* cause);
    void static onSubscribeSuccess(void* context, MQTTAsync_successData* response);
    void static onSubscribeFailure(void* context, MQTTAsync_failureData* response);
    void static onSendSuccess(void* context, MQTTAsync_successData* response);
    int static onMessageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message);
    void static onDeliveryComplete(void* context, MQTTAsync_token token);
};

#endif // MQTT_CALLBACK_H
