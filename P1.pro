#-------------------------------------------------
#
# Project created by QtCreator 2019-04-10T16:01:15
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



unix {
LIBS += /usr/local/Cellar/assimp/4.1.0/lib/libassimp.4.dylib
INCLUDEPATH += /usr/local/Cellar/assimp/4.1.0/include
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myglwidget.cpp \
    model.cpp

HEADERS += \
        mainwindow.h \
    myglwidget.h \
    modelloader.h \
    model.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    shader.qrc

DISTFILES += \
    sample_texture.jpg

