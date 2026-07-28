#pragma once
#include "FileData.h"
#include <QString>
#include <QStringList>
#include <QProgressDialog>
#include <QHash>

class FileExport
{

public:

    struct CsvPriceData
    {
        QString price1;
        QString price2;
    };

    static QStringList buildDatModifiedLines(
        const FileData& data,
        const QString& headerLine,
        const QString& targetCurrency,
        double exchangeRate,
        QProgressDialog* progress
    );

    static bool saveDat(
        const QStringList& lines,
        const QString& filePath,
        QString* error
    );

    static bool saveCsv(
        const QStringList& lines,
        const QString& filePath,
        QString* error
    );

    static bool parseDatPrice(const QString& text, double& value);

    static QStringList buildCsvModifiedLines(
        const FileData& data,
        const QString& headerLine,
        const QString& targetCurrency,
        double exchangeRate,
        QProgressDialog* progress
    );

    static QHash<QString, FileExport::CsvPriceData> buildPriceMapFromCsv(
        const FileData& dataCsv
    );

    static QStringList buildDatModifiedLinesWithCsvPrices(
        const FileData& data,
        const FileData& dataCsv,
        const QString& headerLine,
        const QString& targetCurrency,
        double exchangeRate,
        QProgressDialog* progress
    );

    static bool createBat(
        const QString& folderPath,
        QString* error = nullptr
    );
};