#-------------------------------------------------
#
# Project created by QtCreator 2011-08-26T11:01:04
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/../../src/common
DEPENDPATH += $$PWD/../../src/common

SOURCES += tst_testtest.cpp \
    ../../src/common/audiobuffer.cpp \
    ../../src/common/circularbuffer.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../src/common/audiobuffer.h \
    ../../src/common/circularbuffer.h \
    precomp.h

PRECOMPILED_HEADER = precomp.h
