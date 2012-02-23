#ifndef _DBUS_H_
#define _DBUS_H_

#ifdef HAVE_DBUS
#include <QImage>
#include <QtDBus>

bool showNotification(const QString summary, const QString text,
  const QImage image);

QDBusArgument& operator<<(QDBusArgument& arg, const QImage& image);
const QDBusArgument& operator>>(const QDBusArgument& arg, QImage&);

#endif // def HAVE_DBUS
#endif // _DBUS_H_
// vim: set tw=80 et sw=2 ts=2:
