//
// Created by Jino on 2021/07/14.
//

#include <filesystem>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QtGui/QActionGroup>
#include <QTimer>

#include "chollian.h"
#include "gui.h"

GUI::GUI(const std::string &path) : m_color(Color::True),
                                    m_imgType(ImageType::FullDome),
                                    m_resolution(Resolution(2880, 1800)),
                                    m_is_automatically_update(false),
                                    m_executable_parent_path(std::filesystem::path(path).parent_path().string()){
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Tray!");

    // Create menu items
    QMenu *menu = new QMenu(this);
    QAction *about_action = menu->addAction("About(TBD)");
    menu->addSection("");
    QAction *update_wallpaper_action = menu->addAction("Update wallpaper now");
    QAction *auto_update_action = menu->addAction("Update wallpaper per 10 minutes");

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
    QActionGroup *set_color_group = new QActionGroup(this);
    set_color_group->setExclusive(true);
    set_color_group->addAction(color_rgb_true_action);
    set_color_group->addAction(color_natural_action);
    auto_update_action->setCheckable(true);
    auto_update_action->setChecked(false);
    color_rgb_true_action->setCheckable(true);
    color_rgb_true_action->setChecked(true);
    color_natural_action->setCheckable(true);

    res_action_group->setExclusive(true);

    // Connect menu items with slot
    connect(update_wallpaper_action, &QAction::triggered, this, [this](){change_wallpaper_slot();});
    connect(auto_update_action, &QAction::triggered, this, [this](){switch_automatically_update_slot();});
    connect(color_rgb_true_action, &QAction::triggered, this, [this](){set_color_slot(Color::True);});
    connect(color_natural_action, &QAction::triggered, this, [this](){set_color_slot(Color::Natural);});
    connect(quit_action, &QAction::triggered, this, [this](){quit_slot();});

    trayIcon->setContextMenu(menu);
    trayIcon->show();

#ifdef __APPLE__
    std::string icon_path = m_executable_parent_path + "/../Resources/icon.png";
#endif
    trayIcon->setIcon(QIcon(QString::fromStdString(icon_path)));

    // Generate timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this](){change_wallpaper_slot();});
}

void GUI::change_wallpaper_slot() const {
    UTCTime utcTime;
    utcTime.adjust_target_time();
    const std::string url = url_generator_chollian(m_imgType, m_color, utcTime);
    const std::string img_binary = image_downloader(url);

    // Stop if downloaded data is reasonably small
    if(img_binary.length() < 200000){
        // TODO : Notify if update is unable
        return;
    }
    const std::string filename = generate_filename(utcTime, m_color, m_imgType, m_resolution.first, m_resolution.second);
    Image img = Image(img_binary);
    img.to_any_resolution(m_resolution.first, m_resolution.second, 100);
    img.set_as_wallpaper(filename, m_executable_parent_path);
}

void GUI::switch_automatically_update_slot(){
    if(!m_is_automatically_update){
        // Turn on automatic update
        // 1. Update now
        // 2. Set `m_is_automatically_update` true
        // 3. Start timer
        change_wallpaper_slot();
        m_is_automatically_update = true;
        // Update per 10 minutes
        m_timer->start(600000);
    }
    else{
        // Turn off automatic update
        // 1. Set `m_is_automatically_update` false
        // 2. Stop timer
        m_is_automatically_update = false;
        m_timer->stop();
    }
}

void GUI::generate_resolution_menus(QMenu *res_menu, QActionGroup *res_action_group, const std::vector<Resolution> &res_list){
    // Generate 4:3 resolution presets
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