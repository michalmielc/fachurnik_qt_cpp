#pragma once
#include <QString>
#include"HeaderData.h"


struct FileData
{
    QString fullPath;
    QString fileName;
    QString extension;
    QString content;
    int lineCount;

    HeaderData header;
    bool headerLoaded = false;
};
