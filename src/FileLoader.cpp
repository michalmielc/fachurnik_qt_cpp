#include "FileLoader.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>


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


    if (p.size() > 22)
        h.catalogNumber = p[22];  // K56

    if (p.size() > 24)
        h.salesRep = p[24];      //GVL

    return h;
}


FileData FileLoader::loadFile(
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