QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../bin/Kinect2Images
TEMPLATE = app

DEFINES += NOMINMAX

INCLUDEPATH += $$PWD/include

DEPENDPATH += $$PWD/lib

PRE_TARGETDEPS += $$PWD/lib/x64/Kinect20.lib
LIBS += -L$$PWD/lib/x64/ -lKinect20 \

SOURCES += main.cpp\
        Kinect2Images.cpp \
        Kinect2_Thread.cpp \
    viz3d_dem.cpp

HEADERS  += Kinect2Images.h \
            Kinect2_Thread.h

FORMS    += Kinect2Images.ui

#================================
#Including VIZ libraries

#TEMPLATE = app
#CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt

include (opencv32_contrib_vc12.pri)


