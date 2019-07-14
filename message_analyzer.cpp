#include "message_analyzer.h"
#include <QDebug>
#include "parser.h"
#include "serializer.h"

static const QString CMD_DEVICEID						= "CMD_DEVICEID";
static const QString CMD_ROOTPATH						= "CMD_ROOTPATH";
static const QString CMD_PATHFILE						= "CMD_PATHFILE";
static const QString CMD_FILE_DOWNLOAD_TO_SERVER		= "CMD_FILE_DOWNLOAD_TO_SERVER";
static const QString CMD_FILE_UPLOAD_TO_CLIENT			= "CMD_FILE_UPLOAD_TO_CLIENT";
static const QString CMD_FILE_UPLOAD_TO_SERVER			= "CMD_FILE_UPLOAD_TO_SERVER";
static const QString CMD_RESTOREFAC					    = "CMD_RESTOREFAC";
static const QString CMD_MACHINEINFO					= "CMD_MACHINEINFO";
static const QString CMD_PING							= "CMD_PING";
static const QString CMD_DISCONNECT						= "CMD_DISCONNECT";
static const QString CMD_FILE_UPLOAD_TO_SERVER_SUCCESS  = "CMD_FILE_UPLOAD_TO_SERVER_SUCCESS";
static const QString CMD_FILE_DELETE                    = "CMD_FILE_DELETE";
static const QString CMD_FILE_DELETE_SUCCESS            = "CMD_FILE_DELETE_SUCCESS";
static const QString CMD_CHKRESULT						= "CMD_CHKRESULT";
static const QString CMD_TMPDATA_FILES					= "CMD_TMPDATA_FILES";
static const QString CMD_CHKERRDATA						= "CMD_CHKERRDATA";
static const QString CMD_TIMECALIBRATION                = "CMD_TIMECALIBRATION";
static const QString CMD_LOGINPSD                       = "CMD_LOGINPSD";
static const QString CMD_LOGINPSD_VERIFY                = "CMD_LOGINPSD_VERIFY";
static const QString CMD_ADJUSTTEMP_GET					= "CMD_ADJUSTTEMP_GET";
static const QString CMD_ADJUSTTEMP_SET			    	= "CMD_ADJUSTTEMP_SET";
static const QString CMD_RESETTIME                      = "CMD_RESETTIME";
static const QString CMD_PATIENTINFO                    = "CMD_PATIENTINFO";
static const QString CMD_POSITION                       = "CMD_POSITION";
static const QString CMD_MACHINEERRDATA                 = "CMD_MACHINEERRDATA";
static const QString CMD_QCRESULT						= "CMD_QCRESULT";

const QString MessageAnalyzer::kMachineIdKey = "machine_id";
const QString MessageAnalyzer::kPanelErrorKey = "panel_error";
const QString MessageAnalyzer::kPositionKey = "position";

/*
bool MessageAnalyzer::Analyze(const QMQTT::Message &message, QMQTT::Message &new_message)
{
    QString topic;
    QByteArray payload;

    bool ok = Analyze(message.payload(),topic, payload);
    if (ok) {
        new_message.setTopic(topic);
        new_message.setPayload(payload);
    }
    return ok;
}
*/
bool MessageAnalyzer::Analyze(const QByteArray &payload, QString &new_topic, QByteArray &new_payload)
{
    QVariantMap map;
//    QString new_topic;

    QString data(payload);
    QString mid;

    if (data.startsWith("CMD_DISCONNECT")) {
        mid = data.mid(26);
        qDebug() << "CMD_DISCONNECT" << mid;

        map.insert(kMachineIdKey, mid);
        new_topic = getTopicString(RECEIVE_OFFLINE);
    } else {
        QByteArray inBlock = payload;

        QDataStream in(&inBlock, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_7);
        QString cmdtype;
        in >> cmdtype;
        if ((QString::compare(cmdtype, CMD_MACHINEINFO, Qt::CaseSensitive)) == 0) {
            QString clientid;
            in >> clientid;

            mid = clientid.mid(12);
            qDebug() << "CMD_MACHINEINFO" << mid;

            map.insert(kMachineIdKey, mid);
            new_topic = getTopicString(RECEIVE_ONLINE);

        } else if (QString::compare(cmdtype, CMD_CHKERRDATA, Qt::CaseSensitive) == 0) {
            QString clientid;
            QByteArray json;
            in >> clientid >> json;
            mid = clientid.mid(12);
            qDebug() << "CMD_CHKERRDATA" << mid << json;

            QJson::Parser parser;
            bool ok;
            QVariantMap result = parser.parse(json, &ok).toMap();
            if (ok){
                map.insert(kMachineIdKey, mid);
                map.insert(kPanelErrorKey, result);
                new_topic = getTopicString(RECEIVE_PANEL_ERROR);
            }

        } else if (QString::compare(cmdtype, CMD_POSITION, Qt::CaseSensitive) == 0) {
            QString clientid;
            uint time,lac,ci;
            int mcc,mnc;
            in >> clientid >> time >> mcc >> mnc >> lac >> ci;
            mid = clientid.mid(12);
            qDebug() << mid << " received position" << mcc << mnc << lac << ci;
            QVariantList list;
            list.append(mcc);
            list.append(mnc);
            list.append(lac);
            list.append(ci);
            map.insert(kMachineIdKey, mid);
            map.insert(kPositionKey, list);
            new_topic = getTopicString(RECEIVE_POSITION);
        }
    }
    if (!map.isEmpty() && !new_topic.isEmpty()) {
        QJson::Serializer serializer;
        bool ok;
        new_payload = serializer.serialize(map, &ok);
        return ok;
    }
    return false;
}

QString MessageAnalyzer::getTopicString(MessageType message_type)
{
    QString topic;
    switch (message_type) {
    case RECEIVE_ONLINE:
        topic = "online_message";
        break;
    case RECEIVE_OFFLINE:
        topic = "offline_message";
        break;
    case RECEIVE_PANEL_ERROR:
        topic = "panel_error_message";
        break;
    case RECEIVE_POSITION:
        topic = "position_message";
        break;
    case RETAINED_ONLINE_STATE:
        topic = "retained_online_state";
        break;
    default:
        topic = "unknown_message";
        break;
    }
    return "dispather/" + topic;
}
