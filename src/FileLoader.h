#pragma once

#include "FileData.h"
#include <functional>

class FileLoader
{
public:
    static FileData loadDatFile(
        const QString& path,
        std::function<void(int)> progressCallback = nullptr
    );

    QVector<QPair<QString, QString>> loadCsvFile(
        const QString& path,
        std::function<void(int)> progressCallback);
};

