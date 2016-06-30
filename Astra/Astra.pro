#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T15:46:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Astra
TEMPLATE = app
QT+= serialport
CONFIG +=C++11


SOURCES += main.cpp\
        mainwindow.cpp \
    com_port_main.cpp \
    com_port1.cpp \
    comport.cpp \
    proccommand.cpp \
    getstatus.cpp \
    readflash.cpp \
    writeflash.cpp \
    reset.cpp

HEADERS  += mainwindow.h \
    com_port_main.h \
    com_port1.h \
    comport.h \
    proccommand.h \
    operation.h \
    getstatus.h \
    readflash.h \
    writeflash.h \
    reset.h \
    frameinfo.h

FORMS    += mainwindow.ui \
    com_port1.ui

RESOURCES += \
    icons.qrc
RC_FILE = myapp.rc


DISTFILES +=

