#ifndef view_PyqWidget_hpp
#define view_PyqWidget_hpp

#include <QVBoxLayout>
#include <QWidget>

#include "view/PyqItem.hpp"

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
    QVBoxLayout* m_pyqItemLayout = nullptr;
};

#endif
