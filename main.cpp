#include "fpInject.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    fpInject window;

    window.show();

    return app.exec();
}
