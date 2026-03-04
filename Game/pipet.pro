TEMPLATE = app
TARGET = pipet
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
	start_class/*.cc \
	game_class/*.cc \
	main.cc

HEADERS += \
	game_class/*.h \
	start_class/*.h 
