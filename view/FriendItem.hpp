#ifndef view_FriendItem_hpp
#define view_FriendItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FriendItem;
}
QT_END_NAMESPACE

class FriendItem : public QWidget {
    Q_OBJECT

public:
    explicit FriendItem(QWidget* parent = nullptr);

    ~FriendItem() override;

    void setAvatar(const QString& url);

    void setNickName(const QString& nickName);

    void setStatus(const QString& status);

    void setFeeling(const QString& feeling);

private:
    Ui::FriendItem* ui;
};

#endif
