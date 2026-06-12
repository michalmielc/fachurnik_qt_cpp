#include"Page_1_DatFileToEshopFile.h"
#include"OpenFileDialog.h"
#include <QFileDialog>


void Page_1_DatFileToEshopFile::initialize()
{
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome1,
        ui.stackedWidget,
        ui.pageMenu);

    ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo);
    ComboBoxHelper::loadCurrencies(ui.comBoxCurrency);
    ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG);
    ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep);

    QObject::connect(
        ui.btnOpenFile,
        &QPushButton::clicked,
        [this]()
        {
            onChooseFileClicked();
        });
};

void Page_1_DatFileToEshopFile::onChooseFileClicked()
    {
        QString fileName = QFileDialog::getOpenFileName(
            nullptr,
            "Wybierz plik DAT",
            "",
            "DAT files (*.dat);;All files (*.*)"
        );

        if (!fileName.isEmpty())
        {
            ui.lblFilePath->setText(fileName);
        }
    }



//.............................do sprawdzenia i opisania
// 
/*



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
        setLabel(ui.lblFileCountLines, "Brak nag³ówka H| w pliku", "red");
        hideShowGrpBox(false);
    }

    progress.setValue(0);

    if (hasDifferentCurrencyInLines(currentFileData, &progress))
    {
        ui.lblControlCurrency->setText("Warning: Different currencies have been detected in the item lines.");
        ui.lblControlCurrency->setStyleSheet("color: red; font-weight: bold;");
        ui.lblControlCurrency->show();

        QMessageBox::critical(
            this,
            "ERROR!",
            "DIFFERENT CURRENCIES DETECTED IN THE FILE"
        );

    }
    else
    {
        ui.lblControlCurrency->clear();
        ui.lblControlCurrency->hide();
    }

    QMessageBox::information(
        this,
        "FILE",
        "File loaded successfully"
    );

    progress.finish();

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
            QMessageBox::warning(this, "B³¹d CSV", error);
            return;
        }

        savedPath += "\n" + csvPath;
    }

    progress.finish();

    if (!ok)
    {
        QMessageBox::warning(this, "B³¹d", error);
        return;
    }

    QMessageBox::information(this, "OK", "Zapisano:\n" + savedPath);
}

// CONTROL CURRENCY

bool Fachurnik_C::hasDifferentCurrencyInLines(
    const FileData& data,
    QProgressDialog* progress
)
{
    QString headerCurrency = data.header.currency.trimmed();

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

        if (!lineCurrency.isEmpty() &&
            lineCurrency != headerCurrency)
        {
            return true;
        }
    }

    return false;
}

*/