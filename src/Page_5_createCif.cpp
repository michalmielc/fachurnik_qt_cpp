#include "Page_5_createCif.h"
#include"OpenFileDialog.h"
#include "FileLoader.h"
#include <qmessagebox.h>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include "ui_Fachurnik_C.h"
#include <QFileInfo>
#include "FileExportCif.h"

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

    

    ui.tableWidgetCifColumns->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidgetCifColumns->setSelectionMode(QAbstractItemView::SingleSelection);

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
    ui.tableWidgetCifColumns->setColumnCount(2);
    ui.tableWidgetCifColumns->setRowCount(fields.size());

   // ui.tableWidgetCifColumns->setHorizontalHeaderLabels({ "Field",      "Podsumowanie"       });

    for (int i = 0; i < fields.size(); ++i)
    {
        QString fieldName = fields[i].trimmed();

        ui.tableWidgetCifColumns->setItem(i, 0, new QTableWidgetItem(fieldName));
        ui.tableWidgetCifColumns->setItem(i, 1, new QTableWidgetItem(""));
    }

    ui.tableWidgetCifColumns->resizeColumnsToContents();
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
    ui.radBtn51->setChecked(settings.isStaticField);
    ui.radBtn52->setChecked(settings.isEmpty);
    ui.radBtn53->setChecked(settings.fromFile);

    ui.lineEditStaticVal->setText(settings.staticValue);

    ui.lblFilePath_52->setText("PATH: " + settings.path);
    ui.lblFileName_52->setText("FILE: " + settings.fileName);
    ui.lblColName_52->setText("COLUMN NAME: " + settings.fileColName);
    ui.lblColNum_53->setText("COLUMN NUM: " + QString::number(settings.fileColNum));

    ui.ckBoxRemove->setChecked(settings.removeSemAndApo);
    ui.ckBoxConvPrice->setChecked(settings.convertPrice);
    ui.ckBoxCutStr->setChecked(settings.cutString);
    ui.lineEdit_NumOfChar->setText(QString::number(settings.lengthToCut));
    ui.ckBoxAddTemNum->setChecked(settings.addItemNumAtEnd);
    ui.ckBoxInsertApost->setChecked(settings.insertApo);
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

    currentCsvHeaders = FileLoader::loadCsvHeaders(path);

    QFileInfo info(path);

    setLabel(ui.lblFilePath_52, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_52, "FILE: " + info.fileName(), "blue");

    ui.comBoxColumnFromCsv->clear();
    ui.comBoxColumnFromCsv->addItems(currentCsvHeaders);
}
// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_5_createCif::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}
