TARGET = QTGL

OBJECTS_DIR = obj

isEqual(QT_MAJOR_VERSION, 5){
  cache()
	DEFINES += QT5BUILD
}

CONFIG -= app_bundle
CONFIG += console

QT += opengl core

SOURCES += $$PWD/src/*.cpp

HEADERS += $$PWD/include/*.hpp

INCLUDEPATH += ./include

OTHER_FILES += shaders/*.glsl

include($(HOME)/NGL/UseNGL.pri)
