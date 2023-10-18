#ifndef view_FriendWidget_hpp
#define view_FriendWidget_hpp

#include <QVBoxLayout>
#include <QWidget>

#include "view/FriendItem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class FriendWidget;
}
QT_END_NAMESPACE

class FriendWidget : public QWidget {
    friend class Mock;
    Q_OBJECT

public:
    explicit FriendWidget(QWidget* parent = nullptr);

    ~FriendWidget() override;

    void addFriendItem(FriendItem* friendItem);

private:
    Ui::FriendWidget* ui;
    QVBoxLayout* m_friendItemLayout = nullptr;
};

#endif
