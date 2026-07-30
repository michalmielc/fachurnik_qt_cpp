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

    struct DatCompareItem
    {
        QString articleKey;
        QString currency;
        QString price1;
        QString price2;
        QString quantity1;
        QString quantity2;
        QString productGroup;

        double price1Value = 0.0;
        double price2Value = 0.0;

        bool price1Valid = false;
        bool price2Valid = false;
    };

    struct DatCompareResult
    {
        QStringList comparisonCsv;
        QStringList missingArticlesCsv;
        QStringList summaryCsv;

        int matchingRecords = 0;
        int differentRecords = 0;
        int onlyInFirstFile = 0;
        int onlyInSecondFile = 0;
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

    static DatCompareResult compareDatFiles(
        const FileData& firstFile,
        const FileData& secondFile,
        QProgressDialog* progress = nullptr
    );

    static QString buildDatArticleKey(
        const QStringList& parts
    );

    static QHash<QString, DatCompareItem> buildDatCompareMap(
        const FileData& data
    );
};