cmake_minimum_required(VERSION 3.16)
project(ChollianWallpaper)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(CURL REQUIRED)

include_directories(ext)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DCOMPILER_NAME="${CMAKE_CXX_COMPILER_ID}")
add_definitions(-DCOMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
add_definitions(-DCURL_VERSION="${CURL_VERSION_STRING}")

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
        src/logger.h
        src/about.cpp
        src/about.h)

target_link_libraries(ChollianWallpaper CURL::libcurl)
target_link_libraries(ChollianWallpaper Qt6::Widgets)

