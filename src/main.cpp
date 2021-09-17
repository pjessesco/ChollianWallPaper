#include <QApplication>
#include <filesystem>

#include "gui.h"

int main(int argc, char *argv[]) {

    std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    GUI gui;

    return app.exec();
}

