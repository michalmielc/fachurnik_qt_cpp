#include "Page_2_CloneDatFilesFromDatFile.h"
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


void Page_2_CloneDatFilesFromDatFile::initialize()
{
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome2,
        ui.stackedWidget,
        ui.pageMenu);

    ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo_2);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrencyHeader_2);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrencyLines_2);
    ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG_2);
    ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep_2);

    QObject::connect(
        ui.btnOpenFile_2,
        &QPushButton::clicked,
        [this]()
        {
            onChooseDatFileClicked();
        });

    QObject::connect(
        ui.btnOpenFile_21,
        &QPushButton::clicked,
        [this]()
        {
            onChooseCsvFileClicked();
        });

    hideShowGrpBox(false);


    QObject::connect(ui.pushBtnExport_2, &QPushButton::clicked,
        [this]()
        {
            saveClonedFilesToDesktop(currentFileData);
        });



};

//FUNCITONS READ CSV CUSTOMERS LIST
void Page_2_CloneDatFilesFromDatFile::onChooseCsvFileClicked() 
{
    
    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik CSV",
        "CSV Files (*.csv)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_21, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_21, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_21, "", "red");

        ui.groupBoxFileExp_2->setVisible(false);
        return;
    }

    FileProcessingProgress progress;
    progress.show();

    currentCsvData = FileLoader::loadCsvFileCustomerList(
        path,
        [&](int value)
        {
            progress.setValue(value);
            QApplication::processEvents();
        }
    );

    QFileInfo info(path);

    setLabel(ui.lblFilePath_21, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_21, "FILE: " + info.fileName(), "blue");
    setLabel(
        ui.lblFileCountLines_21,
        "ITEM LINES: " + QString::number(currentCsvData.size()),
        "blue"
    );

    progress.setValue(100);
    QApplication::processEvents();

    QMessageBox::information(
        nullptr,
        "CSV FILE",
        "CSV file loaded successfully"
    );

    progress.finish();

    ui.groupBoxFileExp_2->setVisible(true);
}

//FUNCITONS READ DAT BASIC PRICE DAT FILE
void Page_2_CloneDatFilesFromDatFile::onChooseDatFileClicked()

{
    QString path =  OpenFileDialog::openFile(
            nullptr,
            "Wybierz plik DAT",
            "DAT Files (*.dat)"
        );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_2, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_2, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_2, "", "red");

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

    setLabel(ui.lblFilePath_2, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_2, "FILE: " + currentFileData.fileName, "blue");
    setLabel(
        ui.lblFileCountLines_2,
        "ITEM LINES: " + QString::number(currentFileData.lineCount),
        "blue"
    );

    if (currentFileData.headerLoaded)
    {
        loadHeaderToUi(currentFileData.header);
        hideShowGrpBox(true);
        ui.groupBoxFileExp_2->setVisible(false);
    }
    else
    {
        setLabel(ui.lblFileCountLines_2, "Brak nagłówka H| w pliku", "red");
        hideShowGrpBox(false);
    }

    progress.setValue(0);

    bool differentCurrency =
        hasDifferentCurrencyInLines(currentFileData, &progress);

    progress.setValue(100);
    QApplication::processEvents();


    setLabelCurrencyLine(differentCurrency);

    setComboByText(ui.comBoxCurrencyLines_2, currentFileData.header.currencyInLine, false);


    QMessageBox::information(
        nullptr,
        "FILE",
        "File loaded successfully"
    );

    progress.finish();
}


// CONTROL BEHAVIOR -------------------------------------------------
// SHOW/HIDE GRPBOX
void Page_2_CloneDatFilesFromDatFile::hideShowGrpBox(bool b)
{
    ui.groupBoxDCh_2->setVisible(b);
    ui.groupBoxHeader_2->setVisible(b);
    ui.groupBoxFileExp_2->setVisible(b);
}

// SET COMOBOX AFTER READING HEADER
void Page_2_CloneDatFilesFromDatFile::setComboByText(QComboBox* comboBox, const QString& text, bool startsWith)
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
void Page_2_CloneDatFilesFromDatFile::setCheckBoxValue(QCheckBox* checkBox, const bool val) {

    if (val)
    {
        checkBox->setChecked(true);
    }
}

// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_2_CloneDatFilesFromDatFile::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}

void Page_2_CloneDatFilesFromDatFile::setLabelCurrencyLine(bool value)
{
    if (value)
    {
        ui.lblControlCurrency_2->clear();
        ui.lblControlCurrency_2->setText("Warning: Different currencies have been detected in the item lines.");
        ui.lblControlCurrency_2->setStyleSheet("color: red; font-weight: bold; font-family:Bahnschrift; font-size: 11pt");
        ui.lblControlCurrency_2->show();

        QMessageBox::critical(
            nullptr,
            "ERROR!",
            "DIFFERENT CURRENCIES DETECTED IN THE FILE"
        );
    }
    else
    {
        ui.lblControlCurrency_2->clear();
        ui.lblControlCurrency_2->setText("All lines have the same currency");
        ui.lblControlCurrency_2->setStyleSheet("color: green; font-weight: bold; font-family:Bahnschrift;font-size: 11pt ");
        ui.lblControlCurrency_2->show();
    }
}

