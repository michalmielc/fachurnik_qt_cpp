#include "FileExportCif.h"
#include "FileLoader.h"
#include "QHash"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <algorithm>
#include <QRandomGenerator>

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
    QVector<QPair<QString, QString>> items,
    std::function<void(int, int)> progressCallback)
{
    const int totalColumns = cifColumns.size();

    for (int i = 0; i < totalColumns; ++i)
    {
        const columnSettings& settings = cifColumns[i].second;

        if (settings.isStaticField)
        {
            for (auto& item : items)
            {
                if (!item.second.isEmpty())
                    item.second += ",";

                item.second += "\"" + settings.staticValue + "\"";
            }
        }
        else if (settings.isEmpty)
        {
            for (auto& item : items)
            {
                if (!item.second.isEmpty())
                    item.second += ",";

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

            //SET DO USUWANIA ART BEZ CENY
            QSet<QString> itemsToRemove;

            for (auto& item : items)
            {
   

                if (!item.second.isEmpty())
                    item.second += ",";

                QString value = sourceMap.value(item.first, "");

                // BRAK DANYCH CALY ARTYKUL PRZEZNACZONY DO UUSUNIECIA
                //DODANIE DO SET
                if (settings.removeItemFromCat && value.isEmpty())
                {
                    itemsToRemove.insert(item.first);
                    continue;
                }

                if (settings.fillRandomVal && value.isEmpty())
                {
                    const QList<QString> values = sourceMap.values();

                    QList<QString> nonEmptyValues;
                    for (const QString& v : values)
                    {
                        if (!v.isEmpty())
                            nonEmptyValues.append(v);
                    }

                    if (!nonEmptyValues.isEmpty())
                    {
                        int index = QRandomGenerator::global()->bounded(nonEmptyValues.size());
                        value = nonEmptyValues[index];
                    }
                }

                // Zamiana ceny: 12,53 -> 12.53
                if (settings.convertPrice)
                {
                    value.replace(',', '.');
                }

                // Skracanie tekstu
                if (settings.cutString && settings.lengthToCut > 0)
                {
                    if (settings.addItemNumAtEnd)
                    {
                        const QString itemNumber = item.first;
                        const int maxLength = settings.lengthToCut;

                        // Ile miejsca pozostaje na podstawow¹ wartoœæ
                        const int availableValueLength =
                            maxLength - itemNumber.length();

                        if (availableValueLength > 0)
                        {
                            value = value.left(availableValueLength);
                            value += itemNumber;
                        }
                        else
                        {
                            // Numer artyku³u sam jest d³u¿szy ni¿ limit
                            value = itemNumber.left(maxLength);
                        }
                    }
                    else
                    {
                        value = value.left(settings.lengthToCut);
                    }
                }

                item.second += "\"" + value + "\"";
            }

            items.erase(
                std::remove_if(
                    items.begin(),
                    items.end(),
                    [&](const auto& item)
                    {
                        return itemsToRemove.contains(item.first);
                    }),
                items.end()
                        );
        }

        if (progressCallback)
            progressCallback(i + 1, totalColumns);
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