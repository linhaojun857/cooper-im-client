#ifndef view_LRWIDGET_HPP
#define view_LRWIDGET_HPP

#include <QPushButton>
#include <QWidget>

#include "view/MainWidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class LRWidget;
}
QT_END_NAMESPACE

class LRWidget : public QWidget {
    Q_OBJECT

public:
    explicit LRWidget(QWidget* parent = nullptr);

    ~LRWidget() override;

    void setMainWidget(MainWidget* mainWidget);

private:
    void setLineEditLimit();
    // 0: login 1: register
    void getVFCode(int type);

    void userLogin();

    void userRegister();

    void resetLoginWidget();

    void resetRegisterWidget();

private:
    Ui::LRWidget* ui;
    MainWidget* m_mainWidget = nullptr;
};

#endif
