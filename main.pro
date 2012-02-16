TEMPLATE = app
CONFIG += debug_and_release \
  ordered \
  silent
TARGET = s0trayicon
QT += network
QT += core
QT += gui
SOURCES = main.cpp
HEADERS += main.h
RESOURCES = resources.qrc
win32 {
  RC_FILE = res/resources.rc
}

macx {
  ICON = res/s0trayicon.icns
}
macx:universal {
  CONFIG += x86 ppc
}
macx:noicon {
  QMAKE_INFO_PLIST = res/Info-noicon.plist
}
macx:!noicon {
  QMAKE_INFO_PLIST = res/Info-icon.plist
}
