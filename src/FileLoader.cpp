#include "FileLoader.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>


// PARSER OF HEADER
static HeaderData parseHeaderLine(const QString& line)
{
    HeaderData h;

    h.headerLine = line;

    QStringList p = line.split('|');

    if (p.size() > 1)
        h.customerNo = p[1];      // 737

    if (p.size() > 4)
        h.distrChannel = p[4];    // np 04

    if (p.size() > 5)
        h.currency = p[5];        // EUR

    if (p.size() > 7)
        h.dateFrom = p[7];        // 20250801

    if (p.size() > 8)
        h.dateTo = p[8];          // 20260731

    if (p.size() > 18)
        h.discountGrp = p[18];    //gr rabatowa

    if (p.size() > 19)
        h.alloySurcharge = (p[19] == "X");  //dod stopowy

    if (p.size() > 20)
        h.specialOffers = (p[20] == "X");    //oferta


    if (p.size() > 23)
        h.catalogNumber = QString::number(p[23].toInt());  // 0056 konwersja do 56

    if (p.size() > 24)
        h.salesRep = p[24];      //GVL

    return h;
}

// READ DAT FILE
FileData FileLoader::loadDatFile(
    const QString& path,
    std::function<void(int)> progressCallback)
{
    FileData data;

    data.lineCount = 0;
    data.fullPath = path;

    QFileInfo info(path);
    data.fileName = info.fileName();
    data.extension = info.suffix();

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    qint64 fileSize = file.size();

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (data.lineCount == 0 && line.startsWith("H|"))
        {
            data.header = parseHeaderLine(line);
            data.header.headerLine = line; //nag³ówek ca³y
            data.headerLoaded = true;
        }


        data.content += line + "\n";
        data.lineCount++;

        if (progressCallback && fileSize > 0)
        {
            int progress = static_cast<int>(
                (file.pos() * 100) / fileSize
                );

            progressCallback(progress);
        }
    }

    file.close();

    return data;
}

//READ CSV FILE CUSTOMER NO AND SALES REP
QVector<QPair<QString, QString>> FileLoader::loadCsvFileCustomerList(
    const QString& path,
    std::function<void(int)> progressCallback)
{
    QVector<QPair<QString, QString>> data;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    qint64 fileSize = file.size();

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList parts = line.split(';'); // lub ','

        if (parts.size() < 2)
            continue;

        data.append(qMakePair(
            parts[0].trimmed(),
            parts[1].trimmed()
        ));

        if (progressCallback && fileSize > 0)
        {
            int progress = static_cast<int>(
                (file.pos() * 100) / fileSize
                );

            progressCallback(progress);
        }
    }

    return data;
}

//READ CSV FILE PRICELIST
FileData FileLoader::loadCsvPriceFile(
    const QString& path,
    std::function<void(int)> progressCallback)
{
    FileData data;

    data.lineCount = 0;
    data.fullPath = path;

    QFileInfo info(path);
    data.fileName = info.fileName();
    data.extension = info.suffix();

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    qint64 fileSize = file.size();

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        data.content += line + "\n";
        data.lineCount++;

        if (progressCallback && fileSize > 0)
        {
            int progress = static_cast<int>(
                (file.pos() * 100) / fileSize
                );

            progressCallback(progress);
        }
    }

    file.close();

    return data;
}

// READ TO MAP ITEM NUM, BUILDING CIF ROW
QVector<QPair<QString, QString>> FileLoader::loadCsvItemNumbers(
    const QString& path,
    std::function<void(int)> progressCallback)
{
    QVector<QPair<QString, QString>> data;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    qint64 fileSize = file.size();

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);


    if (!in.atEnd())
        in.readLine(); // pomiñ nag³ówek

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList parts = line.split(';'); // lub ','

        if (parts.isEmpty())
            continue;

        data.append({ parts[0].trimmed(), QString() });

        if (progressCallback && fileSize > 0)
        {
            int progress = static_cast<int>(
                (file.pos() * 100) / fileSize
                );

            progressCallback(progress);
        }
    }

    return data;
}

// READ  CIF FILE
FileData FileLoader::loadCifFile(
    const QString& path,
    std::function<void(int)> progressCallback)
{
    FileData data;

    data.lineCount = 0;
    data.fullPath = path;

    QFileInfo info(path);
    data.fileName = info.fileName();
    data.extension = info.suffix();

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    qint64 fileSize = file.size();

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        // Zakoñcz wczytywanie po napotkaniu pierwszej linii "DATA..."

            if (line.startsWith("DATA"))
                break;
      
        data.content += line + "\n"; ///?O  CO TO???
        data.header.headerCif << line;
        data.lineCount++;

        if (progressCallback && fileSize > 0)
        {
            int progress = static_cast<int>(
                (file.pos() * 100) / fileSize
                );

            progressCallback(progress);
        }
    }

    //WCZYTANIE DO QSTRINGLIST LINES Z CIF
    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.startsWith("END OF DATA"))
            break;

        data.cifLines << line;
    }

    file.close();

    return data;
}

//READ CS HEADERS CHOOS COLUMN
QStringList FileLoader::loadCsvHeaders(const QString& path)
{
    QStringList headers;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return headers;

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    if (!in.atEnd())
    {
        QString firstLine = in.readLine().trimmed();
        headers = firstLine.split(';', Qt::KeepEmptyParts);

        for (QString& h : headers)
            h = h.trimmed();
    }

    return headers;
}

//READ DATA FROM SOURCE CSV 

QHash<QString, QString> FileLoader::loadCsvColumnToHash(
    const QString& path,
    int valueColumn,
    std::function<void(int)> progressCallback)
{
    QHash<QString, QString> data;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    qint64 fileSize = file.size();

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);


    if (!in.atEnd())
        in.readLine(); // pomiñ nag³ówek

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList parts = line.split(';', Qt::KeepEmptyParts);

        // musi istnieæ klucz oraz wybrana kolumna
        if (parts.size() <= valueColumn)
            continue;

        QString itemNumber = parts[0].trimmed();
        QString value = parts[valueColumn].trimmed();
        //USUNIÊCIE APOSTROFÓW JESLI S¥ 
        value.remove('"');

        if (!itemNumber.isEmpty())
            data.insert(itemNumber, value);

        if (progressCallback && fileSize > 0)
        {
            int progress = static_cast<int>(
                (file.pos() * 100) / fileSize);

            progressCallback(progress);
        }
    }

    return data;
}

//RAED TZ FILE
QHash<QString, double> FileLoader::loadTZFile(
    const QString& path,
    std::function<void(int)> progressCallback
)
{
    QHash<QString, double> data;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return data;

    const qint64 fileSize = file.size();

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // Pomijamy nag³ówek:
    // Material;LZ August 2026
    if (!in.atEnd())
        in.readLine();

    while (!in.atEnd())
    {
        const QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        const QStringList parts =
            line.split(';', Qt::KeepEmptyParts);

        if (parts.size() < 2)
            continue;

        const QString articleNo = parts[0].trimmed();

        QString percentText = parts[1].trimmed();

        percentText.remove('%');
        percentText.replace(',', '.');

        bool ok = false;
        const double percent = percentText.toDouble(&ok);

        if (!articleNo.isEmpty() && ok)
            data.insert(articleNo, percent);

        if (progressCallback && fileSize > 0)
        {
            const int progress = static_cast<int>(
                (file.pos() * 100) / fileSize
                );

            progressCallback(progress);
        }
    }

    file.close();

    return data;
}