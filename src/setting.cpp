// Every satellite images used in this project are copyrighted by the Korea Meteorological
// Administration, and they can NOT be used for commercial purposes.
//
// Chollian Wallpaper is distributed under the MIT License.
//
// Copyright (c) 2021-2022 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "downloader.h"
#include "image.h"
#include "setting.h"

void config_to_file(const std::filesystem::path& filepath, Color color, DownloadOption download_option, const Resolution& res, bool auto_update, float height_ratio) {
    std::string config_str = config_to_str(color, download_option, res, auto_update, height_ratio);
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << config_str;
        file.close();
    }
}


inline std::string config_to_str(Color color, DownloadOption download_option, const Resolution& res, bool auto_update, float height_ratio) {
    const std::string& color_str = color_map.at(color);
    const std::string& download_option_str = download_option_map.at(download_option);
    const std::string& res_str = res_map.at(res);
    const std::string auto_update_str = std::to_string(auto_update);
    const std::string& ratio_str = height_ratio_map.at(height_ratio);

    return color_str + "\n" + download_option_str + "\n" + res_str + "\n" + auto_update_str + "\n" + ratio_str;
}


std::tuple<Color, DownloadOption, Resolution, bool, float> file_to_config(const std::filesystem::path& filepath) {
    std::ifstream file(filepath);
    std::stringstream config_ss;

    if (file.is_open()) {
        std::string line;
        while(std::getline(file, line)){
            config_ss<<line<<"\n";
        }
        file.close();
    }
    return str_to_config(config_ss.str());
}

std::tuple<Color, DownloadOption, Resolution, bool, float> str_to_config(const std::string& str) {

    std::vector<std::string> results;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, '\n')) {
        results.push_back(token);
    }

    Color color;
    DownloadOption download_option;
    Resolution res;
    bool auto_update;
    float height_ratio;

    for (auto [c, str] : color_map) {
        if (str == results[0]) {
            color = c;
            break;
        }
    }

    for (auto [d, str] : download_option_map) {
        if (str == results[1]) {
            download_option = d;
            break;
        }
    }

    for (auto [r, str] : res_map) {
        if (str == results[2]) {
            res = r;
            break;
        }
    }

    auto_update = std::stoi(results[3]);

    for (auto [h, str] : height_ratio_map) {
        if (str == results[4]) {
            height_ratio = h;
            break;
        }
    }

    return {color, download_option, res, auto_update, height_ratio};
}

