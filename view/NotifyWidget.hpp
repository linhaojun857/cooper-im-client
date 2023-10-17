#ifndef view_NotifyWidget_hpp
#define view_NotifyWidget_hpp

#include <QList>
#include <QVBoxLayout>
#include <QWidget>

#include "view/NewFriendItem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class NotifyWidget;
}
QT_END_NAMESPACE

class NotifyWidget : public QWidget {
    Q_OBJECT

public:
    explicit NotifyWidget(QWidget* parent = nullptr);

    ~NotifyWidget() override;

    void addNewFriendItem(NewFriendItem* item);

private:
    Ui::NotifyWidget* ui;
    QList<NewFriendItem*> m_newFriendItems;
    QVBoxLayout* m_layout = nullptr;
};

#endif
