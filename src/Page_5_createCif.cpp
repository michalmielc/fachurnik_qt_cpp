#include "Page_5_createCif.h"
#include"OpenFileDialog.h"
#include "FileLoader.h"
#include <qmessagebox.h>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include "ui_Fachurnik_C.h"

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
            onChooseFileClicked();
        });


    QObject::connect(
        ui.btnReadCifColFields,
        &QPushButton::clicked,
        [this]()
        {
            readHeadersColumnFromCif();
        });

    ui.tableWidgetCifColumns->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidgetCifColumns->setSelectionMode(QAbstractItemView::SingleSelection);

};

//DO POPRAWY TA FUNKCJA 
void Page_5_createCif::onChooseFileClicked() {

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

    ui.tableWidgetCifColumns->clear();
    ui.tableWidgetCifColumns->setColumnCount(2);
    ui.tableWidgetCifColumns->setRowCount(fields.size());

    ui.tableWidgetCifColumns->setHorizontalHeaderLabels({
        "Field",
        "Podsumowanie"
        });

    for (int i = 0; i < fields.size(); ++i)
    {
        QString fieldName = fields[i].trimmed();

        ui.tableWidgetCifColumns->setItem(i, 0, new QTableWidgetItem(fieldName));
        ui.tableWidgetCifColumns->setItem(i, 1, new QTableWidgetItem(""));
    }

    ui.tableWidgetCifColumns->resizeColumnsToContents();
}


// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_5_createCif::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}
