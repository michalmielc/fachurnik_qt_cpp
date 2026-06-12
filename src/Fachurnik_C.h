#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Fachurnik_C.h"

//PAGES
class Page_1_DatFileToEshopFile;
class Page_2_CloneDatFilesFromDatFile;

class Fachurnik_C : public QMainWindow
{
    Q_OBJECT

public:
    Fachurnik_C(QWidget* parent = nullptr);
    ~Fachurnik_C();


private:
    Ui::Fachurnik_CClass ui;

    //PAGES
    Page_1_DatFileToEshopFile* page1 = nullptr;
    Page_2_CloneDatFilesFromDatFile* page2 = nullptr;

private slots:

    void onMenuDoubleClicked(QTreeWidgetItem* item, int column);
    void onMenuClicked(QTreeWidgetItem* item, int column);
};
