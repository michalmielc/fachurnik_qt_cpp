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

private:
	QVector<QPair<QString, columnSettings>> cifColumns;
};