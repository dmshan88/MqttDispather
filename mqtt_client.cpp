#include "mqtt_client.h"
#include <QDebug>
#include "mqtt_callback.h"
#include "settings.h"
#include "message_analyzer.h"


MqttClient*  MqttClient::_instance = 0;

MqttClient*  MqttClient::Instance()
{
    if (0 == _instance) {
        _instance = new MqttClient();
    }
    return _instance;
}

MqttClient::MqttClient(): client_(0)
{
}

MqttClient::~MqttClient()
{
    qDebug() << "release mqttclient";
    if (client_) {
        MQTTAsync_destroy(&client_);
    }

    if (conn_opts_.username) {
        qDebug() << conn_opts_.username;
        delete conn_opts_.username;
    }
    if (conn_opts_.password) {
        qDebug() << conn_opts_.password;
        delete conn_opts_.password;
    }
    if (conn_opts_.will && conn_opts_.will->topicName) {
        delete conn_opts_.will->topicName;
    }
}

void MqttClient::Connect()
{
    InitClient();
    int rc = MQTTAsync_connect(client_, &conn_opts_);
    if (rc != MQTTASYNC_SUCCESS) {
        qDebug("Failed to start connect, return code %d\n", rc);
    }
}

void MqttClient::InitClient()
{
    if (client_)
        return;
    qDebug() << "init";
    Settings *setting = Settings::Instance();
    char *username = new char[50];
    strcpy(username, setting->getUsername().toLatin1().data());
    char *password = new char[50];
    strcpy(password, setting->getPassword().toLatin1().data());
    char *will_topic = new char[50];
    strcpy(will_topic, MessageAnalyzer::getTopicString(MessageAnalyzer::RETAINED_ONLINE_STATE).toLatin1().data());

    QString server_uri = QString("tcp://%1:%2").arg(setting->getHost()).arg(setting->getPort());

    MQTTAsync_create(&client_, server_uri.toLatin1().data(), setting->getClientId().toLatin1().data(),
                     MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
    MQTTAsync_setCallbacks(client_, this, MqttCallback::onConnectionLost,
                           MqttCallback::onMessageArrived, MqttCallback::onDeliveryComplete);
    MQTTAsync_setConnected(client_, this, MqttCallback::OnConnected);
    conn_opts_ = MQTTAsync_connectOptions_initializer;
    conn_opts_.keepAliveInterval = 10;
    conn_opts_.cleansession = 1;
    conn_opts_.onSuccess = MqttCallback::onConnectSuccess;
    conn_opts_.onFailure = MqttCallback::onConnectFailure;
    conn_opts_.context = this;
    conn_opts_.username = username;
    conn_opts_.password = password;
    conn_opts_.connectTimeout = 10;
    conn_opts_.automaticReconnect = 1;

    will_ = MQTTAsync_willOptions_initializer;
    will_.topicName = will_topic;
    will_.qos = 2;
    will_.retained = 1;
    will_.message = "off";
    conn_opts_.will = &will_;
}

void MqttClient::Subscribe()
{
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = MqttCallback::onSubscribeSuccess;
    opts.onFailure = MqttCallback::onSubscribeFailure;
    opts.context = this;
    int rc = MQTTAsync_subscribe(client_, "mnchip_mqtt_server", 2, &opts);
    if (rc != MQTTASYNC_SUCCESS) {
        qDebug() << "Subscribe error" << rc;
    }
}

void MqttClient::Send(const char *topic, MQTTAsync_message *message)
{
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = MqttCallback::onSendSuccess;
    opts.context = this;

    int rc = MQTTAsync_sendMessage(client_, topic, message, &opts);
    if (rc != MQTTASYNC_SUCCESS) {
        qDebug() << "Send error" << rc;
    }
//    MQTTAsync_freeMessage(&message);
}
