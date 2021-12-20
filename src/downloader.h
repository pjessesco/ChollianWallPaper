//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include "utctime.h"

enum class Color{
    True,
    Natural,
    WaterVapor,
    Cloud,
    Ash
};

enum class DownloadOption{
    Quality,
    Performance
};

std::string url_generator_chollian(DownloadOption option,
                                   Color color,
                                   const UTCTime &date);

std::string image_downloader(const std::string &url);

std::string generate_filename(const UTCTime &time,
                              Color color,
                              int width,
                              int height);
