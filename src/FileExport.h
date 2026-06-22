#pragma once

#include "FileData.h"

#include <QString>
#include <QStringList>
#include <QProgressDialog>

class FileExport
{
public:
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
};