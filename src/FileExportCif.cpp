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

//ZAPIS PLIKU PO PODMIANIE KOLUMNY
bool FileExportCif::saveRebuiltCifFile(
    const FileData& fileData,
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

    QString header = fileData.content;

    // Aktualizacja ITEMCOUNT
    QStringList headerLines =
        header.split('\n', Qt::KeepEmptyParts);

    for (QString& line : headerLines)
    {
        if (line.startsWith("ITEMCOUNT:"))
        {
            line =
                "ITEMCOUNT:" +
                QString::number(fileData.cifLines.size());

            break;
        }
    }

    // HEADER
    for (const QString& line : headerLines)
    {
        if (!line.isEmpty())
            out << line << "\n";
    }

    // DATA
    out << "DATA\n";

    // PRZEBUDOWANE WIERSZE
    for (const QString& line : fileData.cifLines)
        out << line << "\n";

    // END
    out << "ENDOFDATA\n";

    cifFile.close();

    return true;
}

QStringList FileExportCif::parseCifLine(const QString& line)
{
    QStringList columns;
    QString currentValue;

    bool insideQuotes = false;

    for (int i = 0; i < line.size(); ++i)
    {
        const QChar character = line[i];

        if (character == '"')
        {
            // Podwójny cudzys³ów wewn¹trz wartoœci
            if (insideQuotes &&
                i + 1 < line.size() &&
                line[i + 1] == '"')
            {
                currentValue += '"';
                ++i;
            }
            else
            {
                insideQuotes = !insideQuotes;
            }
        }
        else if (character == ',' && !insideQuotes)
        {
            columns.append(currentValue);
            currentValue.clear();
        }
        else
        {
            currentValue += character;
        }
    }

    columns.append(currentValue);

    return columns;
}

QString FileExportCif::rebuildCifLine(const QStringList& columns)
{
    QStringList result;
    result.reserve(columns.size());

    for (QString value : columns)
    {
        value.replace("\"", "\"\"");

        result.append("\"" + value + "\"");
    }

    return result.join(',');
}

void FileExportCif::replaceColumnInCif(
    FileData& fileData,
    int idColumnIndex,
    int targetColumnIndex,
    const columnSettings& settings,
    std::function<void(int, int)> progressCallback)
{
    if (idColumnIndex < 0 || targetColumnIndex < 0)
        return;

    QHash<QString, QString> sourceMap;

    if (settings.fromFile)
    {
        sourceMap = FileLoader::loadCsvColumnToHash(
            settings.path,
            settings.fileColNum
        );
    }

    QStringList randomValues;

    if (settings.fromFile && settings.fillRandomVal)
    {
        const QList<QString> values = sourceMap.values();

        for (const QString& value : values)
        {
            if (!value.isEmpty())
                randomValues.append(value);
        }
    }

    QStringList newCifLines;
    newCifLines.reserve(fileData.cifLines.size());

    const int totalLines = fileData.cifLines.size();

    for (int i = 0; i < totalLines; ++i)
    {
        const QString& line = fileData.cifLines[i];

        QStringList columns = parseCifLine(line);

        // Wiersz ma za ma³o kolumn
        if (idColumnIndex >= columns.size() ||
            targetColumnIndex >= columns.size())
        {
            newCifLines.append(line);
            continue;
        }

        // ID artyku³u pobrane z wybranej kolumny CIF
        const QString itemId =
            columns[idColumnIndex].trimmed();

        QString newValue;

        if (settings.isStaticField)
        {
            newValue = settings.staticValue;
        }
        else if (settings.isEmpty)
        {
            newValue.clear();
        }
        else if (settings.fromFile)
        {
            // Mapowanie: ID artyku³u -> wartoœæ z pliku
            newValue = sourceMap.value(itemId, QString());

            if (settings.removeItemFromCat &&
                newValue.isEmpty())
            {
                // Nie dodajemy wiersza do wyniku
                continue;
            }

            if (settings.fillRandomVal &&
                newValue.isEmpty() &&
                !randomValues.isEmpty())
            {
                const int randomIndex =
                    QRandomGenerator::global()->bounded(
                        randomValues.size()
                    );

                newValue = randomValues[randomIndex];
            }
        }
        else
        {
            // Nie wybrano ¿adnego sposobu podmiany
            newCifLines.append(line);
            continue;
        }

        if (settings.convertPrice)
        {
            newValue.replace(',', '.');
        }

        if (settings.cutString &&
            settings.lengthToCut > 0)
        {
            if (settings.addItemNumAtEnd)
            {
                const int availableLength =
                    settings.lengthToCut - itemId.length();

                if (availableLength > 0)
                {
                    newValue =
                        newValue.left(availableLength) +
                        itemId;
                }
                else
                {
                    newValue =
                        itemId.left(settings.lengthToCut);
                }
            }
            else
            {
                newValue =
                    newValue.left(settings.lengthToCut);
            }
        }

        // Podmiana tylko jednej wybranej kolumny
        columns[targetColumnIndex] = newValue;

        // Ponowne z³o¿enie ca³ego wiersza
        newCifLines.append(rebuildCifLine(columns));

        if (progressCallback)
        {
            progressCallback(i + 1, totalLines);
        }
    }

    fileData.cifLines = std::move(newCifLines);
}