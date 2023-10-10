#ifndef COOPER_IM_CLIENT_VIEW_LRWIDGET_HPP_
#define COOPER_IM_CLIENT_VIEW_LRWIDGET_HPP_

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
    Ui::LRWidget* ui;
};

#endif
