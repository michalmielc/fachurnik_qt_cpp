#include "BtnBackToMenu.h"

void BtnBackToMenu::backToMenu(
    QPushButton* button,
    QStackedWidget* stackedWidget,
    QWidget* menuPage)
{
    QObject::connect(button,
        &QPushButton::clicked,
        [stackedWidget, menuPage]()
        {
            stackedWidget->setCurrentWidget(menuPage);
        });
}