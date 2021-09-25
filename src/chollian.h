//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <string>
#include <filesystem>

#include <QApplication>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QActionGroup>
#include <QTimer>

#include "downloader.h"
#include "image.h"
#include "logger.h"
#include "about.h"


class Chollian : public QLabel{
    Q_OBJECT
public:
    Chollian();

public slots:
    // Change wallpaper considering m_imgType and m_color;
    void change_wallpaper_slot(ImageType imgType, Color color, Resolution resolution);

    void set_type_slot(ImageType type){
        LOG("Switch image type");
        m_imgType = type;
    }

    void set_color_slot(Color color){
        LOG("Switch color mode");
        m_color = color;
    }

    void quit_slot() const{
        QCoreApplication::quit();
    }

    void set_resolution_slot(const Resolution &resolution){
        m_resolution = resolution;
    }

    void switch_automatically_update_slot();

    void enable_button(bool enable);

private:

    void generate_resolution_menus(QMenu *res_menu, QActionGroup *res_action_group, const std::vector<Resolution> &res_list);

    ImageType m_imgType;
    Color m_color;
    Resolution m_resolution;
    QAction *m_update_wallpaper_action;
    QAction *m_auto_update_action;
    bool m_is_automatically_update;
    QTimer *m_timer;
    About *m_about_window;

#if defined __APPLE__
    const std::string m_RESOURCE_PATH = std::filesystem::current_path().string() + "/../Resources/";
#elif defined _WIN32
    const std::string m_RESOURCE_PATH = std::filesystem::current_path().string() + "/resources/";
#endif
};