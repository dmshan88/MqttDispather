#ifndef MESSAGE_ANALYZER_H
#define MESSAGE_ANALYZER_H
#include <QString>
#include <QByteArray>

//#include "qmqtt_message.h"
class MessageAnalyzer
{    
public:
    enum MessageType{
        RECEIVE_UNKNOWN, RECEIVE_ONLINE, RECEIVE_OFFLINE, RECEIVE_PANEL_ERROR, RECEIVE_POSITION,
        RETAINED_ONLINE_STATE
    };
//    static bool Analyze(const QMQTT::Message &message, QMQTT::Message &new_message);
    static bool Analyze(const QByteArray &payload, QString &new_topic, QByteArray &new_payload);
    static QString getTopicString(MessageType message_type);
private:
    const static QString kMachineIdKey;
    const static QString kPanelErrorKey;
    const static QString kPositionKey;
};

#endif // MESSAGE_ANALYZER_H
