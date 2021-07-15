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
        QAction *update_wallpaper_action = new QAction("Update background", this);
        QAction *color_rgb_true_action = new QAction("RGB True");
        QAction *color_natural_action = new QAction("Natural");
        QAction *quit_action = new QAction("Quit");

        menu->addAction(update_wallpaper_action);
        menu->addSection("Colors");
        menu->addAction(color_rgb_true_action);
        menu->addAction(color_natural_action);
        menu->addSeparator();
        menu->addAction(quit_action);

        QActionGroup *set_color_group = new QActionGroup(this);
        set_color_group->setExclusive(true);
        set_color_group->addAction(color_rgb_true_action);
        set_color_group->addAction(color_natural_action);
        color_rgb_true_action->setCheckable(true);
        color_rgb_true_action->setChecked(true);
        color_natural_action->setCheckable(true);

        connect(update_wallpaper_action, SIGNAL(triggered()), this, SLOT(change_wallpaper_slot()));
        connect(color_rgb_true_action, SIGNAL(triggered()), this, SLOT(set_color_rgb_true_slot()));
        connect(color_natural_action, SIGNAL(triggered()), this, SLOT(set_color_natural_slot()));
        connect(quit_action, SIGNAL(triggered()), this, SLOT(quit_slot()));

        trayIcon->setContextMenu(menu);
        trayIcon->show();
        trayIcon->setIcon(QIcon("../icon.png"));
    };

public slots:
    void change_wallpaper_slot(){
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

    void set_color_rgb_true_slot(){
        m_color = Color::True;
    }

    void set_color_natural_slot(){
        m_color = Color::Natural;
    }

    void quit_slot(){
        QCoreApplication::quit();
    }


private:

    QSystemTrayIcon *trayIcon;
    ImageType m_imgType;
    Color m_color;
};