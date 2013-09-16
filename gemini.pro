#-------------------------------------------------
#
# Project created by QtCreator 2013-09-14T18:53:49
#
# James Feister
# cisc360 Gemini Project
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gemini
TEMPLATE = app


SOURCES += main.cpp\
        gemini.cpp \
    cpu.cpp \
    ipr.cpp \
    memory.cpp


HEADERS  += gemini.h \
    cpu.h \
    ipr.h \
    memory.h

FORMS    += gemini.ui

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
    doc/project1.cpp
