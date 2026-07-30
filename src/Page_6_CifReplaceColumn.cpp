#include "Page_6_CifReplaceColumn.h"
#include "ui_Fachurnik_C.h"
#include "OpenFileDialog.h"
#include "FileLoader.h"
#include <qmessagebox.h>
#include "FileExportCif.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QColor>
#include <qmessagebox.h>
#include <QDebug>

//DONE
void Page_6_CifReplaceColumn::initialize() {

    BtnBackToMenu::backToMenu(
        ui.btnBackToHome6,
        ui.stackedWidget,
        ui.pageMenu);

    QObject::connect(
        ui.btnOpenFile_6,
        &QPushButton::clicked,
        [this]()
        {
            onChooseCifFileClicked();
        });

    QObject::connect(
        ui.btnReadCifColFields_6,
        &QPushButton::clicked,
        [this]()
        {
            readHeadersColumnFromCif();
        });


    QObject::connect(ui.tableWidgetCifColumns_2, &QTableWidget::cellClicked,
        [this](int row, int column) {
            onTableCifColumnClicked(row, column);
        });

    QObject::connect(
        ui.btnReplaceColumn,
        &QPushButton::clicked,
        [this]()
        {
            replaceColumnButton();
        });

    QObject::connect(
        ui.btnOpenFileCsvColumn_61,
        &QPushButton::clicked,
        [this]()
        {
            loadSourceColumnFromCsv();
        });


    QObject::connect(
        ui.comBoxColumnFromCsv_6,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int index)
        {
            if (index < 0)
                return;

            ui.lblColName_69->setText(ui.comBoxColumnFromCsv_6->currentText());
            ui.lblColNum_691->setText(QString::number(index));
        });


    QObject::connect(
        ui.pushBtnExport_6,
        &QPushButton::clicked,
        [this]()
        {
            createCifFile();
        });


    ui.tableWidgetCifColumns_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidgetCifColumns_2->setSelectionMode(QAbstractItemView::SingleSelection);

    //MARK CIF COLUMN WHEN EIDTING
    ui.tableWidgetCifColumns_2->setStyleSheet(
        "QTableWidget::item:selected {"
        "background-color: #3399ff;"
        "color: white;"
        "}"
        "QTableWidget::item:selected:!active {"
        "background-color: #3399ff;"
        "color: white;"
        "}"
    );
}

