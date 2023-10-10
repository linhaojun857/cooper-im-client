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
    void getVFCode(QPushButton* pushButton);

    void userLogin();

    void userRegister();

    void resetLoginWidget();

    void resetRegisterWidget();

private:
    Ui::LRWidget* ui;
};

#endif
