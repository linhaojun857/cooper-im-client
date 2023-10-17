#ifndef view_GroupWidget_hpp
#define view_GroupWidget_hpp

#include <QVBoxLayout>
#include <QWidget>

#include "view/GroupItem.hpp"

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

    void addGroupItem(GroupItem* groupItem);

private:
    Ui::GroupWidget* ui;
    QMap<int, GroupItem*> m_groupItems;
    QVBoxLayout* m_groupItemsLayout = nullptr;
};

#endif
