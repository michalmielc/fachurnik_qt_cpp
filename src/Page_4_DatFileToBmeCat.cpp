#include"Page_4_DatFileToBmeCat.h"
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


void Page_4_DatFileToBmeCat::initialize()
{
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome_4,
        ui.stackedWidget,
        ui.pageMenu);

    ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo_4);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrencyHeader_4);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrencyLines_4);
    ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG_4);
    ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep_4);

    QObject::connect(
        ui.btnOpenFile_4,
        &QPushButton::clicked,
        [this]()
        {
            onChooseFileClicked();
        });


    hideShowGrpBox(false);


    QObject::connect(ui.radBtnEUR_4, &QRadioButton::clicked,
        [this]()
        {
            ui.lineEditExchangeRate_4->setText("1,00");

            ui.radBtnEUR_4->setStyleSheet("background-color: yellow;");

            ui.radBtnPLN_4->setStyleSheet("");
        });

    QObject::connect(ui.radBtnPLN_4, &QRadioButton::clicked,
        [this]()
        {
            ui.lineEditExchangeRate_4->setText("1,00");

            ui.radBtnEUR_4->setStyleSheet("");

            ui.radBtnPLN_4->setStyleSheet("background-color: yellow;");
        });

    QDoubleValidator* validator = new QDoubleValidator(0.0, 9999.9999, 4);

    validator->setNotation(QDoubleValidator::StandardNotation);

    ui.lineEditExchangeRate_4->setValidator(validator);


    QObject::connect(ui.pushBtnExport_4, &QPushButton::clicked,
        [this]()
        {
            saveModifiedFileToDesktop(currentFileData);
        });

    ui.radBtnEUR_4->setChecked(true);

};

void Page_4_DatFileToBmeCat::onChooseFileClicked()
{

    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik DAT",
        "DAT Files (*.dat)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_4, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_4, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_4, "", "red");

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

    setLabel(ui.lblFilePath_4, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_4, "FILE: " + currentFileData.fileName, "blue");
    setLabel(
        ui.lblFileCountLines_4,
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
        setLabel(ui.lblFileCountLines_4, "Brak nag³ówka H| w pliku", "red");
        hideShowGrpBox(false);
    }

    progress.setValue(0);

    bool differentCurrency =
        hasDifferentCurrencyInLines(currentFileData, &progress);

    progress.setValue(100);
    QApplication::processEvents();


    setLabelCurrencyLine(differentCurrency);

    setComboByText(ui.comBoxCurrencyLines_4, currentFileData.header.currencyInLine, false);


    QMessageBox::information(
        nullptr,
        "FILE",
        "File loaded successfully"
    );

    progress.finish();
}


// CONTROL BEHAVIOR -------------------------------------------------
// SHOW/HIDE GRPBOX
void Page_4_DatFileToBmeCat::hideShowGrpBox(bool b)
{
    ui.groupBoxDCh_4->setVisible(b);
    ui.groupBoxHeader_4->setVisible(b);
    ui.groupBoxCurrEx_4->setVisible(b);
    ui.groupBoxFileExp_4->setVisible(b);
}

// SET COMOBOX AFTER READING HEADER
void Page_4_DatFileToBmeCat::setComboByText(QComboBox* comboBox, const QString& text, bool startsWith)
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
void Page_4_DatFileToBmeCat::setCheckBoxValue(QCheckBox* checkBox, const bool val) {

    if (val)
    {
        checkBox->setChecked(true);
    }
}

// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_4_DatFileToBmeCat::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}

