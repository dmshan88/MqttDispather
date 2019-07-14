/**
 *\file settings.h
 *
 *\brief
 *
 *
 */
#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>

class Settings
{
public:
    static Settings* Instance();
    void setHost(const QString &host) {setting_->setValue(kFieldHost, host);}
    QString getHost(void) const {return setting_->value(kFieldHost).toString();}
    void setPort(quint16 port) {setting_->setValue(kFieldPort, port);}
    quint16 getPort(void) const {return setting_->value(kFieldPort).toInt();}
    void setClientId(const QString &client_id) {setting_->setValue(kFieldClientId, client_id);}
    QString getClientId(void) const {return setting_->value(kFieldClientId).toString();}
    void setUsername(const QString &username) {setting_->setValue(kFieldUsername, username);}
    QString getUsername(void) const {return setting_->value(kFieldUsername).toString();}
    void setPassword(const QString &password) {setting_->setValue(kFieldPassword, password);}
    QString getPassword(void) const {return setting_->value(kFieldPassword).toString();}

private:
    const static QString kFieldHost;
    const static QString kFieldPort;
    const static QString kFieldClientId;
    const static QString kFieldUsername;
    const static QString kFieldPassword;
    const static QString kDefaultClientId;

    Settings();
    static Settings *instance_;
    QSettings *setting_;
};

#endif // SETTINGS_H
