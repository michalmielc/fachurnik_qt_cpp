#include "FileExport.h"
#include <QRegularExpression>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "qmessagebox.h"
#include <QDir>
#include <QSet>
#include <algorithm>
#include <cmath>



QStringList FileExport::buildDatModifiedLines(
    const FileData& data,
    const QString& headerLine,
    const QString& targetCurrency,
    double exchangeRate,
    QProgressDialog* progress
)
{
    QStringList lines = data.content.split('\n', Qt::KeepEmptyParts);

    if (!lines.isEmpty())
        lines[0] = headerLine;

    int total = lines.size();

    for (int i = 1; i < lines.size(); ++i)
    {
        if (progress && total > 0)
        {
            progress->setValue((i * 80) / total);
            QApplication::processEvents();
        }

        if (!lines[i].startsWith("L|"))
            continue;

        QStringList p = lines[i].split('|', Qt::KeepEmptyParts);

        if (p.size() <= 9)
            continue;

        QString oldCurrency = p[6];

      
            p[6] = targetCurrency;

            double oldValuePrice1 = 0.0;
            double oldValuePrice2 = 0.0;

            bool ok1 = parseDatPrice(p[8], oldValuePrice1);
            bool ok2 = parseDatPrice(p[9], oldValuePrice2);

            if (!ok1 || !ok2)
            {
                QMessageBox::critical(nullptr, "OPERATION ABORTED!", QString("WRONG PRICE VALUE AT LINE: %1").arg(i + 1));
                return {};

            }

            double newValuePrice1 = oldValuePrice1 * exchangeRate;
            double newValuePrice2 = oldValuePrice2 * exchangeRate;

            p[8] = QString::number(newValuePrice1, 'f', 2).replace('.', ',');
            p[9] = QString::number(newValuePrice2, 'f', 2).replace('.', ',');
            lines[i] = p.join('|');
        
    }

    return lines;
}

bool FileExport::saveDat(
    const QStringList& lines,
    const QString& filePath,
    QString* error
)
{
    QFile outFile(filePath);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error)
            *error = "Nie można zapisać pliku DAT.";

        return false;
    }

    QTextStream out(&outFile);

    for (const QString& line : lines)
        out << line << "\n";

    outFile.close();

    return true;
}


bool FileExport::saveCsv(
    const QStringList& lines,
    const QString& filePath,
    QString* error
)
{
    QFile csvFile(filePath);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error)
            *error = "Nie można zapisać pliku CSV.";

        return false;
    }

    QTextStream out(&csvFile);
    out.setEncoding(QStringConverter::Utf8);

    for (const QString& line : lines)
    {
        out << line << "\n";
    }

    csvFile.close();

    return true;
}

