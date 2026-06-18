#include "FileProcessingProgress.h"

#include <QApplication>

FileProcessingProgress::FileProcessingProgress(QWidget* parent)
    : QProgressDialog(parent)
{
    setWindowTitle("Wczytywanie");
    setLabelText("Trwa wczytywanie pliku...");
    setCancelButton(nullptr);

    setRange(0, 100);

    setWindowModality(Qt::WindowModal);

    setValue(0);
}

void FileProcessingProgress::start()
{
    setValue(10);

    QApplication::processEvents();
}

void FileProcessingProgress::finish()
{
    setValue(100);

    QApplication::processEvents();
}