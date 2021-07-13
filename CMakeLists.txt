cmake_minimum_required(VERSION 3.19)
project(ChollianWallpaper)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost COMPONENTS date_time locale REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ChollianWallpaper main.cpp gui.cpp chollian.cpp chollian.h)

target_link_libraries(ChollianWallpaper ${Boost_DATE_TIME_LIBRARY} ${Boost_LOCALE_LIBRARY})
target_link_libraries(ChollianWallpaper CURL::libcurl)
target_link_libraries(ChollianWallpaper Qt6::Widgets)

