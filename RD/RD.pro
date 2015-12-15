TEMPLATE = app

QT += qml quick widgets sql core

TARGET      = RepoDest
TEMPLATE    = app

CONFIG      += c++11\
               static
QTPLUGIN    += qsqloci

SOURCES     +=  main.cpp\
                core/snap7.cpp \
                workarea.cpp \
                threadmanager.cpp \
                sqlhandler.cpp \
                plchandler.cpp \
    clientwindow.cpp

LIBS        += -L"$(MINGW)/lib" "C:\lib\snap7.lib"

HEADERS     +=  \
                core/snap7.h \
                workarea.h \
                threadmanager.h \
                sqlhandler.h \
                plchandler.h \
    clientwindow.h

DISTFILES   +=  core/snap7.lib \
                core/snap7.dll

RC_FILE     =   RepoDest.rc


RESOURCES += qml.qrc \
    img.qrc
ico.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

