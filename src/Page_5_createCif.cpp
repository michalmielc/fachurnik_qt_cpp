#include "Page_5_createCif.h"
#include"OpenFileDialog.h"
#include "FileLoader.h"
#include <qmessagebox.h>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include "ui_Fachurnik_C.h"
#include <QFileInfo>
#include "FileExportCif.h"
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QColor>

void Page_5_createCif::initialize() {

    BtnBackToMenu::backToMenu(
        ui.btnBackToHome5,
        ui.stackedWidget,
        ui.pageMenu);

    QObject::connect(
        ui.btnOpenFile_5,
        &QPushButton::clicked,
        [this]()
        {
            onChooseCifFileClicked();
        });


    QObject::connect(
        ui.btnReadCifColFields,
        &QPushButton::clicked,
        [this]()
        {
            readHeadersColumnFromCif();
        });

    QObject::connect(
        ui.btnOpenFileItemNum,
        &QPushButton::clicked,
        [this]()
        {
            importItemNumbersFromCsv();
        });

    QObject::connect(ui.tableWidgetCifColumns, &QTableWidget::cellClicked,
        [this](int row, int column) {
            onTableCifColumnClicked(row, column);
        });

    QObject::connect(
        ui.btnSaveColSettings,
        &QPushButton::clicked,
        [this]()
        {
            saveCurrentCifColumnSettings();
        });

    QObject::connect(
        ui.btnOpenFileCsvColumn,
        &QPushButton::clicked,
        [this]()
        {
            loadSourceColumnFromCsv();
        });

    
    QObject::connect(
        ui.comBoxColumnFromCsv,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        {
            if (index < 0)
                return;

            ui.lblColName_54->setText(ui.comBoxColumnFromCsv->currentText());
            ui.lblColNum_54->setText(QString::number(index));
        });


    QObject::connect(
        ui.pushBtnExport_5,
        &QPushButton::clicked,
        [this]()
        {
            createCifFile();
        });


    ui.tableWidgetCifColumns->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidgetCifColumns->setSelectionMode(QAbstractItemView::SingleSelection);

    //MARK CIF COLUMN WHEN EIDTING
    ui.tableWidgetCifColumns->setStyleSheet(
        "QTableWidget::item:selected {"
        "background-color: #3399ff;"
        "color: white;"
        "}"
        "QTableWidget::item:selected:!active {"
        "background-color: #3399ff;"
        "color: white;"
        "}"
    );

};

//DO POPRAWY TA FUNKCJA 
void Page_5_createCif::onChooseCifFileClicked() {

    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik CIF",
        "CIF Files (*.cif)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_51, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_51, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_51, "", "red");

        return;
    }

    FileProcessingProgress progress;
    progress.show();

    progress.setValue(0);

    currentCifFileData = FileLoader::loadCifHeaderFile(
        path,
        [&](int value)
        {
            progress.setValue(value);
            QApplication::processEvents();
        }
    );

    ui.plainTextEdit_5->setPlainText(currentCifFileData.content);

    setLabel(ui.lblFilePath_51, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_51, "FILE: " + currentCifFileData.fileName, "blue");
    setLabel(
        ui.lblFileCountLines_51,
        "ITEM LINES: " + QString::number(currentCifFileData.lineCount),
        "blue"
    );





    progress.setValue(100);
    QApplication::processEvents();




    QMessageBox::information(
        nullptr,
        "FILE",
        "File loaded successfully"
    );

    progress.finish();
}

//READ HEADERS COLUMN
void Page_5_createCif::readHeadersColumnFromCif()
{
    QString text = ui.plainTextEdit_5->toPlainText();

    QStringList lines = text.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);

    if (lines.isEmpty())
        return;

    QString lastLine = lines.last().trimmed();

    if (lastLine.startsWith("FIELDNAMES:", Qt::CaseInsensitive))
        lastLine.remove(0, QString("FIELDNAMES:").length());

    QStringList fields = lastLine.split(',', Qt::SkipEmptyParts);

    for (QString& field : fields)
    {
        field = field.trimmed();
    }

    fileExportCif.createCifColumns(fields);

    ui.tableWidgetCifColumns->clear();
    ui.tableWidgetCifColumns->setColumnCount(13);
    ui.tableWidgetCifColumns->setHorizontalHeaderLabels({
    "Field",
    "Static",
    "Static value",
    "Empty",
    "From file",
    "Path",
    "File",
    "CSV column",
    "CSV col num",
    "Remove",
    "Convert",
    "Cut",
    "Apost"
        });
    ui.tableWidgetCifColumns->setRowCount(fields.size());

   // ui.tableWidgetCifColumns->setHorizontalHeaderLabels({ "Field",      "Podsumowanie"       });

    for (int i = 0; i < fields.size(); ++i)
    {
        QString fieldName = fields[i].trimmed();

        ui.tableWidgetCifColumns->setItem(i, 0, new QTableWidgetItem(fieldName));
        ui.tableWidgetCifColumns->setItem(i, 1, new QTableWidgetItem(""));
    }

    ui.tableWidgetCifColumns->resizeColumnsToContents();

    if (!fields.isEmpty())
    {
        ui.tableWidgetCifColumns->selectRow(0);
        onTableCifColumnClicked(0, 0);
    }
}

