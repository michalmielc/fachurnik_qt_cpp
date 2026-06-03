#pragma once

#include "FileData.h"
#include <functional>

class FileLoader
{
public:
    static FileData loadFile(
        const QString& path,
        std::function<void(int)> progressCallback = nullptr
    );
};

