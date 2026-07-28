#include "FileExport.h"
#include <QRegularExpression>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "qmessagebox.h"



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
        outputLines << "articleNo;articleName;dateFrom;dateTo;pricetype;price1;currency1;tax1;price2;currency2;tax2;amount2";

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


