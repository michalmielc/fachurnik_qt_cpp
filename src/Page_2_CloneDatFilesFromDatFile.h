#pragma once
#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "ComboBoxHelper.h"

class Page_2_CloneDatFilesFromDatFile {

public:

    explicit Page_2_CloneDatFilesFromDatFile(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize()
    {
        BtnBackToMenu::backToMenu(
            ui.btnBackToHome2,
            ui.stackedWidget,
            ui.pageMenu);

        ComboBoxHelper::loadCatalogNo(ui.comBoxCatalogNo_2);
        ComboBoxHelper::loadCurrencies(ui.comBoxCurrency_2);
        ComboBoxHelper::loadDiscountGrp(ui.comBoxDiscountG_2);
        ComboBoxHelper::loadSalesRep(ui.comBoxSalesRep_2);
    }

private:
    Ui::Fachurnik_CClass& ui;
};

