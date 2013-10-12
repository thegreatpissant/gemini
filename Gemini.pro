#-------------------------------------------------
#
# Project created by QtCreator 2013-09-14T18:53:49
#
# James Feister
# cisc360 Gemini Project
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gemini
TEMPLATE = app
CONFIG += c++11
INCLUDEPATH = ./Gemini/include

SOURCES += \
    Gemini/src/memory.cpp \
    Gemini/src/main.cpp \
    Gemini/src/gemini.cpp \
    Gemini/src/gemini_types.cpp \
    Gemini/src/gemini_system.cpp \
    Gemini/src/cpu.cpp


HEADERS  += \
    Gemini/include/memory.h \
    Gemini/include/gemini.h \
    Gemini/include/gemini_types.h \
    Gemini/include/gemini_system.h \
    Gemini/include/cpu.h
FORMS    += \
    Gemini/ui/gemini.ui

OTHER_FILES += \
    README.md \
    LICENSE \
    doc/test4.s \
    doc/test3.s \
    doc/test2.s \
    doc/test1.s \
    doc/WindowsFormsApplication1.zip \
    doc/project1.html \
    doc/image30.gif \
    doc/image17.gif \
    doc/image16.gif \
    doc/image13.gif \
    doc/image12.gif \
    doc/image9.gif \
    doc/image8.gif \
    doc/image6.gif \
    doc/image4.gif \
    doc/test5.c \
    doc/project1.cpp \
    doc/project2.html \
    gemini_asm_files/t3.s \
    gemini_asm_files/test1.s \
    gemini_asm_files/test2.s \
    gemini_asm_files/test3.s \
    gemini_asm_files/test4.s \
    gemini_asm_files/test5.s \
    gemini_asm_files/t2.s \
    gemini_asm_files/project2_test2.s \
    gemini_asm_files/project2_test1.s \
    gemini_asm_files/Test_1.s \
    gemini_asm_files/Test_1.c
