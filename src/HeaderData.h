#pragma once

#include <QString>

struct HeaderData
{
    QString headerLine;  // ca³a linia
    QString customerNo;
    QString dateFrom;
    QString dateTo;
    QString currency;
    QString catalogNumber;
    QString discountGrp;
    bool alloySurcharge;
    bool specialOffers;
    QString salesRep;
    QString distrChannel;
    QString exchangeRate;
};