#-------------------------------------------------
#
# Project created by QtCreator 2019-06-05T15:28:12
#
#-------------------------------------------------

QT       += core
QT += network

QT       -= gui

TARGET = LanConnector
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcp_client.cpp \
    protocol/base_protocol.cpp \
    protocol/logindata.cpp \
    protocol/realtimedata.cpp \
    routine.cpp \
    settings.cpp \
    protocol/udp_protocol.cpp \
    modbusrtu_server.cpp

HEADERS += \
    tcp_client.h \
    protocol/base_protocol.h \
    protocol/logindata.h \
    protocol/realtimedata.h \
    routine.h \
    settings.h \
    protocol/udp_protocol.h \
    modbusrtu_server.h
LIBS += -LC:/dev/qt_code/LanConnector/modbus -llibmodbus-5
INCLUDEPATH += modbus
