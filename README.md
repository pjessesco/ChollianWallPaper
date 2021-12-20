# ChollianWallPaper

![macOS](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/macOS.yml/badge.svg)
![Windows](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/windows.yml/badge.svg)

![thumbnail](https://user-images.githubusercontent.com/11532321/135660418-79c9fd8f-a302-4ffa-b7cc-677e965657f8.gif)


Inspired by [himawaripy](https://github.com/boramalper/himawaripy), ChollianWallPaper is simple GUI app that aims to set near-realtime image from [Chollian 2A](https://en.wikipedia.org/wiki/Chollian) as wallpaper.


## Usage
After install and execute, Chollian Wallpaper is visible in your system tray. Right click to use or set.



## Features
 - Set near-realtime satellite image as wallpaper
 - 5 different color tones
<details>
  <summary>Preview</summary>
 
RGB True, Natural
![color tone](https://user-images.githubusercontent.com/11532321/135794556-d5fbc00f-a29c-457f-91fe-0dfcb02a05b8.png)
 
Water Vapor
![wv](https://user-images.githubusercontent.com/11532321/146769555-ffe4242d-a4a2-4fc0-8ceb-7bd85da0e48d.png)

Cloud
![cloud](https://user-images.githubusercontent.com/11532321/146769563-1a3134f2-12e9-4500-9f52-841b7355d8d3.png)

Ash
![ash](https://user-images.githubusercontent.com/11532321/146769571-eafa21ba-aee0-4651-b0ca-dd510e68d4a3.png)
</details>

 - Various image resolution from 1K to 8K
 - Various image ratio (4:3, 16:9, 16:10)
 - 2 download options :
     - `Quality` for higher image quality
     - `Performance` for lower memory/network usage


## Supported OS
- Windows
- macOS (>= Big Sur, both Intel and Apple Silicon)


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
3. Refer [github workflow](https://github.com/pjessesco/ChollianWallPaper/blob/main/.github/workflows/macOS.yml) for more details.


Building Qt can be replaced with installing it using homebrew, and CURL is embedded in macOS natively. Reason for building them is to offer universal binary for both Intel/Apple Silicon. See [2021.10](https://github.com/pjessesco/ChollianWallPaper/tree/2021.10) how we've done it before with homebrew.


### Windows

1. Install [Qt6](https://www.qt.io/download-qt-installer) and [CMake](https://cmake.org/download/)

2. Download pre-built curl. [link](https://curl.se/download.html)

3. Set environment variables.

4. Run below commands

-

    mkdir build
    cd build
    cmake -DCURL_LIBRARY=path/to/libcurl.dll.a -DCURL_INCLUDE_DIR=path/to/include ..
    MSBuild.exe PACKAGE.vcxproj -p:Configuration=Release


## License

**Every satellite images used in this project are copyrighted by the Korea Meteorological Administration. You can not use it for commercial purposes.**

This project is distributed under the [MIT license](https://github.com/pjessesco/ChollianWallPaper/blob/main/LICENSE).



