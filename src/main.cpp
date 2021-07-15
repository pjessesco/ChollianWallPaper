#include <QApplication>
#include "gui.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    GUI gui;

    return app.exec();
}

