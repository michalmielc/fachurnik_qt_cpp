#pragma once
#include <QString>
#include <QWidget>

class OpenFileDialog
{
public:
    static QString openFile(
        QWidget* parent = nullptr,
        const QString& title = "Wybierz plik",
        const QString& filter = "All Files (*.*)"
    );
};
