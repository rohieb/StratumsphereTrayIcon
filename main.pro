TEMPLATE = app
CONFIG += debug_and_release \
  ordered \
  silent
TARGET = s0trayicon
QT = core \
  network \
  gui
SOURCES = main.cpp
HEADERS += main.h
RESOURCES = resources.qrc
win32 {
  RC_FILE = s0trayicon.rc
}

macx:universal {
  CONFIG += x86 ppc
}
macx:noicon {
  QMAKE_INFO_PLIST = Info-noicon.plist
}
macx:!noicon {
  QMAKE_INFO_PLIST = Info-icon.plist
}
