#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Fachurnik_C.h"
#include "FileLoader.h"
#include "FileLoadingProgress.h"

class Fachurnik_C : public QMainWindow
{
    Q_OBJECT

public:
    Fachurnik_C(QWidget* parent = nullptr);
    ~Fachurnik_C();

private slots:
    void onMenuDoubleClicked(QTreeWidgetItem* item, int column);
    void onMenuClicked(QTreeWidgetItem* item, int column);
    void onChooseFileClicked();

private:
    Ui::Fachurnik_CClass ui;

    FileData currentFileData;

    void loadHeaderToUi(const HeaderData& header);
    void setLabel(QLabel* label, const QString& text, const QString& color);
    void setComboByText(QComboBox* comboBox, const QString& text, bool startsWith);
    void hideShowGrpBox(bool b);
    void setCheckBoxValue(QCheckBox* checkBox, const bool val);

    QString buildHeaderLineFromUi(const QString& originalHeaderLine);
    QString distrChannelFromUi() const;

    void saveModifiedFileToDesktop(const FileData& data);
    bool hasDifferentCurrencyInLines(const FileData& data, QProgressDialog* progress);
};