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
 - 2 download options :
     - `Quality` for higher image quality
     - `Performance` for lower memory/network usage


### Experimental feature
 - Use East Asia image

![eastasia](https://user-images.githubusercontent.com/11532321/135794995-660331b5-fb8c-4530-befd-f6d92e0469c5.png)


## Supported OS
- Windows
- macOS (>= Big Sur, both for Intel and Apple Silicon)


## Download

Release binaries are able in [here](https://github.com/pjessesco/ChollianWallPaper/releases/).
Under-development/debug versions are available in [here](https://github.com/pjessesco/ChollianWallPaper/actions).


## Dependency

- CMake
- curl
- Qt6

 
## Build

### macOS
 
    1. Build Qt>=6.2.0 and CURL.
    2. Build Chollian using cmake.
    3. Refer [github workflow](https://github.com/pjessesco/ChollianWallPaper/tree/main/.github/macOS.yml) for more details.

Building Qt can be replaced with installing it using homebrew, and CURL is embedded in macOS natively. Reason for building them is to offer universal binary for both Intel/Apple Silicon, and `scripts/bundle_macos.py` is written for the case.  See [2021.10](https://github.com/pjessesco/ChollianWallPaper/tree/2021.10) how we've done it before with homebrew.


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

To generate installer for Windows, you have to install [NSIS](https://nsis.sourceforge.io/Download). Use can use `scripts/nsis_windows.nsh` but it is hard-coded for github workflows. You should modify it to run on your environment.


## License

**Every satelite images used in this project are copyrighted by the Korea Meteorological Administration. You can not use it for commercial purposes.**

This project is distributed under the [MIT license](https://github.com/pjessesco/ChollianWallPaper/blob/main/LICENSE).



