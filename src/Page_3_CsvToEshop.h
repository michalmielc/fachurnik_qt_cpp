#pragma once

#pragma once

#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "ComboBoxHelper.h"
#include "FileData.h"
#include "FileProcessingProgress.h"

class Page_3_CsvToEshop
{
public:
    explicit Page_3_CsvToEshop(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize();

private:

    FileData currentFileData;

    void onChooseFileClicked();
    void hideShowGrpBox(bool b);
    void setLabel(QLabel* label, const QString& text, const QString& color);
    void setCheckBoxValue(QCheckBox* checkBox, const bool val);
    void loadHeaderToUi(const HeaderData& header);
    void setComboByText(QComboBox* comboBox, const QString& text, bool startsWith);
    bool hasDifferentCurrencyInLines(FileData& data, FileProcessingProgress* progress);
    QString buildHeaderLineFromUi(const QString& originalHeaderLine);
    void saveModifiedFileToDesktop(const FileData& data);
    void setLabelCurrencyLine(bool value);
    Ui::Fachurnik_CClass& ui;
};