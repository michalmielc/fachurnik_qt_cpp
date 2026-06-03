#include "OpenFileDialog.h"

#include <QFileDialog>

QString OpenFileDialog::openFile(
    QWidget* parent,
    const QString& title,
    const QString& filter)
{
    return QFileDialog::getOpenFileName(
        parent,
        title,
        QString(),
        filter
    );
};

