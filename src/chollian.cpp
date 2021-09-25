//
// Created by Jino on 2021/07/14.
//

#include <filesystem>

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
                       m_imgType(ImageType::FullDome),
                       m_resolution(Resolution(2880, 1800)),
                       m_is_automatically_update(false){
    LOG("Chollian Wallpaper started");

    LOG("RESOURCE_PATH : " + m_RESOURCE_PATH);
    if(!std::filesystem::exists(m_RESOURCE_PATH)){
        LOG(m_RESOURCE_PATH + " directory does not exists; Create it");
        std::filesystem::create_directory(m_RESOURCE_PATH);
    }

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Tray!");

    // Create menu items
    QMenu *menu = new QMenu(this);
    QAction *about_action = menu->addAction("About(TBD)");
    menu->addSection("");
    m_update_wallpaper_action = menu->addAction("Update wallpaper now");
    m_auto_update_action = menu->addAction("Update wallpaper every 10 minutes");

    menu->addSection("Type");
    QAction *type_fulldome_action = menu->addAction("Full Dome");
    QAction *type_eastasia_action = menu->addAction("East Asia (experimental)");
    menu->addSection("Colors");
    QAction *color_rgb_true_action = menu->addAction("RGB True");
    QAction *color_natural_action = menu->addAction("Natural");
    menu->addSection("Resolution");
    QMenu *res_menu = menu->addMenu("Resolution");

    QActionGroup *res_action_group = new QActionGroup(this);
    generate_resolution_menus(res_menu, res_action_group, res_list_4_3);
    res_menu->addSeparator();
    generate_resolution_menus(res_menu, res_action_group, res_list_16_9);
    res_menu->addSeparator();
    generate_resolution_menus(res_menu, res_action_group, res_list_16_10);

    menu->addSeparator();
    QAction *quit_action = menu->addAction("Quit");

    // Set menu items (exclusive, range, etc)
    QActionGroup *set_type_group = new QActionGroup(this);
    QActionGroup *set_color_group = new QActionGroup(this);

    set_type_group->setExclusive(true);
    set_type_group->addAction(type_fulldome_action);
    set_type_group->addAction(type_eastasia_action);
    type_fulldome_action->setCheckable(true);
    type_fulldome_action->setChecked(true);
    type_eastasia_action->setCheckable(true);

    set_color_group->setExclusive(true);
    set_color_group->addAction(color_rgb_true_action);
    set_color_group->addAction(color_natural_action);
    color_rgb_true_action->setCheckable(true);
    color_rgb_true_action->setChecked(true);
    color_natural_action->setCheckable(true);

    m_auto_update_action->setCheckable(true);
    m_auto_update_action->setChecked(false);

    res_action_group->setExclusive(true);

    m_about_window = new About();

    // Connect menu items with slot
    connect(about_action, &QAction::triggered, this, [this](){
        this->m_about_window->show();
    });

    connect(m_update_wallpaper_action, &QAction::triggered, this, [this](){change_wallpaper_slot(m_imgType, m_color, m_resolution);});
    connect(m_auto_update_action, &QAction::triggered, this, [this](){switch_automatically_update_slot();});

    connect(type_fulldome_action, &QAction::triggered, this, [this](){set_type_slot(ImageType::FullDome);});
    connect(type_eastasia_action, &QAction::triggered, this, [this](){set_type_slot(ImageType::EastAsia);});

    connect(color_rgb_true_action, &QAction::triggered, this, [this](){set_color_slot(Color::True);});
    connect(color_natural_action, &QAction::triggered, this, [this](){set_color_slot(Color::Natural);});

    connect(quit_action, &QAction::triggered, this, [this](){quit_slot();});

    trayIcon->setContextMenu(menu);
    trayIcon->show();
    trayIcon->setIcon(QIcon(QString::fromStdString(m_RESOURCE_PATH + "icon.png")));

    // Generate timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this](){change_wallpaper_slot(m_imgType, m_color, m_resolution);});
}

void Chollian::change_wallpaper_slot(ImageType imgType, Color color, Resolution resolution) {
    LOG("Task started");

    auto _ = QtConcurrent::task([this, imgType, color, resolution]{
        enable_button(false);
        UTCTime utcTime;
        utcTime.adjust_target_time();
        const std::string url = url_generator_chollian(imgType, color, utcTime);
        LOG("Generated url : " + url);
        const std::string img_binary = image_downloader(url);
        LOG("Downloaded binary size : " + std::to_string(img_binary.length()));
        // Stop if downloaded data is reasonably small
        if(img_binary.length() < 200000){
            LOG("Skip updating wallpaper");
            return;
        }
        const std::string filename = generate_filename(utcTime, color, imgType, resolution.first, resolution.second);
        Image img = Image(img_binary);
        img.to_any_resolution(resolution.first, resolution.second, 100);
        
        if(std::filesystem::exists(m_RESOURCE_PATH)){
            img.write_png(m_RESOURCE_PATH + filename);
            LOG("Save image as "+ m_RESOURCE_PATH + filename);
        }
        else{
            LOG("RESOURCE_PATH not exists : "+ m_RESOURCE_PATH);
            return;
        }

        img.set_as_wallpaper(m_RESOURCE_PATH + filename);
        LOG("Wallpaper updated");

        // Clean up previously stored images
        for (const auto &entry : std::filesystem::directory_iterator(m_RESOURCE_PATH)){
            const std::string entry_filename = entry.path().filename().string();
            if(entry_filename.compare(filename) && entry_filename.compare("icon.png") && entry_filename.compare("log.txt")){
                std::filesystem::remove(entry);
            }
        }
        enable_button(true);
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
        change_wallpaper_slot(m_imgType, m_color, m_resolution);
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

void Chollian::generate_resolution_menus(QMenu *res_menu, QActionGroup *res_action_group, const std::vector<Resolution> &res_list){
    for(Resolution res : res_list){
        const std::string title = std::to_string(res.first)+" x "+std::to_string(res.second);
        QAction *tmp_res_action = res_menu->addAction(QString::fromStdString(title));
        connect(tmp_res_action, &QAction::triggered, this, [this, res](){ set_resolution_slot(res);});
        tmp_res_action->setCheckable(true);
        tmp_res_action->setChecked(false);
        if(res == m_resolution){
            tmp_res_action->setChecked(true);
        }
        res_action_group->addAction(tmp_res_action);
    }
}

void Chollian::enable_button(bool enable){
    m_update_wallpaper_action->setEnabled(enable);
    m_auto_update_action->setEnabled(enable);
}


