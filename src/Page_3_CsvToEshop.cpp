#include "Page_3_CsvToEshop.h"
#include"OpenFileDialog.h"
#include <QFileDialog>
#include <QDoubleValidator>
#include "FileProcessingProgress.h"
#include "FileLoader.h"
#include <qmessagebox.h>
#include "FileExport.h"
#include <qstandardpaths.h>
#include "ui_Fachurnik_C.h"
#include <QDateTime>


void Page_3_CsvToEshop::initialize()
{
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome_3,
        ui.stackedWidget,
        ui.pageMenu);

    ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo_3);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrencyHeader_3);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrencyLines_3);
    ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG_3);
    ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep_3);

    QObject::connect(
        ui.btnOpenFile_3,
        &QPushButton::clicked,
        [this]()
        {
            onChooseFileClicked();
        });


    hideShowGrpBox(false);


    QObject::connect(ui.radBtnEUR_3, &QRadioButton::clicked,
        [this]()
        {
            ui.lineEditExchangeRate_3->setText("1,00");

            ui.radBtnEUR_3->setStyleSheet("background-color: yellow;");

            ui.radBtnPLN_3->setStyleSheet("");
        });

    QObject::connect(ui.radBtnPLN_3, &QRadioButton::clicked,
        [this]()
        {
            ui.lineEditExchangeRate_3->setText("1,00");

            ui.radBtnEUR_3->setStyleSheet("");

            ui.radBtnPLN_3->setStyleSheet("background-color: yellow;");
        });

    QDoubleValidator* validator = new QDoubleValidator(0.0, 9999.9999, 4);

    validator->setNotation(QDoubleValidator::StandardNotation);

    ui.lineEditExchangeRate_3->setValidator(validator);


    QObject::connect(ui.pushBtnExport_3, &QPushButton::clicked,
        [this]()
        {
            saveModifiedFileToDesktop(currentFileData);
        });

    ui.radBtnEUR_3->setChecked(true);

};

void Page_3_CsvToEshop::onChooseFileClicked()
{

    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik DAT",
        "DAT Files (*.dat)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_3, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_3, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_3, "", "red");

        hideShowGrpBox(false);

        return;
    }

    FileProcessingProgress progress;
    progress.show();

    currentFileData = FileLoader::loadDatFile(
        path,
        [&](int value)
        {
            progress.setValue(value);
            QApplication::processEvents();
        }
    );

    setLabel(ui.lblFilePath_3, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_3, "FILE: " + currentFileData.fileName, "blue");
    setLabel(
        ui.lblFileCountLines_3,
        "ITEM LINES: " + QString::number(currentFileData.lineCount),
        "blue"
    );

    if (currentFileData.headerLoaded)
    {
        loadHeaderToUi(currentFileData.header);
        hideShowGrpBox(true);
    }
    else
    {
        setLabel(ui.lblFileCountLines_3, "Brak nag³ówka H| w pliku", "red");
        hideShowGrpBox(false);
    }

    progress.setValue(0);

    bool differentCurrency =
        hasDifferentCurrencyInLines(currentFileData, &progress);

    progress.setValue(100);
    QApplication::processEvents();


    setLabelCurrencyLine(differentCurrency);

    setComboByText(ui.comBoxCurrencyLines, currentFileData.header.currencyInLine, false);


    QMessageBox::information(
        nullptr,
        "FILE",
        "File loaded successfully"
    );

    progress.finish();
}


// CONTROL BEHAVIOR -------------------------------------------------
// SHOW/HIDE GRPBOX
void Page_3_CsvToEshop::hideShowGrpBox(bool b)
{
    ui.groupBoxDCh_3->setVisible(b);
    ui.groupBoxHeader_3->setVisible(b);
    ui.groupBoxCurrEx_3->setVisible(b);
    ui.groupBoxFileExp_3->setVisible(b);
}

// SET COMOBOX AFTER READING HEADER
void Page_3_CsvToEshop::setComboByText(QComboBox* comboBox, const QString& text, bool startsWith)
{
    if (!startsWith)
    {
        int index = comboBox->findText(text);

        if (index >= 0)
            comboBox->setCurrentIndex(index);

        return;
    }

    for (int i = 0; i < comboBox->count(); i++)
    {
        if (comboBox->itemText(i).startsWith(text))
        {
            comboBox->setCurrentIndex(i);
            return;
        }
    }
}

