#ifndef view_MainWidget_HPP_
#define view_MainWidget_HPP_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);

    ~MainWidget() override;

private:
    Ui::MainWidget* ui;
};

#endif
