/**
 * @file main.cpp Main application code
 * A tray icon which shows the current opening status of the Stratumsphere
 * @date 2012-05-07
 * @author Roland Hieber <rohieb@rohieb.name>
 *
 * Copyright (C) 2012 Roland Hieber <rohieb@rohieb.name>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QDateTime>

class StratumsphereTrayIcon : public QObject {
  Q_OBJECT
  QNetworkAccessManager * nam_;
  QMenu * trayMenu_;
  QAction * updateAction_;
  QSystemTrayIcon * trayIcon_;

  QIcon openIcon_;
  QIcon closedIcon_;
  QIcon undefinedIcon_;

  QDateTime lastUpdate_;

  /** update interval in seconds */
  const static int updateInterval_ = 5*60;
  /** timeout interval in seconds */
  const static int timeoutInterval_ = 60;

  // inherited from QObject
  inline virtual void timerEvent(QTimerEvent *) {
    updateStatus();
  }

public:
  enum Status { UNDEFINED = 0, OPEN, CLOSED };
  Status status_;
  QDateTime since_;
  
  StratumsphereTrayIcon();
  virtual ~StratumsphereTrayIcon();

public slots:
  void updateStatus();
protected slots:
  void reply(QNetworkReply*);
  void refresh();
};

