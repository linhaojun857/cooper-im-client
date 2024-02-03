#ifndef view_PostPyqWidget_hpp
#define view_PostPyqWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PostPyqWidget;
}
QT_END_NAMESPACE

class PostPyqWidget : public QWidget {
    Q_OBJECT

public:
    explicit PostPyqWidget(QWidget* parent = nullptr);

    ~PostPyqWidget() override;

private:
    Ui::PostPyqWidget* ui;
};

#endif
