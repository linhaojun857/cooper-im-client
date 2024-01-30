#ifndef view_PyqWidget_hpp
#define view_PyqWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PyqWidget;
}
QT_END_NAMESPACE

class PyqWidget : public QWidget {
    Q_OBJECT

public:
    explicit PyqWidget(QWidget* parent = nullptr);

    ~PyqWidget() override;

private:
    Ui::PyqWidget* ui;
};

#endif