//IMPORT ITRM NUMBERS FROM CIF
void Page_5_createCif::importItemNumbersFromCsv()
{
    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik CSV",
        "CSV Files (*.csv)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_53, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_53, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_53, "", "red");

        return;
    }

    FileProcessingProgress progress;
    progress.show();

    progress.setValue(0);

    currentCsvFileData = FileLoader::loadCsvItemNumbers(
        path,
        [&](int value)
        {
            progress.setValue(value);
            QApplication::processEvents();
        }
    );

    QFileInfo info(path);

    setLabel(ui.lblFilePath_53, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_53, "FILE: " + info.fileName(), "blue");
    setLabel(
        ui.lblFileCountLines_53,
        "ITEM LINES: " + QString::number(currentCsvFileData.size()),
        "blue"
    );





    progress.setValue(100);
    QApplication::processEvents();




    QMessageBox::information(
        nullptr,
        "FILE",
        "File loaded successfully"
    );

    progress.finish();
}

//CLICK ON ROW TABLE COLUMNS SETINGS
void Page_5_createCif::onTableCifColumnClicked(int row, int column)
{
    Q_UNUSED(column);



    auto& cifColumns = fileExportCif.getCifColumns();



    if (row < 0 || row >= cifColumns.size())
        return;

    selectedCifColumnRow = row;

    const auto& settings = cifColumns[row].second;

    ui.lbl_5_FieldName->setText(settings.colName);

    ui.radBtn51->setAutoExclusive(false);
    ui.radBtn52->setAutoExclusive(false);
    ui.radBtn53->setAutoExclusive(false);

    ui.radBtn51->setChecked(false);
    ui.radBtn52->setChecked(false);
    ui.radBtn53->setChecked(false);

    ui.radBtn51->setAutoExclusive(true);
    ui.radBtn52->setAutoExclusive(true);
    ui.radBtn53->setAutoExclusive(true);

    ui.radBtn51->setChecked(settings.isStaticField);
    ui.radBtn52->setChecked(settings.isEmpty);
    ui.radBtn53->setChecked(settings.fromFile);

    ui.lineEditStaticVal->setText(settings.staticValue);

    ui.lblFilePath_54->setText(settings.path);
    ui.lblFileName_54->setText(settings.fileName);
    ui.lblColName_54->setText(settings.fileColName);
    ui.lblColNum_54->setText(QString::number(settings.fileColNum));

    ui.ckBoxRemove->setChecked(settings.removeSemAndApo);
    ui.ckBoxConvPrice->setChecked(settings.convertPrice);
    ui.ckBoxCutStr->setChecked(settings.cutString);
    ui.lineEdit_NumOfChar->setText(QString::number(settings.lengthToCut));
    ui.ckBoxAddTemNum->setChecked(settings.addItemNumAtEnd);
    ui.ckBoxInsertApost->setChecked(settings.insertApo);

    ui.comBoxColumnFromCsv->clear();

    if (!settings.fileColName.isEmpty())
    {
        // Zak³adam, ¿e currentCsvHeaders zawiera wszystkie nag³ówki
        ui.comBoxColumnFromCsv->addItems(currentCsvHeaders);

        int index = ui.comBoxColumnFromCsv->findText(settings.fileColName);

        if (index >= 0)
            ui.comBoxColumnFromCsv->setCurrentIndex(index);
    }
}

//SAVE COLUMNS SETINGS
void Page_5_createCif::saveCurrentCifColumnSettings()
{
    auto& cifColumns = fileExportCif.getCifColumns();

    if (selectedCifColumnRow < 0 || selectedCifColumnRow >= cifColumns.size())
        return;


    auto& settings = cifColumns[selectedCifColumnRow].second;

    settings.isStaticField = ui.radBtn51->isChecked();
    settings.isEmpty = ui.radBtn52->isChecked();
    settings.fromFile = ui.radBtn53->isChecked();

    settings.staticValue = ui.lineEditStaticVal->text();

    settings.removeSemAndApo = ui.ckBoxRemove->isChecked();
    settings.convertPrice = ui.ckBoxConvPrice->isChecked();
    settings.cutString = ui.ckBoxCutStr->isChecked();
    settings.lengthToCut = ui.lineEdit_NumOfChar->text().toInt();
    settings.addItemNumAtEnd = ui.ckBoxAddTemNum->isChecked();
    settings.insertApo = ui.ckBoxInsertApost->isChecked();



    settings.path = ui.lblFilePath_54->text();
    settings.fileName = ui.lblFileName_54->text();
    settings.fileColName = ui.comBoxColumnFromCsv->currentText();
    settings.fileColNum = ui.comBoxColumnFromCsv->currentIndex();
    ui.lblColName_54->setText(ui.comBoxColumnFromCsv->currentText());
    ui.lblColNum_54->setText(QString::number(ui.comBoxColumnFromCsv->currentIndex()));

    updateCifColumnTableRow(selectedCifColumnRow);

    // kolor zapisanego wiersza
    for (int col = 0; col < ui.tableWidgetCifColumns->columnCount(); col++)
    {
        QTableWidgetItem* item =
            ui.tableWidgetCifColumns->item(selectedCifColumnRow, col);

        if (item)
            item->setBackground(QColor(180, 255, 180));
    }


    QMessageBox::information(nullptr, "Saved", "Column settings saved.");


}


