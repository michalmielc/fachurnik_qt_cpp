#include "Fachurnik_C.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Fachurnik_C window;
    window.show();
    return app.exec();
}
