QT += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += "O:\glew-2.1.0\include"

LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    main.cpp \
    myglwidget.cpp \
    Lightspot.cpp \
    NetworkManager.cpp \
    Particles.cpp

HEADERS += \
    myglwidget.h \
    Lightspot.h \
    NetworkManager.h \
    Particles.h