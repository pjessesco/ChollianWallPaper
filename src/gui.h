//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <QApplication>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QActionGroup>
#include <QTimer>

#include "downloader.h"
#include "image.h"


class GUI : public QLabel{
    Q_OBJECT
public:
    GUI(const std::string &path);

public slots:
    // Change wallpaper considering m_imgType and m_color;
    void change_wallpaper_slot() const;

    void set_color_slot(Color color){
        m_color = color;
    }

    void quit_slot() const{
        QCoreApplication::quit();
    }

    void set_resolution_slot(const Resolution &resolution){
        m_resolution = resolution;
    }

    void switch_automatically_update_slot();


private:

    void generate_resolution_menus(QMenu *res_menu, QActionGroup *res_action_group, const std::vector<Resolution> &res_list);

    ImageType m_imgType;
    Color m_color;
    Resolution m_resolution;
    bool m_is_automatically_update;
    QTimer *m_timer;
    std::string m_executable_parent_path;
};