#ifndef view_NotifyWidget_hpp
#define view_NotifyWidget_hpp

#include <QList>
#include <QVBoxLayout>
#include <QWidget>

#include "view/FANItem.hpp"

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

    void addFANItem(FANItem* item);

private:
    Ui::NotifyWidget* ui;
    QList<FANItem*> m_newFriendItems;
    QVBoxLayout* m_layout = nullptr;
};

#endif
