//
// Created by Jino on 2021/07/14.
//

#include <filesystem>
#include <string>
#include <tuple>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QtGui/QActionGroup>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

#include "about.h"
#include "downloader.h"
#include "chollian.h"
#include "logger.h"
#include "setting.h"

Chollian::Chollian() {
    LOG("Chollian Wallpaper started");

    m_about_window = new About();

    LOG("RESOURCE_PATH : " + m_RESOURCE_PATH.string());
    if(!std::filesystem::exists(m_RESOURCE_PATH)){
        LOG(m_RESOURCE_PATH.string() + " directory does not exists; Create it");
        std::filesystem::create_directory(m_RESOURCE_PATH);
    }

    if(!std::filesystem::exists(m_RESOURCE_PATH / "config.txt")){
        LOG("Configuration not found, generate default config.txt");
        config_to_file(m_RESOURCE_PATH / "config.txt", Color::True, DownloadOption::Performance, Resolution(2880, 1800), false, 0.8f);
    }

    std::tie(m_color, m_download_option, m_resolution, m_is_automatically_update, m_height_ratio) = file_to_config(m_RESOURCE_PATH / "config.txt");

    // Create menu items
    auto *menu = new QMenu(this);
    add_action_to_menu(menu, "About", [this](){this->m_about_window->check_update(); this->m_about_window->show();}, false);

    menu->addSection("Update");
    m_update_wallpaper_action = add_action_to_menu(menu, "Update wallpaper now", [this](){change_wallpaper_slot(m_download_option, m_color, m_resolution, m_height_ratio);}, false);
    m_auto_update_action = add_action_to_menu(menu, "Update wallpaper every 10 minutes", [this](){switch_automatically_update_slot();}, true, m_is_automatically_update);
    connect(this, SIGNAL(enable_button_signal(bool)), this, SLOT(enable_button_slot(bool)));

    menu->addSection("Download Option");
    auto *set_download_option_group = new QActionGroup(this);
    set_download_option_group->setExclusive(true);
    for (auto&& [val, str] : download_option_map) {
        add_checkable_action_to_group(menu, set_download_option_group, QString::fromStdString(str), [this, val=val]() {set_download_option(val); }, val==m_download_option);
    }

    menu->addSection("Colors");
    auto *set_color_group = new QActionGroup(this);
    set_color_group->setExclusive(true);
    for (auto&& [val, str] : color_map) {
        add_checkable_action_to_group(menu, set_color_group, QString::fromStdString(str), [this, val=val]() {set_color_slot(val); }, val == m_color);
    }

    menu->addSection("Size");
    auto *set_height_ratio_group = new QActionGroup(this);
    set_height_ratio_group->setExclusive(true);
    QMenu *height_ratio_menu = menu->addMenu("Size");
    for (auto&& [val, str] : height_ratio_map) {
        add_checkable_action_to_group(height_ratio_menu, set_height_ratio_group, QString::fromStdString(str), [this, val=val]() {set_height_ratio_slot(val); }, val == m_height_ratio);
    }

    menu->addSection("Resolution");
    auto *set_resolution_group = new QActionGroup(this);
    set_resolution_group->setExclusive(true);
    QMenu *res_menu = menu->addMenu("Resolution");
    for (auto&& [val, str] : res_map) {
        add_checkable_action_to_group(res_menu, set_resolution_group, QString::fromStdString(str), [this, val=val]() {set_resolution_slot(val); }, val == m_resolution);
    }

    menu->addSeparator();
    add_action_to_menu(menu, "Quit", [](){quit_slot();}, false);
   
    // Set icon
    auto *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Chollian Wallpaper");
    trayIcon->setContextMenu(menu);
    trayIcon->setIcon(QIcon(QString::fromStdString((m_RESOURCE_PATH / "icon.png").string())));
    trayIcon->show();

    // Generate timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this](){change_wallpaper_slot(m_download_option, m_color, m_resolution, m_height_ratio);});
}


void Chollian::change_wallpaper_slot(DownloadOption downloadOption, Color color, Resolution resolution, float height_ratio) {
    LOG("Task started");

    QFuture<void> _ = QtConcurrent::task([this, downloadOption, color, resolution, height_ratio]() -> void {
        emit enable_button_signal(false);
        UTCTime utcTime;
        utcTime.adjust_target_time();
        const std::string url = url_generator_chollian(downloadOption, color, utcTime);
        LOG("Generated url : " + url);

        const std::string img_binary = image_downloader(url);
        LOG("Downloaded binary size : " + std::to_string(img_binary.length()));

        // Stop if downloaded data is reasonably small
        if(img_binary.length() < 100000){
            LOG("Downloaded binary is too small, skip updating wallpaper");
            emit enable_button_signal(true);
            return;
        }
        const std::string filename = generate_filename(utcTime, color, resolution.first, resolution.second, height_ratio);

        Image img = Image(img_binary);
        if(!img.is_available()){
            LOG("Image is not available, skip updating wallpaper");
            emit enable_button_signal(true);
            return;
        }

        img.to_any_resolution(resolution.first, resolution.second, m_height_ratio);
        
        if(std::filesystem::exists(m_RESOURCE_PATH)){
            img.write_png((m_RESOURCE_PATH / filename).string());
            LOG("Save image as "+ (m_RESOURCE_PATH / filename).string());
        }
        else{
            LOG("RESOURCE_PATH not exists : "+ m_RESOURCE_PATH.string());
            emit enable_button_signal(true);
            return;
        }

        img.set_as_wallpaper((m_RESOURCE_PATH / filename).string());
        LOG("Wallpaper updated");

        // Clean up previously stored images
        for (const auto &entry : std::filesystem::directory_iterator(m_RESOURCE_PATH)){
            const std::string entry_filename = entry.path().filename().string();
            if(entry_filename != filename &&
               entry_filename != "icon.png" &&
               entry_filename != "log.txt" &&
               entry_filename != "config.txt"){
                std::filesystem::remove(entry);
            }
        }
        emit enable_button_signal(true);
        LOG("Task ended");
    }).spawn();
}

void Chollian::switch_automatically_update_slot(){
    if(!m_is_automatically_update){
        LOG("Automatic update enabled");
        // Turn on automatic update
        // 1. Update now
        // 2. Set `m_is_automatically_update` true
        // 3. Start timer
        change_wallpaper_slot(m_download_option, m_color, m_resolution, m_height_ratio);
        m_is_automatically_update = true;
        // Update per 10 minutes
        m_timer->start(600000);
    }
    else{
        LOG("Automatic update disabled");
        // Turn off automatic update
        // 1. Set `m_is_automatically_update` false
        // 2. Stop timer
        m_is_automatically_update = false;
        m_timer->stop();
    }
    export_current_setting();
}

inline void Chollian::add_checkable_action_to_group(QMenu* menu, QActionGroup* group, const QString& text, std::function<void()> func, bool is_default) {
    QAction* action = menu->addAction(text);
    group->addAction(action);
    connect(action, &QAction::triggered, this, std::move(func));
    action->setCheckable(true);
    action->setChecked(is_default);
}

inline QAction* Chollian::add_action_to_menu(QMenu* menu, const QString& text, std::function<void()> func, bool is_checkable, bool is_checked) {
    QAction* action = menu->addAction(text);
    connect(action, &QAction::triggered, this, std::move(func));
    if (is_checkable) {
        action->setCheckable(true);
        action->setChecked(is_checked);
    }
    return action;
}


void Chollian::enable_button_slot(bool flag){
    m_update_wallpaper_action->setEnabled(flag);
    m_auto_update_action->setEnabled(flag);
}


