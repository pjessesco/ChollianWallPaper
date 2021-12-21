//
// Created by Jino on 2021/07/14.
//

#include <filesystem>
#include <string>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QtGui/QActionGroup>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

#include "about.h"
#include "downloader.h"
#include "chollian.h"
#include "logger.h"


Chollian::Chollian() : m_color(Color::True),
                       m_download_option(DownloadOption::Performance),
                       m_resolution(Resolution(2880, 1800)),
                       m_is_automatically_update(false){
    LOG("Chollian Wallpaper started");

    m_about_window = new About();

    LOG("RESOURCE_PATH : " + m_RESOURCE_PATH);
    if(!std::filesystem::exists(m_RESOURCE_PATH)){
        LOG(m_RESOURCE_PATH + " directory does not exists; Create it");
        std::filesystem::create_directory(m_RESOURCE_PATH);
    }

    // Create menu items
    QMenu *menu = new QMenu(this);
    add_action_to_menu(menu, "About", [this](){this->m_about_window->show();}, false);

    menu->addSection("Update");
    m_update_wallpaper_action = add_action_to_menu(menu, "Update wallpaper now", [this](){change_wallpaper_slot(m_download_option, m_color, m_resolution);}, false);
    m_auto_update_action = add_action_to_menu(menu, "Update wallpaper every 10 minutes", [this](){switch_automatically_update_slot();}, true);
    connect(this, SIGNAL(enable_button_signal(bool)), this, SLOT(enable_button_slot(bool)));

    menu->addSection("Download Option");
    QActionGroup *set_download_option_group = new QActionGroup(this);
    set_download_option_group->setExclusive(true);
    add_checkable_action_to_group(menu, set_download_option_group, "Performance", [this](){set_download_option(DownloadOption::Performance);}, true);
    add_checkable_action_to_group(menu, set_download_option_group, "Quality", [this](){set_download_option(DownloadOption::Quality);}, false);

    menu->addSection("Colors");
    QActionGroup *set_color_group = new QActionGroup(this);
    set_color_group->setExclusive(true);
    add_checkable_action_to_group(menu, set_color_group, "RGB True", [this](){set_color_slot(Color::True);}, true);
    add_checkable_action_to_group(menu, set_color_group, "Natural", [this](){set_color_slot(Color::Natural);}, false);
    add_checkable_action_to_group(menu, set_color_group, "Water Vapor", [this](){set_color_slot(Color::WaterVapor);}, false);
    add_checkable_action_to_group(menu, set_color_group, "Cloud", [this](){set_color_slot(Color::Cloud);}, false);
    add_checkable_action_to_group(menu, set_color_group, "Ash", [this](){set_color_slot(Color::Ash);}, false);

    menu->addSection("Resolution");
    QActionGroup *set_resolution_group = new QActionGroup(this);
    set_resolution_group->setExclusive(true);
    QMenu *res_menu = menu->addMenu("Resolution");
    generate_resolution_menus(res_menu, set_resolution_group, res_list_4_3);
    res_menu->addSeparator();
    generate_resolution_menus(res_menu, set_resolution_group, res_list_16_9);
    res_menu->addSeparator();
    generate_resolution_menus(res_menu, set_resolution_group, res_list_16_10);

    menu->addSeparator();
    add_action_to_menu(menu, "Quit", [](){quit_slot();}, false);
   
    // Set icon
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Chollian Wallpaper");
    trayIcon->setContextMenu(menu);
    trayIcon->setIcon(QIcon(QString::fromStdString(m_RESOURCE_PATH + "icon.png")));
    trayIcon->show();

    // Generate timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this](){change_wallpaper_slot(m_download_option, m_color, m_resolution);});
}


void Chollian::change_wallpaper_slot(DownloadOption downloadOption, Color color, Resolution resolution) {
    LOG("Task started");

    QFuture<void> _ = QtConcurrent::task([this, downloadOption, color, resolution]() -> void {
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
        const std::string filename = generate_filename(utcTime, color, resolution.first, resolution.second);

        Image img = Image(img_binary);
        if(!img.is_available()){
            LOG("Image is not available, skip updating wallpaper");
            emit enable_button_signal(true);
            return;
        }

        img.to_any_resolution(resolution.first, resolution.second, 100);
        
        if(std::filesystem::exists(m_RESOURCE_PATH)){
            img.write_png(m_RESOURCE_PATH + filename);
            LOG("Save image as "+ m_RESOURCE_PATH + filename);
        }
        else{
            LOG("RESOURCE_PATH not exists : "+ m_RESOURCE_PATH);
            emit enable_button_signal(true);
            return;
        }

        img.set_as_wallpaper(m_RESOURCE_PATH + filename);
        LOG("Wallpaper updated");

        // Clean up previously stored images
        for (const auto &entry : std::filesystem::directory_iterator(m_RESOURCE_PATH)){
            const std::string entry_filename = entry.path().filename().string();
            if(entry_filename != filename && entry_filename != "icon.png" && entry_filename != "log.txt"){
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
        change_wallpaper_slot(m_download_option, m_color, m_resolution);
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
}

inline void Chollian::add_checkable_action_to_group(QMenu* menu, QActionGroup* group, const QString& text, std::function<void()> func, bool is_default) {
    QAction* action = menu->addAction(text);
    group->addAction(action);
    connect(action, &QAction::triggered, this, func);
    action->setCheckable(true);
    action->setChecked(is_default);
}

inline QAction* Chollian::add_action_to_menu(QMenu* menu, const QString& text, std::function<void()> func, bool is_checkable) {
    QAction* action = menu->addAction(text);
    connect(action, &QAction::triggered, this, func);
    if (is_checkable) {
        action->setCheckable(true);
        action->setChecked(false);
    }
    return action;
}

void Chollian::generate_resolution_menus(QMenu *res_menu, QActionGroup *res_action_group, const std::vector<Resolution> &res_list){
    for(Resolution res : res_list){
        const std::string title = std::to_string(res.first)+" x "+std::to_string(res.second);
        QAction *tmp_res_action = add_action_to_menu(res_menu, QString::fromStdString(title), [this, res](){set_resolution_slot(res);}, true);
        if(res == m_resolution){
            tmp_res_action->setChecked(true);
        }
        res_action_group->addAction(tmp_res_action);
    }
}

void Chollian::enable_button_slot(bool flag){
    m_update_wallpaper_action->setEnabled(flag);
    m_auto_update_action->setEnabled(flag);
}


