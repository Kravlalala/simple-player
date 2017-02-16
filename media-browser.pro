#-------------------------------------------------
#
# Project created by QtCreator 2017-02-14T22:40:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videobin.cpp

HEADERS  += mainwindow.h \
    videobin.h

FORMS    += mainwindow.ui

target.path += /home/root/
INSTALLS += target

INCLUDEPATH += /home/ilya/Desktop/sitara/sdk/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/filesystem/usr/include/gstreamer-1.0/ \
               /home/ilya/Desktop/sitara/sdk/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/filesystem/usr/include/glib-2.0/ \
               /home/ilya/Desktop/sitara/sdk/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/filesystem/usr/lib/glib-2.0/include

LIBS += -L"/home/ilya/Desktop/sitara/sdk/ti-processor-sdk-linux-am57xx-evm-03.01.00.06/filesystem/usr/lib" -lgstreamer-1.0 -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -lpcre -lffi
