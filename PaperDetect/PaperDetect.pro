QT += core
QT -= gui

TARGET = PaperDetect
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab



SOURCES += main.cpp \
    videocontroller.cpp \
    paperdectector.cpp

HEADERS += \
    videocontroller.h \
    paperdectector.h \
    utils.h

