#pragma once
#include <QString>
#include <QVector>
#include <QStringList>
#include <QPair>

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
	};


	QVector<QPair<QString, columnSettings>> createCifColumns(const QStringList& fields);
	QVector<QPair<QString, columnSettings>>& getCifColumns()

	{
		return cifColumns;
	}
	QVector<QPair<QString, QString>> buildRows(
		QVector<QPair<QString, QString>> items);



	bool saveCifFile(
		const QStringList& header,
		const QVector<QPair<QString, QString>>& rows,
		const QString& filePath,
		QString* error
	);

private:
	QVector<QPair<QString, columnSettings>> cifColumns;
};