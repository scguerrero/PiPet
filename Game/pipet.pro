TEMPLATE = app
TARGET = pipet
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += start_class/*.cc
SOURCES += game_class/*.cc
SOURCES += main.cc
HEADERS += game_class/*.h
HEADERS += start_class/*.h
