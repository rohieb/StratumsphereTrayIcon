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

StratumsphereTrayIcon::StratumsphereTrayIcon() : QObject(0), nam_(0),
  trayMenu_(0), trayIcon_(0), status_(UNDEFINED) {
  nam_ = new QNetworkAccessManager(this);
  connect(nam_, SIGNAL(finished(QNetworkReply*)), this,
    SLOT(reply(QNetworkReply*)));

  // set up icons
  char sizes[] = {16, 22, 32};
  for(unsigned char size = 0, i = 0; i < sizeof(sizes); size = sizes[i], i++) {
    openIcon_.addFile(QString(":/res/open-%1.png").arg(size));
    closedIcon_.addFile(QString(":/res/closed-%1.png").arg(size));
    undefinedIcon_.addFile(QString(":/res/undefined-%1.png").arg(size));
  }

  // set up menu
  trayMenu_ = new QMenu;
  statusAction_ = new QAction(trayMenu_);
  statusAction_->setEnabled(false);
  QAction * exitAction = new QAction(tr("&Exit"), trayMenu_);
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  trayMenu_->addAction(statusAction_);
  trayMenu_->addAction(exitAction);

  // set up tray icon
  trayIcon_ = new QSystemTrayIcon(this);
  trayIcon_->setContextMenu(trayMenu_);
  trayIcon_->setIcon(undefinedIcon_);
  trayIcon_->show();

  qDebug() << "looking for status";
  statusAction_->setText(tr("Updating…"));
  nam_->get(QNetworkRequest(QUrl("http://rohieb.name/stratum0/status.txt")));
}

StratumsphereTrayIcon::~StratumsphereTrayIcon() {
  if(trayMenu_) {
    delete trayMenu_;
  }
}

void StratumsphereTrayIcon::reply(QNetworkReply* nr) {
  qDebug() << "got a reply!";
  QStringList lines = QString(nr->readAll()).split('\n');
  foreach(const QString& l, lines) {
    QString line = l.trimmed();
    qDebug() << "got line: " << line;
    if(line.startsWith("IsOpen:")) {
      qDebug() << "found line: " << line;
      QString boolPart = line.section(':', 1).trimmed();
      if(boolPart.toLower() == "true") {
        status_ = OPEN;
      } else if(boolPart.toLower() == "false") {
        status_ = CLOSED;
      } else {
        qDebug() << "Oops, I don't know how to interpret that line: " << line;
        status_ = UNDEFINED;
      }
      qDebug() << "status is " << status_;
    } else {
      qDebug() << "discard line";
    }
  }
  nr->deleteLater();

  // set up and show the system tray icon
  if(status_ == StratumsphereTrayIcon::CLOSED) {
    trayIcon_->setIcon(closedIcon_);
    statusAction_->setText(tr("Space is closed"));
  } else if(status_ == StratumsphereTrayIcon::OPEN) {
    trayIcon_->setIcon(openIcon_);
    statusAction_->setText(tr("Space is open"));
  } else {
    trayIcon_->setIcon(undefinedIcon_);
    statusAction_->setText(tr("Could not determine space status"));
  }
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
