//
// Created by Jino on 2021/12/21.
//


#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <fstream>
#include <filesystem>

#include "setting.h"
#include "image.h"
#include "downloader.h"

void config_to_file(const std::filesystem::path& filepath, Color color, DownloadOption download_option, const Resolution& res, bool auto_update) {
    std::string config_str = config_to_str(color, download_option, res, auto_update);
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << config_str;
        file.close();
    }
}


inline std::string config_to_str(Color color, DownloadOption download_option, const Resolution& res, bool auto_update) {
    const std::string& color_str = color_map.at(color);
    const std::string& download_option_str = download_option_map.at(download_option);
    const std::string& res_str = res_map.at(res);
    std::string auto_update_str = std::to_string(auto_update);

    return color_str + "\n" + download_option_str + "\n" + res_str + "\n" + auto_update_str;
}


std::tuple<Color, DownloadOption, Resolution, bool> file_to_config(const std::filesystem::path& filepath) {
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

std::tuple<Color, DownloadOption, Resolution, bool> str_to_config(const std::string& str) {

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

    for (auto [c, str] : color_map) {
        if (str.compare(results[0]) == 0) {
            color = c;
            break;
        }
    }

    for (auto [d, str] : download_option_map) {
        if (str.compare(results[1]) == 0) {
            download_option = d;
            break;
        }
    }

    for (auto [r, str] : res_map) {
        if (str.compare(results[2]) == 0) {
            res = r;
            break;
        }
    }

    auto_update = std::stoi(results[3]);

    return {color, download_option, res, auto_update};
}

