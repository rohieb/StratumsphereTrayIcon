#include <QObject>

class DBusWatcher : public QObject {
  Q_OBJECT
public slots:
  void watch(uint state);
};
