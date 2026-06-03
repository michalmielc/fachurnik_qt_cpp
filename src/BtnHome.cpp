#include "BtnHome.h"

#include <QStackedWidget>

BtnHome::BtnHome(QWidget* parent)
    : QPushButton(parent),
    m_stackedWidget(nullptr),
    m_homePage(nullptr)
{
    setText("Back");

    connect(this,
        &QPushButton::clicked,
        this,
        &BtnHome::goHome);
}

void BtnHome::setStackedWidget(QStackedWidget* stackedWidget)
{
    m_stackedWidget = stackedWidget;
}

void BtnHome::setHomePage(QWidget* page)
{
    m_homePage = page;
}

void BtnHome::goHome()
{
    if (m_stackedWidget && m_homePage)
    {
        m_stackedWidget->setCurrentWidget(m_homePage);
    }
}