QT += core
QT -= gui
QT += multimedia

TARGET = LedDetection
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab

TEMPLATE = app

SOURCES += main.cpp \
    leddetector.cpp \
    threadvideocap.cpp

HEADERS += \
    leddetector.h \
    threadvideocap.h \
    controller.h

