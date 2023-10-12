#ifndef view_MsgWidget_hpp
#define view_MsgWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MsgWidget;
}
QT_END_NAMESPACE

class MsgWidget : public QWidget {
    Q_OBJECT

public:
    explicit MsgWidget(QWidget* parent = nullptr);

    ~MsgWidget() override;

private:
    Ui::MsgWidget* ui;
};

#endif
