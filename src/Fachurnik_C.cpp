#include "Fachurnik_C.h"
#include <QTreeWidget>
#include "Page_Menu.h"
#include "Page_1_DatFileToEshopFile.h"
#include "Page_2_CloneDatFilesFromDatFile.h"

Fachurnik_C::Fachurnik_C(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
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

};


//---------------------------------------------------------
//DESTRUCTOR
Fachurnik_C::~Fachurnik_C()
{}

