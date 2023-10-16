#ifndef COOPER_IM_CLIENT_VIEW_NEWFRIENDITEM_HPP_
#define COOPER_IM_CLIENT_VIEW_NEWFRIENDITEM_HPP_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class NewFriendItem;
}
QT_END_NAMESPACE

class NewFriendItem : public QWidget {
    Q_OBJECT

public:
    explicit NewFriendItem(QWidget* parent = nullptr);

    ~NewFriendItem() override;

    void setAvatar(const QString& url);

    void setNickname(const QString& nickname);

    void setReason(const QString& reason);

private:
    Ui::NewFriendItem* ui;
    QString m_avatarUrl;
    QString m_nickname;
    QString m_reason;
};

#endif
