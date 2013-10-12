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
INCLUDEPATH = ./GeminiAssembler/include

SOURCES += GeminiAssembler/src/main.cpp\
    GeminiAssembler/src/geminiassemblerwindow.cpp \
    GeminiAssembler/src/geminiassemblermodel.cpp \
    GeminiAssembler/src/geminiassemblercontrol.cpp \
    GeminiAssembler/src/invalid_file_dialog.cpp \
    GeminiAssembler/src/gemini_parser.cpp \
    GeminiAssembler/src/compiler.cpp

HEADERS  += GeminiAssembler/include/geminiassemblerwindow.h \
    GeminiAssembler/include/geminiassemblermodel.h \
    GeminiAssembler/include/geminiassemblercontrol.h \
    GeminiAssembler/include/invalid_file_dialog.h \
    GeminiAssembler/include/gemini_parser.h \
    GeminiAssembler/include/gemini_types.h \
    GeminiAssembler/include/compiler.h

FORMS    += GeminiAssembler/ui/geminiassemblerwindow.ui \
    GeminiAssembler/ui/invalid_file_dialog.ui

OTHER_FILES += \
    gemini_asm_files/test5.s \
    gemini_asm_files/test4.s \
    gemini_asm_files/test3.s \
    gemini_asm_files/test2.s \
    gemini_asm_files/test1.s \
    gemini_asm_files/t3.s \
    gemini_asm_files/t2.s \
    gemini_asm_files/project2_test2.s \
    gemini_asm_files/project2_test1.s \
    README.md \
    LICENSE \
    gemini_asm_files/Test_1.s \
    gemini_asm_files/Test_1.c
