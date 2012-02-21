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

#include "dbus.h"
#include "globals.h"
#include <QtDBus>
#include <QCoreApplication>
#include <QImage>

using namespace StratumsphereTrayIcon;

/**
 * main
 */
int main(int argc, char ** argv) {

  QCoreApplication app(argc, argv);
  showNotification("Space is now open", "The Stratumsphere has just opened!",
    QImage(":res/open-128.png"));
  app.processEvents();
  showNotification("Space is now closed", "The Stratumsphere has just closed.",
    QImage(":res/closed-128.png"));
  app.processEvents();
  return 0;
}

/**
 * Show a notification using the org.freedesktop.Notification service
 * @param summary Summary, should be a short string to summarize the
 *  notification
 * @param text The text of the notification
 * @param image Additional image to show among the notification
 */
void StratumsphereTrayIcon::showNotification(const QString summary, 
  const QString text, const QImage image = QImage()) {

  qDBusRegisterMetaType<QImage>();

  QVariantMap hints;
  hints["image_data"] = image;
  QList<QVariant> argumentList;
  argumentList << APPNAME; //app_name
  argumentList << (uint)0;  // replace_id
  argumentList << "";  // app_icon
  argumentList << summary; // summary
  argumentList << text; // body
  argumentList << QStringList();  // actions
  argumentList << hints;  // hints
  argumentList << (int)5000; // timeout in ms

  static QDBusInterface notifyApp("org.freedesktop.Notifications",
    "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
  QDBusReply<int> reply = notifyApp.callWithArgumentList(QDBus::AutoDetect,
    "Notify", argumentList);
  qDebug() << "Reply:" << reply;
  qDebug() << "Error:" << notifyApp.lastError();
}

/**
 * Automatic marshaling of a QImage for org.freedesktop.Notifications.Notify
 *
 * This function is from the Clementine project (see
 * http://www.clementine-player.org) and licensed under the GNU General Public
 * License, version 3 or later.
 *
 * Copyright 2010, David Sansome <me@davidsansome.com>
 */
QDBusArgument& operator<<(QDBusArgument& arg, const QImage& image) {
  if(image.isNull()) {
    arg.beginStructure();
    arg << 0 << 0 << 0 << false << 0 << 0 << QByteArray();
    arg.endStructure();
    return arg;
  }

  QImage scaled = image.scaledToHeight(100, Qt::SmoothTransformation);
  scaled = scaled.convertToFormat(QImage::Format_ARGB32);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
  // ABGR -> ARGB
  QImage i = scaled.rgbSwapped();
#else
  // ABGR -> GBAR
  QImage i(scaled.size(), scaled.format());
  for (int y = 0; y < i.height(); ++y) {
    QRgb* p = (QRgb*) scaled.scanLine(y);
    QRgb* q = (QRgb*) i.scanLine(y);
    QRgb* end = p + scaled.width();
    while (p < end) {
      *q = qRgba(qGreen(*p), qBlue(*p), qAlpha(*p), qRed(*p));
      p++;
      q++;
    }
  }
#endif

  arg.beginStructure();
  arg << i.width();
  arg << i.height();
  arg << i.bytesPerLine();
  arg << i.hasAlphaChannel();
  int channels = i.isGrayscale() ? 1 : (i.hasAlphaChannel() ? 4 : 3);
  arg << i.depth() / channels;
  arg << channels;
  arg << QByteArray(reinterpret_cast<const char*>(i.bits()), i.numBytes());
  arg.endStructure();
  return arg;
}

const QDBusArgument& operator>>(const QDBusArgument& arg, QImage& image) {
  // This is needed to link but shouldn't be called.
  Q_ASSERT(0);
  return arg;
}
// vim: set tw=80 et sw=2 ts=2:
