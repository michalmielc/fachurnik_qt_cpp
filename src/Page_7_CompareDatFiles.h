#pragma once

#include "ui_Fachurnik_C.h"
#include "FileData.h"
#include "FileLoader.h"
#include "FileProcessingProgress.h"

class Page_7_CompareDatFiles
{
public:
    explicit Page_7_CompareDatFiles(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize();

private:

    FileData currentFileData1;
    FileData currentFileData2;

    Ui::Fachurnik_CClass& ui;
    void onChooseFileClicked(int btnNum);
    void setLabel(QLabel* label, const QString& text, const QString& color);
    void setLabelCurrencyLine(bool value, QLabel* label);
    bool hasDifferentCurrencyInLines(
        FileData& data,
        FileProcessingProgress* progress
    );
    void loadDistChannel(const HeaderData& header, QGroupBox* gpBox);

    void saveComparisonFilesToDesktop();
};