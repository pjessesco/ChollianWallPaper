//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <iostream>
#include <string>
#include <tuple>

#include <boost/locale.hpp>
#include <boost/date_time.hpp>

enum class ImageType{
    FullDome = 1,
    EastAsia = 2
};

enum class Color{
    True = 1,
    Natural = 2
};

inline std::string add_zero_padding(int num){
    return num<10?"0"+std::to_string(num):std::to_string(num);
}

inline auto extract_component(const boost::posix_time::ptime &time){
    return std::make_tuple(std::to_string(time.date().year()),
                           add_zero_padding(time.date().month()),
                           add_zero_padding(time.date().day()),
                           add_zero_padding(time.time_of_day().hours()),
                           add_zero_padding(time.time_of_day().minutes()));
}

std::string url_generator_chollian(const ImageType &imageType,
                                   const Color &color,
                                   const boost::posix_time::ptime &date);

void image_downloader(const std::string &url);

boost::posix_time::ptime adjust_target_time(const boost::posix_time::ptime &time);
