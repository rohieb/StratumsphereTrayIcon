//#include "dbus.h"
#include <QtDBus>
#include <QCoreApplication>
#include <QImage>

int main(int argc, char ** argv) {

  QCoreApplication app(argc, argv);
 
  QList<QVariant> argumentList;
  argumentList << "AppName"; //app_name
  argumentList << (uint)0;  // replace_id
  argumentList << "";  // app_icon
  argumentList << "Summary"; // summary
  argumentList << "Body Text."; // body
  argumentList << QStringList();  // actions
//  argumentList << QList<QMap<QString,QDBusVariant> >();  // hints
//  argumentList << QStringList();  // hints
//  argumentList << hints;  // hints
  argumentList << QVariantMap();  // hints
  argumentList << (int)1000; // timeout

  QDBusInterface notifyApp("org.freedesktop.Notifications", 
    "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
  QDBusReply<int> reply = notifyApp.callWithArgumentList(QDBus::AutoDetect,
    "Notify", argumentList);
//  QDBusReply<int> reply = notifyApp.call(QDBus::AutoDetect, "Notify",
//    "AppName", 0, "", "summ", "bdy", QStringList(), 
//    QList<QMap<QString,QDBusVariant> >(), 10000);
  qDebug() << "Reply:" << reply;
  qDebug() << "Error:" << notifyApp.lastError();

  app.processEvents();
  return 0;
}

// vim: set tw=80 et sw=2 ts=2:
