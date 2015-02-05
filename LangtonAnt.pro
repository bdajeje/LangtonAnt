#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T15:59:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LangtonAnt
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    render_area.cpp

HEADERS  += main_window.hpp \
    render_area.hpp

QMAKE_CXXFLAGS += -std=c++11
