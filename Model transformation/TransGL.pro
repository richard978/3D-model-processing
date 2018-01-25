#-------------------------------------------------
#
# Project created by QtCreator 2017-10-29T14:03:47
#
#-------------------------------------------------

QT       += core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TransGL
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    oglwidget.cpp

HEADERS += \
        mainwindow.h \
    oglwidget.h

FORMS += \
        mainwindow.ui

win32:LIBS += -lOpengl32 \
              -lglu32 \
              -lglut
