#pragma once

#include <QString>
#include "FileData.h"
#include <QProgressDialog>

class FileExport
{
public:
    static bool exportModifiedDatToDatDesktop(
        const FileData& data,
        const QString& headerLine,
        const QString& targetCurrency,
        double exchangeRate,
        const QString& customerNo,
        QProgressDialog* progress,
        QString* savedPath = nullptr,
        QString* error = nullptr
    );

    static bool exportModifiedDatToCsvDesktop(
        const QStringList& lines,
        const QString& filePath,
        QString* error
    );

    // FUNKCJA NIE GOTOWA DO EKSPORTU DO EXCELA
    //static bool exportModifiedDatToXlsDesktop(
    //    const QStringList& lines,
    //    const QString& filePath,
    //    QString* error
    //)
}; 
