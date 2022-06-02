// Every satellite images used in this project are copyrighted by the Korea Meteorological
// Administration, and they can NOT be used for commercial purposes.
//
// Chollian Wallpaper is distributed under the MIT License.
//
// Copyright (c) 2021-2022 Jino Park
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <filesystem>
#if defined _WIN32
    #include <windows.h>
#endif

#include <QApplication>

#include "chollian.h"

int main(int argc, char *argv[]) {
    
#if defined _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    Chollian chollian;

    return app.exec();
}

