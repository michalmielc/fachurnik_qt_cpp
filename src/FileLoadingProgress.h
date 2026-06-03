#pragma once

#include <QProgressDialog>

class FileLoadingProgress : public QProgressDialog
{
public:
    explicit FileLoadingProgress(QWidget* parent = nullptr);

    void start();
    void finish();
};