// SET CHECKBOX AFTER READING HEADER
void Page_3_CsvToEshop::setCheckBoxValue(QCheckBox* checkBox, const bool val) {

    if (val)
    {
        checkBox->setChecked(true);
    }
}

// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_3_CsvToEshop::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}

void Page_3_CsvToEshop::setLabelCurrencyLine(bool value)
{
    if (value)
    {
        ui.lblControlCurrency_3->clear();
        ui.lblControlCurrency_3->setText("Warning: Different currencies have been detected in the item lines.");
        ui.lblControlCurrency_3->setStyleSheet("color: red; font-weight: bold; font-family:Bahnschrift; font-size: 11pt");
        ui.lblControlCurrency_3->show();

        QMessageBox::critical(
            nullptr,
            "ERROR!",
            "DIFFERENT CURRENCIES DETECTED IN THE FILE"
        );
    }
    else
    {
        ui.lblControlCurrency_3->clear();
        ui.lblControlCurrency_3->setText("All lines have the same currency");
        ui.lblControlCurrency_3->setStyleSheet("color: green; font-weight: bold; font-family:Bahnschrift;font-size: 11pt ");
        ui.lblControlCurrency_3->show();
    }
}

//FUNCTIONALITIES
//-------------------------------------------------------------------
// READ DATA TO UI FROM FILE
void Page_3_CsvToEshop::loadHeaderToUi(const HeaderData& header)
{
    //CUSTOMER NO
    ui.lineEditCustomerNo_3->setText(header.customerNo);

    //SELECT DIST CHANNEL
    ui.radioButton31->setChecked(header.distrChannel == "01");
    ui.radioButton32->setChecked(header.distrChannel == "02");
    ui.radioButton33->setChecked(header.distrChannel == "03");
    ui.radioButton34->setChecked(header.distrChannel == "04");
    ui.groupBoxDCh_3->setEnabled(false);

    //DATE FROM TO
    ui.lineEditDateFrom_3->setText(header.dateFrom);
    ui.lineEditDateTo_3->setText(header.dateTo);

    //OTHERS
    setComboByText(ui.comBoxCurrencyHeader, header.currency, false);
    ui.comBoxCurrencyHeader_3->setEnabled(false);
    ui.comBoxCurrencyLines_3->setEnabled(false);
    //
    setComboByText(ui.comBoxDiscountG_3, header.discountGrp, false);
    setComboByText(ui.comBoxSalesRep_3, header.salesRep, true);
    setCheckBoxValue(ui.checkBoxAlloySurcharge_3, header.alloySurcharge);
    setCheckBoxValue(ui.checkBoxSpecialOffers_3, header.specialOffers);

    // checkbox GERMAN CATALOG NOT ACTIVE!!!!
}

// CONTROL CURRENCY
bool Page_3_CsvToEshop::hasDifferentCurrencyInLines(
    FileData& data,
    FileProcessingProgress* progress
)
{

    QString firstCurrency;

    QStringList lines = data.content.split('\n', Qt::SkipEmptyParts);
    int total = lines.size();

    for (int i = 0; i < lines.size(); ++i)
    {
        if (progress && total > 0)
        {
            progress->setValue((i * 100) / total);
            QApplication::processEvents();
        }

        const QString& line = lines[i];

        if (!line.startsWith("L|"))
            continue;

        QStringList p = line.split('|', Qt::KeepEmptyParts);

        if (p.size() <= 6)
            continue;

        QString lineCurrency = p[6].trimmed();

        if (lineCurrency.isEmpty())
            continue;

        // pierwsza waluta z pierwszej poprawnej linii L|
        if (firstCurrency.isEmpty())
        {
            firstCurrency = lineCurrency;
            continue;
        }

        // jeœli kolejna waluta jest inna
        if (lineCurrency != firstCurrency)
        {
            data.header.currencyInLine = "MIXED EUR/PLN";
            return true;
        }
    }

    data.header.currencyInLine = firstCurrency;

    return false;
};

