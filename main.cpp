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

#include "main.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QUrl>
#include <QNetworkRequest>
#include <QDebug>
#include <QTextCodec>
#include <QTimer>

/**
 * Constructive constructor takes no parameters.
 */
StratumsphereTrayIcon::StratumsphereTrayIcon() : QObject(0), nam_(0),
  trayMenu_(0), trayIcon_(0), status_(UNDEFINED) {
  nam_ = new QNetworkAccessManager(this);
  connect(nam_, SIGNAL(finished(QNetworkReply*)), this,
    SLOT(reply(QNetworkReply*)));

  // set up icons
  int sizes[] = {16, 22, 32, 64, 128, 256};
  for(unsigned char size = 0, i = 0; i < sizeof(sizes); size = sizes[i], i++) {
    openIcon_.addFile(QString(":/res/open-%1.png").arg(size));
    closedIcon_.addFile(QString(":/res/closed-%1.png").arg(size));
    undefinedIcon_.addFile(QString(":/res/undefined-%1.png").arg(size));
  }

  // set up menu
  trayMenu_ = new QMenu;
  updateAction_ = new QAction(tr("&Update status"), trayMenu_);
  connect(updateAction_, SIGNAL(triggered()), this, SLOT(updateStatus()));
  trayMenu_->addAction(updateAction_);

  QAction * exitAction = new QAction(tr("&Exit"), trayMenu_);
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  trayMenu_->addAction(exitAction);

  // set up tray icon
  trayIcon_ = new QSystemTrayIcon(this);
  trayIcon_->setContextMenu(trayMenu_);
  trayIcon_->setIcon(undefinedIcon_);
  trayIcon_->show();

  // fire up a timer to poll the status every n minutes
  startTimer(updateInterval_ * 1000);

  updateStatus();
}

/**
 * Destructive destructor is destructive.
 */
StratumsphereTrayIcon::~StratumsphereTrayIcon() {
  if(trayMenu_) {
    delete trayMenu_;
  }
}

/**
 * Update the Stratum 0 opening status and the tray icon
 */
void StratumsphereTrayIcon::updateStatus() {
  qDebug() << QDateTime::currentDateTime().toString() << "updating status";
  updateAction_->setText(tr("Updating…"));
  updateAction_->setEnabled(false);
  nam_->get(QNetworkRequest(QUrl("http://rohieb.name/stratum0/status.txt")));

  // after timeout, just update icon to unspecified state
  status_ = UNDEFINED;
  QTimer::singleShot(timeoutInterval_ * 1000, this, SLOT(refresh()));
}

/**
 * Process network reply and update status settings
 */
void StratumsphereTrayIcon::reply(QNetworkReply* nr) {
  qDebug() << "got a reply!";

  QStringList lines = QString(nr->readAll()).split('\n');
  foreach(const QString& l, lines) {
    QString line = l.trimmed();
    qDebug() << "parsing line:" << line;
    if(line.startsWith("IsOpen:")) {
      QString boolPart = line.section(':', 1).trimmed();
      if(boolPart.toLower() == "true") {
        status_ = OPEN;
        qDebug() << "space is open";
      } else if(boolPart.toLower() == "false") {
        status_ = CLOSED;
        qDebug() << "space is open";
      } else {
        qDebug() << "Oops, I don't know how to interpret that line:" << line;
        status_ = UNDEFINED;
      }
      lastUpdate_ = QDateTime::currentDateTime();
      qDebug() << "status is" << status_;
    }
    if(line.startsWith("Since:")) {
      QDateTime dt = QDateTime::fromString(line.section(':', 1).trimmed(),
        Qt::ISODate);
      if(!dt.isValid()) {
        qDebug() << "Oops, I don't know how to interpret that line:" << line;
      } else {
        since_ = dt;
      }
    }
  }
  nr->deleteLater();
  refresh();
};

/**
 * Refresh the icon, the context menu and the tooltip text
 */
void StratumsphereTrayIcon::refresh() {
  qDebug() << "refreshing tray icon";
  updateAction_->setEnabled(true);
  updateAction_->setText(tr("&Update status"));

  QString statusText;
  QIcon * icon;
  if(status_ == StratumsphereTrayIcon::CLOSED) {
    icon = &closedIcon_;
    statusText = tr("Space is closed");
  } else if(status_ == StratumsphereTrayIcon::OPEN) {
    icon = &openIcon_;
    statusText = tr("Space is open");
  } else {
    icon = &undefinedIcon_;
    statusText = tr("Could not determine opening status");
  }
  trayIcon_->setIcon(*icon);
  qApp->setWindowIcon(*icon);

  statusText.append("\n");
  statusText.append(tr("Open since: %1").
    arg(since_.toString(Qt::DefaultLocaleShortDate)));
  statusText.append("\n");
  statusText.append(tr("Last update: %1").
    arg(lastUpdate_.toString(Qt::DefaultLocaleShortDate)));
  trayIcon_->setToolTip(statusText);
}

/******************************************************************************/
/* main function                                                              */
/******************************************************************************/

int main(int argc, char * argv[]) {
  QApplication app(argc, argv);
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  StratumsphereTrayIcon * sti = new StratumsphereTrayIcon;

  int ret = app.exec();
  
  delete sti;
  return ret;
}

// vim: set tw=80 et sw=2 ts=2:
