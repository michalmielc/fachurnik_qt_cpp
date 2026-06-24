#include "Page_Menu.h"

Page_Menu::Page_Menu(Ui::Fachurnik_CClass& ui)
    : ui(ui)
{
}

void Page_Menu::initialize()
{
    buildMenu();

    QObject::connect(
        ui.treeWidgetMenu,
        &QTreeWidget::itemDoubleClicked,
        this,
        &Page_Menu::onMenuDoubleClicked);

    QObject::connect(
        ui.treeWidgetMenu,
        &QTreeWidget::itemClicked,
        this,
        &Page_Menu::onMenuClicked);
}

void Page_Menu::buildMenu()
{
    ui.treeWidgetMenu->clear();
    ui.treeWidgetMenu->setHeaderHidden(true);

    auto* eshop = addRoot("E-SHOP");

    addChild(eshop, "eshop file to E-SHOP");
    addChild(eshop, "create eshop files to E-SHOP");
    addChild(eshop, "csv to E-SHOP");

    auto* bmecat = addRoot("BMECAT");
    addChild(bmecat, "create file to BMECat");

    auto* ariba = addRoot("ARIBA");
    addChild(ariba, "create file to Ariba");

    auto* excel = addRoot("EXCEL");
    addChild(excel, "export to Excel");
    addChild(excel, "compare Dat Files");

    ui.treeWidgetMenu->expandAll();
}

QTreeWidgetItem* Page_Menu::addRoot(const QString& text)
{
    auto* item = new QTreeWidgetItem(ui.treeWidgetMenu);
    item->setText(0, text);
    return item;
}

QTreeWidgetItem* Page_Menu::addChild(QTreeWidgetItem* parent, const QString& text)
{
    auto* item = new QTreeWidgetItem(parent);
    item->setText(0, text);
    return item;
}

void Page_Menu::onMenuDoubleClicked(QTreeWidgetItem* item, int column)
{
    QString text = item->text(0);

    if (text == "eshop file to E-SHOP")
        ui.stackedWidget->setCurrentWidget(ui.pageDatFileToEshop);

    if (text == "create eshop files to E-SHOP")
        ui.stackedWidget->setCurrentWidget(ui.pageCloneFilesEshopToEshop);

    if (text == "csv to E-SHOP")
        ui.stackedWidget->setCurrentWidget(ui.pageCsvToEshop);

    if (text == "create file to BMECat")
        ui.stackedWidget->setCurrentWidget(ui.pageDatToBmeCat);
}


// DESCRIPTIONS
void Page_Menu::onMenuClicked(QTreeWidgetItem* item, int column)
{
    QString text = item->text(0);

    if (text == "eshop file to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji utworzysz plik dat do ESHOP, 
zmodyfikujesz walutę, nr katalogu, kurs, dodatki etc.
Format do wczytania: dat.
Format wyjściowy: dat z zapisem do pulpitu.
Dane można zapisać równocześnie do csv.
Podczas wczytywanie program wykryje, jeśli w pliku sa różne waluty.)");
    }


    if (text == "csv to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji utworzysz plik dat do ESHOP, 
zmodyfikujesz walutę, nr katalogu, kurs, dodatki etc.
Format do wczytania: csv. Obowiązują cztery kolumny nr artykułu; waluta; cena1; cena2
Format wyjściowy: dat z zapisem do pulpitu.
Dane można zapisać równocześnie do csv.
Podczas wczytywanie program wykryje, jeśli w pliku sa różne waluty.)");
    }

    if (text == "create eshop files to E-SHOP")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji sklonujesz
pliki na bazie jednego pliku z konkretną grupą rabatową. Wymagane jest załadowanie listy csv dwukolumnowej
bez nagłówka, w kolejności nr klienta; nr GVLa)");
    }

    if (text == "create file to BMECat")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji utworzysz plik CSV do BMECAT, 
zmodyfikujesz walutę, nr katalogu, kurs, dodatki etc.
Format do wczytania: dat.
Format wyjściowy: csv z zapisem do pulpitu.
Podczas wczytywanie program wykryje, jeśli w pliku sa różne waluty.)");
    }

    if (text == "create file to Ariba")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji utworzysz plik CIF do Ariby, 
zmodyfikujesz walutę, nr katalogu, kurs, dodatki etc.
Format do wczytania: dat.
Format wyjściowy: CIF z zapisem do pulpitu.)");
    }


    if (text == "export to Excel")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji utworzysz plik csv. 
Zmodyfikujesz walutę, nr katalogu, kurs, dodatki etc.
Format do wczytania: dat.
Format wyjściowy: CIF z zapisem do pulpitu.)");
    }

    if (text == "compare Dat Files")
    {
        ui.plainTextEdit->setPlainText(R"(Za pomocą tej opcji porównasz
dwa pliki dat.
Format do wczytania: dat.
Format wyjściowy: CIF z zapisem do pulpitu.)");
    }

   

}