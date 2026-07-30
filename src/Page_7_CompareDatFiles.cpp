
#include"Page_7_CompareDatFiles.h"
#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include <QFileDialog>
#include <qstandardpaths.h>
#include"OpenFileDialog.h"
#include "FileProcessingProgress.h"
#include "FileLoader.h"
#include <qmessagebox.h>
#include "FileExport.h"

void Page_7_CompareDatFiles::initialize()
{
    BtnBackToMenu::backToMenu(
        ui.btnBackToHome7,
        ui.stackedWidget,
        ui.pageMenu);

    QObject::connect(
        ui.btnOpenFile_71,
        &QPushButton::clicked,
        [this]()
        {
            onChooseFileClicked(1);
        });

    QObject::connect(
        ui.btnOpenFile_72,
        &QPushButton::clicked,
        [this]()
        {
            onChooseFileClicked(2);
        });

    QObject::connect(
        ui.pushBtnExport_7,
        &QPushButton::clicked,
        [this]()
        {
            saveComparisonFilesToDesktop();
        }
    );

};

void Page_7_CompareDatFiles::onChooseFileClicked(int btnNum)
{
        if (btnNum != 1 && btnNum != 2)
            return;

        const QString path = OpenFileDialog::openFile(
            nullptr,
            "Wybierz plik DAT",
            "DAT Files (*.dat)"
        );

        QLabel* pathLabel = nullptr;
        QLabel* fileNameLabel = nullptr;
        QLabel* lineCountLabel = nullptr;
        QLabel* currencyLabel = nullptr;
        QGroupBox* distChannelGroup = nullptr;
        FileData* currentData = nullptr;

        if (btnNum == 1)
        {
            pathLabel = ui.lblFilePath_71;
            fileNameLabel = ui.lblFileName_71;
            lineCountLabel = ui.lblFileCountLines_71;
            currencyLabel = ui.lblControlCurrency_71;
            distChannelGroup = ui.groupBoxDCh_71;
            currentData = &currentFileData1;
        }
        else
        {
            pathLabel = ui.lblFilePath_72;
            fileNameLabel = ui.lblFileName_72;
            lineCountLabel = ui.lblFileCountLines_72;
            currencyLabel = ui.lblControlCurrency_72;
            distChannelGroup = ui.groupBoxDCh_72;
            currentData = &currentFileData2;
        }

        if (path.isEmpty())
        {
            setLabel(
                pathLabel,
                "Nie wybrano pliku",
                "red"
            );

            setLabel(
                fileNameLabel,
                "Brak pliku",
                "red"
            );

            setLabel(
                lineCountLabel,
                "",
                "red"
            );

            return;
        }

        FileProcessingProgress progress;
        progress.setValue(0);
        progress.show();

        *currentData = FileLoader::loadDatFile(
            path,
            [&](int value)
            {
                progress.setValue(value);
                QApplication::processEvents();
            }
        );

        setLabel(
            pathLabel,
            "PATH: " + path,
            "blue"
        );

        setLabel(
            fileNameLabel,
            "FILE: " + currentData->fileName,
            "blue"
        );

        setLabel(
            lineCountLabel,
            "ITEM LINES: " +
            QString::number(currentData->lineCount),
            "blue"
        );

        if (!currentData->headerLoaded)
        {
            setLabel(
                lineCountLabel,
                "Brak nag³ówka H| w pliku",
                "red"
            );

            distChannelGroup->setEnabled(false);
        }
        else
        {
            loadDistChannel(
                currentData->header,
                distChannelGroup
            );
        }

        // Sprawdzenie waluty w aktualnie wczytanym pliku
        progress.setValue(0);

        const bool differentCurrency =
            hasDifferentCurrencyInLines(
                *currentData,
                &progress
            );

        progress.setValue(100);
        QApplication::processEvents();

        setLabelCurrencyLine(
            differentCurrency,
            currencyLabel
        );

        progress.finish();

        QMessageBox::information(
            nullptr,
            "FILE",
            "File loaded successfully"
        );

}

// CONTROL CURRENCY
bool Page_7_CompareDatFiles::hasDifferentCurrencyInLines(
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

void Page_7_CompareDatFiles::loadDistChannel(const HeaderData& header, QGroupBox *gpBox)
{
   
    //SELECT DIST CHANNEL

    if (gpBox->objectName() == "groupBoxDCh_71") {
        ui.radioButton_71_1->setChecked(header.distrChannel == "01");
        ui.radioButton_71_2->setChecked(header.distrChannel == "02");
        ui.radioButton_71_3->setChecked(header.distrChannel == "03");
        ui.radioButton_71_4->setChecked(header.distrChannel == "04");
    }

    else {
        ui.radioButton_72_1->setChecked(header.distrChannel == "01");
        ui.radioButton_72_2->setChecked(header.distrChannel == "02");
        ui.radioButton_72_3->setChecked(header.distrChannel == "03");
        ui.radioButton_72_4->setChecked(header.distrChannel == "04");
    }
    gpBox->setEnabled(false);

    // checkbox GERMAN CATALOG NOT ACTIVE!!!!
}
//DONE
void Page_7_CompareDatFiles::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}

