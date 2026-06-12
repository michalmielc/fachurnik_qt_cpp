#include "Fachurnik_C.h"
#include <QTreeWidget>
#include "MenuTreeBuilder.h"
#include "Page_1_DatFileToEshopFile.h"
#include "Page_2_CloneDatFilesFromDatFile.h"

Fachurnik_C::Fachurnik_C(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //--------------------------------------------------

    ui.stackedWidget->setCurrentWidget(ui.pageMenu);



    // INICJALIZACJA PAGE 1

    page1 = new Page_1_DatFileToEshopFile(ui);
    page1->initialize();

    // INICJALIZACJA PAGE 2

    page2 = new Page_2_CloneDatFilesFromDatFile(ui);
    page2->initialize();

};

  

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


//---------------------------------------------------------
//DESTRUCTOR
Fachurnik_C::~Fachurnik_C()
{}

