#include <QApplication>
#include "gui.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    GUI gui(argv[0]);

    return app.exec();
}

