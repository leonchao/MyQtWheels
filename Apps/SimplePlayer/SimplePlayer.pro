#-------------------------------------------------
#
# Project created by QtCreator 2020-09-11T10:18:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimplePlayer
TEMPLATE = app
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../../bin/debug
} else {
    DESTDIR = $$PWD/../../bin/release
}
win32 {
contains(QT_ARCH,x86_64){
    INCLUDEPATH += $$PWD/../../ffmpeg/x64/include

    LIBS += -L$$$$PWD/../../ffmpeg/x64/lib/ -lavcodec \
            -L$$$$PWD/../../ffmpeg/x64/lib/ -lswscale \
            -L$$$$PWD/../../ffmpeg/x64/lib/ -lavutil \
            -L$$$$PWD/../../ffmpeg/x64/lib/ -lavformat
}else{
    INCLUDEPATH += $$PWD/../../ffmpeg/x86/include

    LIBS += -L$$PWD/../../ffmpeg/x86/lib/ -lavcodec \
            -L$$PWD/../../ffmpeg/x86/lib/ -lswscale \
            -L$$PWD/../../ffmpeg/x86/lib/ -lavutil \
            -L$$PWD/../../ffmpeg/x86/lib/ -lavformat
}
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwidget.cpp \
    decodethread.cpp

HEADERS += \
        mainwidget.h \
    decodethread.h

FORMS += \
        mainwidget.ui
