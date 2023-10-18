#ifndef view_FSRWidget_hpp
#define view_FSRWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FSRWidget;
}
QT_END_NAMESPACE

class FSRWidget : public QWidget {
    Q_OBJECT

public:
    explicit FSRWidget(QWidget* parent = nullptr);

    ~FSRWidget() override;

private:
    Ui::FSRWidget* ui;
};

#endif