bool FileExport::parseDatPrice(const QString& text, double& value)
{
    QString s = text.trimmed();

    if (s.isEmpty())
        return false;

    QRegularExpression rx(
        R"(^(?:\d{1,3}(?:\.\d{3})*|\d+)(?:,\d+)?$)"
    );

    if (!rx.match(s).hasMatch())
        return false;

    s.remove('.');
    s.replace(',', '.');

    bool ok = false;
    value = s.toDouble(&ok);

    return ok;
};

   
QStringList FileExport::buildCsvModifiedLines(
        const FileData & data,
        const QString & headerLine,
        const QString & targetCurrency,
        double exchangeRate,
        QProgressDialog * progress
    )
    {
        QStringList sourceLines = data.content.split('\n', Qt::SkipEmptyParts);
        QStringList outputLines;

        //NEW HEADER:
        outputLines << "articleNo;articleName;dateFrom;dateTo;pricetype1;price1;currency1;tax1;amount1;price2;currency2;tax2;amount2";

        int total = sourceLines.size();

        for (int i = 1; i < sourceLines.size(); ++i)
        {
            if (progress && total > 0)
            {
                progress->setValue((i * 80) / total);
                QApplication::processEvents();
            }

            if (!sourceLines[i].startsWith("L|"))
                continue;

            QStringList p = sourceLines[i].split('|', Qt::KeepEmptyParts);

            if (p.size() <= 15)
                continue;

            p[6] = targetCurrency;

            double oldValuePrice1 = 0.0;
            double oldValuePrice2 = 0.0;

            bool ok1 = parseDatPrice(p[8], oldValuePrice1);
            bool ok2 = parseDatPrice(p[9], oldValuePrice2);

            if (!ok1 || !ok2)
            {
                QMessageBox::critical(
                    nullptr,
                    "OPERATION ABORTED!",
                    QString("WRONG PRICE VALUE AT LINE: %1").arg(i + 1)
                );
                return {};
            }

            double newValuePrice1 = oldValuePrice1 * exchangeRate;
            double newValuePrice2 = oldValuePrice2 * exchangeRate;

            QString price1 = QString::number(newValuePrice1, 'f', 2);
            QString price2 = QString::number(newValuePrice2, 'f', 2);

            QString articleNo = p[2];

            if (!p[3].trimmed().isEmpty())
                articleNo += " " + p[3];

            int lowerBound1 = static_cast<int>(p[14].toDouble());
            int lowerBound2 = static_cast<int>(p[15].toDouble());

            QString articleName = p[7].remove(';');

            QStringList newLine;

            newLine
                << articleNo
                << articleName
                << data.header.dateFrom
                << data.header.dateTo
                << "net_customer"
                << price1
                << p[6]
                << "0.23"
                << QString::number(lowerBound1);

            if (newValuePrice2 != 0.0)
            {
                newLine
                    << price2
                    << p[6]
                    << "0.23"
                    << QString::number(lowerBound2);
            }

            outputLines << newLine.join(';');
        }

        return outputLines;
    }

QStringList FileExport::buildDatModifiedLinesWithTZ(
    const FileData& data,
    const QString& headerLine,
    const QString& targetCurrency,
    double exchangeRate,
    const QHash<QString, double>& tzMap,
    bool additionalEnabled,
    double additionalPercent,
    bool additionalIsPlus,
    QProgressDialog* progress
)
{
    QStringList lines = data.content.split('\n', Qt::KeepEmptyParts);

    if (!lines.isEmpty())
        lines[0] = headerLine;

    int total = lines.size();

    for (int i = 1; i < lines.size(); ++i)
    {
        if (progress && total > 0)
        {
            progress->setValue((i * 80) / total);
            QApplication::processEvents();
        }

        if (!lines[i].startsWith("L|"))
            continue;

        QStringList p = lines[i].split('|', Qt::KeepEmptyParts);

        if (p.size() <= 9)
            continue;

        p[6] = targetCurrency;

        double oldValuePrice1 = 0.0;
        double oldValuePrice2 = 0.0;

        bool ok1 = parseDatPrice(p[8], oldValuePrice1);
        bool ok2 = parseDatPrice(p[9], oldValuePrice2);

        if (!ok1 || !ok2)
        {
            QMessageBox::critical(
                nullptr,
                "OPERATION ABORTED!",
                QString("WRONG PRICE VALUE AT LINE: %1").arg(i + 1)
            );

            return {};
        }

        double newValuePrice1 = oldValuePrice1 * exchangeRate;
        double newValuePrice2 = oldValuePrice2 * exchangeRate;

        // ==========================
        // TZ SURCHARGE
        // ==========================

        QString articleNo = p[2].trimmed();

        if (!p[3].trimmed().isEmpty())
            articleNo += " " + p[3].trimmed();

        if (tzMap.contains(articleNo))
        {
            double tzPercent = tzMap.value(articleNo);

            newValuePrice1 *= (1.0 + tzPercent / 100.0);
            newValuePrice2 *= (1.0 + tzPercent / 100.0);
        }

        if (additionalEnabled)
        {
            double factor = additionalPercent / 100.0;

            if (additionalIsPlus)
            {
                newValuePrice1 *= (1.0 + factor);
                newValuePrice2 *= (1.0 + factor);
            }
            else
            {
                newValuePrice1 *= (1.0 - factor);
                newValuePrice2 *= (1.0 - factor);
            }
        }
        // ==========================

        p[8] = QString::number(newValuePrice1, 'f', 2).replace('.', ',');
        p[9] = QString::number(newValuePrice2, 'f', 2).replace('.', ',');

        lines[i] = p.join('|');
    }

    return lines;
}

