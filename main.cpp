#include "gzinjectGUI.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    gzinjectGUI window;

    window.show();

    return app.exec();
}
