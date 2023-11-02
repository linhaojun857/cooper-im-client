#ifndef view_CGWidget_hpp
#define view_CGWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class CGWidget;
}
QT_END_NAMESPACE

class CGWidget : public QWidget {
    Q_OBJECT

public:
    explicit CGWidget(QWidget* parent = nullptr);

    ~CGWidget() override;

private:
    void handleClickCreatePushButton();

private:
    Ui::CGWidget* ui;
};

#endif
