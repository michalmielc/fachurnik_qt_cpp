#pragma once
#include <QProgressDialog>

class FileProcessingProgress : public QProgressDialog
{
public:
    explicit FileProcessingProgress(QWidget* parent = nullptr);

    void start();
    void finish();
};