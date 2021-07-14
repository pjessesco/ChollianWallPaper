#include <iostream>
#include <tuple>

#include <boost/date_time.hpp>

#include <QApplication>

#include "gui.h"
#include "image.h"

int main(int argc, char *argv[]) {


    QApplication app(argc, argv);
    GUI gui;

    return app.exec();
}

