#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T20:54:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RaspberryPI
TEMPLATE = app
target.path = /home/pi/c/qt/raspberrypi
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    RaspberryPI.qrc

INCLUDEPATH += /home/martin/rpi/rasp-pi-rootfs/usr/local/include
LIBS += -L/home/martin/rpi/rasp-pi-rootfs/usr/local/lib -lwiringPi
