#-------------------------------------------------
# Project created by QtCreator
#-------------------------------------------------
TEMPLATE = app
TARGET   = CNCManualController
QT      += core gui
QT      += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += no_keyword

# QT += c++11
QMAKE_CXXFLAGS += -std=c++11

lessThan(QT_VERSION, 5.0) {
    warning("prefere to build it with Qt 5.0")
}

#-------------------------------------------------
# DEFINES
#-------------------------------------------------


#-------------------------------------------------
# DEPENDENCIES
#-------------------------------------------------
include($$PWD/../3rd/3rd.pri)


#-------------------------------------------------
# VERSION
#-------------------------------------------------
VERSION_FILENAME = $$PWD/../version

!exists( $${VERSION_FILENAME} ) {
    error( "Cannot find version file \"$${VERSION_FILENAME}\"" )
}

APP_VERSION = "$$cat($$VERSION_FILENAME)"
DEFINES += APP_VERSION=\\\"$$APP_VERSION\\\"


#-------------------------------------------------
# INCLUDE
#-------------------------------------------------
INCLUDEPATH += $$PWD/../include/


#-------------------------------------------------
# SOURCES
#-------------------------------------------------
include($$PWD/core/core.pri)
include($$PWD/gui/gui.pri)
include($$PWD/joystick/joystick.pri)
include($$PWD/velleman/velleman.pri)
include($$PWD/widgets/widgets.pri)

HEADERS += \
    $$PWD/about.h \
    $$PWD/builddefs.h \
    $$PWD/globals.h \
    $$PWD/mainwindow.h \
    $$PWD/version.h

SOURCES += \
    $$PWD/mainwindow.cpp \
    $$PWD/main.cpp

FORMS += \
    $$PWD/mainwindow.ui


#-------------------------------------------------
# RESOURCES
#-------------------------------------------------
RESOURCES += $$PWD/resources.qrc

win32|unix {
    RC_FILE += $$PWD/resources_win.rc
}


#-------------------------------------------------
# BUILD OPTIONS
#-------------------------------------------------

# Rem: On Ubuntu, directories starting with '.' are hidden by default
win32{
    MOC_DIR =      ./.moc
    OBJECTS_DIR =  ./.obj
    UI_DIR =       ./.ui
}else{
    MOC_DIR =      ./_moc
    OBJECTS_DIR =  ./_obj
    UI_DIR =       ./_ui
}

#-------------------------------------------------
# OUTPUT
#-------------------------------------------------
DESTDIR = $${OUT_PWD}/../../CNCMachineController


#-------------------------------------------------
# INSTALL
#-------------------------------------------------
# Specifies a list of files to be included in the dist target.
# This feature is supported by UnixMake specs only.
DISTFILES += \
    $$PWD/../LICENSE

# instructions for 'make install'
unix{
    target.path = /usr/lib
    INSTALLS += target
}

win32{
    # Copy DLLs to the target directory
    dlls.files += $$shell_quote($$shell_path("$$PWD/../3rd/sdl/SDL-1.2.15/README-SDL.txt"))
    dlls.files += $$shell_quote($$shell_path("$$PWD/../3rd/sdl/SDL-1.2.15/bin/x86/SDL.dll"))
    dlls.files += $$shell_quote($$shell_path("$$PWD/../3rd/velleman/bin/K8055D.dll"))
    dlls.path  =  $$shell_quote($$shell_path($${OUT_PWD}/$${DESTDIR}))
    INSTALLS += dlls
}
