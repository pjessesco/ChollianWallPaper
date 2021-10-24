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

