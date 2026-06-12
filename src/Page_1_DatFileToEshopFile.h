#pragma once

#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"
#include "ComboBoxHelper.h"

class Page_1_DatFileToEshopFile
{
public:
    explicit Page_1_DatFileToEshopFile(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize();
 
private:

    void onChooseFileClicked();
    Ui::Fachurnik_CClass& ui;
};