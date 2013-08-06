#-------------------------------------------------
#
# Project created by QtCreator 2013-07-31T15:28:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CovertAVtoMJPEG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    itemoflistfiles.cpp \
    systemsettings.cpp \
    toolboxsettings.cpp \
    previewplay.cpp

HEADERS  += mainwindow.h \
    systemsettings.h \
    itemoflistfiles.h \
    toolboxsettings.h \
    previewplay.h \
    global.h

FORMS    += mainwindow.ui

RESOURCES += \
    picture.qrc
