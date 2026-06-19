#pragma once

#include "FileData.h"

#include <QString>
#include <QStringList>
#include <QProgressDialog>

class FileExport
{
public:
    static QStringList buildModifiedLines(
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
};