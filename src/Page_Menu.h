#pragma once

#include <QTreeWidget>

class MenuTreeBuilder
{
public:
    static void build(QTreeWidget* treeWidget);

private:
    static QTreeWidgetItem* addRoot(QTreeWidget* treeWidget, const QString& text);
    static QTreeWidgetItem* addChild(QTreeWidgetItem* parent, const QString& text);
};

// GENERATING MENU --------------------------------
ui.treeWidgetMenu->setHeaderHidden(true);

MenuTreeBuilder::build(ui.treeWidgetMenu);

// double click on menu redirection on the page
connect(ui.treeWidgetMenu,
    &QTreeWidget::itemDoubleClicked,
    this,
    &Fachurnik_C::onMenuDoubleClicked);

//click on menu more about functionality
connect(ui.treeWidgetMenu,
    &QTreeWidget::itemClicked,
    this,
    &Fachurnik_C::onMenuClicked);