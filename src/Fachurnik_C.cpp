#include "Fachurnik_C.h"
#include "FileLoader.h"
#include "MenuTreeBuilder.h"
#include "OpenFileDialog.h"
#include "BtnBackToMenu.h"
#include <QTreeWidget>
#include "FileLoadingProgress.h"
#include <QApplication>
#include "ComboBoxHelper.h"
#include "qmessagebox.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>

Fachurnik_C::Fachurnik_C(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //// REDIRECTION TO MAIN PAGE --------------
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome1,
        ui.stackedWidget,
        ui.pageMenu);


    BtnBackToMenu::backToMenu(
        ui.btnBackToHome2,
        ui.stackedWidget,
        ui.pageMenu);
    //------------------------------------------

    ui.stackedWidget->setCurrentWidget(ui.pageMenu);

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


    // open file dialog

    connect(ui.btnOpenFile,
        &QPushButton::clicked,
        this,
        &Fachurnik_C::onChooseFileClicked);

    ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrency);
    ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG);
    ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep);

    hideShowGrpBox(false);

    connect(ui.radBtnEUR, &QRadioButton::clicked,
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

//// open file dialog
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
}

void Fachurnik_C::hideShowGrpBox(bool b)
{
    ui.groupBoxDCh->setVisible(b);
    ui.groupBoxHeader->setVisible(b);
    ui.groupBoxCurrEx->setVisible(b);
    ui.groupBoxFileExp->setVisible(b);
}

void Fachurnik_C::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}

void Fachurnik_C::setComboByText(QComboBox* comboBox, const QString& text,bool startsWith)
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

void Fachurnik_C::loadHeaderToUi(const HeaderData& header)
{
    //CUSTOMER NO
    ui.lineEditCustomerNo->setText(header.customerNo);


    //ZAZNACZENIE DIST CHANNEL
    ui.radioButton1->setChecked(header.distrChannel == "01");
    ui.radioButton2->setChecked(header.distrChannel == "02");
    ui.radioButton3->setChecked(header.distrChannel == "03");
    ui.radioButton4->setChecked(header.distrChannel == "04");

    //DATE FROM TO
    ui.lineEditDateFrom->setText(header.dateFrom);
    ui.lineEditDateTo->setText(header.dateTo);

    setComboByText(ui.comBoxCurrency, header.currency, false);
    setComboByText(ui.comBoxDiscountG, header.discountGrp, false);
    setComboByText(ui.comBoxSalesRep, header.salesRep, true);
    setCheckBoxValue(ui.checkBoxAlloySurcharge, header.alloySurcharge);
    setCheckBoxValue(ui.checkBoxSpecialOffers,header.specialOffers );
        
}

//// menu doubleclick
void Fachurnik_C::onMenuDoubleClicked(QTreeWidgetItem* item, int column)
{
    QString text = item->text(0);

    if (text == "eshop file to E-SHOP")
    {
        ui.stackedWidget->setCurrentWidget(ui.pageEshopToEshop);
    }

    if (text == "create eshop files to E-SHOP")
    {
        ui.stackedWidget->setCurrentWidget(ui.pageCloneFilesEshopToEshop);
    }

}
// menu click
void Fachurnik_C::onMenuClicked(QTreeWidgetItem* item, int column)
{
    QString text = item->text(0);

    if (text == "eshop file to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji zmodyfikujesz  walutę, nr katalogu, dodatki etc.
    Format do wczytania: dat. Format wyjściowy: dat. Dane można zapisać do Excela.)");
    }

    if (text == "create eshop files to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji sklonujesz
       pliki na bazie jednego pliku z konkretną grupą rabatową)");
    }


}

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


QString Fachurnik_C::distrChannelFromUi() const
{
    if (ui.radioButton1->isChecked()) return "01";
    if (ui.radioButton2->isChecked()) return "02";
    if (ui.radioButton3->isChecked()) return "03";
    if (ui.radioButton4->isChecked()) return "04";

    return "";
}

void Fachurnik_C::saveModifiedFileToDesktop(const FileData& data)
{
    QString targetCurrency = ui.comBoxCurrency->currentText();

    double exchangeRate = ui.lineEditExchangeRate->text()
        .replace(",", ".")
        .toDouble();

    QStringList lines = data.content.split('\n', Qt::SkipEmptyParts);

    if (lines.isEmpty())
        return;

    lines[0] = buildHeaderLineFromUi(data.header.headerLine);

    for (int i = 1; i < lines.size(); ++i)
    {
        if (!lines[i].startsWith("L|"))
            continue;

        QStringList p = lines[i].split('|', Qt::KeepEmptyParts);

        if (p.size() <= 8)
            continue;

        QString oldCurrency = p[6];

        if (oldCurrency != targetCurrency)
        {
            p[6] = targetCurrency;

            bool ok = false;
            double oldValue = p[8].replace(",", ".").toDouble(&ok);

            if (ok)
            {
                double newValue = oldValue * exchangeRate;

                p[8] = QString::number(newValue, 'f', 2)
                    .replace(".", ",");
            }

            lines[i] = p.join('|');
        }
    }

    QString desktopPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QString newFilePath =
        desktopPath + QDir::separator() + "NOWY_" + data.fileName;

    QFile outFile(newFilePath);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Błąd", "Nie można zapisać pliku.");
        return;
    }

    QTextStream out(&outFile);

    for (const QString& line : lines)
        out << line << "\n";

    outFile.close();

    QMessageBox::information(this, "OK", "Zapisano:\n" + newFilePath);
}

Fachurnik_C::~Fachurnik_C()
{}