QStringList FileExport::buildDatModifiedLinesWithCsvPrices(
    const FileData& data,
    const FileData& dataCsv,
    const QString& headerLine,
    const QString& targetCurrency,
    double exchangeRate,
    QProgressDialog* progress
)
{
    QHash<QString, CsvPriceData> priceMap = buildPriceMapFromCsv(dataCsv);

    QStringList lines = data.content.split('\n', Qt::KeepEmptyParts);

    if (!lines.isEmpty())
        lines[0] = headerLine;

    int total = lines.size();

    for (int i = 1; i < lines.size(); ++i)
    {
        if (progress && total > 0)
        {
            progress->setValue((i * 80) / total);
            QApplication::processEvents();
        }

        if (!lines[i].startsWith("L|"))
            continue;

        QStringList p = lines[i].split('|', Qt::KeepEmptyParts);

        if (p.size() <= 9)
            continue;

        QString articleNo = p[2].trimmed();

        if (!p[3].trimmed().isEmpty())
            articleNo += " " + p[3].trimmed();

        if (priceMap.contains(articleNo))
        {
            const CsvPriceData& csvPrice = priceMap[articleNo];

            p[8] = csvPrice.price1;
            p[9] = csvPrice.price2;
        }

        p[6] = targetCurrency;

        double oldValuePrice1 = 0.0;
        double oldValuePrice2 = 0.0;

        bool ok1 = parseDatPrice(p[8], oldValuePrice1);
        bool ok2 = parseDatPrice(p[9], oldValuePrice2);

        if (!ok1 || !ok2)
        {
            QMessageBox::critical(
                nullptr,
                "OPERATION ABORTED!",
                QString("WRONG PRICE VALUE AT LINE: %1").arg(i + 1)
            );
            return {};
        }

        double newValuePrice1 = oldValuePrice1 * exchangeRate;
        double newValuePrice2 = oldValuePrice2 * exchangeRate;

        p[8] = QString::number(newValuePrice1, 'f', 2).replace('.', ',');
        p[9] = QString::number(newValuePrice2, 'f', 2).replace('.', ',');

        lines[i] = p.join('|');
    }

    return lines;
}

QHash<QString, FileExport::CsvPriceData> FileExport::buildPriceMapFromCsv(
    const FileData& dataCsv
)
{
    QHash<QString, CsvPriceData> priceMap;

    QStringList lines = dataCsv.content.split('\n', Qt::SkipEmptyParts);

    for (int i = 0; i < lines.size(); ++i)
    {
        QString line = lines[i].trimmed();

        if (line.isEmpty())
            continue;

        QStringList p = line.split(';', Qt::KeepEmptyParts);

        if (p.size() < 3)
            continue;

        QString articleNo = p[0].trimmed();
        QString price1 = p[1].trimmed();
        QString price2 = p[2].trimmed();

        if (articleNo.isEmpty())
            continue;

        priceMap.insert(articleNo, { price1, price2 });
    }

    return priceMap;
}

