#include "FileLoadingProgress.h"

#include <QApplication>

FileLoadingProgress::FileLoadingProgress(QWidget* parent)
    : QProgressDialog(parent)
{
    setWindowTitle("Wczytywanie");
    setLabelText("Trwa wczytywanie pliku...");
    setCancelButton(nullptr);

    setRange(0, 100);

    setWindowModality(Qt::WindowModal);

    setValue(0);
}

void FileLoadingProgress::start()
{
    setValue(10);

    QApplication::processEvents();
}

void FileLoadingProgress::finish()
{
    setValue(100);

    QApplication::processEvents();
}