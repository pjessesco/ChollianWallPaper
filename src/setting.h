//
// Created by Jino on 2021/12/21.
//

#pragma once

#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <filesystem>

#include "image.h"
#include "downloader.h"


void config_to_file(const std::filesystem::path &path, Color color, DownloadOption download_option, const Resolution& res, bool auto_update);

inline std::string config_to_str(Color color, DownloadOption download_option, const Resolution& res, bool auto_update);

std::tuple<Color, DownloadOption, Resolution, bool> file_to_config(const std::filesystem::path &path);

std::tuple<Color, DownloadOption, Resolution, bool> str_to_config(const std::string &str);


