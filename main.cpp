#include <iostream>
#include <tuple>

#include <boost/date_time.hpp>

#include <QApplication>


#include "gui.h"

int main(int argc, char *argv[]) {

    boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
    auto adjusted_time = adjust_target_time(utcTime);

    std::string url = url_generator_chollian(ImageType::FullDome, Color::True, adjusted_time);

    image_downloader(url);

    QApplication app(argc, argv);
    GUI gui;

    return app.exec();
}
