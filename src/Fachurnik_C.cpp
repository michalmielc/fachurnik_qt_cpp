#include "Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "ComboBoxHelper.h"
#include "MenuTreeBuilder.h"
#include <QTreeWidget>
#include <QApplication>
#include "FileLoadingProgress.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include "OpenFileDialog.h"
#include "FileLoader.h"
#include "FileExport.h"

Fachurnik_C::Fachurnik_C(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //// REDIRECTION TO MAIN PAGE ----------------------
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome1,
        ui.stackedWidget,
        ui.pageMenu);

    BtnBackToMenu::backToMenu(
        ui.btnBackToHome2,
        ui.stackedWidget,
        ui.pageMenu);
    //--------------------------------------------------

    ui.stackedWidget->setCurrentWidget(ui.pageMenu);

    // GENERATING MENU --------------------------------
    ui.treeWidgetMenu->setHeaderHidden(true);

    MenuTreeBuilder::build(ui.treeWidgetMenu);

    // double click on menu redirection on the page
    connect(ui.treeWidgetMenu,
        &QTreeWidget::itemDoubleClicked,
        this,
        &Fachurnik_C::onMenuDoubleClicked);
    
    //click on menu more about functionality
    connect(ui.treeWidgetMenu,
        &QTreeWidget::itemClicked,
        this,
        &Fachurnik_C::onMenuClicked);

    //--------------------------------------------------
    // 
    //FILLING DATA INTO COMBOBOXES
    ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrency);
    ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG);
    ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep);


    //.............................do sprawdzenia i opisania
    // 
    // open file dialog
    connect(ui.btnOpenFile,
        &QPushButton::clicked,
        this,
        &Fachurnik_C::onChooseFileClicked);


    hideShowGrpBox(false);

    connect(ui.radBtnEUR, &QRadioButton::clicked,
        this, [this]()
        {
            ui.lineEditExchangeRate->setText("1,00");
        });

    connect(ui.radBtnPLN, &QRadioButton::clicked,
        this, [this]()
        {
            ui.lineEditExchangeRate->setText("1,00");
        });

    QDoubleValidator* validator =
        new QDoubleValidator(0.0, 9999.9999, 4, this);

    validator->setNotation(QDoubleValidator::StandardNotation);

    ui.lineEditExchangeRate->setValidator(validator);


    connect(ui.pushBtnExport, &QPushButton::clicked,
        this, [this]()
        {
            saveModifiedFileToDesktop(currentFileData);
        });
}

//OTHER FUNCTIONS:---------------------------------------
//MENU 
// CHOOSING OPTION FROM MENU
void Fachurnik_C::onMenuDoubleClicked(QTreeWidgetItem* item, int column)
{
    QString text = item->text(0);

    if (text == "eshop file to E-SHOP")
    {
        ui.stackedWidget->setCurrentWidget(ui.pageDatFileToEshop);
    }

    if (text == "create eshop files to E-SHOP")
    {
        ui.stackedWidget->setCurrentWidget(ui.pageCloneFilesEshopToEshop);
    }

}

// DISPLAY FUNCTIONALITY AFTER CHOOSED OPTION FROM MENU
void Fachurnik_C::onMenuClicked(QTreeWidgetItem* item, int column)
{
    QString text = item->text(0);

    if (text == "eshop file to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji zmodyfikujesz  walutę, nr katalogu, dodatki etc.
    Format do wczytania: dat. 
    Format wyjściowy: dat z zapisem do pulpitu. 
    Dane można zapisać równocześnie do Excela.)");
    }

    if (text == "create eshop files to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji sklonujesz
       pliki na bazie jednego pliku z konkretną grupą rabatową)");
    }


}


//PAGES FUNCTIONALITIES:---------------------------------
// 
// OPEN FILE DIALOG 
void Fachurnik_C::onChooseFileClicked()
{
    QString path = OpenFileDialog::openFile(
        this,
        "Wybierz plik DAT",
        "DAT Files (*.dat)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines, "", "red");

        hideShowGrpBox(false);

        return;
    }

    FileLoadingProgress progress(this);
    progress.show();

    currentFileData = FileLoader::loadFile(
        path,
        [&](int value)
        {
            progress.setValue(value);
            QApplication::processEvents();
        }
    );

    progress.finish();

    setLabel(ui.lblFilePath, "PATH: " + path, "blue");
    setLabel(ui.lblFileName, "FILE: " + currentFileData.fileName, "blue");
    setLabel(
        ui.lblFileCountLines,
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
        setLabel(ui.lblFileCountLines, "Brak nagłówka H| w pliku", "red");
        hideShowGrpBox(false);
    }


    if (hasDifferentCurrencyInLines(currentFileData))
    {
        ui.lblControlCurrency->setText("Uwaga: wykryto różne waluty w pozycjach.");
        ui.lblControlCurrency->setStyleSheet("color: red;");
        ui.lblControlCurrency->show();
    }
    else
    {
        ui.lblControlCurrency->clear();
        ui.lblControlCurrency->hide();
    }
}

// HIDE GRPBOX
void Fachurnik_C::hideShowGrpBox(bool b)
{
    ui.groupBoxDCh->setVisible(b);
    ui.groupBoxHeader->setVisible(b);
    ui.groupBoxCurrEx->setVisible(b);
    ui.groupBoxFileExp->setVisible(b);
}

