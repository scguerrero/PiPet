TEMPLATE = app
TARGET = pipet
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    CareWidget.cpp \
    CreateWidget.cpp \
    ModeWidget.cpp \
    TrainWidget.cpp \
    main.cpp \
    window.cpp

HEADERS += \
    CareWidget.h \
    CreateWidget.h \
    ModeWidget.h \
    TrainWidget.h \
    window.h

DISTFILES += \
    to_do
