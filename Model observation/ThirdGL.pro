#-------------------------------------------------
#
# Project created by QtCreator 2017-10-18T19:00:56
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThirdGL
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    oglwidget.cpp

HEADERS += mainwindow.h \
    oglwidget.h

FORMS   += mainwindow.ui

win32:LIBS += -lOpengl32 \
              -lglu32 \
              -lglut
