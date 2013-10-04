#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T19:44:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeminiAssembler
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        geminiassemblerwindow.cpp \
    geminiassemblermodel.cpp \
    geminiassemblercontrol.cpp \
    invalid_file_dialog.cpp

HEADERS  += geminiassemblerwindow.h \
    geminiassemblermodel.h \
    geminiassemblercontrol.h \
    invalid_file_dialog.h

FORMS    += geminiassemblerwindow.ui \
    invalid_file_dialog.ui
