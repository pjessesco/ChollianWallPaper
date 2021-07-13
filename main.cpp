#include <iostream>
#include <tuple>

#include <boost/date_time.hpp>

#include "chollian.h"


int main() {
    std::cout << "Get recent chollian 2a satelite image" << std::endl;

    boost::posix_time::ptime utcTime = boost::posix_time::second_clock::universal_time();
    auto adjusted_time = adjust_target_time(utcTime);

    std::string url = url_generator_chollian(ImageType::FullDome, Color::True, adjusted_time);

    std::cout<<url<<std::endl;
    image_downloader(url);

    return 0;
}
