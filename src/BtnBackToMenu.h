#pragma once

#include <QPushButton>
#include <QStackedWidget>

class BtnBackToMenu
{
public:
    static void backToMenu(
        QPushButton* button,
        QStackedWidget* stackedWidget,
        QWidget* menuPage);
};