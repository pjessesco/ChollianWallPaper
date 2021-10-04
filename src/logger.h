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

    return "[" + std::string(asctime(current_time)).substr(0, 24) + "]";
}

// #if defined __APPLE__
//     const std::string RESOURCE_PATH = std::filesystem::current_path().string() + "/../Resources/";
// #elif defined _WIN32
//     const std::string RESOURCE_PATH = std::filesystem::current_path().string() + "/resources/";
// #endif

#define LOG(str){              \
    std::cout<<current_time_string()<<" "<<str<<std::endl; \
}


