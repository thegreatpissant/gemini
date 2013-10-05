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
INCLUDEPATH = ./include

SOURCES += src/main.cpp\
        src/geminiassemblerwindow.cpp \
    src/geminiassemblermodel.cpp \
    src/geminiassemblercontrol.cpp \
    src/invalid_file_dialog.cpp \
    src/validator.cpp \
    src/gemini_parser.cpp

HEADERS  += include/geminiassemblerwindow.h \
    include/geminiassemblermodel.h \
    include/geminiassemblercontrol.h \
    include/invalid_file_dialog.h \
    include/validator.h \
    include/gemini_parser.h \
    include/gemini_types.h

FORMS    += ui/geminiassemblerwindow.ui \
    ui/invalid_file_dialog.ui
