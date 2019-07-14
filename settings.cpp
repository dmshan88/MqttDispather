/**
 *\file settings.cpp
 *
 *\brief
 *
 *
 */
#include "settings.h"

const QString Settings::kFieldHost = "host";
const QString Settings::kFieldPort = "port";
const QString Settings::kFieldClientId = "client_id";
const QString Settings::kFieldUsername = "username";
const QString Settings::kFieldPassword = "password";
const QString Settings::kDefaultClientId = "";

Settings* Settings::instance_ = 0;
Settings* Settings::Instance()
{
    if (!instance_) {
        instance_ = new Settings();
    }
    return instance_;
}

Settings::Settings()
{
    setting_ = new QSettings("./setting.ini", QSettings::IniFormat);

    if (!setting_->contains(kFieldHost)) {
        setHost("localhost");
    }
    if (!setting_->contains(kFieldPort)) {
        setPort(1883);
    }
    if (!setting_->contains(kFieldClientId)) {
        setClientId(kDefaultClientId);
    }
    if (!setting_->contains(kFieldUsername)) {
        setUsername("");
    }
    if (!setting_->contains(kFieldPassword)) {
        setPassword("");
    }
}
