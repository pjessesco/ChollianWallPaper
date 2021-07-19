//
// Created by Jino on 2021/07/14.
//

#include <QSystemTrayIcon>
#include <QMenu>
#include <QtGui/QActionGroup>

#include "chollian.h"
#include "gui.h"

GUI::GUI() : m_color(Color::True), m_imgType(ImageType::FullDome), m_resolution(Resolution(2880, 1800)){
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Tray!");

    // Create menu items
    QMenu *menu = new QMenu(this);
    QAction *about_action = menu->addAction("About(TBD)");
    menu->addSection("");
    QAction *update_wallpaper_action = menu->addAction("Update background");
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
    color_rgb_true_action->setCheckable(true);
    color_rgb_true_action->setChecked(true);
    color_natural_action->setCheckable(true);

    res_action_group->setExclusive(true);

    // Connect menu items with slot
    connect(update_wallpaper_action, &QAction::triggered, this, [this](){change_wallpaper_slot();});
    connect(color_rgb_true_action, &QAction::triggered, this, [this](){set_color_slot(Color::True);});
    connect(color_natural_action, &QAction::triggered, this, [this](){set_color_slot(Color::Natural);});
    connect(quit_action, &QAction::triggered, this, [this](){quit_slot();});

    trayIcon->setContextMenu(menu);
    trayIcon->show();
    trayIcon->setIcon(QIcon("../icon.png"));
}

void GUI::change_wallpaper_slot() const {
    boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
    auto adjusted_time = adjust_target_time(utcTime);
    const std::string url = url_generator_chollian(m_imgType, m_color, adjusted_time);
    const std::string img_binary = image_downloader(url);
    const std::string filename = generate_filename(adjusted_time, m_color, m_imgType, m_resolution.first, m_resolution.second);
    Image img = Image(img_binary);
    img.to_any_resolution(m_resolution.first, m_resolution.second, 100);
    img.set_as_wallpaper(filename);
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