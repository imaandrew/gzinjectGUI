TEMPLATE = app
TARGET = "gzinjectGUI"
INCLUDEPATH += .

HEADERS += gzinjectGUI.h commandOutput.h gzguiwidget.h settingtype.h preset.h
SOURCES += main.cpp gzinjectGUI.cpp commandOutput.cpp gzguiwidget.cpp preset.cpp

QT += widgets

DESTDIR = build

unix {
    DEFINES += "GIT_VERSION='$(shell git describe --always)'"
    QMAKE_CFLAGS += -g
    QMAKE_CXXFLAGS += -g
}

win32 {
    # https://stackoverflow.com/a/24010395
	DEFINES += GIT_VERSION=$$system(git describe --always)
}