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
#include <QtGui/QActionGroup>

#include "chollian.h"
#include "image.h"

class GUI : public QLabel{

    Q_OBJECT

public:
    GUI() : m_color(Color::True), m_imgType(ImageType::FullDome){
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setToolTip("Tray!");

        /* After that create a context menu of two items */
        QMenu *menu = new QMenu(this);
        QAction *update_wallpaper = new QAction("Update background", this);
        QAction *color_rgb_true = new QAction("RGB True");
        QAction *color_natural = new QAction("Natural");

        menu->addAction(update_wallpaper);
        menu->addSection("Colors");
        menu->addAction(color_rgb_true);
        menu->addAction(color_natural);

        QActionGroup *set_color_group = new QActionGroup(this);
        set_color_group->setExclusive(true);
        set_color_group->addAction(color_rgb_true);
        set_color_group->addAction(color_natural);
        color_rgb_true->setCheckable(true);
        color_rgb_true->setChecked(true);
        color_natural->setCheckable(true);

        connect(update_wallpaper, SIGNAL(triggered()), this, SLOT(change_wallpaper()));
        connect(color_rgb_true, SIGNAL(triggered()), this, SLOT(set_color_rgb_true()));
        connect(color_natural, SIGNAL(triggered()), this, SLOT(set_color_natural()));
        
        trayIcon->setContextMenu(menu);
        trayIcon->show();
        trayIcon->setIcon(QIcon("../icon.png"));
    };

public slots:
    void change_wallpaper(){
        std::cout<<"change wallpaper!\n";
        boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
        auto adjusted_time = adjust_target_time(utcTime);
        std::string url = url_generator_chollian(m_imgType, m_color, adjusted_time);
        std::string img_binary = image_downloader(url);
        Image img = Image(img_binary);
        img.to_any_resolution(2880, 1800, 100);
        img.set_as_wallpaper();
        std::cout<<"done!\n";
    };

    void set_color_rgb_true(){
        m_color = Color::True;
    }

    void set_color_natural(){
        m_color = Color::Natural;
    }



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
    ImageType m_imgType;
    Color m_color;
};