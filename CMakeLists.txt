cmake_minimum_required(VERSION 3.19)
project(ChollianWallpaper)

set(CMAKE_CXX_STANDARD 20)

find_package(QT6 REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost COMPONENTS date_time locale REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})


add_executable(ChollianWallpaper main.cpp)

target_link_libraries(ChollianWallpaper ${Boost_DATE_TIME_LIBRARY} ${Boost_LOCALE_LIBRARY})
target_link_libraries(ChollianWallpaper CURL::libcurl)