void Page_7_CompareDatFiles::setLabelCurrencyLine(bool value, QLabel* label)
{
    if (value)
    {
        label->clear();
        label->setText("Warning: Different currencies have been detected in the item lines.");
        label->setStyleSheet("color: red; font-weight: bold; font-family:Bahnschrift; font-size: 11pt");
        label->show();

        QMessageBox::critical(
            nullptr,
            "ERROR!",
            "DIFFERENT CURRENCIES DETECTED IN THE FILE"
        );
    }
    else
    {
        label->clear();
        label->setText("All lines have the same currency");
        label->setStyleSheet("color: green; font-weight: bold; font-family:Bahnschrift;font-size: 11pt ");
        label->show();
    }
}

// EXPORT COMPARISON FILES TO DESKTOP
void Page_7_CompareDatFiles::saveComparisonFilesToDesktop()
{
    if (currentFileData1.content.trimmed().isEmpty())
    {
        QMessageBox::warning(
            nullptr,
            "Brak pliku",
            "Najpierw wczytaj pierwszy plik DAT."
        );

        return;
    }

    if (currentFileData2.content.trimmed().isEmpty())
    {
        QMessageBox::warning(
            nullptr,
            "Brak pliku",
            "Najpierw wczytaj drugi plik DAT."
        );

        return;
    }

    FileProcessingProgress progress;

    progress.setWindowTitle("Eksport porównania");
    progress.setLabelText("Trwa porównywanie plików...");
    progress.setValue(0);
    progress.show();

    QApplication::processEvents();

    const FileExport::DatCompareResult compareResult =
        FileExport::compareDatFiles(
            currentFileData1,
            currentFileData2,
            &progress
        );

    /*
     * Kontrola, czy porównanie zwróci³o dane.
     * comparisonCsv powinien zawieraæ przynajmniej nag³ówek.
     */
    if (compareResult.comparisonCsv.isEmpty())
    {
        progress.finish();

        QMessageBox::critical(
            nullptr,
            "OPERATION ABORTED!",
            "THE OUTPUT FILES WERE NOT CREATED"
        );

        return;
    }

    const QString timestamp =
        QDateTime::currentDateTime()
        .toString("yyyyMMdd_HHmmss");

    const QString desktopPath =
        QStandardPaths::writableLocation(
            QStandardPaths::DesktopLocation
        );

    /*
     * Plik nr 1:
     * g³ówne porównanie wspólnych artyku³ów.
     */
    const QString comparisonPath =
        desktopPath
        + QDir::separator()
        + "fachurnik_dat_comparison_"
        + timestamp
        + ".csv";

    /*
     * Plik nr 2:
     * artyku³y wystêpuj¹ce tylko w jednym pliku.
     */
    const QString missingArticlesPath =
        desktopPath
        + QDir::separator()
        + "fachurnik_missing_articles_"
        + timestamp
        + ".csv";

    /*
     * Plik nr 3:
     * podsumowanie oraz grupy produktowe
     * z ró¿nicami cenowymi.
     */
    const QString summaryPath =
        desktopPath
        + QDir::separator()
        + "fachurnik_comparison_summary_"
        + timestamp
        + ".csv";

    QString error;

    progress.setLabelText(
        "Zapisywanie g³ównego pliku porównania..."
    );

    progress.setValue(85);
    QApplication::processEvents();

    const bool comparisonOk =
        FileExport::saveCsv(
            compareResult.comparisonCsv,
            comparisonPath,
            &error
        );

    if (!comparisonOk)
    {
        progress.finish();

        QMessageBox::warning(
            nullptr,
            "B³¹d",
            error
        );

        return;
    }

    progress.setLabelText(
        "Zapisywanie listy brakuj¹cych artyku³ów..."
    );

    progress.setValue(90);
    QApplication::processEvents();

    const bool missingArticlesOk =
        FileExport::saveCsv(
            compareResult.missingArticlesCsv,
            missingArticlesPath,
            &error
        );

    if (!missingArticlesOk)
    {
        progress.finish();

        QMessageBox::warning(
            nullptr,
            "B³¹d",
            error
        );

        return;
    }

    progress.setLabelText(
        "Zapisywanie podsumowania..."
    );

    progress.setValue(95);
    QApplication::processEvents();

    const bool summaryOk =
        FileExport::saveCsv(
            compareResult.summaryCsv,
            summaryPath,
            &error
        );

    if (!summaryOk)
    {
        progress.finish();

        QMessageBox::warning(
            nullptr,
            "B³¹d",
            error
        );

        return;
    }

    progress.setValue(100);
    QApplication::processEvents();
    progress.finish();

    QMessageBox::information(
        nullptr,
        "OK",
        QString(
            "Zapisano pliki:\n\n"
            "%1\n"
            "%2\n"
            "%3\n\n"
            "Zgodne rekordy: %4\n"
            "Rekordy z ró¿ni¹ cen: %5\n"
            "Tylko w pliku 1: %6\n"
            "Tylko w pliku 2: %7"
        )
        .arg(comparisonPath)
        .arg(missingArticlesPath)
        .arg(summaryPath)
        .arg(compareResult.matchingRecords)
        .arg(compareResult.differentRecords)
        .arg(compareResult.onlyInFirstFile)
        .arg(compareResult.onlyInSecondFile)
    );
}