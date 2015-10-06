#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T15:46:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Astra
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    com_port_main.cpp \
    com_port1.cpp

HEADERS  += mainwindow.h \
    com_port_main.h \
    com_port1.h

FORMS    += mainwindow.ui \
    com_port1.ui

RESOURCES += \
    icons.qrc
