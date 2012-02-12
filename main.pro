TEMPLATE = app
CONFIG += debug_and_release \
  ordered
QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
TARGET = s0trayicon
QT += network
QT += core
QT += gui
SOURCES = main.cpp
HEADERS += main.h
RESOURCES = resources.qrc