//Creating a batch file to manage the frequency of price list delivery
bool FileExport::createBat(const QString& folderPath, QString* error)
{
    const QString batPath =
        folderPath
        + QDir::separator()
        + "send_price_lists.bat";

    QFile file(batPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error)
        {
            *error =
                "Nie udało się utworzyć pliku BAT:\n"
                + batPath
                + "\n\n"
                + file.errorString();
        }

        return false;
    }

    QTextStream stream(&file);

    stream << "@echo off\r\n";
    stream << "setlocal EnableDelayedExpansion\r\n";
    stream << "\r\n";

    // Folder, w którym znajduje się plik BAT.
    stream << "set \"SRC=%~dp0\"\r\n";

    stream << "\r\n";
    stream << "REM CHECK AND UPDATE THIS PATH\r\n";
    stream << "set \"DST=Z:\\POP\\pi_exchange\\100\\preisdaten\\pi\\2400\"\r\n";

    stream << "\r\n";
    stream << "if not exist \"%DST%\" (\r\n";
    stream << "    echo ERROR: Destination folder does not exist:\r\n";
    stream << "    echo %DST%\r\n";
    stream << "    pause\r\n";
    stream << "    exit /b 1\r\n";
    stream << ")\r\n";

    stream << "\r\n";
    stream << "set /a TOTAL=0\r\n";

    stream << "\r\n";
    stream << "for %%F in (\"%SRC%*.dat\") do (\r\n";
    stream << "    set /a TOTAL+=1\r\n";
    stream << ")\r\n";

    stream << "\r\n";
    stream << "if !TOTAL! EQU 0 (\r\n";
    stream << "    echo No DAT files to move.\r\n";
    stream << "    pause\r\n";
    stream << "    exit /b 0\r\n";
    stream << ")\r\n";

    stream << "\r\n";
    stream << "set /a DONE=0\r\n";
    stream << "echo Files to move: !TOTAL!\r\n";
    stream << "echo.\r\n";

    stream << "\r\n";
    stream << "for %%F in (\"%SRC%*.dat\") do (\r\n";
    stream << "    echo ================================================\r\n";
    stream << "    echo Moving: %%~nxF\r\n";

    stream << "\r\n";
    stream << "    move /Y \"%%~fF\" \"%DST%\\\" >nul\r\n";

    stream << "\r\n";
    stream << "    if exist \"%%~fF\" (\r\n";
    stream << "        echo ERROR: The file still exists in the source folder.\r\n";
    stream << "    ) else (\r\n";
    stream << "        set /a DONE+=1\r\n";
    stream << "        set /a PERCENT=DONE*100/TOTAL\r\n";
    stream << "        echo [!DONE!/!TOTAL!] !PERCENT!%%\r\n";
    stream << "        echo File moved successfully.\r\n";
    stream << "    )\r\n";

    stream << "\r\n";
    stream << "    if !DONE! LSS !TOTAL! (\r\n";
    stream << "        echo Waiting 45 seconds...\r\n";
    stream << "        REM CHECK AND UPDATE THIS INTERVAL\r\n";
    stream << "        timeout /t 45 /nobreak >nul\r\n";
    stream << "    )\r\n";

    stream << ")\r\n";

    stream << "\r\n";
    stream << "echo.\r\n";
    stream << "echo ================================================\r\n";
    stream << "echo Finished. Moved !DONE! of !TOTAL! files.\r\n";
    stream << "pause\r\n";
    stream << "endlocal\r\n";

    file.close();

    return true;
}


QString FileExport::buildDatArticleKey(
    const QStringList& parts
)
{
    if (parts.size() <= 3)
        return {};

    const QString articleNumber =
        parts[2].trimmed();

    const QString articleSize =
        parts[3].trimmed();

    if (articleNumber.isEmpty())
        return {};

    if (articleSize.isEmpty())
        return articleNumber;

    return articleNumber + " " + articleSize;
}

