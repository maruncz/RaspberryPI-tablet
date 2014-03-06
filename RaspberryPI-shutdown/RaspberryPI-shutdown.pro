TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += /home/martin/rpi/rasp-pi-rootfs/usr/local/include
LIBS += -L/home/martin/rpi/rasp-pi-rootfs/usr/local/lib -lwiringPi
target.path=/home/pi/c/qt/raspberrypi
INSTALLS += target
