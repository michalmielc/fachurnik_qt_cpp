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

    // READ HEADER AND DATA FILE PATH ETC
    static FileData loadCifHeaderFile(
        const QString& path,
        std::function<void(int)> progressCallback = nullptr
    );

    //READ ITEM NUMBERS FOR CSV
    static QVector<QPair<QString, QString>> loadCsvItemNumbers(
        const QString& path,
        std::function<void(int)> progressCallback);

    //READ COLUMN NAMES FROM CSV
   static  QStringList loadCsvHeaders(const QString& path);

   //READ SOURCE DATA FORM CSV 
   static QHash<QString, QString> loadCsvColumnToHash(
       const QString& path,
       int valueColumn,
       std::function<void(int)> progressCallback = nullptr);
};

