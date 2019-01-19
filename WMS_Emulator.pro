#-------------------------------------------------
#
# Project created by QtCreator 2018-12-06T17:24:40
#
#-------------------------------------------------

QT       += core gui sql network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WMS_Emulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    idgenerator.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    idgenerator.h \
    dialog.h

CONFIG += static

FORMS    += mainwindow.ui \
    dialog.ui