void Page_5_createCif::loadSourceColumnFromCsv() {

    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik CSV",
        "CSV Files (*.csv)"
    );

    if (path.isEmpty())
        return;

    ui.radBtn53->setChecked(true);

    currentCsvHeaders = FileLoader::loadCsvHeaders(path);

    QFileInfo info(path);

    setLabel(ui.lblFilePath_54, path, "blue");
    setLabel(ui.lblFileName_54,  info.fileName(), "blue");

    ui.radBtn53->setChecked(true);

    ui.comBoxColumnFromCsv->clear();
    ui.comBoxColumnFromCsv->addItems(currentCsvHeaders);

    if (!currentCsvHeaders.isEmpty())
    {
        ui.comBoxColumnFromCsv->setCurrentIndex(0);

        ui.lblColName_54->setText(ui.comBoxColumnFromCsv->currentText());
        ui.lblColNum_54->setText(QString::number(ui.comBoxColumnFromCsv->currentIndex()));
    }
}

void Page_5_createCif::updateCifColumnTableRow(int row) {
    auto& columns = fileExportCif.getCifColumns();

    if (row < 0 || row >= columns.size())
        return;

    const auto& s = columns[row].second;

    ui.tableWidgetCifColumns->setItem(row, 0, new QTableWidgetItem(columns[row].first));
    ui.tableWidgetCifColumns->setItem(row, 1, new QTableWidgetItem(s.isStaticField ? "YES" : ""));
    ui.tableWidgetCifColumns->setItem(row, 2, new QTableWidgetItem(s.staticValue));
    ui.tableWidgetCifColumns->setItem(row, 3, new QTableWidgetItem(s.isEmpty ? "YES" : ""));
    ui.tableWidgetCifColumns->setItem(row, 4, new QTableWidgetItem(s.fromFile ? "YES" : ""));
    ui.tableWidgetCifColumns->setItem(row, 5, new QTableWidgetItem(s.path));
    ui.tableWidgetCifColumns->setItem(row, 6, new QTableWidgetItem(s.fileName));
    ui.tableWidgetCifColumns->setItem(row, 7, new QTableWidgetItem(s.fileColName));
    ui.tableWidgetCifColumns->setItem(row, 8, new QTableWidgetItem(QString::number(s.fileColNum)));
    ui.tableWidgetCifColumns->setItem(row, 9, new QTableWidgetItem(s.removeSemAndApo ? "YES" : ""));
    ui.tableWidgetCifColumns->setItem(row, 10, new QTableWidgetItem(s.convertPrice ? "YES" : ""));
    ui.tableWidgetCifColumns->setItem(row, 11, new QTableWidgetItem(s.cutString ? QString::number(s.lengthToCut) : ""));
    ui.tableWidgetCifColumns->setItem(row, 12, new QTableWidgetItem(s.insertApo ? "YES" : ""));
}


void Page_5_createCif::createCifFile()
{
    FileProcessingProgress progress;
    progress.show();
    progress.setValue(0);

    auto rows = fileExportCif.buildRows(
        currentCsvFileData,
        [&](int current, int total)
        {
            int percent = 100;

            if (total > 0)
                percent = (current * 100) / total;

            progress.setValue(percent);

            progress.setLabelText(
                QString("COLUMN %1/%2")
                .arg(current)
                .arg(total)
            );

            QApplication::processEvents();
        }
    );

    QString savePath = QFileDialog::getSaveFileName(
        nullptr,
        "Zapisz plik CIF",
        "",
        "CIF Files (*.cif)"
    );

    if (savePath.isEmpty())
    {
        progress.finish();
        return;
    }

    QString error;

    bool ok = fileExportCif.saveCifFile(
        currentCifFileData.header.headerCif,
        rows,
        savePath,
        &error
    );

    progress.finish();

    if (!ok)
    {
        QMessageBox::warning(
            nullptr,
            "Error",
            error.isEmpty()
            ? "Nie uda³o siê zapisaæ pliku."
            : error
        );

        return;
    }

    QMessageBox::information(
        nullptr,
        "Export",
        "Plik CIF zapisany."
    );
}


// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_5_createCif::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}
