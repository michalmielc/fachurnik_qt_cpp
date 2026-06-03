#include "MenuTreeBuilder.h"

void MenuTreeBuilder::build(QTreeWidget* treeWidget)
{
    treeWidget->clear();

    auto* eshop = addRoot(treeWidget, "ESHOP");
    addChild(eshop, "eshop file to E-SHOP");
    addChild(eshop, "create eshop files to E-SHOP");

    addChild(eshop, "csv to E-SHOP");

    auto* bmecat = addRoot(treeWidget, "BMECAT");
    addChild(bmecat, "create file to BMECat");

    auto* ariba = addRoot(treeWidget, "ARIBA");
    addChild(ariba, "create file to Ariba");

    auto* excel = addRoot(treeWidget, "EXCEL");
    addChild(excel, "export to Excel");

    treeWidget->expandAll();
}

QTreeWidgetItem* MenuTreeBuilder::addRoot(QTreeWidget* treeWidget, const QString& text)
{
    auto* item = new QTreeWidgetItem(treeWidget);
    item->setText(0, text);
    return item;
}

QTreeWidgetItem* MenuTreeBuilder::addChild(QTreeWidgetItem* parent, const QString& text)
{
    auto* item = new QTreeWidgetItem(parent);
    item->setText(0, text);
    return item;
}