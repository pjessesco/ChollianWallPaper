//
// Created by Jino on 2021/09/17.
//

#pragma once

#include <iostream>
#include <chrono>
#include <fstream>

inline void fill_zero(std::string &str, int len) {
    str.insert(str.begin(), len - str.size(), '0');
}

inline std::string current_time_string() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto current_time = std::localtime(&now);

    std::string hour = std::to_string(current_time->tm_hour);
    std::string min = std::to_string(current_time->tm_min);
    std::string sec = std::to_string(current_time->tm_sec);

    fill_zero(hour, 2);
    fill_zero(min, 2);
    fill_zero(sec, 2);

    return "[" + hour + ":" + min + ":" + sec + "]";
}

// #if defined __APPLE__
//     const std::string RESOURCE_PATH = std::filesystem::current_path().string() + "/../Resources/";
// #elif defined _WIN32
//     const std::string RESOURCE_PATH = std::filesystem::current_path().string() + "/resources/";
// #endif

#define LOG(str){              \
    std::cout<<current_time_string()<<" "<<str<<std::endl; \
}


