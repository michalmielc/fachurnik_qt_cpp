#include "FileExportCif.h"
#include "FileLoader.h"
#include "QHash"
#include <QFile>
#include <QTextStream>

QVector<QPair<QString, FileExportCif::columnSettings>>
FileExportCif::createCifColumns(const QStringList& fields)
{
    cifColumns.clear();
    cifColumns.reserve(fields.size());

    for (const QString& field : fields)
    {
        columnSettings settings;
        settings.colName = field;

        cifColumns.append({ field, settings });
    }

    return cifColumns;
}


QVector<QPair<QString, QString>> FileExportCif::buildRows(
    QVector<QPair<QString, QString>> items)
{

    for (const auto& column : cifColumns)
    {
        const columnSettings& settings = column.second;

        if (settings.isStaticField)
        {
            for (auto& item : items)
            {
                if (!item.second.isEmpty())
                    item.second += ";";

                item.second += "\"" + settings.staticValue + "\"";
            }
        }
        else if (settings.isEmpty)
        {
            for (auto& item : items)
            {
                if (!item.second.isEmpty())
                    item.second += ";";

                item.second += "\"\"";
            }
        }
        else if (settings.fromFile)
        {
            QHash<QString, QString> sourceMap =
                FileLoader::loadCsvColumnToHash(
                    settings.path,
                    settings.fileColNum
                );

            for (auto& item : items)
            {
                if (!item.second.isEmpty())
                    item.second += ";";

                QString value = sourceMap.value(item.first, "");

                item.second += "\"" + value + "\"";
            }
        }
    }

    return items;
}



bool FileExportCif::saveCifFile(
    const QStringList& header,
    const QVector<QPair<QString, QString>>& rows,
    const QString& filePath,
    QString* error
)
{
    QFile cifFile(filePath);

    if (!cifFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error)
            *error = "Nie mo¿na zapisaæ pliku CIF.";

        return false;
    }

    QTextStream out(&cifFile);
    out.setEncoding(QStringConverter::Utf8);

    //ZLICZENIE ILOSCI

    QStringList newHeader = header;

    for (QString& line : newHeader)
    {
        if (line.startsWith("ITEMCOUNT:"))
        {
            line = "ITEMCOUNT:" + QString::number(rows.size());
            break;
        }
    }


    // HEADER
    for (const QString& line : newHeader)
        out << line << "\n";

    // DATA
    out << "DATA\n";

    // ROWS
    for (const auto& row : rows)
        out << row.second << "\n";

    // END
    out << "ENDOFDATA\n";

    cifFile.close();

    return true;
}