//
// Created by Jino on 2021/07/14.
//

#pragma once


#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QtGlobal>
#include <QCloseEvent>

#include "chollian.h"

class GUI : public QLabel{

public:
    GUI() {
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setToolTip("Tray!");

        /* After that create a context menu of two items */
        QMenu * menu = new QMenu(this);
        QAction * viewWindow = new QAction("Update background", this);
        QAction * quitAction = new QAction("Quit", this);

        /* connect the signals clicks on menu items to by appropriate slots.
         * The first menu item expands the application from the tray,
         * And the second menu item terminates the application
         * */
        connect(viewWindow, SIGNAL(triggered()), this, SLOT(change_desktop_background()));
        connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

        menu->addAction(viewWindow);
        menu->addAction(quitAction);

        /* Set the context menu on the icon
         * And show the application icon in the system tray
         * */
        trayIcon->setContextMenu(menu);
        trayIcon->show();
        trayIcon->setIcon(QIcon("../icon.png"));
    };

    void closeEvent(QCloseEvent *event){
#ifdef Q_OS_MACOS
        if (!event->spontaneous() || !isVisible()) {
            return;
        }
#endif
        if (trayIcon->isVisible()) {
            hide();
            event->ignore();
        }
    }

private:
    QSystemTrayIcon *trayIcon;


};