cmake_minimum_required(VERSION 3.19)
project(ChollianWallpaper)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(CURL REQUIRED)

include_directories(ext)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ChollianWallpaper
        src/main.cpp
        src/chollian.cpp
        src/chollian.h
        src/downloader.cpp
        src/downloader.h
        src/image.cpp
        src/image.h
        src/utctime.cpp
        src/utctime.h
        src/logger.h)

target_link_libraries(ChollianWallpaper CURL::libcurl)
target_link_libraries(ChollianWallpaper Qt6::Widgets)

