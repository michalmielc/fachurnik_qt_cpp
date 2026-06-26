#include "Page_5_craeteCif.h"


void Page_5_createCif::initialize() {

};

void Page_5_createCif::onChooseFileClicked() {

};


// STYLE:------------------------------------------------------------
// CUSTOMIZE LABEL COLOR
void Page_5_createCif::setLabel(QLabel* label, const QString& text, const QString& color)
{
    label->setText(text);
    label->setStyleSheet("color: " + color + ";");
}
