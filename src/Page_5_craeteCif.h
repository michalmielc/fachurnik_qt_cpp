#pragma once
#include "ui_Fachurnik_C.h"
#include "BtnBackToMenu.h"

class Page_5_createCif
{
public:
    explicit Page_5_createCif(
        Ui::Fachurnik_CClass& ui)
        : ui(ui)
    {
    }

    void initialize();

private:


    void onChooseFileClicked();

    void setLabel(QLabel* label, const QString& text, const QString& color);


    Ui::Fachurnik_CClass& ui;
};
