cmake_minimum_required(VERSION 3.21)
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

set(CHOLLIAN_VERSION "2021.10")

add_definitions(-DCHOLLIAN_VERSION="${CHOLLIAN_VERSION}")
add_definitions(-DGIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DCOMPILER_NAME="${CMAKE_CXX_COMPILER_ID}")
add_definitions(-DCOMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
add_definitions(-DCURL_VERSION="${CURL_VERSION_STRING}")
add_definitions(-DCMAKE_CONFIG="${CMAKE_BUILD_TYPE}")

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
        src/about.h
        src/dependency_license.h)

target_link_libraries(ChollianWallpaper CURL::libcurl)
target_link_libraries(ChollianWallpaper Qt6::Widgets)

# Requires admin privileges for Windows
if(MSVC)
    set_target_properties(ChollianWallpaper PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
endif()

#################
#   Packaging   #
#################

set(CPACK_PACKAGE_NAME "Chollian Wallpaper")
set(CPACK_PACKAGE_VERSION "${CHOLLIAN_VERSION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/pjessesco/ChollianWallPaper")
set(CPACK_PACKAGE_DESCRIPTION "Wallpaper changer using near-realtime satellite images(Chollian 2A from South Korea)")
set(CPACK_PACKAGE_FILE_NAME "ChollianInstaller")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Chollian Wallpaper")

if(MSVC)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_PACKAGE_NAME "Chollian Wallpaper ${CPACK_PACKAGE_VERSION}")
    set(CPACK_NSIS_DISPLAY_NAME "Chollian Wallpaper ${CPACK_PACKAGE_VERSION}")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDIR /r \"$INSTDIR\resources\"")

    install(TARGETS ChollianWallpaper DESTINATION .)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION . FILES_MATCHING PATTERN "*.*")
    install(FILES "${CURL_INCLUDE_DIR}/../bin/libcurl-x64.dll" DESTINATION .)
    add_custom_command(TARGET ChollianWallpaper POST_BUILD
        COMMAND windeployqt.exe 
                --release
                --verbose 0
                --no-compiler-runtime
                --no-opengl
                --no-opengl-sw
                --no-quick-import
                --no-translations
                --dir ${CMAKE_BINARY_DIR}/qtDeploy $<TARGET_FILE:ChollianWallpaper>
    )
    install(DIRECTORY ${CMAKE_BINARY_DIR}/qtDeploy/ DESTINATION . FILES_MATCHING PATTERN "*.*")
    include(CPack)
elseif(APPLE)
    install(FILES "${CURL_INCLUDE_DIR}/../bin/libcurl-x64.dll" DESTINATION .)
    add_custom_command(TARGET ChollianWallpaper POST_BUILD
        COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/bundle_macos.py
                --qt_dir=/usr/local/Qt-6.2.2/
                --build_dir=${CMAKE_BINARY_DIR}
                --version=${CHOLLIAN_VERSION}
    )
endif()


