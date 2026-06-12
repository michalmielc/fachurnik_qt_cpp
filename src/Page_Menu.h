#pragma once

#include "ui_Fachurnik_C.h"
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class Page_Menu : public QObject
{
    Q_OBJECT

public:
    explicit Page_Menu(Ui::Fachurnik_CClass& ui);

    void initialize();

private:
    Ui::Fachurnik_CClass& ui;

    void buildMenu();
    QTreeWidgetItem* addRoot(const QString& text);
    QTreeWidgetItem* addChild(QTreeWidgetItem* parent, const QString& text);

private slots:
    void onMenuDoubleClicked(QTreeWidgetItem* item, int column);
    void onMenuClicked(QTreeWidgetItem* item, int column);
};