//
// Created by Jino on 2021/09/21.
//

#pragma once

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QScrollArea>

#include <iostream>


class About : public QWidget{

public:
    About() : QWidget() {

        this->setFixedWidth(500);
        this->setFixedHeight(800);

        QLabel *title = new QLabel(this);
        title->setText("Chollian Wallpaper");
        QFont font = title->font();
        font.setPixelSize(30);
        title->setFont(font);
        title->setGeometry(120, 10, 300, 30);

        current_height = 50;
        add_text_line("By Jino Park and contributors");
        add_text_line("<a href=\"https://github.com/pjessesco/ChollianWallPaper\">Repository</a>")->setOpenExternalLinks(true);

        add_title_line("Build Information");
        add_text_line("Commit version : " + std::string(GIT_HASH));
        add_text_line("Compiler : " + std::string(COMPILER_NAME) + " " + std::string(COMPILER_VERSION));
        add_text_line("Qt version : " + std::string(QT_VERSION_STR));
        add_text_line("CURL version : " + std::string(CURL_VERSION));
        add_title_line("Licenses");
        add_text_line("TBD");
    }

    inline QLabel* add_title_line(const std::string &str, int text_height=30, int line_space=0){
        current_height += 5;

        QLabel *text_label = new QLabel(this);
        QFont font = text_label->font();
        font.setPixelSize(20);

        text_label->setText(QString::fromStdString(str));
        text_label->setGeometry(10, current_height, 460, text_height);
        text_label->setFont(font);

        current_height += (text_height + line_space);
        return text_label;
    }

    inline QLabel* add_text_line(const std::string &str, int text_height=20, int line_space=0){
        current_height += 2;
        QLabel *text_label = new QLabel(this);
        text_label->setText(QString::fromStdString(str));
        text_label->setGeometry(20, current_height, 460, text_height);
        current_height += (text_height + line_space);
        return text_label;
    }

    int current_height;
};
