#=====================================================================#
#    This file is part of STOPT project.                              #
#                                                                     #
#    Copyright 2012-2013    by Dmitry Zagnoyko <hiroshidi@gmail.com>  #
#=====================================================================#

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl
QMAKE_CXXFLAGS += -std=gnu++0x -Wno-narrowing -Wno-reorder

TARGET = STOPT

LIBS += -lGLU

SOURCES += main.cpp\
    qmainwin.cpp \
    scene.cpp \
    shapes/shape.cpp \
    support/iodata.cpp \
    analitycs/analitycs3d.cpp \
    analitycs/analitycs2d.cpp \
    analitycs/analitycs.cpp \
    support/glrendersupport.cpp \
    shapes/gpcircle.cpp \
    shapes/oglsphere.cpp \
    shapes/oglbox.cpp \
    shapes/gpbox.cpp \
    winmain.cpp

HEADERS  += qmainwin.h \
    analitycs3d.h \
    scene.h \
    shapes/shape.h \
    support/stoptdef.h \
    support/iodata.h \
    analitycs/analitycs3d.h \
    analitycs/analitycs2d.h \
    analitycs/analitycs.h \
    support/glrendersupport.h \
    shapes/gpcircle.h \
    shapes/oglsphere.h \
    shapes/oglbox.h \
    shapes/gpbox.h \
    winmain.h

FORMS  += qmainwin.ui

RESOURCES += \
    stopt.qrc