//FUNCTIONALITIES
//-------------------------------------------------------------------
// READ DATA TO UI FROM FILE
void Page_2_CloneDatFilesFromDatFile::loadHeaderToUi(const HeaderData& header)
{
    //CUSTOMER NO
    ui.lineEditCustomerNo_2->setText(header.customerNo);

    //SELECT DIST CHANNEL
    ui.radioButton21->setChecked(header.distrChannel == "01");
    ui.radioButton22->setChecked(header.distrChannel == "02");
    ui.radioButton23->setChecked(header.distrChannel == "03");
    ui.radioButton24->setChecked(header.distrChannel == "04");
    ui.groupBoxDCh_2->setEnabled(false);

    //DATE FROM TO
    ui.lineEditDateFrom_2->setText(header.dateFrom);
    ui.lineEditDateTo_2->setText(header.dateTo);

    //OTHERS
    setComboByText(ui.comBoxCurrencyHeader_2, header.currency, false);
    ui.comBoxCurrencyHeader_2->setEnabled(false);
    ui.comBoxCurrencyLines_2->setEnabled(false);
    //
    setComboByText(ui.comBoxDiscountG_2, header.discountGrp, false);
    setComboByText(ui.comBoxSalesRep_2, header.salesRep, true);
    setCheckBoxValue(ui.checkBoxAlloySurcharge_2, header.alloySurcharge);
    setCheckBoxValue(ui.checkBoxSpecialOffers_2, header.specialOffers);

    // checkbox GERMAN CATALOG NOT ACTIVE!!!!
}

// CONTROL CURRENCY
bool Page_2_CloneDatFilesFromDatFile::hasDifferentCurrencyInLines(
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

        // jeśli kolejna waluta jest inna
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
QString Page_2_CloneDatFilesFromDatFile::buildHeaderLineFromUi(
    const QString& originalHeaderLine,
    const QString& customerNo,
    const QString& salesRepNo
)
{
    QStringList h = originalHeaderLine.split('|', Qt::KeepEmptyParts);

    if (h.size() <= 24)
        return originalHeaderLine;

    h[1] = customerNo;      // 1 kolumna CSV
    h[3] = "01";
    h[4] = "02";
    h[5] = ui.comBoxCurrencyHeader_2->currentText();
    h[7] = ui.lineEditDateFrom_2->text();
    h[8] = ui.lineEditDateTo_2->text();
    h[16] = "E-SHOP_EXP_API";
    h[18] = ui.comBoxDiscountG_2->currentText();
    h[19] = ui.checkBoxAlloySurcharge_2->isChecked() ? "X" : "";
    h[20] = ui.checkBoxSpecialOffers_2->isChecked() ? "X" : "";
    h[21] = ui.checkBoxAlloySurcharge->isChecked() ? "X" : "";
    h[22] = "K" + ui.comBoxCatalogNo_2->currentText();
    h[23] = "00" + ui.comBoxCatalogNo_2->currentText();
    h[24] = salesRepNo;     // 2 kolumna CSV

    return h.join('|');
}


// DO POPRAWY TUTAJ SKOŃCZYŁEM
//SPAROWAĆ VECTOR Z PRODUKCJĄ DATÓW Z PONIŻESZJ FDUNCKJI

// EXPORT TO DESKTOP
void Page_2_CloneDatFilesFromDatFile::saveClonedFilesToDesktop(const FileData& data)
{
    FileProcessingProgress progress;    
  
   progress.setWindowTitle("Eksport plików");
   progress.setLabelText("rwa eksport plików...");

    progress.show();
    //DATA FROM DAT FILE
    QStringList datLines = data.content.split('\n');


    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

    QString desktopPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QString folderPath =
        desktopPath
        + QDir::separator()
        + "fachurnik_export_"
        + timestamp;

    QDir dir;

    if (!dir.mkpath(folderPath))
    {
        progress.finish();
        QMessageBox::warning(nullptr, "Błąd", "Nie udało się utworzyć folderu:\n" + folderPath);
        return;
    }

    if (currentCsvData.isEmpty())
    {
        progress.finish();
        QMessageBox::warning(nullptr, "Błąd", "Lista klientów CSV jest pusta.");
        return;
    }

    QString error;
    int total = currentCsvData.size();
    int savedCount = 0;

    for (int i = 0; i < currentCsvData.size(); ++i)
    {
        QString customerNo = currentCsvData[i].first.trimmed();
        QString salesRepNo = currentCsvData[i].second.trimmed();

        if (customerNo.isEmpty())
            continue;

        QStringList clonedLines = datLines;

        for (int j = 0; j < clonedLines.size(); ++j)
        {
            if (clonedLines[j].startsWith("H|"))
            {
                clonedLines[j] = buildHeaderLineFromUi(
                    clonedLines[j],
                    customerNo,
                    salesRepNo
                );
                break;
            }
        }

        QString datPath =
            folderPath
            + QDir::separator()
            + "fachurnik_newfile_"
            + timestamp
            + "_"
            + customerNo
            + ".dat";

        bool ok = FileExport::saveDat(
            clonedLines,
            datPath,
            &error
        );

        if (!ok)
        {
            progress.finish();
            QMessageBox::warning(nullptr, "Błąd", error);
            return;
        }

        savedCount++;

        int value = static_cast<int>((savedCount * 100.0) / total);
        progress.setValue(value);
        QApplication::processEvents();
    }

    progress.setValue(100);
    QApplication::processEvents();
    progress.finish();

    QMessageBox::information(
        nullptr,
        "OK",
        "Zapisano pliki: " + QString::number(savedCount) + "\nFolder:\n" + folderPath
    );
}
