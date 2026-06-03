#include "ComboBoxHelper.h"
#include <QCompleter>

// FILLING CURRENCIES
void ComboBoxHelper::loadCurrencies(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->addItems({ "EUR", "PLN" });
    comboBox->setCurrentText("EUR");
}

// FILLING SALES REP SAP ID
void ComboBoxHelper::loadSalesRep(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->setEditable(true);
    comboBox->addItem("990498 RAFAŁ NAPOROWSKI");
    comboBox->addItem("990497 TOMASZ TOMIŃSKI");
    comboBox->addItem("990496 KRYSTIAN STACHOWIAK");
    comboBox->addItem("990494 GRZEGORZ MIŁOWSKI");
    comboBox->addItem("990493 ANDRZEJ KARAŚ");
    comboBox->addItem("990492 JAN PORĘBSKI");
    comboBox->addItem("990490 KRZYSZTOF KAŹMIERCZAK");
    comboBox->addItem("990489 GRZEGORZ KULA");
    comboBox->addItem("990488 KRZYSZTOF KUPCEWICZ");
    comboBox->addItem("990487 PAWEŁ RYBCZONEK");
    comboBox->addItem("990486 JAROSŁAW MAJCHRZAK");
    comboBox->addItem("990485 MARIUSZ KANABRODZKI");
    comboBox->addItem("990484 GRZEGORZ SIWAK");
    comboBox->addItem("990480 GRZEGORZ GOMUŁA");
    comboBox->addItem("990422 BOGUSŁAW ŚLIPEK");
    comboBox->addItem("990420 JAKUB ZIELIŃSKI");
    comboBox->addItem("990435 URSZULA MIGDAŁEK");
    comboBox->addItem("990425 JACEK WAEHNER");
    comboBox->addItem("990419 PAWEŁ KIERZENKA");
    comboBox->addItem("990415 MARCIN PACHOLSKI");
    comboBox->addItem("990412 ŁUKASZ PLUTA");
    comboBox->addItem("990411 WOJCIECH TOMASZEWSKI");

    comboBox->setEditable(true);
    comboBox->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    comboBox->completer()->setFilterMode(Qt::MatchContains);
         
}

// FILLING SALES REP SAP ID
void ComboBoxHelper::loadCatalogNo(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->addItems({
        "56",
        "57",
        "58",
        "59"
        });
    comboBox->setCurrentText("56");
}

void ComboBoxHelper::loadDiscountGrp(QComboBox* comboBox)
{
    comboBox->clear();
    comboBox->addItems({
        "0", "1", "2",
        "3", "4", "5",
        "6", "7", "A","M"
        });
}