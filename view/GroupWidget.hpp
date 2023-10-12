#ifndef view_GroupWidget_hpp
#define view_GroupWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class GroupWidget;
}
QT_END_NAMESPACE

class GroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit GroupWidget(QWidget* parent = nullptr);

    ~GroupWidget() override;

private:
    Ui::GroupWidget* ui;
};

#endif
