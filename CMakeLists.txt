cmake_minimum_required(VERSION 3.19)
project(ChollianWallpaper)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost COMPONENTS date_time locale REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(ext)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ChollianWallpaper
        src/main.cpp
        src/gui.cpp
        src/gui.h
        src/chollian.cpp
        src/chollian.h
        src/image.cpp
        src/image.h
        src/utctime.cpp
        src/utctime.h)

target_link_libraries(ChollianWallpaper ${Boost_DATE_TIME_LIBRARY} ${Boost_LOCALE_LIBRARY})
target_link_libraries(ChollianWallpaper CURL::libcurl)
target_link_libraries(ChollianWallpaper Qt6::Widgets)

