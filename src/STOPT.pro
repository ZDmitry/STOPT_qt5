#=====================================================================#
#    This file is part of STOPT project.                              #
#                                                                     #
#    Copyright 2012      by Dmitry Zagnoyko <hiroshidi@gmail.com>     #
#=====================================================================#

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=gnu++0x -Wno-narrowing

TARGET = STOPT

SOURCES += main.cpp\
    qmainwin.cpp \
    scene.cpp \
    shapes/shape_derived.cpp \
    shapes/shape.cpp \
    shapes/ogl_shape.cpp \
    support/iodata.cpp \
    analitycs/analitycs3d.cpp \
    analitycs/analitycs2d.cpp \
    analitycs/analitycs.cpp

HEADERS  += qmainwin.h \
    analitycs3d.h \
    scene.h \
    shapes/shape_derived.h \
    shapes/shape.h \
    shapes/ogl_shape.h \
    support/stoptdef.h \
    support/iodata.h \
    analitycs/analitycs3d.h \
    analitycs/analitycs2d.h \
    analitycs/analitycs.h

FORMS  += qmainwin.ui
