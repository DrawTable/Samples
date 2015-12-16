QT += core
QT -= gui

TARGET = ScreenDetect
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app
INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab




SOURCES += main.cpp \
    detectbyhist.cpp \
    hsvprocessor.cpp

HEADERS += \
    detectbyhist.h \
    hsvprocessor.h \
    common.h
