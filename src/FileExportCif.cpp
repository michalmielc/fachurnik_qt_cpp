#include "FileExportCif.h"

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