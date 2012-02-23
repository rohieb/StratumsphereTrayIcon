/**
 * @file dbus.cpp Routines to handle D-Bus communication
 * @date 2012-05-07
 * @author Roland Hieber <rohieb@rohieb.name>
 *
 * Copyright (C) 2012 Roland Hieber <rohieb@rohieb.name>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "nm-test.h"
#include <QtDBus>
#include <QCoreApplication>
#include <QDebug>

void DBusWatcher::watch(uint state) {
  QString stateRepr;
  switch(state) {
    case 0: stateRepr = "NM_STATE_UNKNOWN"; break;
    case 1: stateRepr = "NM_STATE_ASLEEP"; break;
    case 2: stateRepr = "NM_STATE_CONNECTING"; break;
    case 3: stateRepr = "NM_STATE_CONNECTED"; break;
    case 4: stateRepr = "NM_STATE_DISCONNECTED"; break;
    default:
      stateRepr = "an undefined state o_O (this should not happen!)";
      break;
  }
  qDebug() << "State changed to:" << stateRepr;
}

/**
 * main
 */
int main(int argc, char ** argv) {

  QCoreApplication app(argc, argv);
  DBusWatcher * watcher = new DBusWatcher;
  QDBusConnection conn = QDBusConnection::connectToBus(
    QDBusConnection::SystemBus, "org.freedesktop.NetworkManager");
  if(!conn.isConnected()) {
    qDebug() << "Oh. Connection failed:" << conn.lastError();
    return -1;
  }
  conn.connect("org.freedesktop.NetworkManager",
    "/org/freedesktop/NetworkManager", "org.freedesktop.NetworkManager",
    "StateChanged", "u", watcher, SLOT(watch(uint)));

  return app.exec();
}
// vim: set tw=80 et sw=2 ts=2:
