#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T13:24:28
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NewGoBang
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tool.cpp \
    ai.cpp

HEADERS  += mainwindow.h \
    tool.h \
    ai.h \
    constants.h

FORMS    +=

OTHER_FILES +=

RESOURCES += \
    src.qrc
