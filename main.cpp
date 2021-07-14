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

    img.resize(500,500);
    img.remove_watermark();
    img.remove_description();

    img.add_left_right_border(10);


    img.write_png("from_Image_resized1.png");

    return 0;

//    QApplication app(argc, argv);
//    GUI gui;

//    return app.exec();
}