//DONE
void Page_6_CifReplaceColumn::onChooseCifFileClicked() {

    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik CIF",
        "CIF Files (*.cif)"
    );

    if (path.isEmpty())
    {
        setLabel(ui.lblFilePath_61, "Nie wybrano pliku", "red");
        setLabel(ui.lblFileName_62, "Brak pliku", "red");
        setLabel(ui.lblFileCountLines_63, "", "red");

        return;
    }

    FileProcessingProgress progress;
    progress.show();

    progress.setValue(0);

    currentCifFileData = FileLoader::loadCifFile(
        path,
        [&](int value)
        {
            progress.setValue(value);
            QApplication::processEvents();
        }
    );

    ui.plainTextEdit_6->setPlainText(currentCifFileData.content);

    setLabel(ui.lblFilePath_61, "PATH: " + path, "blue");
    setLabel(ui.lblFileName_62, "FILE: " + currentCifFileData.fileName, "blue");
    setLabel(
        ui.lblFileCountLines_63,
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
void Page_6_CifReplaceColumn::readHeadersColumnFromCif()
{
    QString text = ui.plainTextEdit_6->toPlainText();

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

    ui.tableWidgetCifColumns_2->clear();
    ui.tableWidgetCifColumns_2->setColumnCount(13);
    ui.tableWidgetCifColumns_2->setHorizontalHeaderLabels({
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
    ui.tableWidgetCifColumns_2->setRowCount(fields.size());

    // ui.tableWidgetCifColumns->setHorizontalHeaderLabels({ "Field",      "Podsumowanie"       });

    for (int i = 0; i < fields.size(); ++i)
    {
        QString fieldName = fields[i].trimmed();

        ui.tableWidgetCifColumns_2->setItem(i, 0, new QTableWidgetItem(fieldName));
        ui.tableWidgetCifColumns_2->setItem(i, 1, new QTableWidgetItem(""));

        ui.comboBoxCifColumn->addItem(fieldName);
    }

    ui.tableWidgetCifColumns_2->resizeColumnsToContents();

    if (!fields.isEmpty())
    {
        ui.tableWidgetCifColumns_2->selectRow(0);
        onTableCifColumnClicked(0, 0);
    }
}


//CLICK ON ROW TABLE COLUMNS SETINGS
void Page_6_CifReplaceColumn::onTableCifColumnClicked(int row, int column)
{
    Q_UNUSED(column);
    auto& cifColumns = fileExportCif.getCifColumns();

    if (row < 0 || row >= cifColumns.size())
        return;

    selectedCifColumnRow = row;

    const auto& settings = cifColumns[row].second;

    ui.lbl_ColumnToReplace->setText(settings.colName);
    ui.lbl_ColumnToReplace_2->setText(QString::number(row));

    ui.radBtn61->setAutoExclusive(false);
    ui.radBtn62->setAutoExclusive(false);
    ui.radBtn63->setAutoExclusive(false);

    ui.radBtn61->setChecked(false);
    ui.radBtn62->setChecked(false);
    ui.radBtn63->setChecked(false);

    ui.radBtn61->setAutoExclusive(true);
    ui.radBtn62->setAutoExclusive(true);
    ui.radBtn63->setAutoExclusive(true);

    ui.radBtn61->setChecked(settings.isStaticField);
    ui.radBtn62->setChecked(settings.isEmpty);
    ui.radBtn63->setChecked(settings.fromFile);

    ui.lineEditStaticVal_2->setText(settings.staticValue);

    ui.lblFilePath_67->setText(settings.path);
    ui.lblFileName_68->setText(settings.fileName);
    ui.lblColName_69->setText(settings.fileColName);
    ui.lblColNum_691->setText(QString::number(settings.fileColNum));

    ui.ckBoxRemove_2->setChecked(settings.removeSemAndApo);
    ui.ckBoxConvPrice_2->setChecked(settings.convertPrice);
    ui.ckBoxCutStr_2->setChecked(settings.cutString);
    ui.lineEdit_NumOfChar_2->setText(QString::number(settings.lengthToCut));
    ui.ckBoxAddTemNum_2->setChecked(settings.addItemNumAtEnd);
    ui.ckBoxInsertApost_2->setChecked(settings.insertApo);

    ui.ckBoxFillRndVal_2->setChecked(settings.fillRandomVal);
    ui.ckBoxRemoveItemIfIsEmpty_2->setChecked(settings.removeItemFromCat);

    ui.comBoxColumnFromCsv_6->clear();

    if (!settings.fileColName.isEmpty())
    {
        // Zak³adam, ¿e currentCsvHeaders zawiera wszystkie nag³ówki
        ui.comBoxColumnFromCsv_6->addItems(currentCsvHeaders);

        int index = ui.comBoxColumnFromCsv_6->findText(settings.fileColName);

        if (index >= 0)
            ui.comBoxColumnFromCsv_6->setCurrentIndex(index);
    }
}


void Page_6_CifReplaceColumn::updateCifColumnTableRow(int row) {
    auto& columns = fileExportCif.getCifColumns();

    if (row < 0 || row >= columns.size())
        return;

    const auto& s = columns[row].second;

    ui.tableWidgetCifColumns_2->setItem(row, 0, new QTableWidgetItem(columns[row].first));
    ui.tableWidgetCifColumns_2->setItem(row, 1, new QTableWidgetItem(s.isStaticField ? "YES" : ""));
    ui.tableWidgetCifColumns_2->setItem(row, 2, new QTableWidgetItem(s.staticValue));
    ui.tableWidgetCifColumns_2->setItem(row, 3, new QTableWidgetItem(s.isEmpty ? "YES" : ""));
    ui.tableWidgetCifColumns_2->setItem(row, 4, new QTableWidgetItem(s.fromFile ? "YES" : ""));
    ui.tableWidgetCifColumns_2->setItem(row, 5, new QTableWidgetItem(s.path));
    ui.tableWidgetCifColumns_2->setItem(row, 6, new QTableWidgetItem(s.fileName));
    ui.tableWidgetCifColumns_2->setItem(row, 7, new QTableWidgetItem(s.fileColName));
    ui.tableWidgetCifColumns_2->setItem(row, 8, new QTableWidgetItem(QString::number(s.fileColNum)));
    ui.tableWidgetCifColumns_2->setItem(row, 9, new QTableWidgetItem(s.removeSemAndApo ? "YES" : ""));
    ui.tableWidgetCifColumns_2->setItem(row, 10, new QTableWidgetItem(s.convertPrice ? "YES" : ""));
    ui.tableWidgetCifColumns_2->setItem(row, 11, new QTableWidgetItem(s.cutString ? QString::number(s.lengthToCut) : ""));
    ui.tableWidgetCifColumns_2->setItem(row, 12, new QTableWidgetItem(s.insertApo ? "YES" : ""));
}


void Page_6_CifReplaceColumn::loadSourceColumnFromCsv() {

    QString path = OpenFileDialog::openFile(
        nullptr,
        "Wybierz plik CSV",
        "CSV Files (*.csv)"
    );

    if (path.isEmpty())
        return;

    ui.radBtn63->setChecked(true);

    currentCsvHeaders = FileLoader::loadCsvHeaders(path);

    QFileInfo info(path);

    setLabel(ui.lblFilePath_67, path, "blue");
    setLabel(ui.lblFileName_68, info.fileName(), "blue");

    ui.radBtn63->setChecked(true);

    ui.comBoxColumnFromCsv_6->clear();
    ui.comBoxColumnFromCsv_6->addItems(currentCsvHeaders);

    if (!currentCsvHeaders.isEmpty())
    {
        ui.comBoxColumnFromCsv_6->setCurrentIndex(0);

        ui.lblColName_69->setText(ui.comBoxColumnFromCsv_6->currentText());
        ui.lblColNum_691->setText(QString::number(ui.comBoxColumnFromCsv_6->currentIndex()));
    }
}


void Page_6_CifReplaceColumn::createCifFile()
{
    QString savePath = QFileDialog::getSaveFileName(
        nullptr,
        "Zapisz plik CIF",
        "",
        "CIF Files (*.cif)"
    );

    if (savePath.isEmpty())
        return;

    QString error;

    const bool ok =
        fileExportCif.saveRebuiltCifFile(
            currentCifFileData,
            savePath,
            &error
        );

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


void Page_6_CifReplaceColumn::replaceColumnButton()
{
    const int idColumnIndex =
        ui.comboBoxCifColumn->currentIndex();

    if (idColumnIndex < 0)
    {
        QMessageBox::warning(
            nullptr, 
            "Brak kolumny ID",
            "Wybierz kolumnê zawieraj¹c¹ ID artyku³u."
        );

        return;
    }

    const int targetColumnIndex =
        ui.lbl_ColumnToReplace_2->text().toInt();

    if (targetColumnIndex < 0)
    {
        QMessageBox::warning(
            nullptr, 
            "Brak kolumny",
            "Wybierz kolumnê przeznaczon¹ do podmiany."
        );

        return;
    }


    settings.colName = ui.lbl_ColumnToReplace_2->text();

    settings.isStaticField = ui.radBtn61->isChecked();
    settings.isEmpty = ui.radBtn62->isChecked();
    settings.fromFile = ui.radBtn63->isChecked();

    settings.staticValue = ui.lineEditStaticVal_2->text();

    settings.path = ui.lblFilePath_67->text();
    settings.fileName = ui.lblFileName_68->text();
    settings.fileColName = ui.comBoxColumnFromCsv_6->currentText();
    settings.fileColNum = ui.comBoxColumnFromCsv_6->currentIndex();

    settings.removeSemAndApo = ui.ckBoxRemove_2->isChecked();
    settings.convertPrice = ui.ckBoxConvPrice_2->isChecked();
    settings.cutString = ui.ckBoxCutStr_2->isChecked();
    settings.lengthToCut =
        ui.lineEdit_NumOfChar_2->text().toInt();

    settings.addItemNumAtEnd =
        ui.ckBoxAddTemNum_2->isChecked();

    settings.insertApo =
        ui.ckBoxInsertApost_2->isChecked();

    settings.fillRandomVal =
        ui.ckBoxFillRndVal_2->isChecked();

    settings.removeItemFromCat =
        ui.ckBoxRemoveItemIfIsEmpty_2->isChecked();

    auto& columns = fileExportCif.getCifColumns();

    if (targetColumnIndex >= 0 &&
        targetColumnIndex < columns.size())
    {
        columns[targetColumnIndex].second = settings;
        updateCifColumnTableRow(targetColumnIndex);
    }

    FileProcessingProgress progress;
    progress.show();
    progress.setValue(0);

    fileExportCif.replaceColumnInCif(
        currentCifFileData,
        idColumnIndex,
        targetColumnIndex,
        settings,
        [&](int current, int total)
        {
            if (total <= 0)
                return;

            if (current % 1000 == 0 || current == total)
            {
                int percent = (current * 100) / total;

                progress.setValue(percent);

                progress.setLabelText(
                    QString("ROW %1/%2")
                    .arg(current)
                    .arg(total)
                );

                QApplication::processEvents();
            }
        }
    );

    progress.setValue(100);
    QApplication::processEvents();
    progress.finish();

    QMessageBox::information(
        nullptr,
        "Gotowe",
        "Kolumna zosta³a podmieniona."
    );

}

//ZROBIONE
void Page_6_CifReplaceColumn::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}