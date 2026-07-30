#pragma once

#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "ComboBoxHelper.h"
#include "FileData.h"
#include "FileProcessingProgress.h"

class Page_1_DatFileToEshopFile
{
public:
    explicit Page_1_DatFileToEshopFile(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize();
 
private:

    FileData currentFileData;
    FileData currentFileDataCSV;
    QHash<QString, double> currentTZMap;

    void onChooseFileClicked();
    void hideShowGrpBox(bool b);
    void setLabel(QLabel* label, const QString& text, const QString& color);
    void setCheckBoxValue(QCheckBox* checkBox, const bool val);
    void loadHeaderToUi(const HeaderData& header);
    void setComboByText(QComboBox* comboBox, const QString& text, bool startsWith);
    bool hasDifferentCurrencyInLines( FileData& data, FileProcessingProgress* progress);
    QString buildHeaderLineFromUi(const QString& originalHeaderLine);
    void saveModifiedFileToDesktop(const FileData& data);
    void setLabelCurrencyLine(bool value);
    void showHideTzSurcharge(bool visible);
    void showHideDiscountSurcharge(bool visible);
    void onChooseTZCsvFileClicked();

    Ui::Fachurnik_CClass& ui;
};