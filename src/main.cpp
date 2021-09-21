#include <filesystem>

#include <QApplication>

#include "chollian.h"

int main(int argc, char *argv[]) {
    std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    Chollian chollian;

    return app.exec();
}

