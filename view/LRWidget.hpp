#ifndef view_LRWIDGET_HPP_
#define view_LRWIDGET_HPP_

#include <QPushButton>
#include <QWidget>

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

private:
    // 0: login 1: register
    void getVFCode(int type);

    void userLogin();

    void userRegister();

    void resetLoginWidget();

    void resetRegisterWidget();

private:
    Ui::LRWidget* ui;
};

#endif
