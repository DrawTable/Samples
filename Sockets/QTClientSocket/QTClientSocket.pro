QT += core
QT += network
QT -= gui

TARGET = QTClientSocket
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    controller.h \
    worker.h

