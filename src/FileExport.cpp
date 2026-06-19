#include "FileExport.h"
#include <QRegularExpression>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "qmessagebox.h"



QStringList FileExport::buildModifiedLines(
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
        QStringList columns = line.split('|', Qt::KeepEmptyParts);

        for (int i = 0; i < columns.size(); ++i)
        {
            QString value = columns[i];
            value.replace("\"", "\"\"");

            out << "\"" << value << "\"";

            if (i < columns.size() - 1)
                out << ";";
        }

        out << "\n";
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