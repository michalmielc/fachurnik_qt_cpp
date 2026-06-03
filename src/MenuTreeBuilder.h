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