// READ DATA TO UI FROM FILE
void Fachurnik_C::loadHeaderToUi(const HeaderData& header)
{
    //CUSTOMER NO
    ui.lineEditCustomerNo->setText(header.customerNo);


    //SELECT DIST CHANNEL
    ui.radioButton1->setChecked(header.distrChannel == "01");
    ui.radioButton2->setChecked(header.distrChannel == "02");
    ui.radioButton3->setChecked(header.distrChannel == "03");
    ui.radioButton4->setChecked(header.distrChannel == "04");
    ui.groupBoxDCh->setEnabled(false);

    //DATE FROM TO
    ui.lineEditDateFrom->setText(header.dateFrom);
    ui.lineEditDateTo->setText(header.dateTo);

    //OTHERS
    setComboByText(ui.comBoxCurrency, header.currency, false);
    setComboByText(ui.comBoxDiscountG, header.discountGrp, false);
    setComboByText(ui.comBoxSalesRep, header.salesRep, true);
    setCheckBoxValue(ui.checkBoxAlloySurcharge, header.alloySurcharge);
    setCheckBoxValue(ui.checkBoxSpecialOffers, header.specialOffers);
    // checkbox GERMAN CATALOG NOT ACTIVE!!!!
}

// SET COMOBOX AFTER READING HEADER
void Fachurnik_C::setComboByText(QComboBox* comboBox, const QString& text, bool startsWith)
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
void Fachurnik_C::setCheckBoxValue(QCheckBox* checkBox, const bool val) {

    if (val)
    {
        QMessageBox::information(
            nullptr,
            "Debug",
            QString::number(val)
        );
        checkBox->setChecked(true);
    }
}

// STYLE:------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Fachurnik_C::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}

//EXPORT FUNCTIONALITIES:---------------------------------

// DATA READING FROM CONTROLS
QString Fachurnik_C::buildHeaderLineFromUi(const QString& originalHeaderLine)
{
    QStringList h = originalHeaderLine.split('|', Qt::KeepEmptyParts);

    if (h.size() < 14)
        return originalHeaderLine;

    h[1] = ui.lineEditCustomerNo->text();
    h[3] = distrChannelFromUi();
    h[5] = ui.comBoxCurrency->currentText();
    h[6] = ui.comBoxDiscountG->currentText();
    h[7] = ui.lineEditDateFrom->text();
    h[8] = ui.lineEditDateTo->text();
    h[9] = ui.comBoxSalesRep->currentText();
    h[19] = ui.checkBoxAlloySurcharge->isChecked() ? "X" : "";
    h[20] = ui.checkBoxSpecialOffers->isChecked() ? "X" : "";

    return h.join('|');
}

// DATA READING FROM DISTR CHANNEL
QString Fachurnik_C::distrChannelFromUi() const
{
    if (ui.radioButton1->isChecked()) return "01";
    if (ui.radioButton2->isChecked()) return "02";
    if (ui.radioButton3->isChecked()) return "03";
    if (ui.radioButton4->isChecked()) return "04";

    return "";
}

// EXPORT TO DESKTOP
void Fachurnik_C::saveModifiedFileToDesktop(const FileData& data)
{
    FileLoadingProgress progress(this);

    progress.setWindowTitle("Eksport pliku");
    progress.setLabelText("Trwa eksport pliku...");
    progress.show();
  


    QString targetCurrency = ui.comBoxCurrency->currentText();
    
    if (ui.radBtnEUR->isChecked())
    {
        targetCurrency = "EUR";
    }

    if (ui.radBtnPLN->isChecked())
    {
        targetCurrency = "PLN";
    }


    double exchangeRate = ui.lineEditExchangeRate->text()
        .replace(",", ".")
        .toDouble();

    QString customerNo = ui.lineEditCustomerNo->text();

    QString headerLine = buildHeaderLineFromUi(data.header.headerLine);
    
    QString savedPath;
    QString error;


    bool ok = FileExport::exportModifiedDatToDatDesktop(
        data,
        headerLine,
        targetCurrency,
        exchangeRate,
        customerNo,
        &progress,
        &savedPath,
        &error
    );

    if (ui.checkBoxExportToCsv->isChecked())
    {
        QStringList lines = data.content.split('\n', Qt::KeepEmptyParts);
        lines[0] = headerLine;

        QString desktopPath =
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

        QString csvPath =
            desktopPath
            + QDir::separator()
            + "fachurnik_newfile_"
            + customerNo
            + ".csv";

        bool csvOk = FileExport::exportModifiedDatToCsvDesktop(
            lines,
            csvPath,
            &error
        );

        if (!csvOk)
        {
            progress.finish();
            QMessageBox::warning(this, "Błąd CSV", error);
            return;
        }

        savedPath += "\n" + csvPath;
    }

    progress.finish();

    if (!ok)
    {
        QMessageBox::warning(this, "Błąd", error);
        return;
    }

    QMessageBox::information(this, "OK", "Zapisano:\n" + savedPath);
}

// CONTROL CURRENCY
bool Fachurnik_C::hasDifferentCurrencyInLines(const FileData& data)
{
    QString headerCurrency = data.header.currency.trimmed();

    QStringList lines = data.content.split('\n', Qt::SkipEmptyParts);

    for (const QString& line : lines)
    {
        if (!line.startsWith("L|"))
            continue;

        QStringList p = line.split('|', Qt::KeepEmptyParts);

        if (p.size() <= 6)
            continue;

        QString lineCurrency = p[6].trimmed();

        if (!lineCurrency.isEmpty() && lineCurrency != headerCurrency)
            return true;
    }

    return false;
}

//---------------------------------------------------------
//DESTRUCTOR
Fachurnik_C::~Fachurnik_C()
{}

