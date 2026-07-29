#pragma once
#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "FileData.h"
#include "FileProcessingProgress.h"
#include "FileExportCif.h"

class Page_5_CreateCif
{
public:
    explicit Page_5_CreateCif(
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

    void onChooseCifFileClicked();

    void readHeadersColumnFromCif();

    void importItemNumbersFromCsv();

    void onTableCifColumnClicked(int row, int column);

    void saveCurrentCifColumnSettings();

    void loadSourceColumnFromCsv();

    void updateCifColumnTableRow(int row);

    void createCifFile();

    void setLabel(QLabel* label, const QString& text, const QString& color);

    Ui::Fachurnik_CClass& ui;
};
