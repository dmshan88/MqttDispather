#include "tools.h"
#include <QFile>
#include <QMutex>
#include <QDateTime>
const static QString kLogFilename = "log.txt";
void CustomMessageOutput(QtMsgType type, const char *msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type){
        case QtDebugMsg:
            text = QString("Debug");
            break;
        case QtWarningMsg:
            text = QString("Warning");
            break;
        case QtCriticalMsg:
            text = QString("Critical");
            break;
        case QtFatalMsg:
            text = QString("Fatal");
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString message = QString("%1 [%2] %3").arg(current_date_time).arg(text).arg(msg);
    QFile file(kLogFilename);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}
