#-------------------------------------------------
#
# Project created by QtCreator 2014-05-02T21:22:40
#
#-------------------------------------------------

QT       += core gui opengl

LIBS     += -L$$PWD/glew-1.9.0/lib/ -lGLEW -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModelingTool
TEMPLATE = app


SOURCES += main.cpp\
        view.cpp \
    controller.cpp \
    viewport.cpp \
    camera.cpp \
    model.cpp \
    mousecontroller.cpp \
    primitive.cpp \
    cube.cpp \
    torus.cpp \
    sphere.cpp \
    cone.cpp \
    cylinder.cpp \
    grid.cpp \
    scenegraph.cpp

HEADERS  += view.h \
    controller.h \
    viewport.h \
    camera.h \
    model.h \
    mousecontroller.h \
    primitive.h \
    cube.h \
    torus.h \
    sphere.h \
    cone.h \
    cylinder.h \
    grid.h \
    scenegraph.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    shaders/phongVertexShader.vertexShader \
    shaders/phongFragmentShader.fragmentShader \
    shaders/selectionVertexShader.vertexShader \
    shaders/selectionFragmentShader.fragmentShader

INCLUDEPATH += $$PWD/glew-1.9.0/include
DEPENDPATH += $$PWD/glew-1.9.0/include
