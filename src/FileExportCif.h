#pragma once
#include <QString>
#include <QVector>
#include <QStringList>
#include <QPair>
#include <functional>
#include "FileData.h"

class FileExportCif
{
public:
	struct columnSettings {
		QString colName;
		bool isStaticField = false;
		QString staticValue;
		bool isEmpty = false;
		bool fromFile = false;
		QString path;
		QString fileName;
		QString fileColName;
		int fileColNum = -1;
		bool removeSemAndApo = true;
		bool convertPrice = false;
		bool cutString = false;
		int lengthToCut = 0;
		bool addItemNumAtEnd = false;
		bool insertApo = true;
		bool removeItemFromCat = false;
		bool fillRandomVal = false;
	};


	QVector<QPair<QString, columnSettings>> createCifColumns(const QStringList& fields);
	QVector<QPair<QString, columnSettings>>& getCifColumns()

	{
		return cifColumns;
	}
	QVector<QPair<QString, QString>> buildRows(
		QVector<QPair<QString, QString>> items,
		std::function<void(int, int)> progressCallback = nullptr);



	bool saveCifFile(
		const QStringList& header,
		const QVector<QPair<QString, QString>>& rows,
		const QString& filePath,
		QString* error
	);

	QStringList parseCifLine(const QString& line);
	QString rebuildCifLine(const QStringList& columns);

	void replaceColumnInCif(
		FileData& fileData,
		int idColumnIndex,
		int targetColumnIndex,
		const columnSettings& settings,
		std::function<void(int, int)> progressCallback);

	bool saveRebuiltCifFile(
		const FileData& fileData,
		const QString& filePath,
		QString* error
	);
private:
	QVector<QPair<QString, columnSettings>> cifColumns;
};