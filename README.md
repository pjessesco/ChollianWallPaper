# ChollianWallPaper

![macOS](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/macOS.yml/badge.svg)
![Windows](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/windows.yml/badge.svg)

![thumbnail](https://user-images.githubusercontent.com/11532321/135660418-79c9fd8f-a302-4ffa-b7cc-677e965657f8.gif)


Inspired by [himawaripy](https://github.com/boramalper/himawaripy), ChollianWallPaper is simple GUI app that aims to set near-realtime image from [Chollian 2A](https://en.wikipedia.org/wiki/Chollian) as wallpaper.

## Features
 - Set near-realtime satelite image as wallpaper
 - 2 different image tone (RGB True, Natural)

  ![color tone](https://user-images.githubusercontent.com/11532321/135794556-d5fbc00f-a29c-457f-91fe-0dfcb02a05b8.png)

 - Various image resolution from 360p to 8K
 - Various image ratio (4:3, 16:9, 16:10)


### Experimental feature
 - Use East Asia image

![eastasia](https://user-images.githubusercontent.com/11532321/135794995-660331b5-fb8c-4530-befd-f6d92e0469c5.png)



## Download

Pre-built binary is available in `Actions` tab above for
 - [macOS (Intel)](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/macOS.yml) (Not tested yet on Apple Silicon)
 - [Windows (experimental)](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/windows.yml)

## Dependency

- CMake
- curl
- Qt6

 
## Build

Refer [github workflow](https://github.com/pjessesco/ChollianWallPaper/tree/main/.github/workflows) for more details.

### macOS
 
    brew install qt6 cmake
    
    mkdir build
    cd build
    cmake ..
    make
    
To generate bundle for macOS, run `scripts/bundle_macos.py`. 

### Windows

1. Install [Qt6](https://www.qt.io/download-qt-installer) and [CMake](https://cmake.org/download/)

2. Download pre-built curl. [link](https://curl.se/download.html)

3. Set environment variables.

4. Run below commands

-

    mkdir build
    cd build
    cmake -DCURL_LIBRARY=path/to/libcurl.dll.a -DCURL_INCLUDE_DIR=path/to/include ..

5. Open `ChollianWallpaper.sln` and build proejct.

6. You have to copy `libcurl-x64.dll` to the directory where executable exists.

