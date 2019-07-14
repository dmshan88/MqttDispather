#-------------------------------------------------
#
# Project created by QtCreator 2019-07-10T08:38:00
#
#-------------------------------------------------

QT += core
QT += network
QT -= gui

TARGET = MqttDispather
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
 #   mqtt_dispather.cpp \
    settings.cpp \
    tools.cpp \
    message_analyzer.cpp \
    mqtt_client.cpp \
    mqtt_callback.cpp



HEADERS += \
#    mqtt_dispather.h \
    settings.h \
    tools.h \
    message_analyzer.h \
    mqtt_client.h \
    mqtt_callback.h

#include(../QMqtt/qmqtt.pri)

#INCLUDEPATH += ../QMqtt/
win32 {
    INCLUDEPATH += C:/dev/lib/paho/include
    INCLUDEPATH += C:/dev/lib/qjson/include/qjson
    LIBS += -LC:/dev/lib/paho/lib/ -lpaho-mqtt3a
    LIBS += -LC:/dev/lib/qjson/lib/ -lqjson
}
unix {
    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/local/include/qjson
    LIBS += -L/usr/local/lib/ -lpaho-mqtt3a
    LIBS += -L/usr/local/lib/ -lqjson
}
QMAKE_CXXFLAGS += -std=c++0x
