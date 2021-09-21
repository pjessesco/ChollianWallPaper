//
// Created by Jino on 2021/09/21.
//

#pragma once

#include <QApplication>
#include <QWidget>
#include <QLabel>


class About : public QWidget{

public:
    About() : QWidget() {

        this->setFixedWidth(500);
        this->setFixedHeight(500);

        QLabel *title = new QLabel(this);
        title->setText("Chollian Wallpaper");
        QFont font = title->font();
        font.setPixelSize(20);
        title->setFont(font);
        title->setGeometry(10, 10, 200, 30);

        QLabel *githash = new QLabel(this);
        githash->setText("Build version : " + QString::fromStdString(GIT_HASH));
        githash->setGeometry(10, 50, 200, 10);


    }


};
