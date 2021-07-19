//
// Created by Jino on 2021/07/14.
//

#include <QSystemTrayIcon>
#include <QMenu>
#include <QtGui/QActionGroup>

#include "chollian.h"
#include "gui.h"

GUI::GUI() : m_color(Color::True), m_imgType(ImageType::FullDome){
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Tray!");

    // Create menu items
    QMenu *menu = new QMenu(this);
    QAction *update_wallpaper_action = new QAction("Update background", this);
    QAction *color_rgb_true_action = new QAction("RGB True");
    QAction *color_natural_action = new QAction("Natural");
    QAction *quit_action = new QAction("Quit");

    // Set menu items (exclusive, range, etc)
    QActionGroup *set_color_group = new QActionGroup(this);
    set_color_group->setExclusive(true);
    set_color_group->addAction(color_rgb_true_action);
    set_color_group->addAction(color_natural_action);
    color_rgb_true_action->setCheckable(true);
    color_rgb_true_action->setChecked(true);
    color_natural_action->setCheckable(true);


    // Connect menu items with slot
    connect(update_wallpaper_action, &QAction::triggered, this, [this](){change_wallpaper_slot();});
    connect(color_rgb_true_action, &QAction::triggered, this, [this](){set_color_slot(Color::True);});
    connect(color_natural_action, &QAction::triggered, this, [this](){set_color_slot(Color::Natural);});
    connect(quit_action, &QAction::triggered, this, [this](){quit_slot();});

    // Add menu items into the menu
    menu->addAction(update_wallpaper_action);
    menu->addSection("Colors");
    menu->addAction(color_rgb_true_action);
    menu->addAction(color_natural_action);
    menu->addSeparator();
    menu->addAction(quit_action);

    trayIcon->setContextMenu(menu);
    trayIcon->show();
    trayIcon->setIcon(QIcon("../icon.png"));
}

void GUI::change_wallpaper_slot() const {
    boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
    auto adjusted_time = adjust_target_time(utcTime);
    const std::string url = url_generator_chollian(m_imgType, m_color, adjusted_time);
    const std::string img_binary = image_downloader(url);
    const std::string filename = generate_filename(adjusted_time, m_color, m_imgType);
    Image img = Image(img_binary);
    img.to_any_resolution(2880, 1800, 100);
    img.set_as_wallpaper(filename);
}
