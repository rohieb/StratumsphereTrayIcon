TEMPLATE = app
CONFIG += debug_and_release ordered
TARGET = s0trayicon
QT += network core gui
SOURCES = main.cpp
HEADERS += main.h
RESOURCES = resources.qrc

unix {
  PKG = $$system(pkg-config --libs QtDBus)
  contains(PKG, -lQtDBus) {
    message( \
      "QtDBus found, configuring with NetworkManager and Notification support")
    QT += dbus
    DEFINES += HAVE_DBUS
    SOURCES += freedesktop-notification.cpp
    HEADERS += freedesktop-notification.h
  }
  !contains(PKG, -lQtDBus) {
    message( \
      "Configuring without D-Bus. No Notification and NetworkManager support.")
  }
}

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
