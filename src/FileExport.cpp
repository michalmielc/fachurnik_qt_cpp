#include "FileExport.h"
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QApplication>

bool FileExport::exportModifiedDatToDatDesktop(
    const FileData& data,
    const QString& headerLine,
    const QString& targetCurrency,
    double exchangeRate,
    const QString& customerNo,
    QProgressDialog* progress,
    QString* savedPath,
    QString* error
)


{
    QStringList lines = data.content.split('\n', Qt::KeepEmptyParts);

    if (data.content.trimmed().isEmpty())
    {
        if (error) *error = "Brak danych do eksportu. data.content jest puste.";
        return false;
    }

    if (headerLine.trimmed().isEmpty())
    {
        if (error) *error = "Nag³ówek jest pusty.";
        return false;
    }

 
    if (lines.isEmpty())
    {
        if (error) *error = "Plik nie zawiera ¿adnych linii.";
        return false;
    }

 

   
    lines[0] = headerLine;

    int total = lines.size();

    for (int i = 1; i < lines.size(); ++i)
    {
        if (progress)
        {
            progress->setValue((i * 80) / total);
            QApplication::processEvents();
        }


        if (!lines[i].startsWith("L|"))
            continue;

        QStringList p = lines[i].split('|', Qt::KeepEmptyParts);

        if (p.size() <= 8)
            continue;

        QString oldCurrency = p[6];

        if (oldCurrency != targetCurrency)
        {
            p[6] = targetCurrency;

            bool ok = false;
            double oldValuePrice1 = p[8].replace(",", ".").toDouble(&ok);
            double oldValuePrice2 = p[9].replace(",", ".").toDouble(&ok);

            if (ok)
            {
                double newValuePrice1 = oldValuePrice1 * exchangeRate;
                double newValuePrice2 = oldValuePrice2 * exchangeRate;

                p[8] = QString::number(newValuePrice1, 'f', 2)
                    .replace(".", ",");
                p[9] = QString::number(newValuePrice2, 'f', 2)
                    .replace(".", ",");
            }

            lines[i] = p.join('|');
        }
    }

    if (progress)
    {
        progress->setValue(85);
        QApplication::processEvents();
    }

    QString desktopPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QString newFilePath =
        desktopPath
        + QDir::separator()
        + "fachurnik_newfile_"
        + customerNo
        + ".dat";

    QFile outFile(newFilePath);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error) *error = "Nie mo¿na zapisaæ pliku.";
        return false;
    }

    QTextStream out(&outFile);

    for (const QString& line : lines)
        out << line << "\n";

    outFile.close();

    if (progress)
    {
        progress->setValue(95);
        QApplication::processEvents();
    }

    if (savedPath)
        *savedPath = newFilePath;

    return true;
}

 bool FileExport::exportModifiedDatToCsvDesktop(
    const QStringList& lines,
    const QString& filePath,
    QString* error
)
{
    QFile csvFile(filePath);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        if (error) *error = "Nie mo¿na zapisaæ pliku CSV.";
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