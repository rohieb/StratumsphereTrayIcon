#ifndef _DBUS_H_
#define _DBUS_H_

#include <QImage>
#include <QtDBus>

namespace StratumsphereTrayIcon {

void showNotification(const QString summary, const QString text,
  const QImage image);

} // namespace

QDBusArgument& operator<<(QDBusArgument& arg, const QImage& image);
const QDBusArgument& operator>>(const QDBusArgument& arg, QImage& image);

#endif // _DBUS_H_
// vim: set tw=80 et sw=2 ts=2:
