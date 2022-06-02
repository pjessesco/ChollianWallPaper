// Every satellite images used in this project are copyrighted by the Korea Meteorological
// Administration, and they can NOT be used for commercial purposes.
//
// Chollian Wallpaper is distributed under the MIT License.
//
// Copyright (c) 2021-2022 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <filesystem>
#include <string>

#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QTimer>

#include "about.h"
#include "downloader.h"
#include "image.h"
#include "logger.h"
#include "setting.h"

class Chollian : public QWidget{
    Q_OBJECT
public:
    Chollian();

public slots:
    // Change wallpaper considering m_imgType and m_color;
    void change_wallpaper_slot(DownloadOption downloadOption, Color color, Resolution resolution, float height_ratio);

    void set_download_option(DownloadOption option){
        LOG("Switch download option");
        m_download_option = option;
        export_current_setting();
    }

    void set_color_slot(Color color){
        LOG("Switch color mode");
        m_color = color;
        export_current_setting();
    }

    void set_height_ratio_slot(float ratio){
        LOG("Change height ratio");
        m_height_ratio = ratio;
        export_current_setting();
    }

    static void quit_slot() {
        LOG("Quit Chollian Wallpaper");
        QCoreApplication::quit();
    }

    void set_resolution_slot(const Resolution &resolution){
        m_resolution = resolution;
        export_current_setting();
    }

    void switch_automatically_update_slot();

    void enable_button_slot(bool flag);

signals:
    // Implementation is not needed for QT signals
    void enable_button_signal(bool flag);

private:
    inline void add_checkable_action_to_group(QMenu* menu, QActionGroup* group, const QString& text, std::function<void()> func, bool is_default);
    inline QAction* add_action_to_menu(QMenu* menu, const QString& text, std::function<void()> func, bool is_checkable, bool is_checked=false);
    inline void export_current_setting() const{
        config_to_file(m_RESOURCE_PATH / "config.txt", m_color, m_download_option, m_resolution, m_is_automatically_update, m_height_ratio);
    }
    

    Color m_color;
    DownloadOption m_download_option;
    Resolution m_resolution;
    float m_height_ratio;
    QAction *m_update_wallpaper_action;
    QAction *m_auto_update_action;
    bool m_is_automatically_update;
    QTimer *m_timer;
    About *m_about_window;

#if defined __APPLE__
    // TODO : Simplify...
    const std::filesystem::path m_RESOURCE_PATH = std::filesystem::path(std::filesystem::current_path().string() + "/../Resources/");
#elif defined _WIN32
    const std::filesystem::path m_RESOURCE_PATH = std::filesystem::current_path() / "resources/";
#endif
};