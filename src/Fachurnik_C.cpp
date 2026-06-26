#include "Fachurnik_C.h"
#include <QTreeWidget>
#include "Page_Menu.h"
#include "Page_1_DatFileToEshopFile.h"
#include "Page_2_CloneDatFilesFromDatFile.h"
#include "Page_3_CsvToEshop.h"
#include "Page_4_DatFileToBmeCat.h"
#include "Page_5_craeteCif.h"

Fachurnik_C::Fachurnik_C(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->centralWidget()->setStyleSheet("background-color: #f0f0f0;");
    //--------------------------------------------------

    ui.stackedWidget->setCurrentWidget(ui.pageMenu);

    // INICJALIZACJA MENU

    page0 = new Page_Menu(ui);
    page0->initialize();

    // INICJALIZACJA PAGE 1

    page1 = new Page_1_DatFileToEshopFile(ui);
    page1->initialize();

    // INICJALIZACJA PAGE 2

    page2 = new Page_2_CloneDatFilesFromDatFile(ui);
    page2->initialize();


    // INICJALIZACJA PAGE 3

    page3 = new Page_3_CsvToEshop(ui);
    page3->initialize();


    // INICJALIZACJA PAGE 4

    page4 = new Page_4_DatFileToBmeCat(ui);
    page4->initialize();

    // INICJALIZACJA PAGE 4

    page5 = new Page_5_createCif(ui);
    page5->initialize();
};


//---------------------------------------------------------
//DESTRUCTOR
Fachurnik_C::~Fachurnik_C()
{}

