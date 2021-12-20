//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include "utctime.h"

enum class Color{
    True = 1,
    Natural = 2
};

enum class DownloadOption{
    Quality = 1,
    Performance = 2
};

std::string url_generator_chollian(DownloadOption option,
                                   Color color,
                                   const UTCTime &date);

std::string image_downloader(const std::string &url);

std::string generate_filename(const UTCTime &time,
                              Color color,
                              int width,
                              int height);
