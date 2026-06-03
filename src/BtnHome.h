#pragma once
#include <QPushButton>

class QStackedWidget;
class QWidget;

class BtnHome : public QPushButton
{
    Q_OBJECT

public:
    explicit BtnHome(QWidget* parent = nullptr);

    void setStackedWidget(QStackedWidget* stackedWidget);
    void setHomePage(QWidget* page);

private slots:
    void goHome();

private:
    QStackedWidget* m_stackedWidget;
    QWidget* m_homePage;
};

