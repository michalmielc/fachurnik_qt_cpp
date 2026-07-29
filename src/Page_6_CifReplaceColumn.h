#pragma once
#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "FileProcessingProgress.h"
#include "FileExportCif.h"
#include "FileData.h"

class Page_6_CifReplaceColumn
{
public:
    explicit Page_6_CifReplaceColumn(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize();

private:

    FileData currentCifFileData;
    QVector<QPair<QString, QString>> currentCsvFileData;
    FileExportCif fileExportCif;
    int selectedCifColumnRow = -1;

    QStringList currentCsvHeaders;

    FileExportCif::columnSettings settings;

    void onChooseCifFileClicked();

    void readHeadersColumnFromCif();

    void onTableCifColumnClicked(int row, int column);

    void loadSourceColumnFromCsv();

    void createCifFile();

    void updateCifColumnTableRow(int row);

    void replaceColumnButton();

    void setLabel(QLabel* label, const QString& text, const QString& color);

    Ui::Fachurnik_CClass& ui;

};