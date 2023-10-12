#ifndef view_NewFriWidget_hpp
#define view_NewFriWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class NewFriWidget;
}
QT_END_NAMESPACE

class NewFriWidget : public QWidget {
    Q_OBJECT

public:
    explicit NewFriWidget(QWidget* parent = nullptr);

    ~NewFriWidget() override;

private:
    Ui::NewFriWidget* ui;
};

#endif
