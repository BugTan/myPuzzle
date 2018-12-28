#-------------------------------------------------
#
# Project created by QtCreator 2017-07-13T00:16:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = puzzle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    A_star.cpp

HEADERS  += mainwindow.h \
    A_star.h

FORMS    += mainwindow.ui

RESOURCES += \
    drawable.qrc
