TEMPLATE = app
TARGET = "gzinjectGUI"
INCLUDEPATH += .

HEADERS += gzinjectGUI.h commandOutput.h gzguiwidget.h settingtype.h preset.h
SOURCES += main.cpp gzinjectGUI.cpp commandOutput.cpp gzguiwidget.cpp preset.cpp

QT += widgets

DESTDIR = build