QHash<QString, FileExport::DatCompareItem>
FileExport::buildDatCompareMap(
    const FileData& data
)
{
    QHash<QString, DatCompareItem> result;

    const QStringList lines =
        data.content.split(
            '\n',
            Qt::SkipEmptyParts
        );

    for (const QString& sourceLine : lines)
    {
        const QString line =
            sourceLine.trimmed();

        if (!line.startsWith("L|"))
            continue;

        const QStringList parts =
            line.split(
                '|',
                Qt::KeepEmptyParts
            );

        if (parts.size() <= 15)
            continue;

        const QString articleKey =
            buildDatArticleKey(parts);

        if (articleKey.isEmpty())
            continue;

        DatCompareItem item;

        item.articleKey = articleKey;
        item.currency = parts[6].trimmed();
        item.price1 = parts[8].trimmed();
        item.price2 = parts[9].trimmed();
        item.quantity1 = parts[14].trimmed();
        item.quantity2 = parts[15].trimmed();

        if (!parts.isEmpty())
        {
            item.productGroup =
                parts.last().trimmed();
        }

        item.price1Valid =
            parseDatPrice(
                item.price1,
                item.price1Value
            );

        item.price2Valid =
            parseDatPrice(
                item.price2,
                item.price2Value
            );

        result.insert(
            articleKey,
            item
        );
    }

    return result;
}


