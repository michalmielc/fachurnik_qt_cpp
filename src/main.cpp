#include "Fachurnik_C.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>

int main(int argc, char* argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");

    QApplication app(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    app.setStyleSheet(
        "* { color: black; background-color: #f0f0f0; }"
        "QLineEdit, QTextEdit, QPlainTextEdit, QComboBox { background-color: white; color: black; }"
        "QPushButton { background-color: #e6e6e6; color: black; }"
    );

    Fachurnik_C window;
    window.show();

    return app.exec();
}