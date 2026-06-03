#pragma once

#include <QComboBox>

class ComboBoxHelper
{
public:
    static void loadCurrencies(QComboBox* comboBox);
    static void loadSalesRep(QComboBox* comboBox);
    static void loadCatalogNo(QComboBox* comboBox);
    static void loadDiscountGrp(QComboBox* comboBox);
};