#-------------------------------------------------
#
# Project created by QtCreator 2014-05-02T21:22:40
#
#-------------------------------------------------

QT       += core gui opengl

LIBS     += -lGLU -L$$PWD/glew-1.9.0/lib/ -lGLEW

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModelingTool
TEMPLATE = app


SOURCES += main.cpp\
        view.cpp \
    controller.cpp \
    viewport.cpp \
    camera.cpp \
    model.cpp \
    mousecontroller.cpp

HEADERS  += view.h \
    controller.h \
    viewport.h \
    camera.h \
    model.h \
    mousecontroller.h

RESOURCES += \
    resources.qrc


INCLUDEPATH += $$PWD/glew-1.9.0/include
DEPENDPATH += $$PWD/glew-1.9.0/include
