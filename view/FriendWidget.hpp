#ifndef view_FriendWidget_hpp
#define view_FriendWidget_hpp

#include <QWidget>

#include "view/FriendItem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class FriendWidget;
}
QT_END_NAMESPACE

class FriendWidget : public QWidget {
    Q_OBJECT

public:
    explicit FriendWidget(QWidget* parent = nullptr);

    ~FriendWidget() override;

private:
    Ui::FriendWidget* ui;
    QList<FriendItem*> friendItems;
};

#endif
