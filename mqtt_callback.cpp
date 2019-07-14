#include "mqtt_callback.h"
#include <QDebug>
#include "MQTTClient.h"
//#include <QDateTime>
#include "message_analyzer.h"

MqttCallback::MqttCallback()
{
}

void MqttCallback::onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    qDebug() << "onConnectFailure" << response->code << response->message;
}

void MqttCallback::onConnectSuccess(void *context, MQTTAsync_successData *response)
{
    qDebug() << "onConnectSuccess";
}

void MqttCallback::OnConnected(void *context, char *cause)
{
     qDebug() << "OnConnected" << cause;
     MqttClient *client = (MqttClient*)context;
     client->Subscribe();
}

void MqttCallback::onConnectionLost(void *context, char *cause)
{
    qDebug() << "onConnectionLost";
}

void MqttCallback::onSubscribeSuccess(void *context, MQTTAsync_successData *response)
{
    qDebug() << "onSubscribeSuccess";
    char data[] = "on";
    MqttClient *client = (MqttClient*)context;
    MQTTAsync_message mqtt_message = MQTTAsync_message_initializer;
    mqtt_message.qos = 2;
    mqtt_message.payload = data;
    mqtt_message.payloadlen = strlen(data);
    mqtt_message.retained = 1;
    client->Send(MessageAnalyzer::getTopicString(MessageAnalyzer::RETAINED_ONLINE_STATE).toLatin1().data(),
                 &mqtt_message);
}

void MqttCallback::onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    qDebug() << "onSubscribeFailure";
}

void MqttCallback::onSendSuccess(void *context, MQTTAsync_successData *response)
{
    qDebug() << "onSendSuccess" << response->token;
}

int MqttCallback::onMessageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{

    QByteArray payload = QByteArray((char *)message->payload, message->payloadlen);
    qDebug() << "onMessageArrived: " << topicName  << payload;
    QByteArray new_payload;
    QString new_topic;
//    QMQTT::Message new_message;
//    MessageAnalyzer::Analyse(payload,new_topic, new_payload);
    if (MessageAnalyzer::Analyze(payload, new_topic, new_payload)) {
        qDebug() << new_topic << new_payload;
        MqttClient *client = (MqttClient*)context;
        MQTTAsync_message mqtt_message = MQTTAsync_message_initializer;

        char *data = new char[new_payload.length()];
        memcpy(data, new_payload.data(), new_payload.length());
        mqtt_message.qos = 2;
        mqtt_message.payload = data;
        mqtt_message.payloadlen = new_payload.length();//current_time.length();
        mqtt_message.retained = 0;
        client->Send(new_topic.toLatin1().data(), &mqtt_message);
        delete data;
    }
    return 1;
}

void MqttCallback::onDeliveryComplete(void *context, MQTTAsync_token token)
{
    qDebug() << "onDeliveryComplete:" << token;
}
