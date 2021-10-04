//
// Created by Jino on 2021/09/21.
//

#include "about.h"

About::About() : QWidget() {

    this->setFixedWidth(500);
    this->setFixedHeight(900);

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
    add_text_line("Release version : " + CHOLLIAN_VERSION_STR);
    add_text_line("Commit version : " + std::string(GIT_HASH));
    add_text_line("Compiler : " + std::string(COMPILER_NAME) + " " + std::string(COMPILER_VERSION));
    add_text_line("CMake configuration : " + std::string(CMAKE_CONFIG));
    add_text_line("Qt version : " + std::string(QT_VERSION_STR));
    add_text_line("libcurl version : " + std::string(CURL_VERSION));

    add_title_line("Licenses");
    add_text_line("<P><b>"
                  "Every satelite images used in this project are copyrighted by the Korea Meteorological Administration. "
                  "You can NOT use it for commercial purposes."
                  "</b></P>", 55);
    add_text_line("Chollian Wallpaper");
    add_scrollable_text(CHOLLIAN_LICENSE);
    add_text_line("Qt6");
    add_scrollable_text(QT_LICENSE);
    add_text_line("libcurl");
    add_scrollable_text(LIBCURL_LICENSE);
    add_text_line("stb");
    add_scrollable_text(STB_LICENSE);
}

QLabel *About::add_title_line(const std::string &str, int text_height, int below_space) {
    current_height += 5;

    QLabel *text_label = new QLabel(this);
    QFont font = text_label->font();
    font.setPixelSize(20);

    text_label->setText(QString::fromStdString(str));
    text_label->setGeometry(10, current_height, 460, text_height);
    text_label->setFont(font);

    current_height += (text_height + below_space);
    return text_label;
}

QLabel *About::add_text_line(const std::string &str, int text_height, int below_space) {
    current_height += 2;
    QLabel *text_label = new QLabel(this);
    text_label->setWordWrap(true);
    text_label->setText(QString::fromStdString(str));
    text_label->setGeometry(20, current_height, 460, text_height);
    current_height += (text_height + below_space);
    return text_label;
}

QScrollArea *About::add_scrollable_text(const std::string &str, int scroll_height, int below_space) {
    current_height += 5;
    QScrollArea *scroll_area = new QScrollArea(this);
    QLabel *text_label = new QLabel();

    text_label->setText(QString::fromStdString(str));

    scroll_area->setWidgetResizable(true);
    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll_area->setGeometry(20, current_height, 460, scroll_height);
    scroll_area->setWidget(text_label);

    current_height += (scroll_height + below_space);
    return scroll_area;
}
