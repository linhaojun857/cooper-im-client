#ifndef view_PyqWidget_hpp
#define view_PyqWidget_hpp

#include <QVBoxLayout>
#include <QWidget>

#include "view/PostPyqWidget.hpp"
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

    void addPyqItem(PyqItem* pyqItem);

    void clearData();

    void flushData();

    void loadData();

private:
    void handleClickPostPushButton();

    void loadMoreData(int value);

private:
    Ui::PyqWidget* ui;
    QVBoxLayout* m_pyqItemLayout = nullptr;
    PostPyqWidget* m_postPyqWidget = nullptr;
    int m_currentPage = 1;
    int m_pageSize = 10;
};

#endif
