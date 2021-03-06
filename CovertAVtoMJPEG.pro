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
    previewplay.cpp \
    myslider.cpp \
    timeslider.cpp \
    mylistwidget.cpp \
    myframe.cpp

HEADERS  += mainwindow.h \
    systemsettings.h \
    itemoflistfiles.h \
    toolboxsettings.h \
    previewplay.h \
    global.h \
    myslider.h \
    timeslider.h \
    mylistwidget.h \
    myframe.h

FORMS    += mainwindow.ui
RC_FILE += icon.rc
RESOURCES += \
    picture.qrc

OTHER_FILES += \
    icon.rc
