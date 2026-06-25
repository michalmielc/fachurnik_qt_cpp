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

    static QVector<QPair<QString, QString>> loadCsvFileCustomerList(
        const QString& path,
        std::function<void(int)> progressCallback);

    static FileData loadCsvPriceFile(
        const QString& path,
        std::function<void(int)> progressCallback = nullptr
    );
};