//EXPORT FUNCTIONALITIES:--------------------------------------------
// DATA READING FROM CONTROLS
QString Page_3_CsvToEshop::buildHeaderLineFromUi(const QString& originalHeaderLine)
{
    QStringList h = originalHeaderLine.split('|', Qt::KeepEmptyParts);

    if (h.size() < 14)
        return originalHeaderLine;

    h[1] = ui.lineEditCustomerNo_3->text();
    h[3] = "01";
    h[4] = "02";

    if (ui.radBtnEUR_3->isChecked())
    {
        h[5] = "EUR";
    }

    else if (ui.radBtnPLN_3->isChecked())
    {
        h[5] = "PLN";
    }


    h[7] = ui.lineEditDateFrom_3->text();
    h[8] = ui.lineEditDateTo_3->text();
    h[16] = "E-SHOP_EXP_API";
    h[18] = ui.comBoxDiscountG_3->currentText();
    h[19] = ui.checkBoxAlloySurcharge_3->isChecked() ? "X" : "";
    h[20] = ui.checkBoxSpecialOffers_3->isChecked() ? "X" : "";
    h[21] = ui.checkBoxAlloySurcharge_3->isChecked() ? "X" : "";
    h[22] = "K" + ui.comBoxCatalogNo_3->currentText();
    h[23] = "00" + ui.comBoxCatalogNo_3->currentText();
    h[24] = ui.comBoxSalesRep_3->currentText().left(6);

    return h.join('|');
}

// EXPORT TO DESKTOP
void Page_3_CsvToEshop::saveModifiedFileToDesktop(const FileData& data)
{
    FileProcessingProgress progress;

    progress.setWindowTitle("Eksport pliku");
    progress.setLabelText("Trwa eksport pliku...");
    progress.show();

    QString targetCurrency = ui.comBoxCurrencyHeader_3->currentText();

    if (ui.radBtnEUR_3->isChecked())
        targetCurrency = "EUR";

    if (ui.radBtnPLN_3->isChecked())
        targetCurrency = "PLN";

    double exchangeRate = ui.lineEditExchangeRate_3->text()
        .replace(",", ".")
        .toDouble();

    QString customerNo = ui.lineEditCustomerNo_3->text();

    QString headerLine = buildHeaderLineFromUi(data.header.headerLine);

    QStringList modifiedLines = FileExport::buildDatModifiedLines(
        data,
        headerLine,
        targetCurrency,
        exchangeRate,
        &progress
    );

    if (modifiedLines.isEmpty())
    {
        progress.finish();

        QMessageBox::critical(nullptr, "OPERATION ABORTED!", QString("THE OUTPUT FILE WAS NOT CREATED"));

        return;
    }

    QString timestamp =
        QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

    QString desktopPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QString datPath =
        desktopPath
        + QDir::separator()
        + "fachurnik_newfile_"
        + timestamp
        + "_"
        + customerNo
        + ".dat";

    QString savedPath = datPath;
    QString error;

    bool ok = FileExport::saveDat(
        modifiedLines,
        datPath,
        &error
    );

    if (!ok)
    {
        progress.finish();
        QMessageBox::warning(nullptr, "B³¹d", error);
        return;
    }

    if (ui.checkBoxExportToCsv->isChecked())
    {

        FileData exportData = data;

        QStringList modifiedLines = FileExport::buildCsvModifiedLines(
            exportData,
            headerLine,
            targetCurrency,
            exchangeRate,
            &progress
        );

        if (modifiedLines.isEmpty())
        {
            progress.finish();

            QMessageBox::critical(nullptr, "OPERATION ABORTED!", QString("THE OUTPUT FILE WAS NOT CREATED"));

            return;
        }

        QString timestamp =
            QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

        QString desktopPath =
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

        QString error;


        QString csvPath =
            desktopPath
            + QDir::separator()
            + "fachurnik_newfile_"
            + timestamp
            + "_"
            + customerNo
            + ".csv";

        bool csvOk = FileExport::saveCsv(
            modifiedLines,
            csvPath,
            &error
        );


        progress.setValue(100);
        QApplication::processEvents();
        progress.finish();

        QMessageBox::information(nullptr, "OK", "Zapisano:\n" + csvPath);
    }

    progress.setValue(100);
    QApplication::processEvents();
    progress.finish();

    QMessageBox::information(nullptr, "OK", "Zapisano:\n" + savedPath);
}