void Page_4_DatFileToBmeCat::setLabelCurrencyLine(bool value)
{
    if (value)
    {
        ui.lblControlCurrency_4->clear();
        ui.lblControlCurrency_4->setText("Warning: Different currencies have been detected in the item lines.");
        ui.lblControlCurrency_4->setStyleSheet("color: red; font-weight: bold; font-family:Bahnschrift; font-size: 11pt");
        ui.lblControlCurrency_4->show();

        QMessageBox::critical(
            nullptr,
            "ERROR!",
            "DIFFERENT CURRENCIES DETECTED IN THE FILE"
        );
    }
    else
    {
        ui.lblControlCurrency_4->clear();
        ui.lblControlCurrency_4->setText("All lines have the same currency");
        ui.lblControlCurrency_4->setStyleSheet("color: green; font-weight: bold; font-family:Bahnschrift;font-size: 11pt ");
        ui.lblControlCurrency_4->show();
    }
}

//FUNCTIONALITIES
//-------------------------------------------------------------------
// READ DATA TO UI FROM FILE
void Page_4_DatFileToBmeCat::loadHeaderToUi(const HeaderData& header)
{
    //CUSTOMER NO
    ui.lineEditCustomerNo_4->setText(header.customerNo);

    //SELECT DIST CHANNEL
    ui.radioButton41->setChecked(header.distrChannel == "01");
    ui.radioButton42->setChecked(header.distrChannel == "02");
    ui.radioButton43->setChecked(header.distrChannel == "03");
    ui.radioButton44->setChecked(header.distrChannel == "04");
    ui.groupBoxDCh_4->setEnabled(false);

    //DATE FROM TO
    ui.lineEditDateFrom_4->setText(header.dateFrom);
    ui.lineEditDateTo_4->setText(header.dateTo);

    //OTHERS
    setComboByText(ui.comBoxCurrencyHeader_4, header.currency, false);
    setComboByText(ui.comBoxCatalogNo_4, header.catalogNumber, false);

    ui.comBoxCurrencyHeader_4->setEnabled(false);
    ui.comBoxCurrencyLines_4->setEnabled(false);
    //
    setComboByText(ui.comBoxDiscountG_4, header.discountGrp, false);
    setComboByText(ui.comBoxSalesRep_4, header.salesRep, true);
    setCheckBoxValue(ui.checkBoxAlloySurcharge_4, header.alloySurcharge);
    setCheckBoxValue(ui.checkBoxSpecialOffers_4, header.specialOffers);

    // checkbox GERMAN CATALOG NOT ACTIVE!!!!
}

// CONTROL CURRENCY
bool Page_4_DatFileToBmeCat::hasDifferentCurrencyInLines(
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
// SET NEW HEADER
QString Page_4_DatFileToBmeCat::buildHeaderLineFromUi(FileData& data)
{
    //SAVE CATALOG VALID FROM TO INTO VARIABLE
    data.header.dateFrom = ui.lineEditDateFrom_4->text();
    data.header.dateFrom.insert(4, '-');
    data.header.dateFrom.insert(7, '-');
    data.header.dateTo = ui.lineEditDateTo_4->text();
    data.header.dateTo.insert(4, '-');
    data.header.dateTo.insert(7, '-');

    //BMECAT HEADER 
    QString h =
        "article_no; date_from; date_to; price_type1; price_amount1;"
        "price_currecy1; tax1; lower_bound1;" 
        "price_type2; price_amount2; price_currecy2; tax2; lower_bound2;";
    return h;
}

// EXPORT TO DESKTOP
void Page_4_DatFileToBmeCat::saveModifiedFileToDesktop(const FileData& data)
{
    //na potrzeby nie modyikacji data
    FileData exportData = data;
    
    FileProcessingProgress progress;

    progress.setWindowTitle("Eksport pliku");
    progress.setLabelText("Trwa eksport pliku...");
    progress.show();

    QString targetCurrency = ui.comBoxCurrencyHeader->currentText();

    if (ui.radBtnEUR_4->isChecked())
        targetCurrency = "EUR";

    if (ui.radBtnPLN_4->isChecked())
        targetCurrency = "PLN";

    double exchangeRate = ui.lineEditExchangeRate_4->text()
        .replace(",", ".")
        .toDouble();

    QString customerNo = ui.lineEditCustomerNo_4->text();

    QString headerLine = buildHeaderLineFromUi(exportData);

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
