//
// Created by Jino on 2021/09/21.
//

#pragma once

#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include "dependency_license.h"

class About : public QWidget{

public:
    About();

    QLabel* add_title_line(const std::string &str, int text_height=30, int below_space=0);

    QLabel* add_text_line(const std::string &str, int text_height=20, int below_space=0, int ax=20);

    QScrollArea* add_scrollable_text(const std::string &str, int scroll_height=100, int below_space=10);

    void check_update();

    int current_height;
    QLabel *detect_update_label;
    const std::string CHOLLIAN_VERSION_STR = CHOLLIAN_VERSION;
};

