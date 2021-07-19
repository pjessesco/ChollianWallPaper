//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <QApplication>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include "chollian.h"
#include "image.h"

class GUI : public QLabel{
    Q_OBJECT
public:
    GUI();

public slots:
    // Change wallpaper considering m_imgType and m_color;
    void change_wallpaper_slot() const;

    void set_color_rgb_true_slot(){
        m_color = Color::True;
    }

    void set_color_natural_slot(){
        m_color = Color::Natural;
    }

    void quit_slot() const{
        QCoreApplication::quit();
    }

private:
    QSystemTrayIcon *trayIcon;
    ImageType m_imgType;
    Color m_color;
};