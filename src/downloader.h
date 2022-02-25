//
// Created by Jino on 2021/07/14.
//

#pragma once

#include <iostream>
#include <string>
#include <tuple>
#include <map>
#include "utctime.h"

enum class Color{
    True,
    Natural,
    WaterVapor,
    Cloud,
    Ash
};

const std::map<Color, std::string> color_map{
    {Color::True, "True"},
    {Color::Natural, "Natural"},
    {Color::WaterVapor, "WaterVapor"},
    {Color::Cloud, "Cloud"},
    {Color::Ash, "Ash"}
};

enum class DownloadOption{
    Quality,
    Performance
};

const std::map<DownloadOption, std::string> download_option_map{
    {DownloadOption::Quality, "Quality"},
    {DownloadOption::Performance, "Performance"}
};

std::string url_generator_chollian(DownloadOption option,
                                   Color color,
                                   const UTCTime &date);

std::string image_downloader(const std::string &url);

std::string generate_filename(const UTCTime &time,
                              Color color,
                              int width,
                              int height,
                              float height_ratio);

std::string get_latest_version();
