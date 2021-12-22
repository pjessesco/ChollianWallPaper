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


class Chollian : public QWidget{
    Q_OBJECT
public:
    Chollian();

public slots:
    // Change wallpaper considering m_imgType and m_color;
    void change_wallpaper_slot(DownloadOption downloadOption, Color color, Resolution resolution);

    void set_download_option(DownloadOption option){
        LOG("Switch download option");
        m_download_option = option;
    }

    void set_color_slot(Color color){
        LOG("Switch color mode");
        m_color = color;
    }

    static void quit_slot() {
        LOG("Quit Chollian Wallpaper");
        QCoreApplication::quit();
    }

    void set_resolution_slot(const Resolution &resolution){
        m_resolution = resolution;
    }

    void switch_automatically_update_slot();

    void enable_button_slot(bool flag);

signals:
    // Implementation is not needed for QT signals
    void enable_button_signal(bool flag);

private:
    inline void add_checkable_action_to_group(QMenu* menu, QActionGroup* group, const QString& text, std::function<void()> func, bool is_default);
    inline QAction* add_action_to_menu(QMenu* menu, const QString& text, std::function<void()> func, bool is_checkable);

    

    Color m_color;
    DownloadOption m_download_option;
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