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
    settings.cpp \
    protocol/udp_protocol.cpp \
    protocol/base_tcp_protocol.cpp \
    protocol/login_data.cpp \
    protocol/realtime_data.cpp \
    modbus_node.cpp \
    modbus_server.cpp \
    modbus_collector.cpp \
    host_port.cpp \
    main_task.cpp \
    main_timer.cpp

HEADERS += \
    tcp_client.h \
    settings.h \
    protocol/udp_protocol.h \
    protocol/base_tcp_protocol.h \
    protocol/login_data.h \
    protocol/realtime_data.h \
    modbus_node.h \
    modbus_server.h \
    modbus_collector.h \
    host_port.h \
    main_task.h \
    main_timer.h
LIBS += -LC:/dev/qt_code/LanConnector/modbus -llibmodbus-5
INCLUDEPATH += modbus