FileExport::DatCompareResult
FileExport::compareDatFiles(
    const FileData& firstFile,
    const FileData& secondFile,
    QProgressDialog* progress
)
{
    DatCompareResult result;

    const QHash<QString, DatCompareItem> firstMap =
        buildDatCompareMap(firstFile);

    const QHash<QString, DatCompareItem> secondMap =
        buildDatCompareMap(secondFile);

    result.comparisonCsv
        << "articleNo;"
        "currencyFile1;"
        "price1File1;"
        "price2File1;"
        "quantity1File1;"
        "quantity2File1;"
        "currencyFile2;"
        "price1File2;"
        "price2File2;"
        "quantity1File2;"
        "quantity2File2;"
        "price1Difference;"
        "price1Status;"
        "price2Difference;"
        "price2Status;"
        "productGroup";

    result.missingArticlesCsv
        << "status;articleNo";

    QSet<QString> allKeys;

    for (auto it = firstMap.constBegin();
        it != firstMap.constEnd();
        ++it)
    {
        allKeys.insert(it.key());
    }

    for (auto it = secondMap.constBegin();
        it != secondMap.constEnd();
        ++it)
    {
        allKeys.insert(it.key());
    }

    QStringList sortedKeys =
        allKeys.values();

    std::sort(
        sortedKeys.begin(),
        sortedKeys.end(),
        [](const QString& left, const QString& right)
        {
            return QString::localeAwareCompare(
                left,
                right
            ) < 0;
        }
    );

    /*
     * Klucz: grupa produktowa.
     * Wartość: liczba artykułów z różnicami cenowymi.
     */
    QHash<QString, int> differentProductGroups;

    constexpr double priceTolerance = 0.005;

    const int total = sortedKeys.size();

    for (int i = 0; i < total; ++i)
    {
        if (progress && total > 0)
        {
            progress->setValue(
                (i * 100) / total
            );

            progress->setLabelText(
                QString("Porównywanie artykułu %1 z %2")
                .arg(i + 1)
                .arg(total)
            );

            QApplication::processEvents();
        }

        const QString& articleKey =
            sortedKeys[i];

        const bool existsInFirst =
            firstMap.contains(articleKey);

        const bool existsInSecond =
            secondMap.contains(articleKey);

        /*
         * Artykuł tylko w pierwszym pliku.
         */
        if (existsInFirst && !existsInSecond)
        {
            result.missingArticlesCsv
                << QString("ONLY_IN_FILE_1;%1")
                .arg(articleKey);

            ++result.onlyInFirstFile;
            continue;
        }

        /*
         * Artykuł tylko w drugim pliku.
         */
        if (!existsInFirst && existsInSecond)
        {
            result.missingArticlesCsv
                << QString("ONLY_IN_FILE_2;%1")
                .arg(articleKey);

            ++result.onlyInSecondFile;
            continue;
        }

        const DatCompareItem& firstItem =
            firstMap[articleKey];

        const DatCompareItem& secondItem =
            secondMap[articleKey];

        bool price1Equal = false;
        bool price2Equal = false;

        double price1Difference = 0.0;
        double price2Difference = 0.0;

        /*
         * Porównanie ceny 1.
         */
        if (firstItem.price1Valid &&
            secondItem.price1Valid)
        {
            price1Difference =
                std::abs(
                    firstItem.price1Value -
                    secondItem.price1Value
                );

            price1Equal =
                price1Difference <
                priceTolerance;
        }
        else
        {
            price1Equal =
                firstItem.price1 ==
                secondItem.price1;
        }

        /*
         * Porównanie ceny 2.
         */
        if (firstItem.price2Valid &&
            secondItem.price2Valid)
        {
            price2Difference =
                std::abs(
                    firstItem.price2Value -
                    secondItem.price2Value
                );

            price2Equal =
                price2Difference <
                priceTolerance;
        }
        else
        {
            price2Equal =
                firstItem.price2 ==
                secondItem.price2;
        }

        const QString price1DifferenceText =
            QString::number(
                price1Difference,
                'f',
                2
            ).replace('.', ',');

        const QString price2DifferenceText =
            QString::number(
                price2Difference,
                'f',
                2
            ).replace('.', ',');

        const QString price1Status =
            price1Equal
            ? "OK"
            : "DIFFERENCE";

        const QString price2Status =
            price2Equal
            ? "OK"
            : "DIFFERENCE";

        QString productGroup =
            firstItem.productGroup;

        if (productGroup.isEmpty())
            productGroup = secondItem.productGroup;

        QStringList outputRow;

        outputRow
            << articleKey
            << firstItem.currency
            << firstItem.price1
            << firstItem.price2
            << firstItem.quantity1
            << firstItem.quantity2
            << secondItem.currency
            << secondItem.price1
            << secondItem.price2
            << secondItem.quantity1
            << secondItem.quantity2
            << price1DifferenceText
            << price1Status
            << price2DifferenceText
            << price2Status
            << productGroup;

        result.comparisonCsv
            << outputRow.join(';');

        if (price1Equal && price2Equal)
        {
            ++result.matchingRecords;
        }
        else
        {
            ++result.differentRecords;

            /*
             * Zbieramy grupy z obu plików.
             * QSet zapobiega podwójnemu policzeniu tej samej
             * grupy dla jednego artykułu.
             */
            QSet<QString> groupsForArticle;

            if (!firstItem.productGroup.isEmpty())
            {
                groupsForArticle.insert(
                    firstItem.productGroup
                );
            }

            if (!secondItem.productGroup.isEmpty())
            {
                groupsForArticle.insert(
                    secondItem.productGroup
                );
            }

            for (const QString& group : groupsForArticle)
            {
                differentProductGroups[group]++;
            }
        }
    }

    /*
     * Podsumowanie.
     */
    result.summaryCsv
        << "name;value";

    result.summaryCsv
        << QString("matchingRecords;%1")
        .arg(result.matchingRecords);

    result.summaryCsv
        << QString("differentRecords;%1")
        .arg(result.differentRecords);

    result.summaryCsv
        << QString("onlyInFile1;%1")
        .arg(result.onlyInFirstFile);

    result.summaryCsv
        << QString("onlyInFile2;%1")
        .arg(result.onlyInSecondFile);

    result.summaryCsv << "";

    result.summaryCsv
        << "productGroup;differentArticlesCount";

    QStringList sortedGroups =
        differentProductGroups.keys();

    std::sort(
        sortedGroups.begin(),
        sortedGroups.end(),
        [](const QString& left, const QString& right)
        {
            return QString::localeAwareCompare(
                left,
                right
            ) < 0;
        }
    );

    for (const QString& group : sortedGroups)
    {
        result.summaryCsv
            << QString("%1;%2")
            .arg(group)
            .arg(
                differentProductGroups.value(group)
            );
    }

    if (progress)
    {
        progress->setValue(100);
        QApplication::processEvents();
    }

    return result;
}