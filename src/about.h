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

        QLabel *repo_url = new QLabel(this);
        repo_url->setText("Repository : https://github.com/pjessesco/ChollianWallPaper");
        repo_url->setGeometry(10, 60, 400, 20);

        QLabel *githash = new QLabel(this);
        githash->setText("Commit version : " + QString::fromStdString(GIT_HASH));
        githash->setGeometry(10, 80, 200, 20);



    }


};
