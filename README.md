# ChollianWallPaper

![macOS 11](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/macOS11.yml/badge.svg) 
![macOS 10.15](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/macOS10.15.yml/badge.svg) 
![Windows](https://github.com/pjessesco/ChollianWallPaper/actions/workflows/windows.yml/badge.svg)


Inspired by [himawaripy](https://github.com/boramalper/himawaripy), ChollianWallPaper is simple GUI app that aims to set near-realtime image from [Chollian 2A](https://en.wikipedia.org/wiki/Chollian) as wallpaper.

## Download

Pre-built binary is available in `Actions` tab above for Intel macOS.

## Dependency

- CMake
- curl
- Qt6

 
## Build
 
### macOS
 
    brew install qt6 cmake ninja
    
    mkdir build
    cd build
    cmake -GNinja ..
    ninja
    
To generate bundle for macOS, run `scripts/bundle_macos.py`. 

### Windows

TBD

