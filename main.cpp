#include <iostream>
#include <tuple>

#include <boost/date_time.hpp>

#include <QApplication>

#include "gui.h"
#include "image.h"

int main(int argc, char *argv[]) {

    boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
    auto adjusted_time = adjust_target_time(utcTime);

    std::string url = url_generator_chollian(ImageType::FullDome, Color::True, adjusted_time);

    std::string img_binary = image_downloader(url);

    Image img = Image(img_binary);
    img.to_1080p(100);

    img.write_png("1080p.png");


    return 0;

    QApplication app(argc, argv);
    GUI gui;

    return app.exec();
}

